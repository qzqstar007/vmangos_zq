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

#include "qzqstar_vip_str.h"

// Set to Bandiantu,
//	Spell:		10712
//	Creature:	7559

#define	__MENU_NONE						0
#define __MENU_TELEPORT_MAIN			1000
#define __MENU_DUNGEON_RESET_MAIN		2000
#define __MENU_END						10000

#define	__STR(x)		((std::string)(x)).c_str()
#define	__NSTR(x)		(std::to_string(x))
	
#define	__BLUE(x)		"|cff002fa7"##x##"|r"
#define	__GREEN(x)		"|cff00b72f"##x##"|r"
#define	__ORANGE(x)		"|cffe85827"##x##"|r"
#define __RED(x)		"|cfff00019"##x##"|r"
#define __YELLOW(x)		"|cfff9dc24"##x##"|r"

//VIP Spell: 32858
bool GossipHello_VIP(Player *player, Creature *_Creature)
{
	if (player->HasSpell(32858))
	{
		//VIP teleport
		player->ADD_GOSSIP_ITEM(5, __STR(__BLUE("=== |　副　本　内　飞　| ===")), GOSSIP_SENDER_MAIN, __MENU_TELEPORT_MAIN);
		//player->ADD_GOSSIP_ITEM(5, __STR(__BLUE("=== |　其　他　功　能　| ===")), GOSSIP_SENDER_MAIN, __MENU_NONE);
	}

	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
	return true;
}


void SendDefaultMenu_TELEPORT(Player *player, Creature *_Creature, uint32 action)
{
	std::string text = "";
	uint32 __menu_nums = 0;


	if (action == __MENU_TELEPORT_MAIN)
	{
		//Find all of the mapid from _TELEPORT_Locs
		for (size_t i = 0; i < sizeof(_TELEPORT_Locs) / sizeof(_TELEPORT_Locs[0]); ++i) {

			if (player->GetMapId() == _TELEPORT_Locs[i].map_id)
			{
				//add to menu.
				++ __menu_nums;
				player->ADD_GOSSIP_ITEM(5, __STR(_TELEPORT_Locs[i].text), GOSSIP_SENDER_MAIN, _TELEPORT_Locs[i].action_id);
			}
		}

		if (__menu_nums == 0)
		{
			player->ADD_GOSSIP_ITEM(5, __STR(__RED(" ==|　只能在副本使用　|== ")), GOSSIP_SENDER_MAIN, __MENU_NONE);
		}

		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
	}
	else if( action > 1010 && action < __MENU_DUNGEON_RESET_MAIN)
	{
		//Find all of the mapid from _TELEPORT_Locs
		for (size_t i = 0; i < sizeof(_TELEPORT_Locs) / sizeof(_TELEPORT_Locs[0]); ++i) {

			if (action == _TELEPORT_Locs[i].action_id)
			{
				//just teleport to the position.
				player->CLOSE_GOSSIP_MENU();
				player->TeleportTo(_TELEPORT_Locs[i].map_id, _TELEPORT_Locs[i].pos[0], _TELEPORT_Locs[i].pos[1], _TELEPORT_Locs[i].pos[2], _TELEPORT_Locs[i].pos[3]);
			}
		}
	}


}


void SendDefaultMenu_DUNGEN_RESET(Player *player, Creature *_Creature, uint32 action)
{

}


bool GossipSelect_VIP(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
	// Teleport menu
	if (action < __MENU_DUNGEON_RESET_MAIN)
		SendDefaultMenu_TELEPORT(player, _Creature, action);

	else if (action < __MENU_END)
		SendDefaultMenu_DUNGEN_RESET(player, _Creature, action);

	return true;
}


// 18788 - Demonic Sacrifice
struct CustomVipSpell : SpellScript
{
	void OnSuccessfulFinish(Spell* spell) const final
	{
		(spell->m_casterUnit)->ToPlayer()->PlayerTalkClass->ClearMenus();
		(spell->m_casterUnit)->ToPlayer()->M_Spare_Data1 = 1999;
		(spell->m_casterUnit)->ToPlayer()->ADD_GOSSIP_ITEM(5, __STR(__BLUE("=== |　副　本　内　飞　| ===")), GOSSIP_SENDER_MAIN, __MENU_TELEPORT_MAIN);
		(spell->m_casterUnit)->ToPlayer()->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, spell->m_casterUnit->GetObjectGuid());
	}

	void OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action) const final
	{
		//sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "test. %d %d", sender, action);
		SendDefaultMenu_TELEPORT(pPlayer, pCreature, action);
	}
};

SpellScript* GetScript_CustomVipSpell(SpellEntry const*)
{
	return new CustomVipSpell();
}

void AddSC_qzqstar_vip_creatures()
{
	Script* newscript;
	
	newscript = new Script;
	newscript->Name = "qzqstar_vip";
	newscript->pGossipHello = &GossipHello_VIP;
	newscript->pGossipSelect = &GossipSelect_VIP;
	newscript->RegisterSelf(false);


	Script* newscript2;
	newscript2 = new Script;
	newscript2->Name = "spell_vip";
	newscript2->GetSpellScript = &GetScript_CustomVipSpell;
	newscript2->pGossipSelect = &GossipSelect_VIP;
	newscript2->RegisterSelf(false);
}


