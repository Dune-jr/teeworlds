/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <base/color.h>
#include <engine/demo.h>
#include <engine/graphics.h>
#include <generated/protocol.h>
#include <generated/client_data.h>

#include <game/client/ui.h>
#include <game/client/render.h>
#include "damageind.h"

// CDamageInd methods
CDamageInd::CDamageInd()
{
	m_NumItems = 0;
}

CDamageInd::CItem *CDamageInd::CreateI()
{
	if (m_NumItems < MAX_ITEMS)
	{
		CItem *p = &m_aItems[m_NumItems];
		m_NumItems++;
		return p;
	}
	return 0;
}

void CDamageInd::DestroyI(CDamageInd::CItem *i)
{
	m_NumItems--;
	*i = m_aItems[m_NumItems];
}

void CDamageInd::Create(vec2 Pos, vec2 Dir)
{
	CItem *i = CreateI();
	if (i)
	{
		i->m_Pos = Pos;
		i->m_StartTime = Client()->LocalTime();
		i->m_Dir = Dir*-1;
		i->m_StartAngle = (frandom() - 1.0f) * 2.0f * pi;
	}
}

void CDamageInd::OnRender()
{
	Graphics()->TextureSet(g_pData->m_aImages[IMAGE_GAME].m_Id);
	Graphics()->QuadsBegin();
	static float s_LastLocalTime = Client()->LocalTime();
	for(int i = 0; i < m_NumItems;)
	{
		if(Client()->State() == IClient::STATE_DEMOPLAYBACK)
		{
			const IDemoPlayer::CInfo *pInfo = DemoPlayer()->BaseInfo();
			if(pInfo->m_Paused)
				m_aItems[i].m_StartTime += Client()->LocalTime()-s_LastLocalTime;
			else
				m_aItems[i].m_StartTime += (Client()->LocalTime()-s_LastLocalTime)*(1.0f-pInfo->m_Speed);
		}
		else
		{
			if(m_pClient->m_Snap.m_pGameData && m_pClient->m_Snap.m_pGameData->m_GameStateFlags&GAMESTATEFLAG_PAUSED)
				m_aItems[i].m_StartTime += Client()->LocalTime()-s_LastLocalTime;
		}

		float Life = 0.75f - (Client()->LocalTime() - m_aItems[i].m_StartTime);
		if(Life < 0.0f)
			DestroyI(&m_aItems[i]);
		else
		{
			vec2 Pos = mix(m_aItems[i].m_Pos+m_aItems[i].m_Dir*75.0f, m_aItems[i].m_Pos, clamp((Life-0.60f)/0.15f, 0.0f, 1.0f));
			const float Alpha = clamp(Life * 10.0f, 0.0f, 1.0f); // 0.1 -> 0.0 == 1.0 -> 0.0
			Graphics()->SetColor(1.0f*Alpha, 1.0f*Alpha, 1.0f*Alpha, Alpha);
			Graphics()->QuadsSetRotation(m_aItems[i].m_StartAngle + Life * 2.0f);
			RenderTools()->SelectSprite(SPRITE_STAR1);
			RenderTools()->DrawSprite(Pos.x, Pos.y, 48.0f);
			i++;
		}
	}
	s_LastLocalTime = Client()->LocalTime();
	Graphics()->QuadsEnd();
}

void CDamageInd::OnReset()
{
	m_NumItems = 0;
}

// CSoundInd methods
CSoundInd::CSoundInd()
{
	m_NumItems = 0;
}

CSoundInd::CItem *CSoundInd::CreateI()
{
	if (m_NumItems < MAX_ITEMS)
	{
		CItem *p = &m_aItems[m_NumItems];
		m_NumItems++;
		return p;
	}
	return 0;
}

void CSoundInd::DestroyI(CSoundInd::CItem *i)
{
	m_NumItems--;
	*i = m_aItems[m_NumItems];
}

void CSoundInd::Create(vec2 Pos, int Type)
{
	CItem *i = CreateI();
	if (i)
	{
		i->m_Pos = Pos;
		i->m_StartTime = Client()->LocalTime();
		i->m_Type = Type;
	}
}

