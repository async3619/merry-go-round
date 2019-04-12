#include "../includes.hpp"

NodeBase::~NodeBase(void) {}

//////////////////////////////////////////////////////////////////////////
// generator_holder implementations
//////////////////////////////////////////////////////////////////////////
NodeBase::generator_holder::generator_holder(void)
	: assigned(false) {}

NodeBase::generator_holder::~generator_holder(void) {}

node_value_t NodeBase::generator_holder::operator()(node_info_t info) const {
	return this->generator(info);
}

template <>
NodeBase::generator_holder& NodeBase::generator_holder::operator=<std::string>(const std::string& data) {
	NodeString nodeString(data);
	this->generator = [=](node_info_t info) {
		return nodeString.toJS(info);
	};

	return (*this);
}

template <>
NodeBase::generator_holder& NodeBase::generator_holder::operator=<TagLib::String>(const TagLib::String& data) {
	NodeString nodeString(code_cvt_helper::toUtf8(data), true);
	this->generator = [=](node_info_t info) {
		return nodeString.toJS(info);
	};

	return (*this);
}
template <>
NodeBase::generator_holder& NodeBase::generator_holder::operator=<TagLib::String>(TagLib::String&& data) {
	return *this = data;
}

template <>
NodeBase::generator_holder& NodeBase::generator_holder::operator=<const char*>(const char* data) {
	NodeString nodeString(data);
	this->generator = [=](node_info_t info) {
		return nodeString.toJS(info);
	};

	return (*this);
}

template <>
NodeBase::generator_holder& NodeBase::generator_holder::operator=<char>(char data) {
	char buffer[2] = { data, 0 };

	NodeString nodeString(buffer);
	this->generator = [=](node_info_t info) {
		return nodeString.toJS(info);
	};

	return (*this);
}

#define SPECIALIZATION_NODE_OBJECT(type) \
	template <> \
	NodeBase::generator_holder& NodeBase::generator_holder::operator=<type>(const type& data) { \
		this->generator = [=](node_info_t info) { \
			return data.toJS(info); \
		}; \
	 \
		return (*this); \
	} \
	template <>	NodeBase::generator_holder& NodeBase::generator_holder::operator=<type>(type&& data) { return *this = data; }

SPECIALIZATION_NODE_OBJECT(NodeObject);
SPECIALIZATION_NODE_OBJECT(NodeString);
SPECIALIZATION_NODE_OBJECT(NodeBuffer<char>);