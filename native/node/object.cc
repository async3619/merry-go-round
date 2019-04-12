#include "../includes.hpp"

DECLARE_TYPE(NodeObject);

NodeObject::NodeObject(void) {}
NodeObject::~NodeObject(void) {}

bool NodeObject::exists(const std::string& key) {
	return this->dictionary.find(key) != this->dictionary.cend();
}

node_value_t NodeObject::toJS(node_info_t info) const {
	node_object_t nodeObject = node_object_t::New(NODE_ENV);
	NodeString string;

	for (auto& item : this->dictionary) {
		string = item.first;
		nodeObject.Set(string.toJS(info), item.second(info));
	}

	return nodeObject;
}

NodeObject::generator_holder& NodeObject::operator[](const std::string& key) {
	return this->dictionary[key];
}
