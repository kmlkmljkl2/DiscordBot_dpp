#pragma once
#include "dpp/dpp.h"
#include "DiscordBotStuff.h"
class PlaybackHandler
{
private:
	std::vector<std::string> Queue;
	dpp::snowflake TargetGuild;
	bool Playing = false;
	dpp::discord_client* Client;
	bool Stopping = false;

public:
	PlaybackHandler(dpp::discord_client* client, dpp::snowflake guild);
	~PlaybackHandler();
	void Add(std::string url);
	void Remove(int index);
	void TryPlay();
	void Stop();
};
