#include "NotPhotonListener.h"
#include <iostream>
#include <vector>
#include "Helpers.cpp"
#include <dpp/dpp.h>
#include "DiscordBotStuff.cpp"
#include "Logger.cpp"

void Run(NotPhotonListener* Bot)
{
	while (Bot->KeepRunning)
	{
		try
		{
			if (Bot == nullptr)
				break;
			Bot->Client.service();
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
	std::thread t(Run, this);
	t.detach();
}

NotPhotonListener::NotPhotonListener(const NotPhotonListener& old) : Client(*this, "", "01042015_1.28", Photon::ConnectionProtocol::UDP)
{
	std::thread t(Run, this);
	t.detach();
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
	std::cout << "Player " << playerNr << " joined" << std::endl;
}

void NotPhotonListener::leaveRoomEventAction(int playerNr, bool isInactive)
{
	std::cout << "Player " << playerNr << " left" << std::endl;
}

//InRoom Events
void NotPhotonListener::customEventAction(int playerNr, nByte eventCode, const Common::Object& eventContent)
{
	if (eventCode == 200)
	{
		byte three = 3;

		auto hash = ExitGames::Common::ValueObject<Common::Hashtable>(eventContent).getDataCopy();

		//auto intte = ValueObject<int>(playerProperties.getKeys()[i]).getDataCopy() :targetPlayerNr
		// 3 == Chat
		// 5 == 62
	

			if (hash.contains((byte)5) || hash.contains(three))
			{
				auto value = Common::ValueObject<byte>(hash.getValue((byte)5)).getDataCopy();
				Common::Object* RPCName = hash.getValue(three);
				std::string res;
				if (RPCName)
					res = (std::string)RPCName->toString().UTF8Representation().cstr();


				if (value == (byte)62 || res == "\"Chat\"")
				{
					auto args = Common::ValueObject<Common::Object*>((const Common::Object*)hash.getValue<byte>(4)).getDataCopy();

					if (args == nullptr) return;

					auto content = (std::string)args[0].toString().UTF8Representation().cstr();
					auto sender = (std::string)args[1].toString().UTF8Representation().cstr();

					std::cout << args << std::endl;
					std::cout << args << std::endl;

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

					dpp::message msg(this->ChannelId, sender + content);
					msg.set_guild_id(this->GuildId);
					//	DiscordBotStuff::SendMsg(msg);

					std::cout << sender << content << std::endl;
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
	std::cout <<  " connected to cluster \"" << cluster.UTF8Representation().cstr() << "\" of region \"" << region.UTF8Representation().cstr() << "\", user id = " << Client.getUserID().UTF8Representation().cstr() << std::endl;

}

void NotPhotonListener::disconnectReturn(void)
{
	std::cout << "disconnected" << std::endl;
}

void NotPhotonListener::leaveRoomReturn(int errorCode, const Common::JString& errorString)
{
	std::cout << "left Room " << errorCode << " " << errorString.UTF8Representation().cstr() << std::endl;
}


void NotPhotonListener::onStatusChanged(int statusCode)
{
	std::cout << "called" << std::endl;
}

