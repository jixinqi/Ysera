#include "ys.h"
#include "ys_response.h"

void ys::response::init()
{
	_full_msg.clear();
	_msg.clear();
	_status_code = 200;
	_phrase = "OK";
}

const std::string & ys::response::msg() const
{
	return _msg;
}

void ys::response::msg(const std::string & val)
{
	_msg = val;
}

const std::string & ys::response::full_msg() const
{
	return _full_msg;
}

void ys::response::full_msg(const std::string & val)
{
	_full_msg = val;
}

const std::uint16_t ys::response::status_code() const
{
	return _status_code;
}

void ys::response::status_code(const std::uint16_t val)
{
	_status_code = val;
}

const std::string & ys::response::phrase() const
{
	return _phrase;
}

void ys::response::phrase(const std::string & val)
{
	_phrase = val;
}

void ys::response::append_buffer(send_buffer & _send_buffer, std::size_t & length)
{
	length = 0;
	while (length < _send_buffer.size())
	{
		if (!_full_msg.size())
		{
			return;
		}

		auto iter = _full_msg.begin();
		_send_buffer.at(length) = *iter;
		_full_msg.erase(iter);

		length++;
	}
}

ys::response_ext::response_ext(std::shared_ptr<response> _response_p)
:_response_p(_response_p)
{

}

const std::shared_ptr<ys::response> ys::response_ext::response_item() const
{
	return _response_p;
}

void ys::response_ext::build()
{
	std::string result;
	result.append("HTTP/1.1 ")
		  .append(std::to_string(_response_p->status_code()))
		  .append(" ")
		  .append(_response_p->phrase())
		  .append("\r\n");
	result.append("Connection: close\r\n");
	result.append("Server: ys/0.1\r\n");
	result.append("Content-Type: text/plain\r\n");
	result.append("Content-Length: ").append(std::to_string(_response_p->msg().length())).append("\r\n");
	result.append("\r\n");
	result.append(_response_p->msg());
	_response_p->full_msg(result);
}

void ys::response_ext::build_404()
{
	_response_p->status_code(404);
	_response_p->phrase("Not Found");
}

void ys::response_ext::append_buffer(send_buffer & _send_buffer, std::size_t & length)
{
	_response_p->append_buffer(_send_buffer, length);
}
