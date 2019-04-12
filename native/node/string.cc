#include "../includes.hpp"

NodeString::NodeString(void) {}
NodeString::NodeString(const char* str) :
	std::string(str) {}
NodeString::NodeString(const std::string& str) :
	std::string(str) {}
NodeString::NodeString(const NodeString& str) :
	std::string(str) {}
NodeString::NodeString(NodeString&& str) :
	std::string(str) {}

NodeString& NodeString::operator=(const NodeString& rhs) {
	std::string& lhs = (*this);
	lhs = rhs;

	return (*this);
}

node_string_t NodeString::toJS(node_info_t info) const {
	const char* utf8String = unmanaged_multibyte_to_utf8_t::convert(this->c_str());
	return node_string_t::New(NODE_ENV, utf8String);
}
