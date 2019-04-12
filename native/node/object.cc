#include "../includes.hpp"

NodeObject::generator_holder::generator_holder(void) 
	: assigned(false) { }
NodeObject::generator_holder::~generator_holder(void) {}

node_value_t NodeObject::generator_holder::operator()(node_info_t info) const {
	return this->generator(info);
}

NodeObject::NodeObject(void) {}
NodeObject::~NodeObject(void) {}

node_object_t NodeObject::toJS(node_info_t info) const {
	node_object_t nodeObject = node_object_t::New(NODE_ENV);
	NodeString string;

	for (auto& item : this->dictionary) {
		string = item.first;
		nodeObject.Set(string.toJS(info), item.second(info));
	}

	return nodeObject;
}

NodeObject::generator_holder& NodeObject::operator[](std::string key) {
	return this->dictionary[key];
}

//////////////////////////////////////////////////////////////////////////
// generator_holder implementations
//////////////////////////////////////////////////////////////////////////
template <>
NodeObject::generator_holder& NodeObject::generator_holder::operator=<std::string>(const std::string& data) {
	NodeString nodeString(data);
	this->generator = [&](node_info_t info) {
		return nodeString.toJS(info);
	};

	return (*this);
}

template <>
NodeObject::generator_holder& NodeObject::generator_holder::operator=<const char*>(const char* data) {
	NodeString nodeString(data);
	this->generator = [&](node_info_t info) {
		return nodeString.toJS(info);
	};

	return (*this);
}

template <>
NodeObject::generator_holder& NodeObject::generator_holder::operator=<char>(char data) {
	char buffer[2] = { data, 0 };

	NodeString nodeString(buffer);
	this->generator = [&](node_info_t info) {
		return nodeString.toJS(info);
	};

	return (*this);
}