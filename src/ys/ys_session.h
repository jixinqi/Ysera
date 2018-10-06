#ifndef _YS_SESSION_H_
#define _YS_SESSION_H_

namespace ys
{
	class session : public std::enable_shared_from_this<session>
	{
	public:
		session
		(
			std::shared_ptr<boost::asio::io_context> io_context_p,
			boost::asio::ip::tcp::socket socket,
			std::shared_ptr<std::map<std::string, ys::router>> router_map_p
		);
		void start();
	private:
		std::shared_ptr<boost::asio::io_context> io_context_p;
		boost::asio::ip::tcp::socket socket;

		recv_buffer _recv_buffer;
		send_buffer _send_buffer;

		request_ext  _request_ext;
		response_ext _response_ext;

		std::shared_ptr<std::map<std::string, ys::router>> _router_map_p;

		void do_read();
		void do_handle();
		void do_write();
	};
}

#endif