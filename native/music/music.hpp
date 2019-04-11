#ifndef MERRY_GO_ROUND_MUSIC_HPP
#define MERRY_GO_ROUND_MUSIC_HPP

class Music : public Releasable, Object
{
private:
	DISALLOW_COPY_AND_ASSIGN(Music);
	Music(void) = delete;

public:
	enum Type {
		Unknown = 0,
		MPEG = 1,
		Ogg_Vorbis,
		Ogg_FLAC,
		FLAC,
		MPC,
		WavPack,
		Ogg_Speex,
		Ogg_Opus,
		TrueAudio,
		MP4,
		ASF,
		RIFF_AIFF,
		RIFF_WAV,
		APE,
		DSDIFF,
		DSF
	};

public:
	static Music* create(std::string path);

private:
	Music(const std::string& path);
	~Music(void);

public:
	node_string_t title(void) const;
	node_string_t artist(void) const;
	node_string_t album(void) const;
	node_string_t genre(void) const;
	unsigned int year(void) const;
	unsigned int track(void) const;

public:
	const char* type(void);
	std::string tagType(void);
	native_data_t nativeData(void);

private:
	void retrieve(void);

public:
	virtual void release(void) override;

private:
	bool retrieved;
	TagLib::FileStream stream;
	std::string filePath;
	TagLib::FileRef* file;
	TagLib::Tag* tag;

	// type related
	Type fileType;
	std::string tagTypeData;

	// internal
	MusicInternal* musicInternal;
};

#endif // MERRY_GO_ROUND_MUSIC_HPP
