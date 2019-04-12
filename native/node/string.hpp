#ifndef MERRY_GO_ROUND_STRING_HPP
#define MERRY_GO_ROUND_STRING_HPP

class NodeString : public NodeBase, std::string
{
public:
	NodeString(void);
	NodeString(const NodeString& str);
	NodeString(NodeString&& str);
	NodeString(const char* str, bool encoded = false);
	NodeString(const std::string& str);

public:
	NodeString& operator=(const NodeString& rhs);

public:
	virtual node_value_t toJS(node_info_t info) const override;

private:
	bool encoded;
};

#endif // MERRY_GO_ROUND_STRING_HPP
