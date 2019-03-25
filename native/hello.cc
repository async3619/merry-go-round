#include <string>
#include <iostream>

struct Greeter {
	static bool sayHello(std::string name) {
		std::cout << "Hello, " << name << "!\n";
		return true;
	}
};

#include "nbind/nbind.h"

NBIND_CLASS(Greeter) {
  method(sayHello);
}