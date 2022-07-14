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
#include "PhotonListener.h"

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
	static PhotonListener& getBot()
	{
		static PhotonListener test("");
		return test;
	}

	static void Test(const dpp::message_create_t& event)
	{
			ExitGames::LoadBalancing::ConnectOptions options(ExitGames::LoadBalancing::AuthenticationValues(), "cunt", "135.125.239.180", ExitGames::LoadBalancing::ServerType::MASTER_SERVER);
			getBot().Client.connect(options);
			
			std::cout << getBot().Client.getRoomList().getSize() << std::endl;
	}
	static void Disconnect(const dpp::message_create_t& event)
	{
		std::cout << getBot().Client.getRoomList().getSize() << std::endl;
		/*for (int i = 0; Bots.size() > i; i++)
		{
			Bots[i].Client.disconnect();
			event.reply("Disconnected");
		}*/
		//getBot().Client.disconnect();
		//std::cout << getBot().Client.getState() << std::endl;
	}

	static void List(const dpp::message_create_t& event)
	{
		std::stringstream List;

		for (int i = 0; getBot().Client.getRoomList().getSize() > i; i++)
		{
			LoadBalancing::Room *room = getBot().Client.getRoomList()[i];
			
			//List += room->getName();
			List << room->getName().UTF8Representation().cstr() << "\n";
		
		}

		std::cout << "Current Room: " << getBot().Client.getCurrentlyJoinedRoom().getName() << std::endl;
		event.reply("```" + List.str() + "```");
	}
	static void Join(const dpp::message_create_t& event)
	{
		//getBot().Client.opJoinRoom(getBot().Client.getRoomList()[0]->getName(), true);
		getBot().Client.opJoinRandomRoom();
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



