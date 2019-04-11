#include "../includes.hpp"

BufferInformation::BufferInformation(const byte_t* data, std::size_t length) :
	data(data), length(length) { }

BufferManager& BufferManager::getInstance(void) {
	static BufferManager instance;
	return instance;
}

BufferManager::BufferManager(void) {
	char buf[4] = { 0, };
	for (int i = 0; i < 256; ++i) {
		sprintf(buf, "%x", i + 0x100);
        this->byteToHex.emplace_back(&buf[1]);
	}
}
BufferManager::~BufferManager(void) {
	for (auto& item : this->bufferMap) {
		delete[] item.second.data;
	}
}

std::string BufferManager::generateUUID(void) {
	// generate random bytes
	boost::random::uniform_int_distribution<> dist(0, 0xFF);

	auto* bytes = new byte_t[16];
	for (int i = 0; i < 16; ++i) {
		bytes[i] = dist(this->randomByteGenerator);
	}

	// Per 4.4, set bits for version and `clock_seq_hi_and_reserved`
	bytes[6] = (bytes[6] & 0x0F) | 0x40;
	bytes[8] = (bytes[8] & 0x3F) | 0x80;

	char result[256] = { 0, };
	for (std::size_t i = 0, j = 16; i < j; ++i) {
		bool needHyphen = i == 3 || i == 5 || i == 7 || i == 9;
		sprintf(result, "%s%s%s", result, this->byteToHex[bytes[i]].c_str(), needHyphen ? "-" : "");
	}

	delete[] bytes;
	return result;
}

std::string BufferManager::reserve(const byte_t* bytes, std::size_t length) {
	std::string key = this->generateUUID();

	auto* buffer = new byte_t[length];
	memcpy(buffer, bytes, length);

	this->bufferMap.emplace(key, BufferInformation(buffer, length));

	return key;
}

std::string BufferManager::reserve(const char* bytes, std::size_t length) {
	return this->reserve(reinterpret_cast<const byte_t*>(bytes), length);
}

void BufferManager::readReservedBuffer(std::string key, nbind::Buffer targetBuffer) {
	const auto& item = this->bufferMap.find(key);
	if (item == this->bufferMap.end()) {
		NBIND_ERR("given key doesn't exist in table.");
		return;
	}

	auto& bufferInformation = item->second;

	std::memcpy(targetBuffer.data(), bufferInformation.data, bufferInformation.length);
}

#include "../nbind.hpp"

NBIND_CLASS(BufferManager) {
	method(getInstance);
	method(readReservedBuffer);
}