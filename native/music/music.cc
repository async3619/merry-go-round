#include "../includes.hpp"

struct null_internal_t {
	null_internal_t(const TagLib::FileRef* ref) {
		throw std::logic_error("not implemented");
	}
};

using supported_file_types = boost::mpl::vector<
	TagLib::MPEG::File,
	TagLib::Ogg::Vorbis::File,
	TagLib::Ogg::FLAC::File,
	TagLib::FLAC::File,
	TagLib::MPC::File,
	TagLib::WavPack::File,
	TagLib::Ogg::Speex::File,
	TagLib::Ogg::Opus::File,
	TagLib::TrueAudio::File,
	TagLib::MP4::File,
	TagLib::ASF::File,
	TagLib::RIFF::AIFF::File,
	TagLib::RIFF::WAV::File,
	TagLib::APE::File,
	TagLib::DSDIFF::File,
	TagLib::DSF::File
>;

using music_internal_dictionary = boost::mpl::map<
	boost::mpl::pair<TagLib::MPEG::File, MPEGMusicInternal>,
	boost::mpl::pair<TagLib::Ogg::Vorbis::File, null_internal_t>,
	boost::mpl::pair<TagLib::Ogg::FLAC::File, null_internal_t>,
	boost::mpl::pair<TagLib::FLAC::File, null_internal_t>,
	boost::mpl::pair<TagLib::MPC::File, null_internal_t>,
	boost::mpl::pair<TagLib::WavPack::File, null_internal_t>,
	boost::mpl::pair<TagLib::Ogg::Speex::File, null_internal_t>,
	boost::mpl::pair<TagLib::Ogg::Opus::File, null_internal_t>,
	boost::mpl::pair<TagLib::TrueAudio::File, null_internal_t>,
	boost::mpl::pair<TagLib::MP4::File, null_internal_t>,
	boost::mpl::pair<TagLib::ASF::File, null_internal_t>,
	boost::mpl::pair<TagLib::RIFF::AIFF::File, null_internal_t>,
	boost::mpl::pair<TagLib::RIFF::WAV::File, null_internal_t>,
	boost::mpl::pair<TagLib::APE::File, null_internal_t>,
	boost::mpl::pair<TagLib::DSDIFF::File, null_internal_t>,
	boost::mpl::pair<TagLib::DSF::File, null_internal_t>
>;

Napi::FunctionReference Music::constructor;
collector_t<Music> Music::collector([](Music* music) {
	music->release();
});

Napi::Object Music::initialize(Napi::Env env, Napi::Object exports) {
	Napi::HandleScope scope(env);

	Napi::Function func = DefineClass(env, "Music", {
		InstanceMethod("title", &Music::title),
		InstanceMethod("artist", &Music::artist),
		InstanceMethod("album", &Music::album),
		InstanceMethod("genre", &Music::genre),
		InstanceMethod("year", &Music::year),
		InstanceMethod("track", &Music::track),
		InstanceMethod("tagType", &Music::tagType),
		InstanceMethod("nativeData", &Music::nativeData),
		InstanceMethod("fileType", &Music::fileType),
	});

	Music::constructor = Napi::Persistent(func);
	Music::constructor.SuppressDestruct();

	exports.Set("loadFromFile", Napi::Function::New(env, &Music::loadFromFile, "loadFromFile"));
	return exports;
}

void Music::finalize(void) {
	Music::collector.release();
}

node_any_t Music::loadFromFile(node_info_t info) {
	Napi::EscapableHandleScope scope(info.Env());

	node_string_t filePath = info[0].As<node_string_t>();
	Napi::Object music = Music::constructor.New({ filePath });

	return scope.Escape(napi_value(music)).ToObject();
}

Music::Music(node_info_t info) : 
	Napi::ObjectWrap<Music>(info), fileRef(nullptr), musicInternal(nullptr), resolved(false) {
	if (info.Length() <= 0) {
		throw std::invalid_argument("");
	}

	if (info[0].IsString()) {
		this->constructFromFile(info[0].As<node_string_t>());
	}

	Music::collector[this] = this;
}
Music::~Music(void) {}

void Music::constructFromFile(const node_string_t& string) {
	std::string&& cString = string.Utf8Value();
	
	auto* filePath = unmanaged_utf8_to_multibyte_t::convert(cString.c_str());

	boost::filesystem::path path(filePath);
	if (boost::filesystem::exists(path) == false) {
		throw std::logic_error("given file path doesn't exists");
	}

	this->fileRef = new TagLib::FileRef(filePath);
	this->tag = this->fileRef->tag();
}

node_value_t Music::title(node_info_t info) {
	return node_string_t::New(NODE_ENV, TO_UTF8(this->tag->title()));
}
node_value_t Music::artist(node_info_t info) {
	return node_string_t::New(NODE_ENV, TO_UTF8(this->tag->artist()));
}
node_value_t Music::album(node_info_t info) {
	return node_string_t::New(NODE_ENV, TO_UTF8(this->tag->album()));
}
node_value_t Music::genre(node_info_t info) {
	return node_string_t::New(NODE_ENV, TO_UTF8(this->tag->genre()));
}
node_value_t Music::year(node_info_t info) {
	return node_number_t::New(NODE_ENV, this->tag->year());
}
node_value_t Music::track(node_info_t info) {
	return node_number_t::New(NODE_ENV, this->tag->track());
}
node_value_t Music::tagType(node_info_t info) {
	if (!this->resolved)
		this->resolve();
	
	const NodeString& type = this->musicInternal->tagType();
	return type.toJS(info);
}
node_value_t Music::nativeData(node_info_t info) {
	if (!this->resolved)
		this->resolve();

	const NodeObject& data = this->musicInternal->nativeData();
	return data.toJS(info);
}
node_value_t Music::fileType(node_info_t info) {
	if (!this->resolved)
		this->resolve();

	return this->fileTypeName.toJS(info);
}

struct type_finder {
	template <typename T>
	void operator()(boost::type<T>) {
		if (T::isSupported(this->stream)) {
			using target_type = boost::mpl::at<music_internal_dictionary, T>::type;
			auto* t = new boost::mpl::at<music_internal_dictionary, T>::type(file);

			this->callback(reinterpret_cast<MusicInternal*>(t), this->index);
		}

		++this->index;
	}

	std::function<void(MusicInternal*, std::size_t)> callback;
	const TagLib::FileRef* file;
	TagLib::IOStream* stream;
	std::size_t index;
};

void Music::resolve(void) {
	std::vector<std::string> fileTypes = { 
		"MPEG",	"Ogg::Vorbis", "Ogg::FLAC",	"FLAC",
		"MPC", "WavPack", "Ogg::Speex", "Ogg::Opus",
		"TrueAudio", "MP4", "ASF", "RIFF::AIFF", 
		"RIFF::WAV", "APE",	"DSDIFF", "DSF",
	};

	TagLib::File* file = this->fileRef->file();
	std::size_t fileSize = file->length();
	TagLib::ByteVector&& byteVector = file->readBlock(fileSize);
	TagLib::ByteVectorStream stream(byteVector);

	type_finder finder;
	finder.index = 0;
	finder.stream = &stream;
	finder.file = this->fileRef;
	finder.callback = [this, &fileTypes](MusicInternal* result, std::size_t index) {
		this->musicInternal = result;
		this->fileTypeName = fileTypes[index];
	};

	boost::mpl::for_each<supported_file_types, boost::type<boost::mpl::_1>>(finder);

	this->resolved = true;
}

void Music::release(void) {
	if (this->fileRef)
		delete this->fileRef;

	if (this->musicInternal)
		delete this->musicInternal;
}
