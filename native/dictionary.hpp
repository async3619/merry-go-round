#ifndef MERRY_GO_ROUND_DICTIONARY_HPP
#define MERRY_GO_ROUND_DICTIONARY_HPP

template <typename TKey, typename TValue>
class Dictionary
{
private:
	DISALLOW_COPY_AND_ASSIGN(Dictionary);

public:
	Dictionary(void) = default;
	~Dictionary(void) {
		if (!std::is_pointer<TValue>::value)
			return;

		for (auto iter = this->dictionaryBase.begin(); iter != this->dictionaryBase.end(); ++iter) {
			iter->second->release();
		}
	}

public:
	bool exists(const TKey& key) {
		return this->dictionaryBase.find(key) != this->dictionaryBase.end();
	}

public:
	TValue& operator[] (const TKey& key) {
		return this->dictionaryBase[key];
	}

private:
	std::unordered_map<TKey, TValue> dictionaryBase;
};

#endif // MERRY_GO_ROUND_DICTIONARY_HPP
