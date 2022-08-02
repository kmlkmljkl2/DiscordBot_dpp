#include "NotPhotonListener.h"
#include <iostream>
#include <vector>

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
		auto hash = ExitGames::Common::ValueObject<Common::Hashtable>(eventContent).getDataCopy();

		//auto intte = ValueObject<int>(playerProperties.getKeys()[i]).getDataCopy() :targetPlayerNr
		// 3 == Chat
		// 5 == 62



		
		if (hash.contains((byte)5) || hash.contains((byte)3))
		{
			byte three = 3;
			auto value = Common::ValueObject<byte>(hash.getValue((byte)5)).getDataCopy();

			

			if (value == (byte)62)
			{
				std::cout << "Received Chat Event" << std::endl;
			}
			
			if (hash.contains(three))
			{
				Common::Object* RPCName = hash.getValue(three);
				auto res = (std::string)RPCName->toString().UTF8Representation().cstr();
				//std::cout << res << std::endl;



				if (res == "\"Chat\"")
				{
					std::cout << "Alternate received Chat Event" << std::endl;

					auto args = hash.getValue((byte)4);

					auto test = (Common::JVector<char*>)args->toString()[0];

					//std::cout << test[0] << std::endl;
					std::cout << test.getSize() << std::endl;

					std::cout << args[0].toString().UTF8Representation().cstr() << std::endl;


				}
			


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

