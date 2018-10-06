#include "ys.h"

ys::server::server()
:io_context_p(std::make_shared<boost::asio::io_context>())
,router_map_p(std::make_shared<std::map<std::string, ys::router>>())
{
}

void ys::server::add_router(const std::string & hostname, ys::router & router)
{
	if (router_map_p->find(hostname) != router_map_p->end())
	{
		// exit log
	}

	router_map_p->insert({ hostname, router });
}

void ys::server::run()
{
	std::shared_ptr<listener> listener_http = std::make_shared<listener>(io_context_p, std::string("0.0.0.0"), 80, router_map_p);
	listener_http->start();

	io_context_p->run();
}
