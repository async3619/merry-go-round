#ifndef MERRY_GO_ROUND_BUFFER_MANAGER_HPP
#define MERRY_GO_ROUND_BUFFER_MANAGER_HPP

typedef unsigned char	byte_t;

struct BufferInformation 
{
public:
	BufferInformation(const byte_t* data, std::size_t length);

	const byte_t* data;
	std::size_t length;
};

class BufferManager
{
private:
	DISALLOW_COPY_AND_ASSIGN(BufferManager);

public:
	static BufferManager& getInstance(void);

private:
	BufferManager(void);
	~BufferManager(void);

private:
	std::string generateUUID(void);

public:
	std::string reserve(const byte_t* bytes, std::size_t length);
	std::string reserve(const char* bytes, std::size_t length);

public:
	void readReservedBuffer(std::string key, nbind::Buffer targetBuffer);

private:
	std::vector<std::string> byteToHex;
	std::unordered_map<std::string, BufferInformation> bufferMap;
	boost::random::mt19937 randomByteGenerator;
};

#endif // MERRY_GO_ROUND_BUFFER_MANAGER_HPP
