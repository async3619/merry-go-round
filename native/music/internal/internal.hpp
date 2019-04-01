#ifndef MERRY_GO_ROUND_INTERNAL_HPP
#define MERRY_GO_ROUND_INTERNAL_HPP

class MusicInternal 
{
private:
	DISALLOW_COPY_AND_ASSIGN(MusicInternal);
	MusicInternal(void) = delete;

protected:
	MusicInternal(TagLib::File*);
	virtual ~MusicInternal(void) = 0;

public:
	virtual std::string tagType(void) = 0;
};

#endif // MERRY_GO_ROUND_INTERNAL_HPP