static vec3 getColorOfType(int Type)
{
	// Magic value is 3
	float Hue = ((90+Type*3)%100)/100.0f;
	return HslToRgb(vec3(Hue, 84/100.0f, 50/100.0f));
}

static const char* getStringOfSoundType(int Type)
{
	dbg_assert(Type >= 0, "Sonud type < 0");
	const char* apSoundStrs[] = {
		"GUN_FIRE", "SHOTGUN_FIRE", "GRENADE_FIRE", "HAMMER_FIRE", "HAMMER_HIT", "NINJA_FIRE", "GRENADE_EXPLODE", "NINJA_HIT",
		"RIFLE_FIRE", "RIFLE_BOUNCE", "WEAPON_SWITCH", "PLAYER_PAIN_SHORT", "PLAYER_PAIN_LONG", "BODY_LAND", "PLAYER_AIRJUMP",
		"PLAYER_JUMP", "PLAYER_DIE", "PLAYER_SPAWN", "PLAYER_SKID", "TEE_CRY", "HOOK_LOOP", "HOOK_ATTACH_GROUND", "HOOK_ATTACH_PLAYER",
		"HOOK_NOATTACH", "PICKUP_HEALTH", "PICKUP_ARMOR", "PICKUP_GRENADE", "PICKUP_SHOTGUN", "PICKUP_NINJA", "WEAPON_SPAWN", "WEAPON_NOAMMO",
		"HIT", "CHAT_SERVER", "CHAT_CLIENT", "CHAT_HIGHLIGHT", "CTF_DROP", "CTF_RETURN", "CTF_GRAB_PL", "CTF_GRAB_EN", "CTF_CAPTURE", "MENU"
	};
	return apSoundStrs[Type];
}

void CSoundInd::OnRender()
{
	static float s_LastLocalTime = Client()->LocalTime();
	for(int i = 0; i < m_NumItems;)
	{
		if(Client()->State() == IClient::STATE_DEMOPLAYBACK)
		{
			const IDemoPlayer::CInfo *pInfo = DemoPlayer()->BaseInfo();
			if(pInfo->m_Paused)
				m_aItems[i].m_StartTime += Client()->LocalTime()-s_LastLocalTime;
			else
				m_aItems[i].m_StartTime += (Client()->LocalTime()-s_LastLocalTime)*(1.0f-pInfo->m_Speed);
		}
		else
		{
			if(m_pClient->m_Snap.m_pGameData && m_pClient->m_Snap.m_pGameData->m_GameStateFlags&GAMESTATEFLAG_PAUSED)
				m_aItems[i].m_StartTime += Client()->LocalTime()-s_LastLocalTime;
		}

		float Life = 0.75f - (Client()->LocalTime() - m_aItems[i].m_StartTime);
		if(Life < 0.0f)
			DestroyI(&m_aItems[i]);
		else
		{
			vec2 Pos = m_aItems[i].m_Pos;
			const float Alpha = clamp(Life * 10.0f, 0.0f, 1.0f); // 0.1 -> 0.0 == 1.0 -> 0.0
			const float Size = 72.0f*clamp(Life/0.75f, 0.0f, 1.0f); //clamp((Life-0.60f)/0.15f, 0.0f, 1.0f);
			CUIRect IndicatorRect = {Pos.x - Size/2.0f, Pos.y - Size/2.0f, Size, Size};
			vec3 Color = getColorOfType(m_aItems[i].m_Type);
			RenderTools()->DrawUIRect(&IndicatorRect, vec4(Color.r*Alpha, Color.g*Alpha, Color.b*Alpha, Alpha/2.0f), CUI::CORNER_ALL,Size/2.0f);
			if(Life > 0.25f)
			{
				IndicatorRect.y += Size/2.0f - 4.0f;
				UI()->DoLabel(&IndicatorRect, getStringOfSoundType(m_aItems[i].m_Type), 8.0f, CUI::ALIGN_CENTER);
			}
			i++;
		}
	}
	s_LastLocalTime = Client()->LocalTime();
}

void CSoundInd::OnReset()
{
	m_NumItems = 0;
}
