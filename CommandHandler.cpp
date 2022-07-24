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

public:
	static NotPhotonListener& getBot()
	{
		static NotPhotonListener test("");
		return test;
	}

	static void Start(const dpp::message_create_t& event)
	{
		ExitGames::LoadBalancing::ConnectOptions options(ExitGames::LoadBalancing::AuthenticationValues().setData("cunt"), "cunt", "135.125.239.180", ExitGames::LoadBalancing::ServerType::MASTER_SERVER);
		getBot().Client.connect(options);
		getBot().Client.getLocalPlayer().addCustomProperty("name", "crustycunt");
	}
	static void Disconnect(const dpp::message_create_t& event)
	{
		//std::cout << getBot().Client.getRoomList().getSize() << std::endl;
		/*for (int i = 0; Bots.size() > i; i++)
		{
			Bots[i].Client.disconnect();
			event.reply("Disconnected");
		}*/
		getBot().Client.disconnect();
		//std::cout << getBot().Client.getState() << std::endl;
	}
	static void List(const dpp::message_create_t& event)
	{
		std::stringstream List;

		for (int i = 0; getBot().Client.getRoomList().getSize() > i; i++)
		{
			LoadBalancing::Room* room = getBot().Client.getRoomList()[i];
			std::string name = room->getName().UTF8Representation().cstr();
			//List += room->getName();
			auto test = std::regex_replace(name, std::regex("\\[[a-zA-Z0-9\]{6}\\]"), "");

			auto splitted = Helpers::Split(test, '`');

			//auto e = room->getPlayerCount() + "\\" + room->getMaxPlayers();
			auto Data = splitted[0] + std::string(50 - splitted[0].length(), ' ') + splitted[1] + std::string(22 - splitted[1].length(), ' ') /*+ e*/;

			List << Data << "\n";
		}

		event.reply("```" + List.str() + "```");
	}
	static void Join(const dpp::message_create_t& event, std::string args)
	{
		args = args.substr(1);
		
		//getBot().Client.opJoinRoom(getBot().Client.getRoomList()[0]->getName(), true);
		LoadBalancing::Room* Target = NULL;
		for (int i = 0; getBot().Client.getRoomList().getSize() > i; i++)
		{
			LoadBalancing::Room* room = getBot().Client.getRoomList()[i];
			std::string Name = Helpers::ToLower(std::regex_replace(room->getName().UTF8Representation().cstr(), std::regex("\\[[a-zA-Z0-9\]{6}\\]"), ""));

			if (Name.find(args) != std::string::npos)
			{
				std::cout << "Found something" << std::endl;
				Target = room;
			}
		}
		
		if (Target != NULL)
		{
			
			std::cout << "joining" << std::endl;
			getBot().Client.opJoinRoom(Target->getName());
		}
		
		//getBot().Client.opJoinRandomRoom();
		
	}
	static void CurrentRoom(const dpp::message_create_t& event)
	{
		std::cout << "Current Room: " << getBot().Client.getCurrentlyJoinedRoom().getName().UTF8Representation().cstr() << std::endl;
		auto& test = getBot().Client.getMasterserverAddress();
		
		std::cout << test.UTF8Representation().cstr() << std::endl;
	}
	static void SendDc(const dpp::message_create_t& event)
	{
			int target = 245;

			
			for (int i = 0; 20 > i; i++)
			{
				//	getBot().Client.sendDirect(o, sendoption);
				
				try
				{

					ExitGames::Common::Hashtable asd = Common::Hashtable();
					asd.put("test", new char[5000]);
					getBot().Client.opRaiseEvent(true, asd, 173, LoadBalancing::RaiseEventOptions().setTargetPlayers(&target, 1));
					getBot().Client.service();

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
		ExitGames::Common::Hashtable Props;
		Props.put("TestProp", "NoValue");

		ExitGames::LoadBalancing::RoomOptions T(true, true);
	//	T.setLobbyName("FoodForCunts`The Forest III`normal`999999`day``56481");
		T.setLobbyType(NULL);
		T.setMaxPlayers(20);
		T.setCustomRoomProperties(Props);
		T.setEmptyRoomTtl(-1);
		T.setPlayerTtl(500);
		ExitGames::Common::JVector<Common::JString> ListenRoomProps;

		auto LongSTR = std::string(999, 'T');
		ListenRoomProps.addElement(LongSTR.c_str());


		T.setPropsListedInLobby(ListenRoomProps);

		getBot().Client.opCreateRoom("FoodForCunts`The Forest III`normal`999999`day``4879", T);

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