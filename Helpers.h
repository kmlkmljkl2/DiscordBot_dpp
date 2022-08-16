#pragma once
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>

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

	static std::vector<std::string> Split(std::string str, char seperator)
	{
		std::stringstream test(str);
		std::string segment;
		std::vector<std::string> seglist;

		while (std::getline(test, segment, seperator))
		{
			seglist.push_back(segment);
		}
		return seglist;
	}
};