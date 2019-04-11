#include "../includes.hpp"

IMPLEMENT_SINGLETON(StringManager);

bool StringManager::isFinalized = false;

StringManager::StringManager(void) {
	DEBUG_LOG("StringManager Constructed");
}
StringManager::~StringManager(void) {
	for (auto& pair : this->dictionary) {
		std::for_each(pair.second.cbegin(), pair.second.cend(), [](node_string_t string) {
			delete[] string;
		});
	}

	StringManager::isFinalized = true;

	this->dictionary.clear();
	DEBUG_LOG("StringManager Destructed");
}

object_notfier_t StringManager::registerObject(const Object* object) {
	auto iter = this->dictionary.find(object);
	if (iter != this->dictionary.cend()) {
		throw std::logic_error("you cannot register object twice!");
	}

	this->dictionary.insert(std::make_pair(object, std::vector<node_string_t>()));
	return std::bind(&StringManager::onObjectDestruct, this, std::placeholders::_1);
}
char* StringManager::allocateString(const Object* object, const char* original) {
	auto iter = this->dictionary.find(object);
	if (iter == this->dictionary.cend()) {
		throw std::logic_error("you should register object first!");
	}

	// determine original string length by position of null character
	std::size_t length = 0;
	while (true) {
		if (original[length] == '\0') break;
		++length;
	}

	auto* buffer = new char[length + 1];
	std::memcpy(buffer, original, length + 1);

	iter->second.push_back(buffer);
	return buffer;
}

void StringManager::onObjectDestruct(const Object* object) {
	if (StringManager::isFinalized == true) return;

	auto iter = this->dictionary.find(object);
	if (iter == this->dictionary.cend())
		return;

	// destruct all the heap memories that allocated by caller object.
	const auto& stringMap = iter->second;
	for (node_string_t string : stringMap) {
		delete[] string;
	}

	this->dictionary.erase(object);
}