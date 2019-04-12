#ifndef MERRY_GO_ROUND_DICTIONARY_HPP
#define MERRY_GO_ROUND_DICTIONARY_HPP

template <typename TKey, typename TValue>
class ObjectCollector : public std::unordered_map<TKey, TValue>, Releasable
{
private:
	DISALLOW_COPY_AND_ASSIGN(ObjectCollector);

public:
	template <typename T>
	ObjectCollector(T callback) {
		this->releaser = callback;
	}
	~ObjectCollector(void) {
		this->release();
	}

public:
	virtual void release(void) override {
		for (auto& item : (*this)) {
			this->releaser(item.second);
		}

		this->clear();
	}

private:
	std::function<void(TValue&)> releaser;
};

// I used object pointer as a key. due to environment of node.js addon, all of object will be allocated
// at **heap** so all objects will have their own address number which is unique.
template <typename TObject>
using collector_t = ObjectCollector<TObject*, TObject*>;

#endif // MERRY_GO_ROUND_DICTIONARY_HPP
