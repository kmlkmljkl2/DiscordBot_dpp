#pragma once
#include <iostream>
#include <dpp/dispatcher.h>
#include "NotPhotonListener.h"
#include "UrbanDictionaryJson.h"
#include <dpp/dpp.h>
#include "PlaybackHandler.h"

static class CommandHandler
{
private:
	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
	static std::string GetResponse(std::string Url);
	static NotPhotonListener* GetBot(const dpp::message_create_t& event, bool OwnerOnly = false);
	static void RemoveBot(NotPhotonListener* OldBot);
	inline static const std::vector<std::string> ClientStates{
		"Uninitialized",
		"PeerCreated",
		"ConnectingToNameserver",
		"ConnectedToNameserver",
		"DisconnectingFromNameserver",
		"Connecting",
		"Connected",
		"WaitingForCustomAuthenticationNextStepCall",
		"Authenticated",
		"JoinedLobby",
		"DisconnectingFromMasterserver",
		"ConnectingToGameserver",
		"ConnectedToGameserver",
		"AuthenticatedOnGameServer",
		"Joining",
		"Joined",
		"Leaving",
		"Left",
		"DisconnectingFromGameserver",
		"ConnectingToMasterserver",
		"ConnectedComingFromGameserver",
		"AuthenticatedComingFromGameserver",
		"Disconnecting",
		"Disconnected"
	};
	static void HandleChat();
	inline static std::map<dpp::snowflake, PlaybackHandler*> MusicPlayers;
	static bool SameVoiceChat(const dpp::message_create_t& event);
public:
	static inline UrbanDictionaryJson Definitions;
	inline static std::vector<NotPhotonListener*> StoreVector;
	static void Init();

	static void UrbanDictionary(const dpp::message_create_t& event, std::string args = "");
	static void Ahegao(const dpp::message_create_t& event, std::string args = "");
	static void Meow(const dpp::message_create_t& event, std::string args = "");
	static void CreateRoom(const dpp::message_create_t& event, std::string args = "");
	static void Test(const dpp::message_create_t& event, std::string args = "");
	static void Debug(const dpp::message_create_t& event, std::string args = "");
	static void PlayerList(const dpp::message_create_t& event, std::string args = "");
	static void Join(const dpp::message_create_t& event, std::string args);
	static void List(const dpp::message_create_t& event, std::string args = "");
	static void Disconnect(const dpp::message_create_t& event, std::string args = "");
	static void Start(const dpp::message_create_t& event, std::string args = "");
	static void Next(const dpp::message_create_t& event, std::string args = "");
	static void Back(const dpp::message_create_t& event, std::string args = "");

	static void JoinVC(const dpp::message_create_t& event, std::string args = "");
	static void LeaveVC(const dpp::message_create_t& event, std::string args = "");
	static void Play(const dpp::message_create_t& event, std::string args = "");
	static void Skip(const dpp::message_create_t& event, std::string args = "");
	static void Resume(const dpp::message_create_t& event, std::string args = "");
	static void Pause(const dpp::message_create_t& event, std::string args = "");
	static void Queue(const dpp::message_create_t& event, std::string args = "");
	static void Remove(const dpp::message_create_t& event, std::string args = "");
	static void Shuffle(const dpp::message_create_t& event, std::string args = "");
	static void PlayNow(const dpp::message_create_t& event, std::string args = "");
};
