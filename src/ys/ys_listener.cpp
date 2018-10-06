#include "ys.h"

ys::listener::listener(
	std::shared_ptr<boost::asio::io_context> io_context_p,
	const std::string ip,
	const std::uint16_t port,
	std::shared_ptr<std::map<std::string, ys::router>> router_map_p
)
:io_context_p(io_context_p)
,acceptor(*io_context_p, boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip), port))
,_router_map_p(router_map_p)
{
}

void ys::listener::start()
{
	do_accept();
}

void ys::listener::do_accept()
{
	auto self(shared_from_this());
	acceptor.async_accept
	(
		[this, self](boost::system::error_code ec, boost::asio::ip::tcp::socket socket)
		{
			if (!ec)
			{
				std::make_shared<session>(io_context_p,std::move(socket), _router_map_p)->start();
			}

			do_accept();
		}
	);
}
