#include "ys/ys.h"

int main()
{
	ys::server server;

	ys::router default_router;
	default_router.add_handle("/", ys::router_rule::hello);

	//default_router.add_handle("/aaa/bbb/ddd", ys::router_rule::static_file);
	//default_router.add_handle("/aaa/bbb/ccc/ddd/eee", ys::router_rule::static_file);
	//default_router.add_handle("/a/b/asdf", ys::router_rule::static_file);
	
	server.add_router("_",default_router);

	server.run();
}
