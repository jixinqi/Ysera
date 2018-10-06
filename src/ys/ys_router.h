#ifndef _YS_ROUTER_H_
#define _YS_ROUTER_H_

namespace ys
{
	namespace router_rule
	{
		void hello(request &req, response &res);
		void static_file(request &req, response &res);
	}

	class route_tree_node
	{
	public:
		route_tree_node() = default;
		route_tree_node(const std::string sub_path, std::function<void(request&, response&)> handle = nullptr);

		const std::string & path() const;

		std::function<void(request&, response&)> handle() const;
		void handle(const std::function<void(request&, response&)> &handle);

		std::shared_ptr<route_tree_node> get_sub_route_tree_node(const std::string sub_path,bool create_if_not_exist = false);

	private:
		const std::string _path;
		std::function<void(request&, response&)> _handle = nullptr;

		std::list<std::shared_ptr<route_tree_node>> _sub_route_tree_node_l;
	};

	class router
	{
	public:
		router();

		void add_handle(const std::string& path, const std::function<void(request &req,response &res)> handle_fun);
		bool get_handle(const std::string& path, std::function<void(request &req, response &res)> &handle_fun);

		static std::vector<std::string> split_path(const std::string &path);

	private:
		std::shared_ptr<route_tree_node> root;
	};
}

#endif