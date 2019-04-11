#ifndef MERRY_GO_ROUND_OBJECT_HPP
#define MERRY_GO_ROUND_OBJECT_HPP

// Interface
class Object 
{
protected:
	Object(void);
	virtual ~Object(void) = 0;

private:
	object_notfier_t notifier;
};

#endif // MERRY_GO_ROUND_OBJECT_HPP
