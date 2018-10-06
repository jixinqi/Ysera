#ifndef _YS_LISTENER_H_
#define _YS_LISTENER_H_

namespace ys
{
	class listener : public std::enable_shared_from_this<listener>
	{
	public:
		listener(
			std::shared_ptr<boost::asio::io_context> io_context_p,
			const std::string ip,
			const std::uint16_t port,
			std::shared_ptr<std::map<std::string, ys::router>> router_map_p
		);

		void start();

	private:
		std::shared_ptr<boost::asio::io_context> io_context_p;
		std::string   ip;
		std::uint16_t port;
		std::uint8_t  ssl_enable;
		boost::asio::ip::tcp::acceptor acceptor;

		std::shared_ptr<std::map<std::string, ys::router>> _router_map_p;

		void do_accept();
	};
}

#endif
