#ifndef _YS_SERVER_H_
#define _YS_SERVER_H_

namespace ys
{
	class server
	{
	public:
		server();
		void add_router(const std::string & hostname, ys::router & router);
		void run();
	private:
		std::shared_ptr<boost::asio::io_context> io_context_p;
		std::list<std::shared_ptr<listener>> listener_l;

		std::shared_ptr<std::map<std::string, ys::router>> router_map_p;
	};
}

#endif
