#ifndef MERRY_GO_ROUND_INTERNAL_HPP
#define MERRY_GO_ROUND_INTERNAL_HPP

class NodeString;

class MusicInternal 
{
public:
	MusicInternal(void) = delete;
	MusicInternal(const TagLib::FileRef* /* placeholder for child classes */);
	virtual ~MusicInternal(void) = 0;

public:
	virtual const NodeString& tagType(void) = 0;
};

#endif // MERRY_GO_ROUND_INTERNAL_HPP
