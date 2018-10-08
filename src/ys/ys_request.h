#ifndef _YS_REQUEST_H_
#define _YS_REQUEST_H_

namespace ys
{
	class request
	{
	public:
		void init();

		const std::uint64_t& first_line_end() const;
		void first_line_end(const std::uint64_t &val);

		const std::uint64_t& header_end() const;
		void header_end(const std::uint64_t &val);

		const std::uint64_t& body_end() const;
		void body_end(const std::uint64_t &val);

		const std::string& request_method() const;
		void request_method(const std::string& val);

		const std::string& request_uri() const;
		void request_uri(const std::string& val);

		const std::string& request_version() const;
		void request_version(const std::string& val);

		const std::string& msg() const;
		void append_msg(const char c);

		const std::map<std::string, std::string>& header() const;
		void insert_header(const std::pair<std::string, std::string>& pair);

	protected:
		std::string _msg;

		std::uint64_t _first_line_end = 0;
		std::uint64_t _header_end = 0;
		std::uint64_t _body_end = 0;

		std::string   _request_method;
		std::string   _request_uri;
		std::string   _request_version;

		std::map<std::string, std::string> _header;
	};

	class request_ext
	{
	public:
		request_ext(std::shared_ptr<request> _request_p);
		void append_msg(const recv_buffer& _recv_buffer, const std::size_t& length);
		void insert_header(const std::pair<std::string, std::string>& pair);
		bool do_parser();
		const std::shared_ptr<request> request_item() const;
	private:
		std::shared_ptr<request> _request_p;

		bool do_parser_first_line();
		bool do_parser_head();
		bool do_parser_body();
	};
}

#endif