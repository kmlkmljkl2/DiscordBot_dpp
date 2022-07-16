#include <iostream>
#include <algorithm>

static class Helpers
{
public:

	static std::string ToLower(std::string str)
	{
		std::for_each(str.begin(), str.end(), [](char& c) {
			c = ::tolower(c);
			});
		return str;
	}

};