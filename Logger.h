#pragma once
#include <iostream>
#include <iomanip>
#include <ctime>

static class Logger
{
	static inline bool logDebug = true;

	static std::string GetTime()
	{
		time_t result = time(NULL);

		char t[26];
		ctime_s(t, sizeof t, &result);
		std::string a = t;
		//delete t;
		a = a.substr(0, a.find_last_of("\n"));
		return a;
	}

public:

	static void LogDebug(std::string message)
	{
		if (!logDebug) return;
		std::cout << "[" << GetTime() << "]  " << "(DEBUG)" << "\t" << message << std::endl;
	}
	static void LogWarning(std::string message)
	{
		std::cout << "[" << GetTime() << "]  " << "(WARNING)" << "\t" << message << std::endl;
	}
	static  void LogInfo(std::string message)
	{
		std::cout << "[" << GetTime() << "]  " << "(INFO)" << "\t" << message << std::endl;
	}
	static void LogError(std::string message)
	{
		std::cout << "[" << GetTime() << "]  " << "(ERROR)" << "\t" << message << std::endl;
	}
};