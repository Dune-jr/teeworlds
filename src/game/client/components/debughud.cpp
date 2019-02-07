/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <engine/shared/config.h>
#include <engine/graphics.h>
#include <engine/textrender.h>
#include <engine/input.h>
#include <engine/keys.h>

#include <generated/protocol.h>
#include <generated/client_data.h>

#include <game/layers.h>

#include <game/client/gameclient.h>
#include <game/client/animstate.h>
#include <game/client/render.h>

//#include "controls.h"
//#include "camera.h"
#include "debughud.h"
#include "menus.h" // CButtonContainer

void CDebugHud::RenderNetCorrections()
{
	if(!g_Config.m_Debug || g_Config.m_DbgGraphs || !m_pClient->m_Snap.m_pLocalCharacter || !m_pClient->m_Snap.m_pLocalPrevCharacter)
		return;

	float Width = 300*Graphics()->ScreenAspect();
	Graphics()->MapScreen(0, 0, Width, 300);

	/*float speed = distance(vec2(netobjects.local_prev_character->x, netobjects.local_prev_character->y),
		vec2(netobjects.local_character->x, netobjects.local_character->y));*/

	float Velspeed = length(vec2(m_pClient->m_Snap.m_pLocalCharacter->m_VelX/256.0f, m_pClient->m_Snap.m_pLocalCharacter->m_VelY/256.0f))*50;
	float Ramp = VelocityRamp(Velspeed, m_pClient->m_Tuning.m_VelrampStart, m_pClient->m_Tuning.m_VelrampRange, m_pClient->m_Tuning.m_VelrampCurvature);

	const char *paStrings[] = {"velspeed:", "velspeed*ramp:", "ramp:", "Pos", " x:", " y:", "netmsg failed on:", "netobj num failures:", "netobj failed on:"};
	const int Num = sizeof(paStrings)/sizeof(char *);
	const float LineHeight = 6.0f;
	const float Fontsize = 5.0f;

	float x = Width-100.0f, y = 50.0f;
	for(int i = 0; i < Num; ++i)
		TextRender()->Text(0, x, y+i*LineHeight, Fontsize, paStrings[i], -1);

	x = Width-10.0f;
	char aBuf[128];
	str_format(aBuf, sizeof(aBuf), "%.0f", Velspeed/32);
	float w = TextRender()->TextWidth(0, Fontsize, aBuf, -1);
	TextRender()->Text(0, x-w, y, Fontsize, aBuf, -1);
	y += LineHeight;
	str_format(aBuf, sizeof(aBuf), "%.0f", Velspeed/32*Ramp);
	w = TextRender()->TextWidth(0, Fontsize, aBuf, -1);
	TextRender()->Text(0, x-w, y, Fontsize, aBuf, -1);
	y += LineHeight;
	str_format(aBuf, sizeof(aBuf), "%.2f", Ramp);
	w = TextRender()->TextWidth(0, Fontsize, aBuf, -1);
	TextRender()->Text(0, x-w, y, Fontsize, aBuf, -1);
	y += 2*LineHeight;
	str_format(aBuf, sizeof(aBuf), "%d", m_pClient->m_Snap.m_pLocalCharacter->m_X/32);
	w = TextRender()->TextWidth(0, Fontsize, aBuf, -1);
	TextRender()->Text(0, x-w, y, Fontsize, aBuf, -1);
	y += LineHeight;
	str_format(aBuf, sizeof(aBuf), "%d", m_pClient->m_Snap.m_pLocalCharacter->m_Y/32);
	w = TextRender()->TextWidth(0, Fontsize, aBuf, -1);
	TextRender()->Text(0, x-w, y, Fontsize, aBuf, -1);
	y += LineHeight;
	w = TextRender()->TextWidth(0, Fontsize, m_pClient->NetmsgFailedOn(), -1);
	TextRender()->Text(0, x-w, y, Fontsize, m_pClient->NetmsgFailedOn(), -1);
	y += LineHeight;
	str_format(aBuf, sizeof(aBuf), "%d", m_pClient->NetobjNumFailures());
	w = TextRender()->TextWidth(0, Fontsize, aBuf, -1);
	TextRender()->Text(0, x-w, y, Fontsize, aBuf, -1);
	y += LineHeight;
	w = TextRender()->TextWidth(0, Fontsize, m_pClient->NetobjFailedOn(), -1);
	TextRender()->Text(0, x-w, y, Fontsize, m_pClient->NetobjFailedOn(), -1);
}


