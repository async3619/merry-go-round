#include "../includes.hpp"

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
	Napi::ObjectWrap<Music>(info), fileRef(nullptr) {
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

void Music::release(void) {
	delete this->fileRef;
}
