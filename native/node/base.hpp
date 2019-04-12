#ifndef MERRY_GO_ROUND_BASE_HPP
#define MERRY_GO_ROUND_BASE_HPP

typedef std::function<node_value_t(node_info_t info)> generator_t;

template <typename T>
constexpr bool is_number = std::is_arithmetic_v<T> && !std::is_same_v<T, bool> && !std::is_same_v<T, char>;

class NodeBase 
{
protected:
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

		template <typename T, typename std::enable_if_t<(!is_number<T>) && std::is_class_v<T>>* = nullptr>
		generator_holder& operator=(const T& data);

		template <typename T, typename std::enable_if_t<(!is_number<T>) && std::is_class_v<T>>* = nullptr>
		generator_holder& operator=(T&& data);

		template <typename T, typename std::enable_if_t<(!is_number<T>) && (!std::is_class_v<T>)>* = nullptr>
		generator_holder& operator=(T data);

		template <typename T, typename std::enable_if_t<is_number<T>>* = nullptr>
		generator_holder& operator=(T data) {
			this->generator = [=](node_info_t info) {
				return node_number_t::New(NODE_ENV, data);
			};

			return (*this);
		}

	private:
		generator_t generator;
		bool assigned;
	};


protected:
	NodeBase(void) {};
	virtual ~NodeBase(void) = 0;

public:
	virtual node_value_t toJS(node_info_t info) const = 0;
};

#endif // MERRY_GO_ROUND_BASE_HPP
