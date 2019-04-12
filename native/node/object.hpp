#ifndef MERRY_GO_ROUND_OBJECT_HPP
#define MERRY_GO_ROUND_OBJECT_HPP

typedef std::function<node_value_t(node_info_t info)> generator_t;

template <typename T>
using _ = std::conditional_t<std::is_class_v<T>, const T&, T>;

template <typename T>
constexpr bool is_number = std::is_arithmetic_v<T> && !std::is_same_v<T, bool> && !std::is_same_v<T, char>;

class NodeObject : public NodeBase<node_object_t> 
{
private:
	class generator_holder 
	{
	public:
		generator_holder(void);
		~generator_holder(void);

	public:
		bool isAssigned(void) {
			return this->assigned;
		}

	public:
		node_value_t operator()(node_info_t info) const;

		template <typename T, typename Enable = void>
		generator_holder& operator=(_<T> data);

		template <typename T, typename std::enable_if_t<is_number<T>>* = nullptr>
		NodeObject::generator_holder& operator=(T data) {
			this->generator = [=](node_info_t info) {
				return node_number_t::New(NODE_ENV, data);
			};

			return (*this);
		}

	private:
		generator_t generator;
		bool assigned;
	};

public:
	NodeObject(void);
	~NodeObject(void);

public:
	virtual node_object_t toJS(node_info_t info) const override;

public:
	generator_holder& operator[](std::string key);

private:
	std::unordered_map<std::string, generator_holder> dictionary;
};

#endif // MERRY_GO_ROUND_OBJECT_HPP
