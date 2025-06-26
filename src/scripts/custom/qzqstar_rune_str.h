/*
* Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
* Copyright (C) 2009-2011 MaNGOSZero <https://github.com/mangos/zero>
* Copyright (C) 2011-2016 Nostalrius <https://nostalrius.org>
* Copyright (C) 2016-2017 Elysium Project <https://github.com/elysium-project>
*
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

#ifndef _QZQSTAR_RUNE_STR_H
#define _QZQSTAR_RUNE_STR_H

typedef struct __rep_list {
	uint32			rep_level;
	uint32			rep_id;
	std::string		rep_name;
}Rep_List_t;
extern const Rep_List_t _Rep_List[11];



//itemset related structure, players can collect the itemsets
//to upgrade
typedef struct __item_set {
	uint32 			item_set_id;
	uint32 			item_id[10];
}Item_set_t;




typedef struct __rune_spell_menu {
	uint32			flags;
	uint32			spell_id;
	std::string		text; 
	std::string		desc;
}Rune_Spell_Menu_t;

#define	RUNE_SPELLS_NUM				(9)
extern const Rune_Spell_Menu_t _Spells_Menu_Warrior[RUNE_SPELLS_NUM];
extern const Rune_Spell_Menu_t _Spells_Menu_Paladin[RUNE_SPELLS_NUM];
extern const Rune_Spell_Menu_t _Spells_Menu_Hunter[RUNE_SPELLS_NUM];
extern const Rune_Spell_Menu_t _Spells_Menu_Rogue[RUNE_SPELLS_NUM];
extern const Rune_Spell_Menu_t _Spells_Menu_Priest[RUNE_SPELLS_NUM];
extern const Rune_Spell_Menu_t _Spells_Menu_Shaman[RUNE_SPELLS_NUM];
extern const Rune_Spell_Menu_t _Spells_Menu_Mage[RUNE_SPELLS_NUM];
extern const Rune_Spell_Menu_t _Spells_Menu_Warlock[RUNE_SPELLS_NUM];
extern const Rune_Spell_Menu_t _Spells_Menu_Druid[RUNE_SPELLS_NUM];


//here, should be something exactly for string storage
typedef struct __spell_dbc_desc {
	uint32			spell_id;
	uint32			spell_learn_id;	//to be learnt by player
	uint32			dbc_id;
	std::string		text;
}Spell_DBC_t;

extern const Spell_DBC_t __Spells_DBC_Array[630];


extern uint32_t ZQ_GetRandomEQEnchantment(bool isRare);



#endif	//_QZQSTAR_RUNE_STR_H

