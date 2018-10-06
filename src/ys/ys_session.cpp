#include "ys.h"

ys::session::session
(
	std::shared_ptr<boost::asio::io_context> io_context_p,
	boost::asio::ip::tcp::socket socket,
	std::shared_ptr<std::map<std::string, ys::router>> router_map_p
)
:io_context_p(io_context_p)
,socket(std::move(socket))
,_request_ext(std::make_shared<request>())
,_response_ext(std::make_shared<response>())
,_router_map_p(router_map_p)
{
}

void ys::session::start()
{
	_request_ext.request_item()->init();
	_response_ext.response_item()->init();
	do_read();
}

void ys::session::do_read()
{
	auto self(shared_from_this());
	socket.async_receive(
		boost::asio::buffer(_recv_buffer),
		[this, self](boost::system::error_code ec, std::size_t length)
		{
			if (ec)
			{
				return;
			}

			_request_ext.append_msg(_recv_buffer,length);

			bool parse_res = _request_ext.do_parser();
			if (!parse_res)
			{
				return;
			}

			if (
				(!_request_ext.request_item()->first_line_end()) || 
				((_request_ext.request_item()->request_method() == "GET")  && (!_request_ext.request_item()->header_end())) ||
				((_request_ext.request_item()->request_method() == "POST") && (!_request_ext.request_item()->body_end()))
			)
			{
				do_read();
			}
			else
			{
				do_handle();
			}
		}
	);
}

void ys::session::do_handle()
{
	auto self(shared_from_this());

	std::string hostname = "_";

	ys::router *hit_router = nullptr;

	const std::map<std::string, std::string>& header_ref = _request_ext.request_item()->header();
	const auto route_host_iter = header_ref.find("Host");
	const auto route_end_iter  = header_ref.end();
	if (route_host_iter != route_end_iter)
	{
		hostname = header_ref.at("Host");
	}

	if ((_router_map_p->find(hostname) == _router_map_p->end()) && (_router_map_p->find("_") != _router_map_p->end()))
	{
		hit_router = &(_router_map_p->at("_"));
	}
	else
	{
		return;
	}

	std::function<void(request &req, response &res)> handle_fun;
	bool handle_fun_find = hit_router->get_handle(_request_ext.request_item()->request_uri(), handle_fun);

	if (!handle_fun_find)
	{
		_response_ext.build_404();
	}
	else
	{
		handle_fun(*(_request_ext.request_item()), *(_response_ext.response_item()));
	}

	_response_ext.build();

	do_write();
}

void ys::session::do_write()
{
	auto self(shared_from_this());
	std::size_t length = 0;

	_response_ext.append_buffer(_send_buffer, length);
	
	boost::asio::async_write(
		socket,
		boost::asio::buffer(_send_buffer, length),
		[this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (ec)
			{
				return;
			}

			if (_response_ext.response_item()->full_msg().size())
			{
				do_write();
			}
			else
			{
				start();
			}
		}
	);
}
