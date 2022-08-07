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

static class CommandHandler
{
	//static std::thread ChatHandler;

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

	static NotPhotonListener* GetBot(const dpp::message_create_t& event, bool OwnerOnly = false)
	{
		for (int i = 0; StoreVector.size() > i; i++)
		{
			auto BotChannelId = StoreVector[i]->ChannelId;
			auto ChannelId = event.msg.channel_id;
			if (BotChannelId != ChannelId) continue;

			if (!OwnerOnly)
			{
				return StoreVector[i];
			}
			if (StoreVector[i]->CreatorId == event.msg.author.id)
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

	static void HandleChat()
	{
		while (true)
		{
			for (int i = 0; CommandHandler::StoreVector.size() > i; i++)
			{
				auto Bot = CommandHandler::StoreVector[i];
				if (Bot->Client.getState() != 15 || Bot->Chat == "") continue;

				if (Bot->Chat.size() > 2000)
					Bot->Chat = "Message was over 2k Characters, it got ate";


				dpp::message msg(Bot->ChannelId, Bot->Chat);
				msg.set_guild_id(Bot->GuildId);
				Bot->Chat = "";
				DiscordBotStuff::SendMsg(msg);

			}
			std::this_thread::sleep_for(std::chrono::microseconds(1500));
		}
	}
public:
	inline static std::vector<NotPhotonListener*> StoreVector = std::vector<NotPhotonListener*>();

	/*static NotPhotonListener& getBot()
	{
		static NotPhotonListener test("");
		return test;
	}*/
	static void Init()
	{
		std::thread t(HandleChat);
		t.detach();
	}

	static void Start(const dpp::message_create_t& event)
	{
		auto OldBot = GetBot(event);
		if (OldBot != nullptr)
		{
			if (OldBot->CreatorId == event.msg.author.id && OldBot->Client.getState() != 15); // If creator called -s again and bot is not inside a Room
			{
				//remove(StoreVector.begin(), StoreVector.end(), OldBot);
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
		ExitGames::LoadBalancing::ConnectOptions options(ExitGames::LoadBalancing::AuthenticationValues().setUserID("notsocrusty"), "cunt", "135.125.239.180", ExitGames::LoadBalancing::ServerType::MASTER_SERVER);
		Bot->Client.connect(options);

		std::cout << Bot << std::endl;
		Bot->Client.getLocalPlayer().addCustomProperty("name", "NoodleDoodleTesting");
		Bot->Client.getLocalPlayer().addCustomProperty("dead", true);
		Bot->Client.getLocalPlayer().addCustomProperty("kills", 69);
		Bot->Client.getLocalPlayer().addCustomProperty("deaths", -1);
		Bot->Client.getLocalPlayer().addCustomProperty("max_dmg", 420);
		Bot->Client.getLocalPlayer().addCustomProperty("total_dmg", 420);




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
		auto Bot = GetBot(event, true);
		if (Bot == nullptr) return;

		Bot->Client.disconnect();
		Bot->KeepRunning = false;
		Bot->~NotPhotonListener();
		RemoveBot(Bot);
		//delete Bot;

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
		auto Bot = GetBot(event, true);
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
	static void PlayerList(const dpp::message_create_t& event)
	{
		auto Bot = GetBot(event);
		if (Bot == nullptr) return;
		if (Bot->Client.getState() != 15) return;
		std::string List = "```";
		auto RoomName = Helpers::Split(std::regex_replace(Bot->Client.getCurrentlyJoinedRoom().getName().UTF8Representation().cstr(), std::regex("\\[[a-zA-Z0-9\]{6}\\]"), ""), '`');
		List += "Roomname: " + RoomName[0] + "\n";
		List += "Current Playercount: " + std::to_string(Bot->Client.getCurrentlyJoinedRoom().getPlayerCount()) + "/" + std::to_string(Bot->Client.getCurrentlyJoinedRoom().getMaxPlayers()) + "\n";
		List += "Room: " + RoomName[1] + "\n";


		for (int i = 0; Bot->Client.getCurrentlyJoinedRoom().getPlayers().getSize() > i; i++)
		{
			auto player = Bot->Client.getCurrentlyJoinedRoom().getPlayers()[i];
			auto props = player->getCustomProperties();

			List += player->getIsMasterClient() ? "[M]" : "[P]";
			if (props.contains("isTitan"))
			{
				bool IsTitan = Common::ValueObject<int>(props.getValue("isTitan")).getDataCopy() == 2;
				List += IsTitan ? "[T]" : "[H]";
				 
			}
			else
			{
				List += "[H]";
			}
			List += " [";
			auto BiggestId = std::to_string(Bot->Client.getCurrentlyJoinedRoom().getPlayers().getLastElement()->getNumber());

			List += std::string("0", BiggestId.size() - std::to_string(player->getNumber()).size());
			List += std::to_string(player->getNumber()) + "] ";

			std::string Name;
			//name
			if (props.contains("name"))
			{
				Name = std::regex_replace(Common::ValueObject<Common::JString>(props.getValue("name")).getDataCopy().UTF8Representation().cstr(), std::regex("\\[[a-zA-Z0-9\]{6}\\]"), "");
			}
			else
			{
				Name = "Nameless";
			}
			if (Name.size() > 17)
				Name.erase(17, Name.size());

			List += Name + std::string(" ", 23 - Name.size());

			std::string Stats;
			if (props.contains("kills"))
			{
				Stats += std::to_string(Common::ValueObject<int>(props.getValue("kills")).getDataCopy()) + "/";
			}
			else
			{
				Stats += "1-1/";
			}
			if (props.contains("deaths"))
			{
				Stats += std::to_string(Common::ValueObject<int>(props.getValue("deaths")).getDataCopy()) + "/";
			}
			else
			{
				Stats += "2-1/";
			}
			if (props.contains("max_dmg"))
			{
				Stats += std::to_string(Common::ValueObject<int>(props.getValue("max_dmg")).getDataCopy()) + "/";
			}
			else
			{
				Stats += "3-1/";
			}
			if (props.contains("total_dmg"))
			{
				Stats += std::to_string(Common::ValueObject<int>(props.getValue("total_dmg")).getDataCopy());
			}
			else
			{
				Stats += "4-1";
			}

			List += Stats + std::string(" ", 20 - Stats.size());

			if (props.contains("dead"))
			{
				bool IsDead = Common::ValueObject<bool>(props.getValue("dead")).getDataCopy();
				List += IsDead ? "Dead" : "Alive";
			}
			else
			{
				List += "Dead";
			}

			List += "\n";
			std::cout << List << std::endl;
		}
		List += "```";
		Bot->Chat += List;
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
		List << "State:				" << te << " " + std::to_string(Bot->Client.getState()) << "\n";
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
		/*dpp::message msg(event.msg.channel_id, "test");
		msg.set_guild_id(event.msg.guild_id);
		DiscordBotStuff::SendMsg(msg);*/
	}

	static void CreateRoom(const dpp::message_create_t& event)
	{
		return;
		auto Bot = GetBot(event, true);
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