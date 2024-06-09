#pragma once
#include <iostream>

class StringComparator
{
public:
	bool operator()(const std::string& s1, const std::string& s2);
};

