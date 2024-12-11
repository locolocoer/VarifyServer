#pragma once
#include "const.h"
class Defer
{
public:
	Defer(std::function<void()>);
	~Defer();
	std::function<void()> _func;
};

