#include "includes.hpp"

Music* Music::create(std::string path) {
	static Dictionary<std::string, Music*> dictionary;
	CONV_ARGUMENT_ENCODING(path);

	if (dictionary.exists(path)) {
		return dictionary[path];
	}

	auto* music = new Music(path);
	if (music->file.isNull()) {
		delete music;
		return nullptr;
	}

	dictionary[path] = music;
	return music;
}

Music::Music(const std::string& path) :
	path(path), file(path.c_str()), tag(nullptr) {
	this->tag = file.tag();
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

void Music::release(void) {
	delete this;
}

NBIND_CLASS(Music) {
	method(create);
	method(title);
	method(artist);
	method(album);
	method(genre);
	method(year);
	method(track);
}
