#ifndef MERRY_GO_ROUND_MPEG_HPP
#define MERRY_GO_ROUND_MPEG_HPP

class MPEGMusicInternal : public MusicInternal 
{
public:
	MPEGMusicInternal(void) = delete;
	MPEGMusicInternal(const TagLib::FileRef* file);
	~MPEGMusicInternal(void);

private:
	void initialize(void);
	void initializeId3v2(void);
	void initializeId3v1(void);
	void initializeAPE(void);

public:
	virtual const NodeString& tagType(void) override;
	virtual const NodeObject& nativeData(void) override;

private:
	NodeObject nativeDataStore;
	NodeString tagTypeString;
	TagLib::MPEG::File* file;
};

#endif // MERRY_GO_ROUND_MPEG_HPP
