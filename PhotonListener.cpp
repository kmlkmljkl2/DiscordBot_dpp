#include "NotPhotonListener.h"

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
	//std::cout << "CustomEventAction" << std::endl;
	if (eventCode == 200)
	{
		auto dict = ExitGames::Common::ValueObject<Common::Hashtable>(eventContent).getDataCopy();

		// 3 == Chat
		// 5 == 62
		if (dict.contains((byte)5))
		{
			auto test = dict.getValue((byte)5);
			std::cout << "Value of Byte 5 " << test->toString().cstr() << std::endl;
		}
		if (dict.contains((byte)5) && (byte)dict.getValue((byte)5) == (byte)62 /*|| dict.contains((byte)3) && (char*)dict.getValue((byte)3) == "Chat"*/)
		{
			std::cout << "had it" << std::endl;
		
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

