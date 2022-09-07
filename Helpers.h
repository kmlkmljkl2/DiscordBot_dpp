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
	static std::string TrimBeginning(std::string& str)
	{
		if (str == "") return "";
		while (str[0] == ' ')
		{
			str.erase(0, 1);
		}
		return str;
	}
	static std::string TrimEnd(std::string& str)
	{
		if (str == "") return "";
		while (str[str.length() - 1] == ' ')
		{
			str.erase(str.length() - 1, 1);
		}
		return str;
	}
	static std::string TrimStartEnd(std::string& str)
	{
		TrimBeginning(str);
		return TrimEnd(str);
	}

};