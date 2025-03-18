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

#include "qzqstar_custom.h"

#define	__STR(x)		((std::string)(x)).c_str()
#define	__NSTR(x)		(std::to_string(x))

#define	__BLUE(x)		"|cff002fa7"##x##"|r"
#define	__GREEN(x)		"|cff00b72f"##x##"|r"
#define	__ORANGE(x)		"|cffe85827"##x##"|r"
#define __RED(x)		"|cfff00019"##x##"|r"
#define __YELLOW(x)		"|cfff9dc24"##x##"|r"



#define __MENU_BG_MAIN					(1000)
#define	__MENU_BG_RW_OFFSET				(100)
#define __MENU_BG_MAIN_WS				(__MENU_BG_MAIN	+	10)
#define __MENU_BG_MAIN_AB				(__MENU_BG_MAIN	+	20)
#define __MENU_BG_MAIN_AV				(__MENU_BG_MAIN	+	30)

bool BG_Menus(Player *player, Creature *_Creature, uint32 sender,  uint32 action)
{
	std::string text = "";
	uint32 __menu_nums = 0;

	if (action == __MENU_BG_MAIN)
	{
		//Add to join the battlegrounds
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("＝＝＝＝选择战场＝＝＝＝　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN);

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR("①战歌峡谷　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN_WS);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR("②阿拉希盆地　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN_AB);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR("③奥特兰克山谷　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN_AV);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("＝＝＝＝奖章兑换＝＝＝＝　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR("①战歌奖章　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN_WS + __MENU_BG_RW_OFFSET);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR("②阿拉希奖章　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN_AB + __MENU_BG_RW_OFFSET);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR("③奥特兰克奖章　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN_AV + __MENU_BG_RW_OFFSET);


		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		return true;
	}
	else if (action > __MENU_BG_MAIN && action<__MENU_BG_MAIN + __MENU_BG_RW_OFFSET)
	{
		auto bgType = action == __MENU_BG_MAIN_WS ? BATTLEGROUND_WS : action == __MENU_BG_MAIN_AB ? BATTLEGROUND_AB : BATTLEGROUND_AV;

		player->InterruptSpellsWithChannelFlags(AURA_INTERRUPT_INTERACTING_CANCELS);
		player->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_INTERACTING_CANCELS);
		player->GetSession()->SendBattleGroundList(_Creature->GetGUID(), bgType);
	}
	else if (action > __MENU_BG_MAIN + __MENU_BG_RW_OFFSET)
	{
		action -= __MENU_BG_RW_OFFSET;
		auto bgType = action == __MENU_BG_MAIN_WS ? BATTLEGROUND_WS : action == __MENU_BG_MAIN_AB ? BATTLEGROUND_AB : BATTLEGROUND_AV;

		if (player);
	}

	return true;
}

void AddSC_qzqstar_custom()
{
	Script* newscript;

	//Add battle ground npc
	newscript = new Script;
	newscript->Name = "qzqstar_battleground";
	newscript->pGossipHello = [](Player *p, Creature *c) -> bool { return BG_Menus(p, c, 0, __MENU_BG_MAIN); };
	newscript->pGossipSelect = &BG_Menus;
	newscript->RegisterSelf(false);
}


