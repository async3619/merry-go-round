#ifndef MERRY_GO_ROUND_FILE_HPP
#define MERRY_GO_ROUND_FILE_HPP

class File 
{
protected:
	File(void) = delete;
	File(const std::string& path);
	virtual ~File(void) = 0;

public:
	virtual bool opened(void) const = 0;
	virtual std::int64_t size(void) const = 0;

public:
	virtual bool open(void) = 0;
	virtual bool close(void) = 0;
	virtual bool read(char* buffer, std::size_t length, std::size_t begin = 0) = 0;
	
protected:
	const std::string path;
};

#	if defined(_MERRY_GO_ROUND_USE_POSIX)
#		include "file.posix.hpp"
using PlatformFile = PosixFile;
#	elif defined(_MERRY_GO_ROUND_USE_WIN32)
#		include "file.win32.hpp"
using PlatformFile = Win32File;
#	endif
#endif // MERRY_GO_ROUND_FILE_HPP
