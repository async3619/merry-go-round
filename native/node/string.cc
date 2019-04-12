#include "../includes.hpp"

NodeString::NodeString(void) :
	encoded(false) {}
NodeString::NodeString(const char* str, bool encoded) :
	std::string(str), encoded(encoded) {}
NodeString::NodeString(const std::string& str) :
	std::string(str), encoded(false) {}
NodeString::NodeString(const NodeString& str) :
	std::string(str), encoded(str.encoded) {}
NodeString::NodeString(NodeString&& str) :
	std::string(str), encoded(str.encoded) {}

NodeString& NodeString::operator=(const NodeString& rhs) {
	std::string& lhs = (*this);
	lhs = rhs;

	this->encoded = rhs.encoded;
	return (*this);
}

node_value_t NodeString::toJS(node_info_t info) const {
	const char* utf8String = this->encoded ? this->c_str() : unmanaged_multibyte_to_utf8_t::convert(this->c_str());
	return node_string_t::New(NODE_ENV, utf8String);
}
