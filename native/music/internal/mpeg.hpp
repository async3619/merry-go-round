#ifndef MERRY_GO_ROUND_MPEG_HPP
#define MERRY_GO_ROUND_MPEG_HPP

class MPEGMusicInternal : public MusicInternal 
{
private:
	DISALLOW_COPY_AND_ASSIGN(MPEGMusicInternal);
	MPEGMusicInternal(void) = delete;

public:
	MPEGMusicInternal(TagLib::File*);
	~MPEGMusicInternal(void);

public:
	virtual std::string tagType(void) override;

private:
	std::string tagTypeName;
	TagLib::MPEG::File* mpegFile;
};

#endif // MERRY_GO_ROUND_MPEG_HPP
