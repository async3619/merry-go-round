#include "../includes.hpp"

NodeArray::NodeArray(void) { }
NodeArray::~NodeArray(void) { }

node_value_t NodeArray::toJS(node_info_t info) const {
	node_array_t result = node_array_t::New(NODE_ENV);
	std::size_t i = 0;
	for (auto& item : this->items) {
		result[i++] = item(info);
	}

	return result;
}
