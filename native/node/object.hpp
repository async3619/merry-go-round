#ifndef MERRY_GO_ROUND_OBJECT_HPP
#define MERRY_GO_ROUND_OBJECT_HPP

class NodeObject : public NodeBase 
{
private:
	DEFINE_TYPE(NodeObject);

public:
	NodeObject(void);
	~NodeObject(void);

public:
	bool exists(const std::string& key);

public:
	virtual node_value_t toJS(node_info_t info) const override;

public:
	generator_holder& operator[](const std::string& key);

private:
	std::unordered_map<std::string, generator_holder> dictionary;
};

#endif // MERRY_GO_ROUND_OBJECT_HPP
