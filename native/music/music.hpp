#ifndef MERRY_GO_ROUND_MUSIC_HPP
#define MERRY_GO_ROUND_MUSIC_HPP

class Music : public Napi::ObjectWrap<Music>, Releasable
{
private:
	static Napi::FunctionReference constructor;
	static collector_t<Music> collector;

public:
	static Napi::Object initialize(Napi::Env env, Napi::Object exports);
	static void finalize(void);

public:
	static node_any_t loadFromFile(node_info_t info);

public:
	Music(node_info_t info);
	~Music(void);

private:
	void constructFromFile(const node_string_t& string);

public:
	node_value_t title(node_info_t info);
	node_value_t artist(node_info_t info);
	node_value_t album(node_info_t info);
	node_value_t genre(node_info_t info);
	node_value_t year(node_info_t info);
	node_value_t track(node_info_t info);
	node_value_t tagType(node_info_t info);
	node_value_t nativeData(node_info_t info);

private:
	void resolve(void);

private:
	virtual void release(void) override;

private:
	MusicInternal* musicInternal;
	TagLib::FileRef* fileRef;
	TagLib::Tag* tag;
};

#endif // MERRY_GO_ROUND_MUSIC_HPP
