#include "../includes.hpp"
#if defined(_MERRY_GO_ROUND_USE_WIN32)

Win32File::Win32File(const std::string& path) :
	File(path), file(INVALID_HANDLE_VALUE) {}
Win32File::~Win32File(void) {
	this->close();
}

bool Win32File::opened(void) const {
	return file != INVALID_HANDLE_VALUE;
}
std::int64_t Win32File::size(void) const {
	HANDLE fileHandle = CreateFile(this->path.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fileHandle == INVALID_HANDLE_VALUE) {
		return -1;
	}

	LARGE_INTEGER size;
	if (!GetFileSizeEx(fileHandle, &size)) {
		CloseHandle(fileHandle);
		return -1;
	}

	CloseHandle(fileHandle);
	return size.QuadPart;
}

bool Win32File::open(void) {
	this->file = CreateFile(this->path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE) {
		return false;
	}

	return true;
}
bool Win32File::close(void) {
	if (!this->opened()) {
		return false;
	}

	if (CloseHandle(this->file) == FALSE) {
		return false;
	}

	return true;
}
bool Win32File::read(char* buffer, std::size_t length, std::size_t begin /*= 0*/) {
	if (!this->opened()) {
		return false;
	}

	BOOL result = SetFilePointer(this->file, begin, NULL, FILE_BEGIN);
	if (result == INVALID_SET_FILE_POINTER) {
		return false;
	}

	DWORD byteRead = 0;
	result = ReadFile(this->file, buffer, length, &byteRead, NULL);
	if (!result) {
		return false;
	}

	return true;
}

#endif