#include "../includes.hpp"
#if defined(_MERRY_GO_ROUND_USE_POSIX)

PosixFile::PosixFile(const std::string& path) :
	File(path), file(-1) {}
PosixFile::~PosixFile(void) {}

bool PosixFile::opened(void) const {
	return this->file != -1;
}

std::int64_t PosixFile::size(void) const {
	struct stat st;
	stat(this->path.c_str(), &st);

	return st.st_size;
}

bool PosixFile::open(void) {
	this->file = ::open(this->path.c_str(), O_RDONLY);
	if (this->file == -1) {
		return false;
	}

	return true;
}
bool PosixFile::close(void) {
	if (::close(this->file) == -1) {
		return false;
	}

	return true;
}
bool PosixFile::read(char* buffer, std::size_t length, std::size_t begin /*= 0*/) {
	if (::lseek(this->file, begin, SEEK_SET) == -1) {
		return false;
	}

	if (::read(this->file, buffer, length) == -1) {
		return false;
	}

	return true;
}

#endif
