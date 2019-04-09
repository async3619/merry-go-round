#ifndef MERRY_GO_ROUND_DEBUG_HPP
#define MERRY_GO_ROUND_DEBUG_HPP

#define DEBUG_LOG(x) std::cout << __FILE__ << "(" << __LINE__ << "): " << x << std::endl

inline std::ostream& operator << (std::ostream& s, const wchar_t*& str) {
	std::wcout << str;
	return s;
}

inline std::ostream& operator << (std::ostream& s, wchar_t*& str) {
	std::wcout << str;
	return s;
}

#endif // MERRY_GO_ROUND_DEBUG_HPP
