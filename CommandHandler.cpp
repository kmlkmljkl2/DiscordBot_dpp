#include "CommandHandler.h"
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
#include "Helpers.h"
#include "Command.h"
#include "UrbanDictionaryJson.h"
#include "DiscordBotStuff.h"

size_t CommandHandler::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

std::string CommandHandler::GetResponse(std::string Url)
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
	else
	{
		std::cout << "No curl" << std::endl;
	}
	return "";
}

NotPhotonListener* CommandHandler::GetBot(const dpp::message_create_t& event, bool OwnerOnly)
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

void CommandHandler::RemoveBot(NotPhotonListener* OldBot)
{
	for (int i = 0; StoreVector.size() > i; i++)
	{
		if (StoreVector[i] == OldBot)
		{
			StoreVector[i] = NULL;
			StoreVector.erase(StoreVector.begin() + i);
		}
	}
}

void CommandHandler::HandleChat()
{
	while (true)
	{
		for (int i = 0; StoreVector.size() > i; i++)
		{
			auto Bot = StoreVector[i];
			if (Bot->Client.getState() != 15 || Bot->Chat == "") continue;

			if (Bot->Chat.size() > 2000)
				Bot->Chat = "Message was over 2k Characters, it got ate";
			//std::cout << Bot->Chat.size() << std::endl;
			dpp::message msg(Bot->ChannelId, Bot->Chat);
			msg.set_guild_id(Bot->GuildId);
			Bot->Chat = "";
			try
			{
				DiscordBotStuff::SendMsg(msg);
			}
			catch (std::exception ex)
			{
				std::cout << "Message error AoTTG -> Discord: " << ex.what() << std::endl;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
	}
}

void CommandHandler::Init()
{
	std::thread(HandleChat).detach();
}

void CommandHandler::UrbanDictionary(const dpp::message_create_t& event, std::string args)
{
	std::string Url = "https://api.urbandictionary.com/v0/define?term=" + args;
	std::string response_string = GetResponse(Url);
	json json = json::parse(response_string);

	UrbanDictionaryJson ud(json["list"]);
	if (ud.Definitions.empty())
	{
		event.send("No definition found");
		return;
	}
	Definitions = ud;
	event.send(ud.Definitions[0] + "\t1" + "/" + std::to_string(ud.Definitions.size()));
}

void CommandHandler::Ahegao(const dpp::message_create_t& event, std::string args)
{
	std::string Url = "https://g.tenor.com/v1/search?q=ahegao&media_filter=minimal&key=1D4ZQ37D7W46&limit=1&pos=" + std::to_string(1 + (rand() % 100));

	std::string response_string = GetResponse(Url);
	json j = json::parse(response_string);

	std::string result;
	result = j["results"][0]["media"][0]["gif"]["url"];
	event.send(result);
}

void CommandHandler::Meow(const dpp::message_create_t& event, std::string args)
{
	std::string Url = "https://g.tenor.com/v1/search?q=meow&media_filter=minimal&key=1D4ZQ37D7W46&limit=1&pos=" + std::to_string(1 + (rand() % 100));

	std::string response_string = GetResponse(Url);
	json j = json::parse(response_string);

	std::string result;
	result = j["results"][0]["media"][0]["gif"]["url"];
	event.send(result);
}

void CommandHandler::CreateRoom(const dpp::message_create_t& event)
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

void CommandHandler::Test(const dpp::message_create_t& event, std::string args)
{

}

void CommandHandler::Debug(const dpp::message_create_t& event, std::string args)
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

void CommandHandler::PlayerList(const dpp::message_create_t& event, std::string args)
{
	auto Bot = GetBot(event);
	if (Bot == nullptr) return;
	if (Bot->Client.getState() != 15) return;

	std::stringstream List;

	List << "```";
	//std::string List = "```";
	auto RoomName = Helpers::Split(std::regex_replace(Bot->Client.getCurrentlyJoinedRoom().getName().UTF8Representation().cstr(), std::regex("\\[[a-zA-Z0-9\]{6}\\]"), ""), '`');
	List << "Roomname: " + RoomName[0] + "\n";
	List << "Current Playercount: " + std::to_string(Bot->Client.getCurrentlyJoinedRoom().getPlayerCount()) + "/" + std::to_string(Bot->Client.getCurrentlyJoinedRoom().getMaxPlayers()) + "\n";
	List << "Room: " + RoomName[1] + "\n";

	for (int i = 0; Bot->Client.getCurrentlyJoinedRoom().getPlayers().getSize() > i; i++)
	{
		auto player = Bot->Client.getCurrentlyJoinedRoom().getPlayers()[i];
		auto props = player->getCustomProperties();

		List << (props.contains("NoodleDoodle") ? "[B]" : player->getIsMasterClient() ? "[M]" : "[P]");

		if (props.contains("isTitan"))
		{
			bool IsTitan = Common::ValueObject<int>(props.getValue("isTitan")).getDataCopy() == 2;
			List << (IsTitan ? "[T]" : "[H]");
		}
		else
		{
			List << "[H]";
		}
		List << " [";
		auto BiggestId = std::to_string(Bot->Client.getCurrentlyJoinedRoom().getPlayers().getLastElement()->getNumber());
		//std::cout << BiggestId.size() - std::to_string(player->getNumber()).size() << std::endl;
		List << std::string(BiggestId.size() - std::to_string(player->getNumber()).size(), '0');
		//std::cout << List.str() << std::endl;
		List << std::to_string(player->getNumber()) + "] ";

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
		if (Name.size() > 20)
			Name.erase(20, Name.size());

		List << Name + std::string(23 - Name.size(), ' ');

		std::string Stats;
		if (props.contains("kills"))
		{
			Stats += std::to_string(Common::ValueObject<int>(props.getValue("kills")).getDataCopy()) + "/";
		}
		else
		{
			Stats += "-1/";
		}
		if (props.contains("deaths"))
		{
			Stats += std::to_string(Common::ValueObject<int>(props.getValue("deaths")).getDataCopy()) + "/";
		}
		else
		{
			Stats += "-1/";
		}
		if (props.contains("max_dmg"))
		{
			Stats += std::to_string(Common::ValueObject<int>(props.getValue("max_dmg")).getDataCopy()) + "/";
		}
		else
		{
			Stats += "-1/";
		}
		if (props.contains("total_dmg"))
		{
			Stats += std::to_string(Common::ValueObject<int>(props.getValue("total_dmg")).getDataCopy());
		}
		else
		{
			Stats += "-1";
		}

		List << Stats + std::string(20 - Stats.size(), ' ');

		if (props.contains("dead"))
		{
			bool IsDead = Common::ValueObject<bool>(props.getValue("dead")).getDataCopy();
			List << (IsDead ? "Dead" : "Alive");
		}
		else
		{
			List << "Dead";
		}

		List << "\n";
	}
	List << "```";
	//std::cout << List.size() << std::endl;

	event.send(List.str());
	//Bot->Chat += List.str();
}

void CommandHandler::Join(const dpp::message_create_t& event, std::string args)
{
	auto Bot = GetBot(event, true);
	if (Bot == nullptr) return;
	args = Helpers::ToLower(args);

	LoadBalancing::Room* Target = NULL;
	for (int i = 0; Bot->Client.getRoomList().getSize() > i; i++)
	{
		LoadBalancing::Room* room = Bot->Client.getRoomList()[i];
		std::string Name = Helpers::ToLower(std::regex_replace(room->getName().UTF8Representation().cstr(), std::regex("\\[[a-zA-Z0-9\]{6}\\]"), ""));

		if (Name.find(args) != std::string::npos)
		{
			//Logger::LogDebug("Join Found a room");
			Target = room;
			break;
		}
	}

	if (Target != NULL)
	{
		std::cout << "joining" << std::endl;
		Bot->Client.opJoinRoom(Target->getName());
		bool createChannel = false;
		auto guild = dpp::find_guild(event.msg.guild_id);
		dpp::snowflake parentId;
		for (int i = 0; guild->channels.size() > i; i++)
		{
			auto channel = dpp::find_channel(guild->channels[i]);


			if (channel == NULL) continue;
			std::cout << channel->name << std::endl;
			if ((Helpers::ToLower(channel->name) == "aottg rooms" || Helpers::ToLower(channel->name) == "aottg room") && channel->is_category())
			{
				createChannel = true;
				parentId = channel->id;
				break;
			}
		}

		
		if (!createChannel) return;

		auto channel = dpp::channel();
		channel.guild_id = event.msg.guild_id;
		channel.name =  Helpers::Split(std::regex_replace(Target->getName().UTF8Representation().cstr(), std::regex("\\[[a-zA-Z0-9\]{6}\\]"), ""),'`')[0];
		channel.set_parent_id(parentId);

			DiscordBotStuff().DiscordBot->channel_create(channel, [Bot](const dpp::confirmation_callback_t& event)
				{
					if (event.is_error())
					{
						std::cout << event.get_error().message << std::endl;
						return;
					}
					//	std::cout << event.get<dpp::channel>().id << std:: endl;
					Bot->ChannelId = event.get<dpp::channel>().id;
					Bot->CreatedChannel = true;

				});
		
		


	}
}


void CommandHandler::List(const dpp::message_create_t& event, std::string args)
{
	auto Bot = GetBot(event);
	if (Bot == nullptr) return;
	std::stringstream List;

	for (int i = 0; Bot->Client.getRoomList().getSize() > i; i++)
	{
		LoadBalancing::Room* room = Bot->Client.getRoomList()[i];
		auto cleanname = std::regex_replace(room->getName().ANSIRepresentation().cstr(), std::regex("\\[[a-zA-Z0-9\]{6}\\]"), "");
		auto splitted = Helpers::Split(cleanname, '`');

		std::string RoomInfo;

		std::string RoomName = splitted[0];
		if (RoomName.size() > 26)
			RoomName = RoomName.substr(0, 23) + "...";

		RoomInfo += RoomName + std::string(27 - RoomName.size(), ' ') + " | ";

		RoomInfo += splitted[1] + std::string(21 - splitted[1].size(), ' ') + " | ";

		std::string PlayerCount = std::to_string(room->getPlayerCount());
		std::string MaxPlayerCount = std::to_string(room->getMaxPlayers());

		RoomInfo += PlayerCount + std::string(4 - PlayerCount.size(), ' ') + "/";
		RoomInfo += MaxPlayerCount + std::string(4 - MaxPlayerCount.size(), ' ') + "|";
		RoomInfo += room->getIsOpen() ? "" : "CLOSED";
		RoomInfo += splitted[5] == "" ? "" : R"([PWD])";

		List << RoomInfo << "\n";
	}

	event.send("```" + List.str() + "```");
}

void CommandHandler::Disconnect(const dpp::message_create_t& event, std::string args)
{
	auto Bot = GetBot(event, true);
	if (Bot == nullptr) return;

	Bot->Client.disconnect();
	Bot->KeepRunning = false;
	//Bot->~NotPhotonListener();
	RemoveBot(Bot);
	//delete Bot;

	event.send(Bot->CreatedChannel ? "Channel will get deleted in 5 seconds" : "Disconnected");
	if (!Bot->CreatedChannel)		return;
	
	std::this_thread::sleep_for(std::chrono::seconds(5));

	DiscordBotStuff().DiscordBot->channel_delete(event.msg.channel_id, [](const dpp::confirmation_callback_t& event)
		{
			if (event.is_error())
			{
				std::cout << event.get_error().message << std::endl;
			}
		});


}

void CommandHandler::Start(const dpp::message_create_t& event, std::string args)
{
	auto OldBot = GetBot(event);
	if (OldBot != nullptr)
	{
		if (OldBot->CreatorId == event.msg.author.id && OldBot->Client.getState() != 15); // If creator called -s again and bot is not inside a Room
		{
			//remove(StoreVector.begin(), StoreVector.end(), OldBot);
			OldBot->KeepRunning = false;
			//OldBot->~NotPhotonListener();
			RemoveBot(OldBot);
			goto Continue;
		}
		event.reply("Active Bot in Channel");
		return;
	}
Continue:
	if (args.size() == 0)
		args = "eu";

	Common::JString Ip = "";
	args = Helpers::ToLower(args);
	if (args == "eu")
		Ip = "135.125.239.180";
	else if (args == "us")
		Ip = "142.44.242.29";
	else if (args == "asia")
		Ip = "51.79.164.137";
	else if (args == "sa")
		Ip = "172.107.193.233";

	if (Ip == "")
	{
		event.send("Enter a valid Region!");
		return;
	}

	NotPhotonListener* Bot = new NotPhotonListener("");
	Bot->CreatorId = event.msg.author.id;
	Bot->GuildId = event.msg.guild_id;
	Bot->ChannelId = event.msg.channel_id;
	ExitGames::LoadBalancing::ConnectOptions options(ExitGames::LoadBalancing::AuthenticationValues().setUserID("notsocrusty"), "crustycunt", Ip, ExitGames::LoadBalancing::ServerType::MASTER_SERVER);
	Bot->Client.connect(options);

	std::cout << Bot << std::endl;
	Bot->Client.getLocalPlayer().addCustomProperty("name", "NoodleDoodleTesting");
	Bot->Client.getLocalPlayer().addCustomProperty("dead", true);
	Bot->Client.getLocalPlayer().addCustomProperty("kills", 69);
	Bot->Client.getLocalPlayer().addCustomProperty("deaths", -1);
	Bot->Client.getLocalPlayer().addCustomProperty("max_dmg", 420);
	Bot->Client.getLocalPlayer().addCustomProperty("total_dmg", 420);
	Bot->Client.getLocalPlayer().addCustomProperty("NoodleDoodle", "I'm a Discord Bot");

	StoreVector.push_back(Bot);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	if (Bot->Client.getState() != LoadBalancing::PeerStates::JoinedLobby)
	{
		event.reply("Failed to join Server, try again");
		//Debug(event);
		Disconnect(event);
		return;
	}

	List(event);
}

void CommandHandler::Next(const dpp::message_create_t& event, std::string args)
{
	if (Definitions.Definitions.empty()) return;

	auto definition = Definitions.GetNext();

	if (definition == "") return;

	event.send(definition);
}

void CommandHandler::Back(const dpp::message_create_t& event, std::string args)
{
	if (Definitions.Definitions.empty()) return;

	auto definition = Definitions.GetPrevious();

	if (definition == "") return;

	event.send(definition);
}