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

#include "qzqstar_summon_boss.h"
#include "Chat.h"

#define	__MENU_NONE						0
#define __MENU_SUMMON_MAIN				 1000
#define __MENU_SUMMON_CONFIRM_OFFSET	  200
#define __MENU_END						10000

#define	__STR(x)		((std::string)(x)).c_str()
#define	__NSTR(x)		(std::to_string(x))

#define	__BLUE(x)		"|cff002fa7"##x##"|r"
#define	__GREEN(x)		"|cff00b72f"##x##"|r"
#define	__ORANGE(x)		"|cffe85827"##x##"|r"
#define __RED(x)		"|cfff00019"##x##"|r"
#define __YELLOW(x)		"|cfff9dc24"##x##"|r"

const Boss_Pick_t _boss_picks[] = 	//All action id and boss id
{
	{ 10, 38111, __XSTR("| 召唤第①级　BOSS  (LEVEL: 25)  |"),{ 30511, 30511, 30511, 30511 } },
	{ 20, 38112, __XSTR("| 召唤第②级　BOSS  (LEVEL: 45)  |"),{ 30521, 30521, 30521, 30521 } },
	{ 30, 38113, __XSTR("| 召唤第③级　BOSS  (LEVEL: 58)  |"),{ 30531, 30531, 30531, 30531 } },
	{ 40, 38114, __XSTR("| 召唤第④级　BOSS  (LEVEL: 60)  |"),{ 30541, 30541, 30541, 30541 } },
	{ 50, 38115, __XSTR("| 召唤第⑤级　BOSS  (LEVEL: 60)  |"),{ 30551, 30551, 30551, 30551 } },
	{ 60, 38116, __XSTR("| 召唤第⑥级　BOSS  (LEVEL: 60)  |"),{ 30561, 30561, 30561, 30561 } },
	{ 70, 38117, __XSTR("| 召唤第⑦级　BOSS  (LEVEL: 60)  |"),{ 30571, 30571, 30571, 30571 } },
	{ 80, 38118, __XSTR("| 召唤第⑧级　BOSS  (LEVEL: 60)  |"),{ 30581, 30581, 30581, 30581 } }
};


void SendDefaultMenu_BOSS(Player *player, Creature *_Creature, uint32 action)
{
	std::string text = "";
	uint32 __menu_nums = 0;

	if (action == __MENU_SUMMON_MAIN)
	{
		//Find all of the action and bosses from _boss_picks
		for (size_t i = 0; i < sizeof(_boss_picks) / sizeof(_boss_picks[0]); ++i) {

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR(_boss_picks[i].text), GOSSIP_SENDER_MAIN, __MENU_SUMMON_MAIN + _boss_picks[i].action_id);
		}

		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		return;
	}
	else if (action > __MENU_SUMMON_MAIN && action < __MENU_SUMMON_MAIN + __MENU_SUMMON_CONFIRM_OFFSET)
	{
		//Find all of the acttions from  _boss_picks
		for (size_t i = 0; i < sizeof(_boss_picks) / sizeof(_boss_picks[0]); ++i) {

			if (action - __MENU_SUMMON_MAIN == _boss_picks[i].action_id)
			{
				//check if player has enough items
				text = _boss_picks[i].text;
				text.append(__STR("　->　"));
				text.append(__STR(__RED("需要花费十个BOSS召唤石。 .")));


				player->ADD_GOSSIP_ITEM(5, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);
				player->ADD_GOSSIP_ITEM(5, __STR(__RED("=====| 确定 | 返回 |=====")), GOSSIP_SENDER_MAIN, action + __MENU_SUMMON_CONFIRM_OFFSET);
				player->ADD_GOSSIP_ITEM(5, __STR(__GREEN("=====| 取消 | 返回 |=====")), GOSSIP_SENDER_MAIN, __MENU_SUMMON_MAIN);


				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
				return;
			}
		}
	}
	else if (action >  __MENU_SUMMON_MAIN + __MENU_SUMMON_CONFIRM_OFFSET && action < __MENU_END)
	{
		//Find all of the acttions from  _boss_picks
		for (size_t i = 0; i < sizeof(_boss_picks) / sizeof(_boss_picks[0]); ++i) {

			if (action - __MENU_SUMMON_MAIN - __MENU_SUMMON_CONFIRM_OFFSET  == _boss_picks[i].action_id)
			{
				//check the materials
				if (player->HasItemCount(_boss_picks[i].item_id, 10))
				{
					//delete the item count
					player->DestroyItemCount(_boss_picks[i].item_id, 10, true);
					player->ADD_GOSSIP_ITEM(5, __STR(__RED("====| 祝你顺利! |=====")), GOSSIP_SENDER_MAIN, __MENU_SUMMON_MAIN);
					
					//call the boss
					auto _creatureID = PickRandomValue(_boss_picks[i].boss_id[0], _boss_picks[i].boss_id[1], _boss_picks[i].boss_id[2], _boss_picks[i].boss_id[3]);
					auto _boss = _Creature->SummonCreature(_creatureID, -13201.0f, 296.0f, 21.9f, 4.5f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 3600000);
					
					//auto _boss = _Creature->SummonCreature(_creatureID, -13201.0f, 296.0f, 21.9f, 4.5f, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 600000);

					//Save
					sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "Player:[%s] summoned the creature:%s. LEVEL:%d", player->GetName(), _boss->GetName(), _boss_picks[i].action_id / 10);

					//Announce
					auto const& sessions = sWorld.GetAllSessions();
					for (const auto& itr : sessions)
					{
						if (WorldSession* session = itr.second)
						{
							Player* _player = session->GetPlayer();
							if (_player && _player->IsInWorld())
							{
								//todo, tell them
								ChatHandler(_player).PSendSysMessage(9004, player->GetName(), _boss->GetName());
							}
						}
					}
				}
				else
				{
					player->ADD_GOSSIP_ITEM(5, __STR(__GREEN("====| 材料不够 | 返回 |=====")), GOSSIP_SENDER_MAIN, __MENU_SUMMON_MAIN);
				}
				
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
				return;
			}
		}
	}

}


bool Gossip_BOSS_Hello(Player *player, Creature *_Creature)
{
	SendDefaultMenu_BOSS(player, _Creature, __MENU_SUMMON_MAIN);
	return true;
}

bool Gossip_BOSS_Select(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
	// BOSS select menu
	if (action < __MENU_END) SendDefaultMenu_BOSS(player, _Creature, action);
	return true;
}

void AddSC_qzqstar_summon_boss()
{
	Script* newscript;

	newscript = new Script;
	newscript->Name = "qzqstar_summon_boss";
	newscript->pGossipHello = &Gossip_BOSS_Hello;
	newscript->pGossipSelect = &Gossip_BOSS_Select;
	newscript->RegisterSelf(false);
}


