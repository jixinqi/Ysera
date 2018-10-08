#include "ys.h"

void ys::router_rule::hello(request & req, response & res)
{
	std::string msg = "Hello, world!\n";
	res.msg(msg);
}

void ys::router_rule::static_file(request &req, response &res)
{
}

ys::route_tree_node::route_tree_node(const std::string sub_path, std::function<void(request&, response&)> handle)
:_path(sub_path)
,_handle(handle)
{
}

const std::string & ys::route_tree_node::path() const
{
	return _path;
}

std::function<void(ys::request&, ys::response&)> ys::route_tree_node::handle() const
{
	return _handle;
}

void ys::route_tree_node::handle(const std::function<void(request&, response&)>& handle)
{
	_handle = handle;
}

std::shared_ptr<ys::route_tree_node> ys::route_tree_node::get_sub_route_tree_node(const std::string sub_path, bool create_if_not_exist)
{
	auto i = _sub_route_tree_node_l.begin();
	for (; i != _sub_route_tree_node_l.end(); i++)
	{
		if ((*i)->path() == sub_path)
		{
			return *i;
		}
		if ((*i)->path() > sub_path)
		{
			break;
		}
	}
	
	if (!create_if_not_exist)
	{
		return nullptr;
	}

	std::shared_ptr<ys::route_tree_node> new_node(std::make_shared<ys::route_tree_node>(sub_path));
	_sub_route_tree_node_l.insert(i, new_node);
	return new_node;
}

ys::router::router()
:root(std::make_shared<route_tree_node>())
{
}

void ys::router::add_handle(const std::string & path, const std::function<void(request &req, response &res)> handle_fun)
{
	if (path.size() == 0 || path.at(0) != '/')
	{
		// EXIT LOG
	}

	std::vector<std::string> sub_path_vec = split_path(path);
	std::shared_ptr<route_tree_node> route_tree_node_ptr = root;

	for (auto &sub_path : sub_path_vec)
	{
		route_tree_node_ptr = route_tree_node_ptr->get_sub_route_tree_node(sub_path,true);
	}

	route_tree_node_ptr->handle(handle_fun);
}

bool ys::router::get_handle(const std::string& path, std::function<void(request &req, response &res)> &handle_fun)
{
	if (path.size() == 0 || path.at(0) != '/')
	{
		// EXIT LOG
	}

	std::vector<std::string> sub_path_vec = split_path(path);
	std::shared_ptr<route_tree_node> route_tree_node_ptr = root;

	for (auto &sub_path : sub_path_vec)
	{
		route_tree_node_ptr = route_tree_node_ptr->get_sub_route_tree_node(sub_path);
		if (route_tree_node_ptr == nullptr)
		{
			return false;
		}
	}

	handle_fun = route_tree_node_ptr->handle();
	return true;
}

std::vector<std::string> ys::router::split_path(const std::string & path)
{
	std::string temp;
	std::vector<std::string> sub_path_vec;
	for (std::size_t i = 0; i != path.size(); i++)
	{
		temp.push_back(path.at(i));
		if (i + 1 != path.size() && path.at(i + 1) == '/')
		{
			sub_path_vec.push_back(temp);
			temp.clear();
		}
	}
	sub_path_vec.push_back(temp);
	return sub_path_vec;
}
