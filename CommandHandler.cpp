

#include <dpp/dispatcher.h>
#include <iostream>
#include "curl.h"
#include "nlohmann/json.hpp"
#include <dpp/dpp.h>
#include "Client.h"
#include "Common-cpp/inc/Common.h"
#include "AuthenticationValues.h"
#include "Enums/ServerType.h"
#include "Listener.cpp"
#include "NotPhotonListener.h"
#include <regex>
#include "Helpers.cpp"
#include "DiscordBotStuff.cpp"

using namespace dpp;
using json = nlohmann::json;
//for (auto& el1 : j["results"][0]["media"][0]["gif"]["url"].items())
			  //{
			  //   // std::cout << el1.key() << '\n';
			  //    std::cout << el1.key() << ": " << el1.value() << std::endl;

			  //    for (auto& el2 : el1.value())
			  //    {
			  //        //here
			  //        for (auto& el3 : el2.items())
			  //        {
			  //           // std::cout << el3.value() << '\n';

			  //            for (auto& el : el3.value())
			  //            {
			  //                //no longer able to filter
			  //               // std::cout << el.dump() << '\n';

			  //            }

			  //        }

			  //    }
			  //}

static class CommandHandler
{
	inline static const std::vector<std::string> ClientStates
	{
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
	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
	{
		((std::string*)userp)->append((char*)contents, size * nmemb);
		return size * nmemb;
	}

	static std::string GetResponse(std::string Url)
	{
		auto curl = curl_easy_init();
		if (curl)
		{
			curl_easy_setopt(curl, CURLOPT_URL, Url.c_str());
			std::string response_string;
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
			curl_easy_perform(curl);
			//std::cout << response_string << std::endl;

			curl_easy_cleanup(curl);
			curl_global_cleanup();
			curl = NULL;

			return response_string;
		}
		return "";
	}

	static NotPhotonListener* GetBot(const dpp::message_create_t& event)
	{
		for (int i = 0; StoreVector.size() > i; i++)
		{
			auto BotChannelId = StoreVector[i]->ChannelId;
			auto ChannelId = event.msg.channel_id;
			if (BotChannelId == ChannelId)
			{
				return StoreVector[i];
			}
		}
		return nullptr;
	}

	static void RemoveBot(NotPhotonListener* OldBot)
	{
		for (int i = 0; StoreVector.size() > i; i++)
		{
			if (StoreVector[i] == OldBot)
			{
			//	std::cout << "removed a Bot from the Vector" << std::endl;
				StoreVector.erase(StoreVector.begin() + i);
			}
		}
	}

public:
	inline static std::vector<NotPhotonListener*> StoreVector = std::vector<NotPhotonListener*>();

	/*static NotPhotonListener& getBot()
	{
		static NotPhotonListener test("");
		return test;
	}*/

	static void Start(const dpp::message_create_t& event)
	{
		auto OldBot = GetBot(event);
		if (OldBot != nullptr)
		{
			if (OldBot->CreatorId == (long long)event.msg.author.id);
			{
				//	std::cout << "Old Size "  << StoreVector.size() << std::endl;
				remove(StoreVector.begin(), StoreVector.end(), OldBot);
				//std::cout << "Removed Bot " << StoreVector.size() << std::endl;

				OldBot->KeepRunning = false;
				OldBot->~NotPhotonListener();
				RemoveBot(OldBot);
				goto Continue;
			}
			event.reply("Active Bot in Channel");
			return;
		}
		Continue:
		NotPhotonListener* Bot = new NotPhotonListener("");
		Bot->CreatorId = event.msg.author.id;

		Bot->GuildId = event.msg.guild_id;
		Bot->ChannelId = event.msg.channel_id;
		ExitGames::LoadBalancing::ConnectOptions options(ExitGames::LoadBalancing::AuthenticationValues().setUserID("crustycunt"), "cunt", "135.125.239.180", ExitGames::LoadBalancing::ServerType::MASTER_SERVER);
		Bot->Client.connect(options);
		Bot->Client.getLocalPlayer().addCustomProperty("name", "crustycunt");
		StoreVector.push_back(Bot);
		SLEEP(600);
		if (Bot->Client.getState() != LoadBalancing::PeerStates::JoinedLobby)
		{
			event.reply("Failed to join Server, try again");
			//Debug(event);
			Disconnect(event);
			return;
		}


		List(event);
		
	}
	static void Disconnect(const dpp::message_create_t& event)
	{
		auto Bot = GetBot(event);
		if (Bot == nullptr) return;

		Bot->Client.disconnect();
		Bot->KeepRunning = false;
		Bot->~NotPhotonListener();
		RemoveBot(Bot);
		event.send("Disconnected");

	}
	static void List(const dpp::message_create_t& event)
	{
		auto Bot = GetBot(event);
		if (Bot == nullptr) return;
		std::stringstream List;

		for (int i = 0; Bot->Client.getRoomList().getSize() > i; i++)
		{
			LoadBalancing::Room* room = Bot->Client.getRoomList()[i];
			std::string name = room->getName().UTF8Representation().cstr();
			auto test = std::regex_replace(name, std::regex("\\[[a-zA-Z0-9\]{6}\\]"), "");

			auto splitted = Helpers::Split(test, '`');

			auto Data = splitted[0] + std::string(50 - splitted[0].length(), ' ') + splitted[1] + std::string(22 - splitted[1].length(), ' ') /*+ e*/;

			List << Data << "\n";
		}

		event.send("```" + List.str() + "```");
	}
	static void Join(const dpp::message_create_t& event, std::string args)
	{
		auto Bot = GetBot(event);
		if (Bot == nullptr) return;
		args = args.substr(1);
		LoadBalancing::Room* Target = NULL;
		for (int i = 0; Bot->Client.getRoomList().getSize() > i; i++)
		{
			LoadBalancing::Room* room = Bot->Client.getRoomList()[i];
			std::string Name = Helpers::ToLower(std::regex_replace(room->getName().UTF8Representation().cstr(), std::regex("\\[[a-zA-Z0-9\]{6}\\]"), ""));

			if (Name.find(args) != std::string::npos)
			{
				std::cout << "Found something" << std::endl;
				Target = room;
				break;
			}
		}
		
		if (Target != NULL)
		{
			std::cout << "joining" << std::endl;
			Bot->Client.opJoinRoom(Target->getName());
		}
	}
	static void Debug(const dpp::message_create_t& event)
	{
		auto Bot = GetBot(event);
		if (Bot == nullptr) return;
		std::stringstream List;
		auto te = ClientStates[Bot->Client.getState()];
	//	auto test = ExitGames::LoadBalancing::PeerStates::(8);
		List << "```\nCreator:		" << Bot->CreatorId << "\n";
		List << "Channel:			" << Bot->ChannelId << "\n";
		List << "State:				" << te << "\n";
		List << "Ping:				" << Bot->Client.getRoundTripTime() << "\n";
		List << "Ping variance:		" << Bot->Client.getRoundTripTimeVariance() << "```";

		event.send(List.str());
	}
	/*static std::vector<NotPhotonListener*>& StoreVector()
	{
		static std::vector<NotPhotonListener*> test;
		return test;
	}*/

	static void Test(const dpp::message_create_t& event)
	{
		dpp::message msg(event.msg.channel_id, "test");
		msg.set_guild_id(event.msg.guild_id);

		DiscordBotStuff::SendMsg(msg);

	//	DiscordBot.message_create_sync(msg);


	}
	static void SendDc(const dpp::message_create_t& event)
	{
		auto Bot = GetBot(event);
		if (Bot == nullptr) return;

			int target = 245;

			
			for (int i = 0; 20 > i; i++)
			{
				//	getBot().Client.sendDirect(o, sendoption);
				
				try
				{

					ExitGames::Common::Hashtable asd = Common::Hashtable();
					asd.put("test", new char[5000]);
					Bot->Client.opRaiseEvent(true, asd, 173, LoadBalancing::RaiseEventOptions().setTargetPlayers(&target, 1));
					Bot->Client.service();

				}
				catch (...)
				{
					std::cout << "error" << std::endl;
				}
			}

		//delete ppp;

	}
	static void CreateRoom(const dpp::message_create_t& event)
	{
		auto Bot = GetBot(event);
		if (Bot == nullptr) return;

		ExitGames::Common::Hashtable Props;
		Props.put("TestProp", "NoValue");

		ExitGames::LoadBalancing::RoomOptions T(true, true);
		T.setLobbyType(NULL);
		T.setMaxPlayers(20);
		T.setCustomRoomProperties(Props);
		T.setEmptyRoomTtl(-1);
		T.setPlayerTtl(500);
		ExitGames::Common::JVector<Common::JString> ListenRoomProps;

		auto LongSTR = std::string(999, 'T');
		ListenRoomProps.addElement(LongSTR.c_str());


		T.setPropsListedInLobby(ListenRoomProps);

		Bot->Client.opCreateRoom("FoodForCunts`The Forest III`normal`999999`day``4879", T);

	}


	//"https://g.tenor.com/v1/search?q=meow&media_filter=minimal&key=1D4ZQ37D7W46&limit=1&pos=" + std::to_string(1 + (rand() % 10))

	static void Meow(const dpp::message_create_t& event)
	{
		std::string Url = "https://g.tenor.com/v1/search?q=meow&media_filter=minimal&key=1D4ZQ37D7W46&limit=1&pos=" + std::to_string(1 + (rand() % 100));

		std::string response_string = GetResponse(Url);
		json j = json::parse(response_string);

		std::string result;
		result = j["results"][0]["media"][0]["gif"]["url"];
		event.send(result);
	}
	static void Ahegao(const dpp::message_create_t& event)
	{
		std::string Url = "https://g.tenor.com/v1/search?q=ahegao&media_filter=minimal&key=1D4ZQ37D7W46&limit=1&pos=" + std::to_string(1 + (rand() % 100));

		std::string response_string = GetResponse(Url);
		json j = json::parse(response_string);

		std::string result;
		result = j["results"][0]["media"][0]["gif"]["url"];
		event.send(result);
	}
};