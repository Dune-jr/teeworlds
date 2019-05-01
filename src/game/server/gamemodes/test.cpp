#include "test.h"
#include <engine/server.h>
#include <engine/shared/protocol.h>
#include <game/server/gamecontext.h>
#include <game/server/player.h>
#include <game/server/entities/character.h>
#include <game/my_protocol.h>

// TODO: move this
template<typename T>
void CGameControllerTEST::SendDukNetObj(const T& NetObj, int CID)
{
	CMsgPacker Msg(NETMSG_DUCK_NETOBJ, false);
	Msg.AddInt((int)T::NET_ID);
	Msg.AddInt((int)sizeof(NetObj));
	Msg.AddRaw(&NetObj, sizeof(NetObj));
	Server()->SendMsg(&Msg, MSGFLAG_VITAL, CID);
}

CGameControllerTEST::CGameControllerTEST(class CGameContext *pGameServer)
: IGameController(pGameServer)
{
	m_pGameType = "TEST";

	// load duck mod
	if(!Server()->LoadDuckMod("", "", "data/mods/test"))
	{
		dbg_msg("server", "failed to load duck mod");
	}
}

void CGameControllerTEST::Tick()
{
	IGameController::Tick();

	vec2 HookBlockPos = vec2(800, 500);
	vec2 HookBlockSize = vec2(100, 50);
	vec2 HookBlockVel = vec2(0, 0);

	for(int p = 0; p < MAX_PLAYERS; p++)
	{
		if(GameServer()->m_apPlayers[p])
		{
			if(GameServer()->m_apPlayers[p]->IsDummy())
				continue;

			CNetObj_DebugRect Rect;
			Rect.id = 0;
			Rect.x = HookBlockPos.x;
			Rect.y = HookBlockPos.y;
			Rect.w = HookBlockSize.x;
			Rect.h = HookBlockSize.y;
			Rect.color = 0xffff00ff;
			SendDukNetObj(Rect, p);

			CNetObj_HookBlock NetHookBlock;
			NetHookBlock.m_Id = 0;
			NetHookBlock.m_PosX = HookBlockPos.x;
			NetHookBlock.m_PosY = HookBlockPos.y;
			NetHookBlock.m_VelX = HookBlockVel.x;
			NetHookBlock.m_VelY = HookBlockVel.y;
			NetHookBlock.m_Width = HookBlockSize.x;
			NetHookBlock.m_Height = HookBlockSize.y;
			SendDukNetObj(NetHookBlock, p);
		}
	}
}
