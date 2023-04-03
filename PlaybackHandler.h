#pragma once
#include "dpp/dpp.h"
#include "DiscordBotStuff.h"
#include "YoutubeMusicObject.h"
#include "Helpers.h"
#include <algorithm> // for std::shuffle
#include <vector>    // for std::vector
#include <random>    // for std::default_random_engine

class PlaybackHandler
{
private:
	const int reservedBytes = 11520;
	bool Debugging = false;
	bool EndlessLoop = true;

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
	void GetData(std::string url);
	std::vector<uint16_t> AudioBuffer;
	void StartAudio();
	std::shared_mutex myMutex;
	uint16_t* FfmpegBuffer = new uint16_t[reservedBytes];
	uint16_t* VoiceBuffer = new uint16_t[reservedBytes];

public:
	double PlaybackDelay = 46.004;

	PlaybackHandler(dpp::discord_client* client, dpp::snowflake guild);
	~PlaybackHandler();
	void Add(std::string url);
	void Remove(int index);
	void TryPlay();
	void Stop();
	void Pause();
	void Resume();
	void Skip();
	void Shuffle();
	std::string QueueString();
};
