#ifndef MERRY_GO_ROUND_FILE_POSIX_HPP
#define MERRY_GO_ROUND_FILE_POSIX_HPP
#	if defined(_MERRY_GO_ROUND_USE_POSIX)

class PosixFile : public File 
{
public:
	PosixFile(const std::string& path);
	~PosixFile(void);

public:
	virtual bool opened(void) const override;
	virtual std::int64_t size(void) const override;

public:
	virtual bool open(void) override;
	virtual bool close(void) override;
	virtual bool read(char* buffer, std::size_t length, std::size_t begin = 0) override;

private:
	int file;
};

#	endif
#endif // MERRY_GO_ROUND_FILE.POSIX_HPP
