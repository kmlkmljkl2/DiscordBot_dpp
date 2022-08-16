#include "NotPhotonListener.h"
#include <iostream>
#include <vector>
#include "Helpers.h"
#include <dpp/dpp.h>
#include "Logger.cpp"
#include <regex>

void NotPhotonListener::Run()
{
	while (KeepRunning)
	{
		try
		{
			Client.service();
			SLEEP(10);
		}
		catch (std::exception err)
		{
			std::cout << err.what() << std::endl;
			SLEEP(10);
		}
	}
}

NotPhotonListener::NotPhotonListener(std::string str) : Client(*this, "", "01042015_1.28", Photon::ConnectionProtocol::UDP)
{
	std::thread(&NotPhotonListener::Run, this).detach();
}

NotPhotonListener::NotPhotonListener(const NotPhotonListener& old) : Client(*this, "", "01042015_1.28", Photon::ConnectionProtocol::UDP)
{
	std::thread(&NotPhotonListener::Run, this).detach();

}

NotPhotonListener::~NotPhotonListener()
{
}

void NotPhotonListener::debugReturn(int debugLevel, const Common::JString& string)
{
	std::cout << "DebugReturn: " << debugLevel << " " << string.UTF8Representation().cstr() << std::endl;
}

void NotPhotonListener::connectionErrorReturn(int errorCode)
{
	std::cout << "connectionErrorReturn: " << errorCode << std::endl;
}

void NotPhotonListener::clientErrorReturn(int errorCode)
{
	std::cout << "clientErrorReturn: " << errorCode << std::endl;
}

void NotPhotonListener::warningReturn(int warningCode)
{
	std::cout << "warningReturn: " << warningCode << std::endl;
}

void NotPhotonListener::serverErrorReturn(int errorCode)
{
	std::cout << "serverErrorReturn: " << errorCode << std::endl;
}

void NotPhotonListener::joinRoomEventAction(int playerNr, const Common::JVector<int>& playernrs, const LoadBalancing::Player& player)
{
	//std::cout << "Player " << playerNr << " joined" << std::endl;
	std::string JoineMsg = "[" + std::to_string(playerNr) + "] ";
	if (player.getCustomProperties().contains("name"))
	{
		JoineMsg += std::regex_replace(std::string(player.getCustomProperties().getValue("name")->toString().UTF8Representation().cstr()).substr(1), std::regex("\\[[a-zA-Z0-9\]{6}\\]"), "");
		JoineMsg.pop_back();
	}
	else
	{
		JoineMsg += "Nameless";
	}
	JoineMsg += " joined the room!";
	Chat += JoineMsg;

}

void NotPhotonListener::leaveRoomEventAction(int playerNr, bool isInactive)
{
	/*for (int i = 0; this->Client.getCurrentlyJoinedRoom().getPlayers().getSize() > i; i++)
	{
		auto p = this->Client.getCurrentlyJoinedRoom().getPlayers()[i];
		std::cout << p->getNumber() << std::endl;
		if (p->getNumber() == playerNr)
		{
			std::cout << "Was still inside :lickge:" << std::endl;
		}
	}*/
	std::cout << "Player " << playerNr << " left" << std::endl;
	Chat += "[" + std::to_string(playerNr) + "]\tleft the Room";
}

//InRoom Events
void NotPhotonListener::customEventAction(int playerNr, nByte eventCode, const Common::Object& eventContent)
{
	if (eventCode == 200)
	{
		byte three = 3;

		auto hash = ExitGames::Common::ValueObject<Common::Hashtable>(eventContent).getDataCopy();

		// 3 == Chat
		// 5 == 62

		if (hash.contains((byte)5) || hash.contains(three))
		{
			auto value = Common::ValueObject<byte>(hash.getValue((byte)5)).getDataCopy();
			Common::Object* RPCName = hash.getValue(three);
			std::string res;
			if (RPCName)
			{
				res = std::string(RPCName->toString().UTF8Representation().cstr());
			}

			if (value == (byte)62 || res == "\"Chat\"")
			{
				auto args = Common::ValueObject<Common::Object*>((const Common::Object*)hash.getValue<byte>(4)).getDataCopy();
				//auto args2 = Common::ValueObject<Common::JString>((const Common::Object*)hash.getValue<byte>(4)).getDataCopy();


				if (args == nullptr)
				{
					std::cout << "Object* was null" << std::endl;
					return;
				}
				auto content = std::string(args[0].toString().UTF8Representation().cstr());
				auto sender = std::string(args[1].toString().UTF8Representation().cstr());



				if (sender == "\"\"")
				{
					sender = "";
				}
				else
				{
					sender.pop_back();
					sender += ": ";
					sender = sender.substr(1);
				}
				content = content.substr(1);
				content.pop_back();

				Chat += "[" + std::to_string(playerNr) + "] " + std::regex_replace(sender + content, std::regex(std::string(R"(<[^>]*>)")), "") + "\n";

				/*dpp::message msg(this->ChannelId, std::regex_replace(sender + content, std::regex(std::string(R"(<[^>]*>)")), ""));
				msg.set_guild_id(this->GuildId);
				DiscordBotStuff::SendMsg(msg);*/
				ExitGames::Common::MemoryManagement::deallocateArray(args);
			}

		}
	}
}

void NotPhotonListener::connectReturn(int errorCode, const Common::JString& errorString, const Common::JString& region, const Common::JString& cluster)
{
	//std::cout << "connectReturn " << errorCode << " " << errorString.UTF8Representation().cstr() << " " << region.UTF8Representation().cstr() << " " << cluster.UTF8Representation().cstr() << std::endl;

	if (errorCode)
	{
		//std::cout << ExitGames::Common::DebugLevel::ERRORS, L"error code: %d   error message: %ls", errorCode, errorString.cstr();
		return;
	}
	std::cout << " connected to cluster \"" << cluster.UTF8Representation().cstr() << "\" of region \"" << region.UTF8Representation().cstr() << "\", user id = " << Client.getUserID().UTF8Representation().cstr() << std::endl;
}

void NotPhotonListener::disconnectReturn(void)
{
	std::cout << "disconnected" << std::endl;
}

void NotPhotonListener::leaveRoomReturn(int errorCode, const Common::JString& errorString)
{
	std::cout << "left Room " << errorCode << " " << errorString.UTF8Representation().cstr() << std::endl;
}

void NotPhotonListener::onMasterClientChanged(int id, int oldID)
{
	std::cout << "MC changed to "  << id << std::endl;
}

void NotPhotonListener::onStatusChanged(int statusCode)
{
	std::cout << "called" << std::endl;
}