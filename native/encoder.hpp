#ifndef MERRY_GO_ROUND_ENCODER_HPP
#define MERRY_GO_ROUND_ENCODER_HPP

typedef wchar_t wchar;

class Encoder {
public:
	static Encoder& getInstance(void);

public:
	std::string toMultiByte(const std::string& utf8Text);
	std::string toUTF8(const std::string& multibyteText);

private:
	Encoder(void);
	Encoder(const Encoder&) = delete;
	Encoder& operator= (const Encoder&) = delete;

private:
	void allocate(size_t desiredSize);

private:
	std::size_t allocatedSize;
	char* multibyte;
	wchar* unicode;
};

#define TO_UTF8(x) Encoder::getInstance().toUTF8(x)
#define TO_ANSI(x) Encoder::getInstance().toMultiByte(x)
#define CONV_ARGUMENT_ENCODING(x) x = TO_ANSI(x)

#endif // MERRY_GO_ROUND_ENCODER_HPP
