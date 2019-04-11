#include "includes.hpp"

Object::Object(void) : 
	notifier(StringManager::getInstance().registerObject(this)) { }
Object::~Object(void) {
	this->notifier(this);
}
