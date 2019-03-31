#include "includes.hpp"

#define memzero(x, t) memset(x, 0, this->allocatedSize * sizeof(t))

Encoder& Encoder::getInstance(void) {
	static Encoder instance;
	return instance;
}

Encoder::Encoder(void) : 
	allocatedSize(0), multibyte(nullptr), unicode(nullptr) {
}

std::string Encoder::toMultiByte(const std::string& utf8Text) {
	const char* rawBuffer = utf8Text.c_str();

	int length = MultiByteToWideChar(CP_UTF8, NULL, rawBuffer, strlen(rawBuffer), NULL, NULL);

	this->allocate(length);
	memzero(this->unicode, wchar);
	
	MultiByteToWideChar(CP_UTF8, NULL, rawBuffer, strlen(rawBuffer), this->unicode, length);

	length = WideCharToMultiByte(CP_ACP, NULL, unicode, -1, NULL, NULL, NULL, NULL);

	this->allocate(length);
	memzero(this->multibyte, char);

	WideCharToMultiByte(CP_ACP, 0, this->unicode, -1, this->multibyte, length, NULL, NULL);

	return this->multibyte;
}
std::string Encoder::toUTF8(const std::string& multibyteText)
{
	const char* rawBuffer = multibyteText.c_str();

	int length = MultiByteToWideChar(CP_ACP, 0, rawBuffer, strlen(rawBuffer), NULL, NULL);

	this->allocate(length);
	memzero(this->unicode, wchar);

	MultiByteToWideChar(CP_ACP, 0, rawBuffer, strlen(rawBuffer), this->unicode, length);

	length = WideCharToMultiByte(CP_UTF8, 0, unicode, lstrlenW(unicode), NULL, 0, NULL, NULL);

	this->allocate(length);
	memzero(this->multibyte, char);

	WideCharToMultiByte(CP_UTF8, 0, unicode, lstrlenW(unicode), this->multibyte, length, NULL, NULL);

	return this->multibyte;
}

void Encoder::allocate(size_t desiredSize)
{
	if (desiredSize < this->allocatedSize) 
		return;

	size_t size = 1;
	while (true) {
		size *= 2;
	
		if (desiredSize < size)
			break;
	}

	this->allocatedSize = size;
	this->multibyte = new char[size];
	this->unicode = new wchar[size];
}
