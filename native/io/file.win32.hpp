#ifndef MERRY_GO_ROUND_FILE_WIN32_HPP
#define MERRY_GO_ROUND_FILE_WIN32_HPP
#	if defined(_MERRY_GO_ROUND_USE_WIN32)

class Win32File : public File
{
public:
	Win32File(const std::string& path);
	~Win32File(void);

public:
	virtual bool opened(void) const override;
	virtual std::int64_t size(void) const override;

public:
	virtual bool open(void) override;
	virtual bool close(void) override;
	virtual bool read(char* buffer, std::size_t length, std::size_t begin = 0) override;

private:
	HANDLE file;
};

#	endif
#endif // MERRY_GO_ROUND_FILE.WIN32_HPP
