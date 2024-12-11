#include "Defer.h"

Defer::Defer(std::function<void()> func):_func(func)
{
}

Defer::~Defer()
{
	_func();
}
