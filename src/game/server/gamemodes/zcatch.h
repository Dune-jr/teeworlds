/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_GAMEMODES_ZCATCH_H
#define GAME_SERVER_GAMEMODES_ZCATCH_H
#include <initializer_list>
#include <set>
#include <game/server/gamecontroller.h>
#include <game/server/gamemodes/zcatch/rankingserver.h>

class CGameControllerZCATCH : public IGameController
{
public:
	CGameControllerZCATCH(class CGameContext *pGameServer);
	~CGameControllerZCATCH();

	// game
	virtual void Tick();
	virtual void DoWincheckRound();
	
	virtual void OnPlayerInfoChange(class CPlayer *pPlayer);
	virtual void OnPlayerConnect(class CPlayer *pPlayer);
	virtual void OnPlayerDisconnect(class CPlayer *pPlayer);

	virtual void OnCharacterSpawn(class CCharacter *pChr);
	virtual int OnCharacterDeath(class CCharacter *pVictim, class CPlayer *pKiller, int Weapon);

	virtual void DoTeamChange(class CPlayer *pPlayer, int Team, bool DoChatMsg=true);

	virtual void EndRound();


	/**
	 * Intercept chat messages in order to respond to chat commands
	 */
	virtual void OnChatMessage(int ofID, int Mode, int toID, const char *pText);

	/**
	 * Intercept callvotes in order to respond to them.
	 * Return true in order for the calling of the votes to proceed.
	 */
	virtual bool OnCallvoteOption(int ClientID, const char* pDescription, const char* pCommand, const char* pReason);
	virtual bool OnCallvoteBan(int ClientID, int KickID, const char* pReason);
	virtual bool OnCallvoteSpectate(int ClientID, int SpectateID, const char* pReason);


	virtual void OnReset();

private:

	// store initial weapon mode, in order 
	//to prevent players from changing it mid game.
	int m_WeaponMode;

	// not changable mid game.
	int m_SkillLevel;

	/**
	 * When a player enters the game, and the game is
	 * currently running, the player has to be added 
	 * as victim to the dominating player.
	 * As there can be multiple dominating players, 
	 * one has to be chosen from multiple dominating players.
	 */
	class CPlayer* ChooseDominatingPlayer(int excludeID=-1);

	/**
	 * Send server message to the individual player about
	 * their ingame statistics,
	 */
	void ShowPlayerStatistics(class CPlayer *pOfPlayer);

	/**
	 * The value of enemies left to catch is kept alive and visible,
	 * but is not updated.
	 * Info: No calculations are done in here!
	 */
	void RefreshBroadcast();

	/**
	 * This, contraty to @RefreshBroadcast, updates the enemies left to catch
	 * counter for specified/every player/s, and sends the updated value.
	 */
	void UpdateBroadcastOf(std::initializer_list<int> IDs);
	void UpdateBroadcastOfEverybody();

	/**
	 * allows the usage of UpdateSkinsOf({multiple, ids})
	 * sends to everyone that the skin information of those
	 * multiple ids has changed. (color change)
	 */
	void UpdateSkinsOf(std::initializer_list<int> IDs);
	void UpdateSkinsOfEverybody();
	
	/**
	 * Used to track, whether we reached the players count needed to
	 * end a round or whether we went below that treshold.
	 */
	int m_PreviousIngamePlayerCount;
	int m_IngamePlayerCount;


	// refresh time in ticks, after how many ticks 
	// the broadcast refresh is being resent.
	int m_BroadcastRefreshTime;


	// an encapsulated class, that handles the database connection.
	IRankingServer* m_pRankingServer;

	// initializes ranking server
	void InitRankingServer();

	// fill s player stats with data from the database
	void RetrieveRankingData(int ofID);

	// saves player data to the database
	void SaveRankingData(int ofID);

	// depending on the current mod, grenade, laser, etc, 
	// a prefix is created for the specific database columns, fields, etc.
	std::string GetDatabasePrefix();


	// based on the Total Players caught, the winner 
	// receives a specific score rating.
	// calculates an 10 * e^(x/5) / e^((MAX_PLAYERS - 1) / 5) function, 
	// that is normalized between 0 and 10 and discretized to integer values.
	int CalculateScore(int PlayersCaught);


	// if a player requests data from the database, the retrieved messages are firstly pu into this queue
	// in order for them to be handled in the main thread. As sending packages to specific clients can only be done in
	// the main thread, because teeworlds does not support multithreading on the server side.
	std::mutex m_MessageQueueMutex;
	std::vector<std::pair<int, std::vector<std::string>> > m_MessageQueue;

	// try to send messages in the MessageQueue to the requesting players.
	void ProcessMessageQueue();

	// handles the filling of the MessageQueue for the /rank <nickname> command
	void RequestRankingData(int requestingID, std::string ofNickname);

	// handles the filling of the MessageQueue for the /top command
	void RequestTopRankingData(int requestingID, std::string key);

};

#endif
