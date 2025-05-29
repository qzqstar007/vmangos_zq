/* Copyright (C) 2009 - 2010 ScriptDevZero <http://github.com/scriptdevzero/scriptdevzero>
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "scriptPCH.h"
#include "custom.h"
#include "ScriptedAI.h"
#include <ctime>

#include "Group.h"

#include "qzqstar_hs.h"
#include "QzqstarAchievements.h"
#include "qzqstar_id.h"
#include "qzqstar_rune_str.h"


#define	__MENU_NONE						0
#define	__MENU_MAIN						1
#define __MENU_HOME_MAIN				2
#define __MENU_GAZAGAN_MAIN				3
#define __MENU_SIZE						999
#define __MENU_MODE_MAIN				1000
#define __MENU_CITIES_MAIN				2000
#define __MENU_SUISHEN_MAIN				5000
#define __MENU_TEAM_MAIN				6000
#define __MENU_ZITIAO_MAIN				7000
#define __MENU_END						20000

#define	__STR(x)		((std::string)(x)).c_str()
#define	__NSTR(x)		(std::to_string(x))
	
#define	__BLUE(x)		"|cff002fa7"##x##"|r"
#define	__GREEN(x)		"|cff00b72f"##x##"|r"
#define	__ORANGE(x)		"|cffe85827"##x##"|r"
#define __RED(x)		"|cfff00019"##x##"|r"
#define __YELLOW(x)		"|cfff9dc24"##x##"|r"



void _Main_Menus(Player *player)
{
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);

	if (player->GetLevel() == 1)
	{
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("☆☆　挑战模式（一级可选）☆☆　")), GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
	}

	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　使用炉石　＜＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_HOME_MAIN);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　传送加基森　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_GAZAGAN_MAIN);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　主城传送　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　字条法宝　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_ZITIAO_MAIN);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　随身功能　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);

	if(player->GetLevel() >= 2)
	{
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　团队功能　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TEAM_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
	}
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
}

bool static __localHandleGroupCommand(Player *pPlayer, uint32 action)
{
    //Player* pPlayer = m_session->GetPlayer();
    Group* pGroup = pPlayer->GetGroup();
    if (!pGroup)
    {
		pPlayer->PSendSysMessage(__STR("该功能只能组队使用。 "));
        return false;
    }

    for (GroupReference* itr = pGroup->GetFirstMember(); itr != nullptr; itr = itr->next())
    {
        if (Player* pMember = itr->getSource())
        {
            if (pMember == pPlayer)
                continue;

			//check the action
			switch(action)
			{
				case 10:
				{
					//summon request
					pMember->SendSummonRequest(pPlayer->GetObjectGuid(), pPlayer->GetMapId(), pPlayer->GetZoneId(), pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ());
					break;
				}

				case 20:
				{
					//revive request
					if (pMember->IsDead())
					{
						pMember->ResurrectPlayer(0.2f);
						pMember->SpawnCorpseBones();
					}
					break;
				}

				case 30:
				{
					//fufill request
					if (pMember->IsAlive())
					{
						pMember->SetHealth(pMember->GetMaxHealth());
						if (pMember->GetPowerType() == POWER_MANA)
							pMember->SetPower(POWER_MANA, pMember->GetMaxPower(POWER_MANA));
					}

					break;
				}
			}
		}
    }

	pPlayer->PSendSysMessage(__STR("团队功能已成功释放。 "));
    return true;
}

struct CustomHSSpell : SpellScript
{
	void OnSuccessfulFinish(Spell* spell) const final
	{
		if(Player *player = (spell->m_casterUnit)->ToPlayer())
		{
			player->PlayerTalkClass->ClearMenus();
			player->M_Gossip_Spell_Ind = 1998;
			
			_Main_Menus(player);
		}

		else
		{
			sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "Error of Spell_HS, not a player??.");
		}
	}

	void OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action) const final
	{
		//sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "test. %d %d", sender, action);
		pPlayer->CLOSE_GOSSIP_MENU();

		if(action == __MENU_MAIN)
		{
			_Main_Menus(pPlayer);	return;
		}
		else if(action == __MENU_HOME_MAIN)
		{
			pPlayer->TeleportToHomebind(); return;
		}
		else if (action == __MENU_GAZAGAN_MAIN)
		{
			pPlayer->CastSpell(pPlayer, 23441, true); return;
		}
		else if (action >= __MENU_MODE_MAIN && action <= __MENU_MODE_MAIN + __MENU_SIZE)
		{
			// display the challenging mode
			uint32 _Mode = sQZAchievements.GetChallengeMode(pPlayer);
			if(action == __MENU_MODE_MAIN)
			{
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝　挑战模式状态　＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
				if(_Mode & CHALLENGING_MODE_ONELIFE) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　一命模式　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
				else pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＝＞　开启一命模式　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN + 1);

				if(_Mode & CHALLENGING_MODE_MANUFACT) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　工匠模式　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
				else pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＝＞　开启工匠模式　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN + 2);

				if(_Mode & CHALLENGING_MODE_TASK) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　任务模式　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
				else pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＝＞　开启任务模式　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN + 3);

				if(_Mode & CHALLENGING_MODE_COLLECT) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　收藏模式　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
				else pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＝＞　开启收藏模式　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN + 4);

				if(_Mode & CHALLENGING_MODE_RICH) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　富豪模式　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
				else pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＝＞　开启富豪模式　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN + 5);

				if(_Mode >= CHALLENGING_MODE_KILLER_HUMAN) 
				{
					if(_Mode & CHALLENGING_MODE_KILLER_HUMAN)pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　杀手模式（人形）　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
					else if(_Mode & CHALLENGING_MODE_KILLER_BEAST) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　杀手模式（野兽）　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
					else if(_Mode & CHALLENGING_MODE_KILLER_UNDEAD) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　杀手模式（亡灵）　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
				}
				else
				{
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞　开启杀手模式（人形）　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN + 10);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞　开启杀手模式（野兽）　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN + 11);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞　开启杀手模式（亡灵）　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN + 12);
				}

			}

			else if(action >= __MENU_MODE_MAIN + 1 && action <= __MENU_MODE_MAIN + 19)
			{
				//change the challenging mode
				auto _realAction = action - __MENU_MODE_MAIN;
				switch(_realAction)
				{
					case 1:	 _Mode |= CHALLENGING_MODE_ONELIFE; break;
					case 2:	 _Mode |= CHALLENGING_MODE_MANUFACT; break;
					case 3:	 _Mode |= CHALLENGING_MODE_TASK; break;
					case 4:	 _Mode |= CHALLENGING_MODE_COLLECT; break;
					case 5:	 _Mode |= CHALLENGING_MODE_RICH; break;
					case 10: _Mode |= CHALLENGING_MODE_KILLER_HUMAN; break;
					case 11: _Mode |= CHALLENGING_MODE_KILLER_BEAST; break;
					case 12: _Mode |= CHALLENGING_MODE_KILLER_UNDEAD; break;
				}
				sQZAchievements.SetChallengeMode(pPlayer, _Mode);
				pPlayer->M_Challenge_Mode = _Mode;
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　挑战模式已开启　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN);
			}

			pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
		}
		else if (action >= __MENU_CITIES_MAIN && action <= __MENU_CITIES_MAIN + __MENU_SIZE)
		{
			//display the cities
			if(action == __MENU_CITIES_MAIN)
			{
				if (pPlayer->GetTeam() == ALLIANCE)
				{
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＝＞　暴风城　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 11);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＝＞　铁炉堡　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 12);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＝＞　达纳苏斯　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 13);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);

					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＞　人类出生地　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 14);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＞　矮人出生地　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 15);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＞　精灵出生地　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 16);
				}
				else //HORDE
				{

					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＝＞　奥格瑞玛　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 21);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＝＞　幽暗城　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 22);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＝＞　雷霆崖　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 23);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);

					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＞　兽人出生地　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 24);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＞　亡灵出生地　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 25);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＞　牛头出生地　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 26);

				}

				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＝＞　永望镇　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 31);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＝＞　藏宝海湾　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 32);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＝＞　棘齿城　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 33);
				
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
				return;
			}

			else
			{
				uint32 _realAction = action - __MENU_CITIES_MAIN;
				switch(_realAction)
				{
				case 11:	pPlayer->TeleportTo(MAP_EASTERN_KINGDOMS, -8828.231445f, 627.927490f, 94.055664f, 0.0f); return;
				case 12:	pPlayer->TeleportTo(MAP_EASTERN_KINGDOMS, -4917.0f, -955.0f, 502.0f, 0.0f); return;
				case 13:	pPlayer->TeleportTo(MAP_KALIMDOR, 9962.712891f, 2280.142822f, 1341.394409f, 0.0f); return;
				
				case 14:	pPlayer->TeleportTo(MAP_EASTERN_KINGDOMS, -8943.133789f, -132.934921f, 83.704269f, 0.0f); return;
				case 15:	pPlayer->TeleportTo(MAP_EASTERN_KINGDOMS, -6231.106445f, 332.270477f, 383.153931f, 0.0f); return;
				case 16:	pPlayer->TeleportTo(MAP_KALIMDOR, 10329.918945f, 833.500305f, 1326.260620f, 0.0f); return;

				case 21:	pPlayer->TeleportTo(MAP_KALIMDOR, 1437.0f, -4421.0f, 25.24f, 1.65f); return;
				case 22:	pPlayer->TeleportTo(MAP_EASTERN_KINGDOMS, 1822.0999f, 238.638855f, 60.694809f, 0.0f); return;
				case 23:	pPlayer->TeleportTo(MAP_KALIMDOR, -1272.703735f, 116.886490f, 131.016861f, 0.0f); return;

				case 24:	pPlayer->TeleportTo(MAP_KALIMDOR, -602.1253f, -4262.4208f, 38.956341f, 0.0f); return;
				case 25:	pPlayer->TeleportTo(MAP_EASTERN_KINGDOMS, 1663.517f, 1678.187744f, 120.5303f, 0.0f); return;
				case 26:	pPlayer->TeleportTo(MAP_KALIMDOR, -2914.16992f, -266.061798f, 53.658211f, 0.0f); return;

				case 31:	pPlayer->TeleportTo(MAP_KALIMDOR, 6755.33f, -4658.09f, 724.8f, 3.4049f); return;
				case 32:	pPlayer->TeleportTo(MAP_EASTERN_KINGDOMS, -14462.0f, 460.0f, 16.1f, 3.4049f); return; 
				case 33:	pPlayer->TeleportTo(MAP_KALIMDOR, -952.2f, -3746.3f, 5.66f, 1.1f); return;
				}
			}
		}

		else if(action >= __MENU_SUISHEN_MAIN && action <= __MENU_SUISHEN_MAIN + __MENU_SIZE)
		{
			uint32 _vipFeature = sQZAchievements.GetVIPFeatures(pPlayer);

			if(action == __MENU_SUISHEN_MAIN)
			{
				if (_vipFeature & 0x01) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　召唤维修机器人　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 10);
				if (_vipFeature & 0x02)	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　召唤移动银行　　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 20);
				if (_vipFeature & 0x04)	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　召唤中立拍卖师　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 30);
				if (_vipFeature & 0x08)	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　召唤猎人兽栏　　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 40);

				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);
				if (!(_vipFeature & 0x01)) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞购买维修机器（100点券）＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 10 + 100);
				if (!(_vipFeature & 0x02)) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞购买移动银行（100点券）＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 20 + 100);
				if (!(_vipFeature & 0x04)) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞购买中立拍卖师（100点券）＜＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 30 + 100);
				if (!(_vipFeature & 0x08)) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞购买猎人兽栏（100点券）＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 40 + 100);

				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MAIN);

				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
			}

			else if (action>__MENU_SUISHEN_MAIN && action < __MENU_SUISHEN_MAIN + 99)
			{
				switch (action - __MENU_SUISHEN_MAIN)
				{
				case 10: pPlayer->CastSpell(pPlayer, ZQ_SPELL_SUMMON_ROBOT, true); return; //summon robot;
				case 20: pPlayer->CastSpell(pPlayer, ZQ_SPELL_SUMMON_BANK, true); return; //summon bank;
				case 30: pPlayer->CastSpell(pPlayer, ZQ_SPELL_SUMMON_AH, true); return; //summon ah;
				case 40: pPlayer->CastSpell(pPlayer, ZQ_SPELL_SUMMON_STABLE, true); return; //summon stable;
				}
			}

			else if (action > __MENU_SUISHEN_MAIN + 100 && action < __MENU_SUISHEN_MAIN + 199)
			{
				//check if player has enough vouchers
				
				if(!pPlayer->HasItemCount(ZQ_ITEM_VOUCHER, 100))
				{
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞　点券不够，返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MAIN);
				}
				else
				{
					pPlayer->DestroyItemCount(ZQ_ITEM_VOUCHER, 100, true, true);
					switch (action - __MENU_SUISHEN_MAIN - 100)
					{
						case 10: sQZAchievements.SetVIPFeatures(pPlayer, VIP_SUISHEN_ROBOT);  	break;
						case 20: sQZAchievements.SetVIPFeatures(pPlayer, VIP_SUISHEN_BANK); 	break;
						case 30: sQZAchievements.SetVIPFeatures(pPlayer, VIP_SUISHEN_AH); 		break;
						case 40: sQZAchievements.SetVIPFeatures(pPlayer, VIP_SUISHEN_STABLE); 	break;
					}	
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　开通成功，返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MAIN);	
				}
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
				return;
			}
		}

		else if(action >= __MENU_TEAM_MAIN && action <= __MENU_TEAM_MAIN + __MENU_SIZE)
		{
			uint32 _vipFeature = sQZAchievements.GetVIPFeatures(pPlayer);

			if(action == __MENU_TEAM_MAIN)
			{
				if (_vipFeature & 0x10) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　队伍召唤　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TEAM_MAIN + 10);
				if (_vipFeature & 0x20)	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　全体复活　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TEAM_MAIN + 20);
				if (_vipFeature & 0x40)	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　全体恢复　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TEAM_MAIN + 30);

				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);
				if (!(_vipFeature & 0x10)) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞购买队伍召唤（300点券）＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TEAM_MAIN + 10 + 100);
				if (!(_vipFeature & 0x20)) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞购买全体复活（300点券）＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TEAM_MAIN + 20 + 100);
				if (!(_vipFeature & 0x40)) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞购买全体恢复（300点券）＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TEAM_MAIN + 30 + 100);

				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MAIN);

				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
			}
			else if (action>__MENU_TEAM_MAIN && action < __MENU_TEAM_MAIN + 99)
			{
				__localHandleGroupCommand(pPlayer, action - __MENU_TEAM_MAIN);
				return;
			}
			else if (action > __MENU_TEAM_MAIN + 100 && action < __MENU_TEAM_MAIN + 199)
			{
				//check if player has enough vouchers
				if(!pPlayer->HasItemCount(ZQ_ITEM_VOUCHER, 300))
				{
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞　点券不够，返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MAIN);	
				}
				else
				{
					pPlayer->DestroyItemCount(ZQ_ITEM_VOUCHER, 300, true, true);
					switch (action - __MENU_TEAM_MAIN - 100)
					{
						case 10: sQZAchievements.SetVIPFeatures(pPlayer, VIP_TEAM_SUMMON);  	break;
						case 20: sQZAchievements.SetVIPFeatures(pPlayer, VIP_TEAM_REVIVE); 		break;
						case 30: sQZAchievements.SetVIPFeatures(pPlayer, VIP_TEAM_FULLFILL); 	break;
					}	
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　开通成功，返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MAIN);

				}

				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
				return;
			}
		}

		else if(action >= __MENU_ZITIAO_MAIN && action <= __MENU_ZITIAO_MAIN + __MENU_SIZE)
		{
			AchievementsEntry _aEntry = sQZAchievements.GetZitiaosInfo(pPlayer);
			auto localIdx = pPlayer->GetSession()->GetSessionDbLocaleIndex();
			std::string _str = "";

			#define _FREE_SLOTS  		(4)
			#define _NEED_VOUCHER(X)   (100*((X) - _FREE_SLOTS + 1))

			uint32 spell_id[8];

			//init the spell_id
			spell_id[0] = _aEntry.data1; spell_id[1] = _aEntry.data2; spell_id[2] = _aEntry.data3; spell_id[3] = _aEntry.data4;
			spell_id[4] = _aEntry.data5; spell_id[5] = _aEntry.data6; spell_id[6] = _aEntry.data7; spell_id[7] = _aEntry.data8;

			//check how many spells player has
			uint32 _spells_num_have = 0;
			for (size_t i = 0; i < 8; i++)
			{
				if(spell_id[i]!= 0)
				{
					_spells_num_have++;
				}
			}

			//Zitiao means equip spell effect extract, player can extract the spell effect from equipment;
			if(action == __MENU_ZITIAO_MAIN)
			{
				//Display the spells that player has
				//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝　已提取特效列表　＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝点击字条新增或替换　＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
				_str = __STR("|cff002fa7＝＝你总共： ");
				_str.append(__NSTR(_aEntry.subType));
				_str.append(__STR("条，已使用 "));
				_str.append(__NSTR(_spells_num_have));
				_str.append(__STR("条＝＝|r "));
				
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(_str), GOSSIP_SENDER_MAIN, __MENU_NONE);
				for (size_t i = 0; i < 8; i++)
				{
					if(spell_id[i] != 0)
					{
						for (size_t j = 0; j < sizeof(__Spells_DBC_Array) / sizeof(__Spells_DBC_Array[0]); j++)
						{
							if (spell_id[i] == __Spells_DBC_Array[j].spell_learn_id)
							{
								_str = __STR("|cff066e22字条[");
								_str.append(__NSTR(i+1));
								_str.append(__STR("]=> "));
								_str.append(__Spells_DBC_Array[j].text);
								_str.append(__STR("r"));
								pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(_str), GOSSIP_SENDER_MAIN, __MENU_ZITIAO_MAIN + (i+1));
								break;
							}
						}
					}
					else
					{
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("＝＝＞　空字条、点击添加　＜＝＝　"), GOSSIP_SENDER_MAIN, __MENU_ZITIAO_MAIN + (i + 1));
					}
				}

				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
				return;
			}

			else if (action >= __MENU_ZITIAO_MAIN + 1 && action <= __MENU_ZITIAO_MAIN + 8)
			{
				//Display the spells that player has in this slot
				uint32 _slot_selected = action - __MENU_ZITIAO_MAIN - 1;
				
				if(spell_id[_slot_selected] == 0)
				{
					//Means this is a empty slot, so player can add a spell to this slot

					if(_spells_num_have >= _aEntry.subType)
					{
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞　已达到最大提取数量　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						_str = __STR("|cfff00019点击开启额外空槽，花费点券： ");
						_str.append(__NSTR(_NEED_VOUCHER(_spells_num_have)));
						_str.append(__STR("|r"));
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(_str), GOSSIP_SENDER_MAIN, __MENU_ZITIAO_MAIN + 100);
						pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
						return;
					}
				}

				// now display the spells that player has in this slot, and find the first item in bag
				// to add to this slot.
				// 1. display the spells that player has in this slot
				//Now need to find the first bag
				auto pItem = pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START);

				if (!pItem || (pItem->GetProto()->Class != ITEM_CLASS_WEAPON && pItem->GetProto()->Class != ITEM_CLASS_ARMOR) || pItem->GetProto()->Spells[0].SpellId == 0
					|| pItem->GetProto()->Quality > 4
					)
				{
					std::string text = __BLUE("[|请将可提取绿字字条的装备放在角色行囊里第一个格子|]");
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_ZITIAO_MAIN);
					pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());

					return;
				}

				// 2. find the first item in bag to add to this slot.
				//should declare the 3 spell id
				uint32 spell_id[3];
				uint32 spell_dbc_id[3];
				std::string spell_text[3];

				for (size_t i = 0; i < 3; i++)
				{
					spell_id[i] = 0; spell_dbc_id[i] = 0;
					spell_text[i] = __STR("|该字条无法转移|");

					spell_id[i] = pItem->GetProto()->Spells[i].SpellId;

					//sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "Item spell: %d", spell_id[i]);
					if (spell_id[i] != 0)
					{
						for (size_t j = 0; j < sizeof(__Spells_DBC_Array) / sizeof(__Spells_DBC_Array[0]); j++)
						{
							if (spell_id[i] == __Spells_DBC_Array[j].spell_id)
							{
								spell_dbc_id[i] = __Spells_DBC_Array[j].dbc_id;
								spell_text[i] = __Spells_DBC_Array[j].text;
								break;
							}
						}
					}
				}

				// 3. add the item to this slot.
				std::string text = __RED("[待提取装备]");
				auto item_destroy_local = sObjectMgr.GetItemLocale(pItem->GetProto()->ItemId);
				auto item_destroy_text = (item_destroy_local == nullptr ? pItem->GetProto()->Name1 : item_destroy_local->Name[localIdx]);
				text.append(item_destroy_text);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);

				for (size_t i = 0; i < 3; i++)
				{
					if (spell_id[i])
					{
						text = __GREEN("提取字条＝＞");
						text.append(spell_text[i]);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(text), GOSSIP_SENDER_MAIN, __MENU_ZITIAO_MAIN + 200 + _slot_selected * 10 + i);
					}
				}
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
			}

			else if (action >= __MENU_ZITIAO_MAIN + 100 && action <= __MENU_ZITIAO_MAIN + 199)
			{
				if(action == __MENU_ZITIAO_MAIN + 100)
				{
					//Means player wants to add a empty slot to the zitiaos info
					if(!pPlayer->HasItemCount(ZQ_ITEM_VOUCHER, _NEED_VOUCHER(_spells_num_have)))	
					{
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞　点券不够，返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_ZITIAO_MAIN);
						pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
						return;	
					}

					else
					{
						pPlayer->DestroyItemCount(ZQ_ITEM_VOUCHER, _NEED_VOUCHER(_spells_num_have), true, true);
						_aEntry.subType++;
						sQZAchievements.SetZitiaosInfo(pPlayer, _aEntry);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　开通成功，返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_ZITIAO_MAIN);
						pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
						return;
					}
				}
			}

			else if (action >= __MENU_ZITIAO_MAIN + 200 && action <= __MENU_ZITIAO_MAIN + 299)
			{
				//Means player wants to add a spell to this slot
				uint32 _fb_slot_selected = (action - __MENU_ZITIAO_MAIN - 200)/10;
				uint32 _item_spell_selected = (action - __MENU_ZITIAO_MAIN - 200)%10;
				
				uint32 _oldSpellId = spell_id[_fb_slot_selected];
				uint32 _newSpellId = 0;

				auto pItem = pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START);
				if (!pItem || (pItem->GetProto()->Class != ITEM_CLASS_WEAPON && pItem->GetProto()->Class != ITEM_CLASS_ARMOR) || pItem->GetProto()->Spells[_item_spell_selected].SpellId == 0
					|| pItem->GetProto()->Quality > 4)
				{
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __RED("此装备错误！请联系GM。。 "), GOSSIP_SENDER_MAIN, __MENU_ZITIAO_MAIN);
					pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
					return;
				}

				//iterate the spelllist
				for (size_t j = 0; j < sizeof(__Spells_DBC_Array) / sizeof(__Spells_DBC_Array[0]); j++)
				{
					if (pItem->GetProto()->Spells[_item_spell_selected].SpellId == __Spells_DBC_Array[j].spell_id)
					{
						//found the spell id, now check if the spell is in the spelllist
						_newSpellId = __Spells_DBC_Array[j].spell_learn_id;
						break;
					}
				}

				if(!_newSpellId || _oldSpellId == _newSpellId || pPlayer->HasSpell(_newSpellId))
				{
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __RED("玩家已经有此字条，或装备错误！请联系GM。。 "), GOSSIP_SENDER_MAIN, __MENU_ZITIAO_MAIN);		
					pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
					return;
				}
				else
				{
					if(_oldSpellId != 0)
					{
						pPlayer->RemoveSpell(_oldSpellId);
					}

					pPlayer->LearnSpell(_newSpellId, false);

					spell_id[_fb_slot_selected] = _newSpellId;
					_aEntry.data1 = spell_id[0]; _aEntry.data2 = spell_id[1]; _aEntry.data3 = spell_id[2]; _aEntry.data4 = spell_id[3];
					_aEntry.data5 = spell_id[4]; _aEntry.data6 = spell_id[5]; _aEntry.data7 = spell_id[6]; _aEntry.data8 = spell_id[7];
					sQZAchievements.SetZitiaosInfo(pPlayer, _aEntry);

					//delete the item
					pPlayer->DestroyItem(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START, true);

					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __GREEN("字条提取成功，返回查看！"), GOSSIP_SENDER_MAIN, __MENU_ZITIAO_MAIN);
					pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
					return;
				}
			}
		}
	}
};

SpellScript* GetScript_CustomHSSpell(SpellEntry const*)
{
	return new CustomHSSpell();
}

void AddSC_qzqstar_spell_hs()
{
	Script* newscript2;
	newscript2 = new Script;
	newscript2->Name = "qzqstar_spell_hs";
	newscript2->GetSpellScript = &GetScript_CustomHSSpell;
	//newscript2->pGossipSelect = &GossipHello_HS;
	newscript2->RegisterSelf(false);
}


