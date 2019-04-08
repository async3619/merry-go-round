#include "../includes.hpp"

const char* FILE_TYPE_STRINGS[] = {
	"Unknown",
	"MPEG",
	"Ogg::Vorbis",
	"Ogg::FLAC",
	"FLAC",
	"MPC",
	"WavPack",
	"Ogg::Speex",
	"Ogg::Opus",
	"TrueAudio",
	"MP4",
	"ASF",
	"RIFF::AIFF",
	"RIFF::WAV",
	"APE",
	"DSDIFF",
	"DSF"
};

Music* Music::create(std::string path) {
	static Dictionary<std::string, Music*> dictionary;
	CONV_ARGUMENT_ENCODING(path);

	if (dictionary.exists(path)) {
		return dictionary[path];
	}

	Music* music = new Music(path);
	if (!music->stream.isOpen() || music->file->isNull()) {
		delete music;
		return nullptr;
	}

	dictionary[path] = music;
	return music;
}

Music::Music(const std::string& path) :
	filePath(path), stream(path.c_str()), file(nullptr), tag(nullptr), 
	retrieved(false), fileType(Unknown), musicInternal(nullptr) {
	this->file = new TagLib::FileRef(&this->stream);
	this->tag = file->tag();
}
Music::~Music(void) {}

std::string Music::title(void) const {
	return TO_UTF8(this->tag->title().toCString());
}
std::string Music::artist(void) const {
	return TO_UTF8(this->tag->artist().toCString());
}
std::string Music::album(void) const {
	return TO_UTF8(this->tag->album().toCString());
}
std::string Music::genre(void) const {
	return TO_UTF8(this->tag->genre().toCString());
}
unsigned int Music::year(void) const {
	return this->tag->year();
}
unsigned int Music::track(void) const {
	return this->tag->track();
}

const char* Music::type(void) {
	if (!this->retrieved) {
		this->retrieve();
	}

	return FILE_TYPE_STRINGS[static_cast<std::size_t>(this->fileType)];
}
std::string Music::tagType(void) {
	if (!this->retrieved) {
		this->retrieve();
	}

	if (this->musicInternal == nullptr) {
		return "Unknown";
	}

	return this->musicInternal->tagType();
}

native_data_t Music::nativeData(void) {
	if (!this->retrieved) {
		this->retrieve();
	}

	return this->musicInternal->nativeData();
}

struct TypeChecker {
	template <typename T>
	TypeChecker(TagLib::FileStream& f, T& callback) : 
		t(Music::MPEG), f(f), found(false), callback(callback) { }

	template <typename U>
	void operator()(boost::type<U>) {
		if (found) 
			return;
		else if (U::isSupported(&this->f)) {
			found = true;
			this->callback(this->t);
		}

		if (this->t != Music::DSF && this->t != Music::Unknown)
			this->t = static_cast<Music::Type>(static_cast<int>(this->t) + 1);
		else
			this->t = Music::Unknown;
	}

	bool found;
	std::function<void(Music::Type)> callback;
	TagLib::FileStream& f;
	Music::Type t;
};

void Music::retrieve(void) {
	using namespace TagLib;
	if (this->retrieved)
		return;

	typedef boost::mpl::list<
		MPEG::File,
		Ogg::Vorbis::File,
		Ogg::FLAC::File,
		FLAC::File,
		MPC::File,
		WavPack::File,
		Ogg::Speex::File,
		Ogg::Opus::File,
		TrueAudio::File,
		MP4::File,
		ASF::File,
		RIFF::AIFF::File,
		RIFF::WAV::File,
		APE::File,
		DSDIFF::File,
		DSF::File
	> supported_file_types;

	TypeChecker checker(this->stream, [&](Music::Type t) {
		this->fileType = t;
	});
	boost::mpl::for_each<supported_file_types, boost::type<boost::mpl::_>>(checker);

	File* f = this->file->file();
	if (this->fileType == Music::MPEG) {
		this->musicInternal = new MPEGMusicInternal(f);
	}

	this->retrieved = true;
}
void Music::release(void) {
	delete this;
}

#include <nbind/nbind.h>

using NativeMusic = Music;

NBIND_CLASS(NativeMusic) {
	method(create);
	method(title);
	method(artist);
	method(album);
	method(genre);
	method(year);
	method(track);
	method(type);
	method(tagType);
	method(nativeData);
}
