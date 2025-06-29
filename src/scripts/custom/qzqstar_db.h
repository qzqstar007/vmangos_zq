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

#ifndef _QZQSTAR_DB_H
#define _QZQSTAR_DB_H


typedef struct _level_QuestID {
	int32 reqLevel;
	int32 questID;
	//std::string questTitle;
}Level_QuestID_t;

extern Level_QuestID_t DBHelper_GetQuestByLevel(Player *);


typedef struct _level_ItemMatsID {
	int32 reqLevel;
	int32 itemID;
	//std::string itemName;
}Level_ItemMatsID_t;

extern Level_ItemMatsID_t DBHelper_GetItemMatsByLevel(Player *, int32);


typedef struct _level_ItemEqID {
	int32 reqLevel;
	int32 itemID;
	//std::string itemName;
}Level_ItemEqID_t;

extern Level_ItemEqID_t DBHelper_GetItemEqByLevel(Player *, int32);
extern Level_ItemEqID_t DBHelper_GetItemEqByCreatureLevel(Creature *, int32);

typedef struct _level_enchantID {
	int32 reqLevel;
	int32 enchantID;
}Level_EnchantID_t;

extern Level_EnchantID_t DBHelper_GetRandEnchantIDByLevel(int32 itemLevel);


//Equipment list for the world
typedef struct _level_weaponList_common {
	int32 world_level;
	int32 item_level;
	int32 eq_list[12];
} EQ_Collect_Common_t;
extern const EQ_Collect_Common_t * DBHelper_GetEQByWorldLevel(uint32 level);
extern const EQ_Collect_Common_t * DBHelper_GetEQByProfession(uint32 prof);



//Equipment list for the dungeon
typedef struct _level_weaponList_dungeon {
	int32 dungen_id;
	int32 reward_apsp[4];	//four dungeons difficulties, such as 1,2,3,4
	int32 eq_list[6];
}EQ_Collect_Dungeon_t;

extern const EQ_Collect_Dungeon_t * DBHelper_GetEQByDungeonID(int32 mapId);
extern int COUNT_ONES(uint32_t x) ;





#endif	//_QZQSTAR_DB_H

