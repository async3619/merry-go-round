#include "../includes.hpp"

#if defined(_DEBUG)
#	undef new
char MemoryManager::MemoryMap::buffer[4096];

MemoryManager::MemoryMap::MemoryMap(void* pointer, std::size_t size, char const* path, int line) :
	pointer(pointer), size(size), path(path), line(line) { }

void MemoryManager::MemoryMap::tryDump(void) const {
	std::sprintf(MemoryManager::MemoryMap::buffer, "Memory leak detected!\n\tallocated %zd bytes at heap but not deallocated.\n\tcheck out code at %s(%d)\n", size, path, line);
	std::printf(MemoryManager::MemoryMap::buffer);
}

MemoryManager* MemoryManager::instance = nullptr;
bool MemoryManager::isFianlized = false;

MemoryManager* MemoryManager::getInstance(void) {
	if (MemoryManager::instance == nullptr && MemoryManager::isFianlized == false) {
		instance = new MemoryManager();
	}

	return instance;
}

MemoryManager::MemoryManager(void) {}
MemoryManager::~MemoryManager(void) {
	MemoryManager::isFianlized = true;
	this->checkMemoryLeak();
}

void* MemoryManager::alloc(std::size_t size, char const* path, int line) noexcept(false) {
	// allocate first
	void* allocatedPointer = malloc(size);
	if (!allocatedPointer) throw std::bad_alloc();

	this->memoryDictionary.emplace(std::piecewise_construct, std::forward_as_tuple(allocatedPointer), std::forward_as_tuple(allocatedPointer, size, path, line));

	return allocatedPointer;
}
void MemoryManager::free(void* pointer) throw() {
	if (MemoryManager::isFianlized) {
		::free(pointer);
		return;
	}
	
	if (!pointer || this->memoryDictionary.find(pointer) == this->memoryDictionary.cend()) return;
	::free(pointer);

	if (!MemoryManager::isFianlized)
		this->memoryDictionary.erase(pointer);
}

void MemoryManager::checkMemoryLeak(void) {
	for (const auto& item : this->memoryDictionary) {
		item.second.tryDump();
	}

#if defined(_WIN32)
	if (this->memoryDictionary.size() > 0) {
		DebugBreak();
	}
#endif
}

void MemoryManager::release(void) {
	delete this;
}

#	define new new(__FILE__, __LINE__)
#endif
