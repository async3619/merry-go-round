#ifndef MERRY_GO_ROUND_BASE_HPP
#define MERRY_GO_ROUND_BASE_HPP

template <typename NodeData>
class NodeBase 
{
protected:
	NodeBase(void) {};
	virtual ~NodeBase(void) = 0;

public:
	virtual NodeData toJS(node_info_t info) const = 0;
};

template <typename NodeData>
NodeBase<NodeData>::~NodeBase(void) {}

#endif // MERRY_GO_ROUND_BASE_HPP
