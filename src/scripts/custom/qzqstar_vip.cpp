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

// VIP spells
struct CustomVipSpell : SpellScript
{
	void OnSuccessfulFinish(Spell* spell) const final
	{
		(spell->m_casterUnit)->ToPlayer()->PlayerTalkClass->ClearMenus();
		(spell->m_casterUnit)->ToPlayer()->M_Gossip_Spell_Ind = 1999;
		(spell->m_casterUnit)->ToPlayer()->ADD_GOSSIP_ITEM(5, __STR(__BLUE("=== |　副　本　内　飞　| ===")), GOSSIP_SENDER_MAIN, __MENU_TELEPORT_MAIN);
		(spell->m_casterUnit)->ToPlayer()->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, spell->m_casterUnit->GetObjectGuid());
	}

	void OnGossipSelect(Player* player, Creature* _Creature, uint32 sender, uint32 action) const final
	{
		std::string text = "";
		uint32 __menu_nums = 0;


	}
};

void AddSC_qzqstar_vip_creatures()
{

	Script* newscript = new Script;
    newscript->Name = "qzqstar_spell_vip";
    newscript->GetSpellScript = [](SpellEntry const*) -> SpellScript* { return new CustomVipSpell(); };
    newscript->RegisterSelf();

}


