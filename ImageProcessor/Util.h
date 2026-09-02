#pragma once
#include <string>
#include <vector>

namespace util
{
	std::vector<std::string> stringSplit(std::string origin, std::string sep);
	unsigned int clamp(int value, int min, int max);
}

