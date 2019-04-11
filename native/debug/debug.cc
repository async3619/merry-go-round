#include "../includes.hpp"

#if defined(_DEBUG)
#	undef new

void* operator new(size_t size, char const* path, int line) throw(std::bad_alloc) {
	return MemoryManager::getInstance()->alloc(size, path, line);
}
void* operator new[](size_t size, char const* path, int line) throw(std::bad_alloc) {
	return MemoryManager::getInstance()->alloc(size, path, line);
}
void operator delete(void* address) noexcept {
	MemoryManager::getInstance()->free(address);
}
void operator delete[](void* address) noexcept {
	MemoryManager::getInstance()->free(address);
}

#	define new new(__FILE__, __LINE__)
#endif