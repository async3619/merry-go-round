#ifndef MERRY_GO_ROUND_MUSIC_HPP
#define MERRY_GO_ROUND_MUSIC_HPP

class Music : public Releasable
{
private:
	DISALLOW_COPY_AND_ASSIGN(Music);
	Music(void) = delete;

public:
	static Music* create(std::string path);

private:
	Music(const std::string& path);
	~Music(void);

public:
	std::string title(void) const;
	std::string artist(void) const;
	std::string album(void) const;
	std::string genre(void) const;
	unsigned int year(void) const;
	unsigned int track(void) const;

public:
	virtual void release(void) override;

private:
	std::string path;
	TagLib::FileRef file;
	TagLib::Tag* tag;
};

#endif // MERRY_GO_ROUND_MUSIC_HPP
