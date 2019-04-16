#include "../includes.hpp"
#if defined(_MERRY_GO_ROUND_USE_POSIX)

PosixFile::PosixFile(const std::string& path) :
	File(path), file(nullptr) {}
PosixFile::~PosixFile(void) = default;

bool PosixFile::opened(void) const {
	return this->file != nullptr;
}

std::int64_t PosixFile::size(void) const {
	struct stat st{};
	stat(this->path.c_str(), &st);

	return st.st_size;
}

bool PosixFile::open(void) {
	this->file = std::fopen(this->path.c_str(), "r");
    return this->file != nullptr;

}
bool PosixFile::close(void) {
    return std::fclose(this->file) != EOF;

}
bool PosixFile::read(char* buffer, std::size_t length, std::size_t begin /*= 0*/) {
	if (std::fseek(this->file, begin, SEEK_SET) == -1) {
		return false;
	}

    return std::fread(buffer, sizeof(char), length, this->file) > 0;

}

#endif
