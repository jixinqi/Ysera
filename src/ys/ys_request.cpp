#include "ys.h"

void ys::request::init()
{
	std::size_t end = 0;

	if (_body_end)
	{
		end = _body_end;
	}
	else if (_header_end)
	{
		end = _header_end;
	}

	if (end)
	{
		for (std::size_t i = 0; i != end; i++)
		{
			auto iter = _msg.begin();
			_msg.erase(iter);
		}
	}
	else
	{
		_msg.clear();
	}

	_first_line_end = 0;
	_header_end = 0;
	_body_end = 0;
	_request_method.clear();
	_request_uri.clear();
	_request_version.clear();
	_header.clear();
}

const std::uint64_t & ys::request::first_line_end() const
{
	return _first_line_end;
}

const std::uint64_t & ys::request::header_end() const
{
	return _header_end;
}

const std::uint64_t & ys::request::body_end() const
{
	return _body_end;
}

const std::string & ys::request::request_method() const
{
	return _request_method;
}

const std::string & ys::request::request_uri() const
{
	return _request_uri;
}

const std::string & ys::request::request_version() const
{
	return _request_version;
}

const std::string & ys::request::msg() const
{
	return _msg;
}

const std::map<std::string, std::string> ys::request::header() const
{
	return _header;
}

void ys::request::first_line_end(const std::uint64_t & val)
{
	_first_line_end = val;
}

void ys::request::header_end(const std::uint64_t & val)
{
	_header_end = val;
}

void ys::request::body_end(const std::uint64_t & val)
{
	_body_end = val;
}

void ys::request::request_method(const std::string & val)
{
	_request_method = val;
}

void ys::request::request_uri(const std::string & val)
{
	_request_uri = val;
}

void ys::request::request_version(const std::string & val)
{
	_request_version = val;
}

void ys::request::append_msg(const char c)
{
	_msg.push_back(c);
}

void ys::request::insert_header(const std::pair<std::string, std::string>& pair)
{
	_header.insert(pair);
}

ys::request_ext::request_ext(std::shared_ptr<request> _request_p)
:_request_p(_request_p)
{
}

void ys::request_ext::append_msg(const recv_buffer & _recv_buffer, const std::size_t& length)
{
	for (std::size_t i = 0; i != length; i++)
	{
		_request_p->append_msg(_recv_buffer.at(i));
	}
}

void ys::request_ext::insert_header(const std::pair<std::string, std::string>& pair)
{
	_request_p->insert_header(pair);
}

bool ys::request_ext::do_parser()
{
	if (!do_parser_first_line())
	{
		return false;
	}
	if (!do_parser_head())
	{
		return false;
	}
	if (!do_parser_body())
	{
		return false;
	}
	return true;
}

const std::shared_ptr<ys::request> ys::request_ext::request_item() const
{
	return _request_p;
}

bool ys::request_ext::do_parser_first_line()
{
	if (_request_p->first_line_end())
	{
		return true;
	}

	const std::string split_str("\r\n");
	const std::size_t split_str_len = split_str.size();

	const std::size_t pos = _request_p->msg().find(split_str);
	if (pos == _request_p->msg().npos)
	{
		return (_request_p->msg().length() + split_str_len <= YS_FIRSR_LINE_MAX_SIZE);
	}
	else if (pos + split_str_len > YS_FIRSR_LINE_MAX_SIZE)
	{
		return false;
	}

	std::vector<std::string> first_line_item_vec;
	std::string temp_item;
	for (std::size_t i = 0; i != pos; i++)
	{
		unsigned char c = _request_p->msg().at(i);
		if (c == ' ')
		{
			first_line_item_vec.push_back(temp_item);
			temp_item.clear();
		}
		else
		{
			temp_item.push_back(c);
		}
	}
	first_line_item_vec.push_back(temp_item);

	if (first_line_item_vec.size() != 3)
	{
		return false;
	}

	_request_p->request_method(first_line_item_vec.at(0));
	_request_p->request_uri(first_line_item_vec.at(1));
	_request_p->request_version(first_line_item_vec.at(2));

	_request_p->first_line_end(pos + split_str_len);

	return true;
}

bool ys::request_ext::do_parser_head()
{
	if (_request_p->header_end())
	{
		return true;
	}

	const std::string split_str("\r\n\r\n");
	const std::size_t split_str_len = split_str.size();

	const std::size_t pos = _request_p->msg().find(split_str);
	if (pos == _request_p->msg().npos)
	{
		return (_request_p->msg().length() + split_str_len - _request_p->first_line_end() <= YS_HEAD_MAX_SIZE);
	}
	else if (pos + split_str_len - _request_p->first_line_end() > YS_HEAD_MAX_SIZE)
	{
		return false;
	}

	std::string raw_request_header = _request_p->msg().substr(_request_p->first_line_end(), pos - _request_p->first_line_end() + 2);
	std::size_t last_line_break_index = 0;
	std::size_t line_break_index = 0;
	std::string line;
	std::string key;
	std::string value;

	while (line_break_index != raw_request_header.npos && line_break_index < raw_request_header.length())
	{
		last_line_break_index = line_break_index;
		line_break_index = raw_request_header.find("\r\n", line_break_index);
		line = raw_request_header.substr(last_line_break_index, line_break_index - last_line_break_index);
		line_break_index += 2;

		std::size_t colon_index = line.find(":");
		if (colon_index == line.npos)
		{
			return false;
		}

		key = line.substr(0, colon_index);
		value = line.substr(colon_index + 1, line.length() - 1);
		for (std::size_t erase_space_index = 0; erase_space_index != value.length(); erase_space_index++)
		{
			if (value.at(erase_space_index) != ' ')
			{
				value.erase(0, erase_space_index);
				break;
			}
		}

		_request_p->insert_header(std::pair<std::string, std::string>(key, value));
	}

	_request_p->header_end(pos + split_str_len);

	return true;
}

bool ys::request_ext::do_parser_body()
{
	return true;
}
