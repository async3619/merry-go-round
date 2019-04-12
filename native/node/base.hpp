#ifndef MERRY_GO_ROUND_BASE_HPP
#define MERRY_GO_ROUND_BASE_HPP

typedef std::function<node_value_t(node_info_t info)> generator_t;

template <typename T>
constexpr bool is_number = std::is_arithmetic_v<T> && !std::is_same_v<T, bool> && !std::is_same_v<T, char>;

class NodeBase 
{
public:
	class generator_holder 
	{
	public:
		generator_holder(void);
		generator_holder(const generator_holder& holder) :
			generator(holder.generator), assigned(true) {}
		generator_holder(generator_holder&& holder) : 
			generator(holder.generator), assigned(true) {}
		~generator_holder(void);

	public:
		bool isAssigned(void) {
			return this->assigned;
		}

	public:
		node_value_t operator()(node_info_t info) const;

		generator_holder& operator=(const generator_holder& data) {
			this->generator = data.generator;
			this->assigned = true;
			return (*this);
		}
		generator_holder& operator=(generator_holder&& data) {
			return this->operator=(data);
		}

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
	virtual std::size_t type(void) const = 0;
	virtual node_value_t toJS(node_info_t info) const = 0;
};

#define DEFINE_TYPE(type_name) \
	private: \
		static std::size_t __id; \
	public: \
		static std::size_t typeId() { return type_name::__id; } \
		virtual std::size_t type() const override { return type_name::__id; } \
		template <typename T> \
		bool instanceof(void) { return type_name::__id == T::typeId(); }

#define DECLARE_TYPE(type_name) \
	std::size_t type_name::__id = reinterpret_cast<std::size_t>(&type_name::__id);

#endif // MERRY_GO_ROUND_BASE_HPP
