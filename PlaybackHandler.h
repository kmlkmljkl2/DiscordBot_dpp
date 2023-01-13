#pragma once
#include "dpp/dpp.h"
#include "DiscordBotStuff.h"
#include "YoutubeMusicObject.h"
class PlaybackHandler
{
private:
	std::vector<YoutubeMusicObject> Queue;
	dpp::snowflake TargetGuild;
	bool Playing = false;
	dpp::discord_client* Client;
	bool Stopping = false;
	bool Pausing = false;
	bool Skipping = false;
	std::string GetLength(std::string url);
	std::chrono::high_resolution_clock::time_point StartTime;
	std::string GetName(std::string url);
public:
	PlaybackHandler(dpp::discord_client* client, dpp::snowflake guild);
	~PlaybackHandler();
	void Add(std::string url);
	void Remove(int index);
	void TryPlay();
	void Stop();
	void Pause();
	void Resume();
	void Skip();
	std::string QueueString();
};
