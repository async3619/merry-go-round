#ifndef MERRY_GO_ROUND_DEBUG_HPP
#define MERRY_GO_ROUND_DEBUG_HPP

#if defined(_DEBUG)
#	define DEBUG_LOG(x) std::cout << __FILE__ << "(" << __LINE__ << "): " << x << std::endl
#	include "memory-manager.hpp"

// overload allocation/deallocation operator to find memory leak 
void* operator new(size_t size, char const* path, int line) noexcept(false);
void* operator new[](size_t size, char const* path, int line) noexcept(false);
void operator delete(void* address) noexcept;
void operator delete[](void* address) noexcept;

#	define new new(__FILE__, __LINE__)

inline std::ostream& operator << (std::ostream& s, const wchar_t*& str) {
	std::wcout << str;
	return s;
}

inline std::ostream& operator << (std::ostream& s, wchar_t*& str) {
	std::wcout << str;
	return s;
}
#else
#	define DEBUG_LOG(x)
#	define new new
#endif

#endif // MERRY_GO_ROUND_DEBUG_HPP
