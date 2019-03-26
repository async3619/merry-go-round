#include "includes.hpp"

Music* Music::create(void)
{
	return nullptr;
}

Music::Music(void)
{
}
Music::~Music(void)
{
}

NBIND_CLASS(Music) {
	method(create);
}
