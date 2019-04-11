#ifndef MERRY_GO_ROUND_INTERNAL_HPP
#define MERRY_GO_ROUND_INTERNAL_HPP

typedef std::vector<std::vector<node_string_t>> native_data_t;

class MusicInternal : public Object
{
protected:
	MusicInternal(void) = delete;
	MusicInternal(MusicInternal&) = delete;
	MusicInternal(TagLib::File*);

public:
	virtual ~MusicInternal(void) = 0;

public:
	virtual node_string_t tagType(void) = 0;
	virtual native_data_t nativeData(void) = 0;
};

#endif // MERRY_GO_ROUND_INTERNAL_HPP