void CDebugHud::MakeKeyButton(CUIRect Area, int Key, bool DynamicWidth, float w, float h)
{
	if(Key == KEY_MOUSE_1)
	{
		CUIRect Button = {Area.x+w/8.f, Area.y+h, w, h};
		MakeIcon(Button, IMAGE_MOUSEICONS, SPRITE_MOUSE1, 0, 0, vec2(3.f*w/4.f, h));
	}
	else if(Key == KEY_MOUSE_2)
	{
		CUIRect Button = {Area.x+w/8.f, Area.y+h, w, h};
		MakeIcon(Button, IMAGE_MOUSEICONS, SPRITE_MOUSE2, 0, 0, vec2(3.f*w/4.f, h));
	}
	else
	{
		CUIRect Button = {Area.x, Area.y, w, h};
		if(DynamicWidth && Key == KEY_SPACE) // extend key size for space
		{
			Button.x -= Button.w/2.f;
			Button.w *= 2.f;
		}
		
		if(Input()->KeyIsPressed(Key))
		{
			RenderTools()->DrawUIRect(&Button, vec4(0.3f, 0.3f, 0.3f, 0.66f) , CUI::CORNER_ALL, 4.0f);
		  	TextRender()->TextOutlineColor(0.0f, 0.0f, 0.0f, 0.25f);
			TextRender()->TextColor(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			RenderTools()->DrawUIRect(&Button, vec4(0.9f, 0.9f, 0.9f, 0.66f) , CUI::CORNER_ALL, 4.0f);
		  	TextRender()->TextOutlineColor(1.0f, 1.0f, 1.0f, 0.25f);
			TextRender()->TextColor(0.0f, 0.0f, 0.0f, 1.0f);
		}
		Button.y += Button.h/2-4.0f;

		char aBuf[8];
		str_format(aBuf, DynamicWidth ? 8 : 4, Input()->KeyName(Key)); // truncate to 3 chars if no dynamic width
		aBuf[0] = str_uppercase(aBuf[0]);
		UI()->DoLabel(&Button, aBuf, 6.0f, CUI::ALIGN_CENTER);
	}
}

void CDebugHud::MakeLabel(CUIRect Area, const char* pStr, float x, float y, float w, float h)
{
	CUIRect Label = {Area.x + x, Area.y + y, w, h};
  	TextRender()->TextOutlineColor(0.0f, 0.0f, 0.0f, 0.3f);
	TextRender()->TextColor(1.0f, 1.0f, 1.0f, 1.0f);
	UI()->DoLabel(&Label, pStr, 6.0f, CUI::ALIGN_CENTER);
}

void CDebugHud::MakeIcon(CUIRect Area, int ImageId, int SpriteId, float x, float y, vec2 Size, vec4 Color)
{
	CUIRect Icon = {Area.x + x, Area.y + y - Size.y, Size.x, Size.y};
	if(true)
	{
		Graphics()->TextureSet(g_pData->m_aImages[ImageId].m_Id);
		Graphics()->QuadsBegin();
		if(SpriteId != -1)
			RenderTools()->SelectSprite(SpriteId);
		else
			Graphics()->QuadsSetSubset(1, 0, 0, 1);
		IGraphics::CQuadItem QuadItem(Icon.x, Icon.y, Icon.w, Icon.h);
		Graphics()->SetColor(Color.r, Color.g, Color.b, Color.a);
		Graphics()->QuadsDrawTL(&QuadItem, 1);
	}
	else
	{
		Graphics()->TextureSet(g_pData->m_aImages[IMAGE_GAME].m_Id);
		Graphics()->QuadsBegin();
		RenderTools()->SelectSprite(SPRITE_WEAPON_SHOTGUN_PROJ);
		IGraphics::CQuadItem QuadItem(Icon.x, Icon.y, 8.f, 8.f);
		Graphics()->SetColor(Color.r, Color.g, Color.b, Color.a);
		Graphics()->QuadsDrawTL(&QuadItem, 1);
	}
	Graphics()->QuadsEnd();
}

void CDebugHud::RenderHints()
{
	// render hints
	if(!g_Config.m_DbgTuning)
	{
		// const int Mode = 0; // original
		const int Mode = 1; // alternative

		const float Height = 300.0f;
		const float Width = Height*Graphics()->ScreenAspect();
		const float UsedWidthRatio = 5.f/7.f;
		Graphics()->MapScreen(0.0f, 0.0f, Width, Height);

		CUIRect Area;
		Area.x = (1-UsedWidthRatio)/2.f*Width;
		Area.y = Height-70.0f;
		Area.w = UsedWidthRatio*Width;
		Area.h = 50.f;
		RenderTools()->DrawUIRect(&Area, vec4(0.0f, 0.0f, 0.0f, 0.5f) , CUI::CORNER_ALL, 5.0f);

		{
			// quit button
			CUIRect Button, Row;
			float TopOffset = 12.0f;
			Area.HSplitTop(TopOffset, &Row, 0);
			Row.VSplitRight(TopOffset/* - 3.0f*/, &Row, &Button);

			static CMenus::CButtonContainer s_QuitButton;
			// draw red-blending button

			// buttonfade
			float Fade = 0.0f;
			{
				if(UI()->HotItem() == s_QuitButton.GetID())
				{
					s_QuitButton.m_FadeStartTime = Client()->LocalTime();
					Fade = 0.6f;
				}
				else
					Fade = max(0.0f, s_QuitButton.m_FadeStartTime -  Client()->LocalTime() + 0.6f);
			}

			vec4 Color = mix(vec4(0.f, 0.f, 0.f, 0.25f), vec4(1.f/0xff*0xf9, 1.f/0xff*0x2b, 1.f/0xff*0x2b, 0.75f), Fade /*ButtonFade(&s_QuitButton, 0.6f, 0)/0.6f*/);
			RenderTools()->DrawUIRect(&Button, Color, CUI::CORNER_ALL, 3.0f);

			// draw non-blending X
			CUIRect XText = Button;
			// XText.HMargin(Button.h>=20.0f?2.0f:1.0f, &XText);
			
			UI()->DoLabel(&XText, "\xE2\x9C\x95", XText.h*0.8f, CUI::ALIGN_CENTER);
			if(UI()->DoButtonLogic(s_QuitButton.GetID(), "\xE2\x9C\x95", 0, &Button))
			// if(DoButton_SpriteCleanID(&s_QuitButton, IMAGE_FRIENDICONS, SPRITE_FRIEND_X_A, &Button, false))
				g_Config.m_DbgTuning = 1;
		}
		
		Area.HMargin(10.0f, &Area);
		Area.VMargin(10.0f, &Area);

		const float ButtonSize = 12.0f;
		const float Spacing = ButtonSize+1.0f;
		Area.x += 10.f;
		Area.y += 10.f;

		MakeLabel(Area, Localize("Move"), 0, Spacing, Spacing*2);
		MakeKeyButton(Area, KEY_A);
		Area.x += Spacing;
		MakeKeyButton(Area, KEY_D);
		Area.x += Spacing;

		Area.x += Spacing/2;

		Area.x += Spacing/2;
		MakeKeyButton(Area, KEY_SPACE, true);
		MakeLabel(Area, Localize("Jump"), 0, Spacing, Spacing);
		Area.x += Spacing;

		Area.x += Spacing;

		MakeLabel(Area, Localize("Fire & Hook"), 0, Spacing, Spacing*2);
		MakeKeyButton(Area, KEY_MOUSE_1);
		Area.x += Spacing;
		MakeKeyButton(Area, KEY_MOUSE_2);
		Area.x += Spacing;

		Area.x += Spacing/2;

		MakeLabel(Area, Localize("Change weapon"), 0, Spacing, Spacing*5);
		if(Mode == 0)
		{
			MakeKeyButton(Area, KEY_1);
			MakeIcon(Area, IMAGE_GAME, SPRITE_WEAPON_HAMMER_BODY, 0, -1, vec2(4, 3)*3.f);
			Area.x += Spacing;
			MakeKeyButton(Area, KEY_2);
			MakeIcon(Area, IMAGE_GAME, SPRITE_WEAPON_GUN_BODY, 0, -2, vec2(4, 2)*3.f);
			Area.x += Spacing;
			MakeKeyButton(Area, KEY_3);
			MakeIcon(Area, IMAGE_GAME, SPRITE_WEAPON_SHOTGUN_BODY, -1, -3, vec2(8, 2)*2.f);
			Area.x += Spacing;
			MakeKeyButton(Area, KEY_4);
			MakeIcon(Area, IMAGE_GAME, SPRITE_WEAPON_GRENADE_BODY, 0, -3, vec2(7, 2)*2.f);
			Area.x += Spacing;
			MakeKeyButton(Area, KEY_5);
			MakeIcon(Area, IMAGE_GAME, SPRITE_WEAPON_LASER_BODY, 0, -2, vec2(7, 3)*2.f);
			Area.x += Spacing;
		}
		else
		{
			MakeKeyButton(Area, KEY_1);
			MakeIcon(Area, IMAGE_GAME, SPRITE_WEAPON_HAMMER_CURSOR, 2, -2, vec2(2, 2)*4.f);
			Area.x += Spacing;
			MakeKeyButton(Area, KEY_2);
			MakeIcon(Area, IMAGE_GAME, SPRITE_WEAPON_GUN_CURSOR, 2, -2, vec2(2, 2)*4.f);
			Area.x += Spacing;
			MakeKeyButton(Area, KEY_3);
			MakeIcon(Area, IMAGE_GAME, SPRITE_WEAPON_SHOTGUN_CURSOR, 2, -2, vec2(2, 2)*4.f);
			Area.x += Spacing;
			MakeKeyButton(Area, KEY_4);
			MakeIcon(Area, IMAGE_GAME, SPRITE_WEAPON_GRENADE_CURSOR, 2, -2, vec2(2, 2)*4.f);
			Area.x += Spacing;
			MakeKeyButton(Area, KEY_5);
			MakeIcon(Area, IMAGE_GAME, SPRITE_WEAPON_LASER_CURSOR, 2, -2, vec2(2, 2)*4.f);
			Area.x += Spacing;
		}

		Area.x += Spacing;

		const vec4 CRCWhite(1, 1, 1, 0.25);
		const vec4 CRCTeam(0.4, 1, 0.4, 0.4);
		const vec4 CRCWhisper(0, 0.5, 1, 0.5);

		MakeLabel(Area, Localize("Chat"), 0, Spacing, Spacing*3);
		MakeIcon(Area, IMAGE_EMOTICONS, SPRITE_DOTDOT, 1, -2, vec2(10.f, 10.f), CRCWhite);
		MakeKeyButton(Area, KEY_T);
		Area.x += Spacing;	
		MakeIcon(Area, IMAGE_EMOTICONS, SPRITE_DOTDOT, 1, -2, vec2(10.f, 10.f), CRCTeam);
		MakeKeyButton(Area, KEY_Y);
		Area.x += Spacing;
		if(Mode == 0)
			MakeIcon(Area, IMAGE_CHATWHISPER, -1, 1, -3, vec2(12.f, 6.f), CRCWhisper);
		else
			MakeIcon(Area, IMAGE_EMOTICONS, SPRITE_DOTDOT, 1, -2, vec2(10.f, 10.f), CRCWhisper);
		MakeKeyButton(Area, KEY_X);
		Area.x += Spacing;

		Area.x += Spacing;

		MakeLabel(Area, Localize("Menu"), 0, Spacing, Spacing);
		MakeKeyButton(Area, KEY_ESCAPE);
		Area.x += Spacing;
		
  		TextRender()->TextOutlineColor(0.0f, 0.0f, 0.0f, 0.3f);
		TextRender()->TextColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

void CDebugHud::RenderTuning()
{
	// render tuning
	if(!g_Config.m_DbgTuning)
		return;

	CTuningParams StandardTuning;

	Graphics()->MapScreen(0, 0, 300*Graphics()->ScreenAspect(), 300);

	float y = 50.0f;
	int Count = 0;
	for(int i = 0; i < m_pClient->m_Tuning.Num(); i++)
	{
		char aBuf[128];
		float Current, Standard;
		m_pClient->m_Tuning.Get(i, &Current);
		StandardTuning.Get(i, &Standard);

		if(Standard == Current)
			TextRender()->TextColor(1,1,1,1.0f);
		else
			TextRender()->TextColor(1,0.25f,0.25f,1.0f);

		float w;
		float x = 5.0f;

		str_format(aBuf, sizeof(aBuf), "%.2f", Standard);
		x += 20.0f;
		w = TextRender()->TextWidth(0, 5, aBuf, -1);
		TextRender()->Text(0x0, x-w, y+Count*6, 5, aBuf, -1);

		str_format(aBuf, sizeof(aBuf), "%.2f", Current);
		x += 20.0f;
		w = TextRender()->TextWidth(0, 5, aBuf, -1);
		TextRender()->Text(0x0, x-w, y+Count*6, 5, aBuf, -1);

		x += 5.0f;
		TextRender()->Text(0x0, x, y+Count*6, 5, m_pClient->m_Tuning.m_apNames[i], -1);

		Count++;
	}

	y = y+Count*6;

	Graphics()->TextureClear();
	Graphics()->BlendNormal();
	Graphics()->LinesBegin();
	float Height = 50.0f;
	float pv = 1;
	IGraphics::CLineItem Array[100];
	for(int i = 0; i < 100; i++)
	{
		float Speed = i/100.0f * 3000;
		float Ramp = VelocityRamp(Speed, m_pClient->m_Tuning.m_VelrampStart, m_pClient->m_Tuning.m_VelrampRange, m_pClient->m_Tuning.m_VelrampCurvature);
		float RampedSpeed = (Speed * Ramp)/1000.0f;
		Array[i] = IGraphics::CLineItem((i-1)*2, y+Height-pv*Height, i*2, y+Height-RampedSpeed*Height);
		//Graphics()->LinesDraw((i-1)*2, 200, i*2, 200);
		pv = RampedSpeed;
	}
	Graphics()->LinesDraw(Array, 100);
	Graphics()->LinesEnd();
	TextRender()->TextColor(1,1,1,1);
}

void CDebugHud::OnRender()
{
	RenderHints();
	RenderTuning();
	RenderNetCorrections();
}
