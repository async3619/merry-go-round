#ifndef MERRY_GO_ROUND_RELEASABLE_HPP
#define MERRY_GO_ROUND_RELEASABLE_HPP

class Releasable
{
protected:
	Releasable(void) = default;
	~Releasable(void) = default;

public:
	virtual void release(void) = 0;
};

#endif // MERRY_GO_ROUND_RELEASABLE_HPP
