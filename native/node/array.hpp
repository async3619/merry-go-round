#ifndef MERRY_GO_ROUND_ARRAY_HPP
#define MERRY_GO_ROUND_ARRAY_HPP

class NodeArray : public NodeBase
{
private:
	DEFINE_TYPE(NodeArray);

public:
	NodeArray(void);
	~NodeArray(void);

public:
	template <typename T>
	void push_back(const T& value) {
		generator_holder holder;
		holder = value;

		this->items.push_back(holder);
	}

	template <typename T>
	void push_back(T&& value) {
		generator_holder holder;
		holder = value;

		this->items.push_back(holder);
	}

	void push_back(generator_holder&& holder) {
		this->items.push_back(holder);
	}

public:
	virtual node_value_t toJS(node_info_t info) const override;

private:
	std::vector<generator_holder> items;
};

#endif // MERRY_GO_ROUND_ARRAY_HPP
