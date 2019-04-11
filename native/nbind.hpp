#if defined(new) && defined(_DEBUG)
#	undef new
#endif

#include <nbind/nbind.h>

#if defined(_DEBUG)
#	define new new(__FILE__, __LINE__)
#endif