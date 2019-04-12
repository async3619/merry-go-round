#ifndef MERRY_GO_ROUND_ARRAY_HPP
#define MERRY_GO_ROUND_ARRAY_HPP

class NodeArray : public NodeBase
{
public:
	NodeArray(void);
	~NodeArray(void);

public:
	template <typename T>
	void push_back(const T& value) {
		generator_holder holder;
		holder = value;

		this->push_back(holder);
	}

	template <typename T>
	void push_back(T&& value) {
		generator_holder holder;
		holder = value;

		this->push_back(holder);
	}

public:
	virtual node_value_t toJS(node_info_t info) const override;

private:
	std::vector<generator_holder> items;
};

#endif // MERRY_GO_ROUND_ARRAY_HPP
