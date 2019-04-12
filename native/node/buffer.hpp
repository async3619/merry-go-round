#ifndef MERRY_GO_ROUND_BUFFER_HPP
#define MERRY_GO_ROUND_BUFFER_HPP

template <class T>
class NodeBuffer : public NodeBase
{
private:
	DEFINE_TYPE(NodeBuffer<T>);

private:
	NodeBuffer(void) = delete;

public:
	NodeBuffer(std::size_t length) :
		data(new T[length]), length(length) {}

	NodeBuffer(const T* buffer, std::size_t length) :
		data(new T[length]), length(length) {
		std::memcpy(this->data, buffer, sizeof(T) * length);
	}

	NodeBuffer(const NodeBuffer<T>& buffer) :
		data(new T[buffer.length]), length(buffer.length) {
		std::memcpy(this->data, buffer.data, sizeof(T) * buffer.length);
	}

	NodeBuffer(NodeBuffer<T>&& buffer) :
		data(new T[buffer.length]), length(buffer.length) {
		std::memcpy(this->data, buffer.data, sizeof(T) * buffer.length);
	}

	~NodeBuffer(void) {
		delete[] this->data;
	}

public:
	const T* get(void) const {
		return this->data;
	}
	T* get(void) {
		return this->data;
	}

public:
	NodeBuffer<T>& operator=(const NodeBuffer<T>& rhs) {
		this->data = new T[rhs.length];
		this->length = rhs.length;

		std::memcpy(this->data, rhs.data, sizeof(T) * rhs.length);

		return (*this);
	}
	NodeBuffer<T>& operator=(const TagLib::ByteVector& rhs) {
		this->length = rhs.size();
		this->data = new T[this->length];

		std::memcpy(this->data, rhs.data(), sizeof(T) * this->length);

		return (*this);
	}

public:
	virtual node_value_t toJS(node_info_t info) const override {
		return node_buffer_t<T>::New(NODE_ENV, this->data, this->length);
	}

private:
	T* data;
	std::size_t length;
};

template <typename T>
std::size_t NodeBuffer<T>::__id = reinterpret_cast<std::size_t>(&NodeBuffer<T>::__id);

#endif // MERRY_GO_ROUND_BUFFER_HPP
