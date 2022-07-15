#include "NotPhotonListener.h"

void Run(NotPhotonListener* list)
{
	while (list->KeepRunning)
	{
		list->Client.service();
		SLEEP(100);
	}
}

NotPhotonListener::NotPhotonListener(std::string str) : Client(*this, "", "01042015_1.28", Photon::ConnectionProtocol::UDP)
{
	std::thread t(Run, this);
	//t.join();
	t.detach();
}

NotPhotonListener::~NotPhotonListener()
{
	KeepRunning = false;
}

void NotPhotonListener::ConnectTest(void)
{
	ExitGames::LoadBalancing::ConnectOptions options(ExitGames::LoadBalancing::AuthenticationValues(), "cunt", "135.125.239.180", ExitGames::LoadBalancing::ServerType::MASTER_SERVER);

	Client.connect(options);
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

void NotPhotonListener::customEventAction(int playerNr, nByte eventCode, const Common::Object& eventContent)
{
	std::cout << "CustomEventAction" << std::endl;
}

void NotPhotonListener::connectReturn(int errorCode, const Common::JString& errorString, const Common::JString& region, const Common::JString& cluster)
{
	//std::cout << "connectReturn " << errorCode << " " << errorString.UTF8Representation().cstr() << " " << region.UTF8Representation().cstr() << " " << cluster.UTF8Representation().cstr() << std::endl;

	if (errorCode)
	{
		//std::cout << ExitGames::Common::DebugLevel::ERRORS, L"error code: %d   error message: %ls", errorCode, errorString.cstr();
		return;
	}
	std::cout << ExitGames::Common::DebugLevel::INFO +  L" connected to cluster " << cluster.UTF8Representation().cstr() << L" of region " << region.UTF8Representation().cstr() << L", user id = " + Client.getUserID() << std::endl;

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

