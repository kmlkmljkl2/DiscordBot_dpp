#pragma once
#include <iostream>
class YoutubeMusicObject
{
public:
	std::string Url;
	std::string Duration;
	std::string Name;
	YoutubeMusicObject(std::string url, std::string name, std::string duration) : Url(url), Duration(duration), Name(name)
	{

	}
};
