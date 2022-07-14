#include "Listener.h"
#include "Common.h"
#include "Player.h"
using namespace ExitGames;

class Listener : ExitGames::LoadBalancing::Listener
{
	// Inherited via Listener
	virtual void debugReturn(int debugLevel, const Common::JString& string) override
	{
	}
	virtual void connectionErrorReturn(int errorCode) override
	{
	}
	virtual void clientErrorReturn(int errorCode) override
	{
	}
	virtual void warningReturn(int warningCode) override
	{
		
	}
	virtual void serverErrorReturn(int errorCode) override
	{

	}
	virtual void joinRoomEventAction(int playerNr, const Common::JVector<int>& playernrs, const LoadBalancing::Player& player) override
	{
	}
	virtual void leaveRoomEventAction(int playerNr, bool isInactive) override
	{
	}
	virtual void customEventAction(int playerNr, nByte eventCode, const Common::Object& eventContent) override
	{
	}
	virtual void connectReturn(int errorCode, const Common::JString& errorString, const Common::JString& region, const Common::JString& cluster) override
	{
	}
	virtual void disconnectReturn(void) override
	{
	}
	virtual void leaveRoomReturn(int errorCode, const Common::JString& errorString) override
	{
	}
};