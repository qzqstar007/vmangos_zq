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

extern int32 DBHelper_GetRandEnchantIDByLevel(int32 itemLevel);
extern int32 DBHelper_GetSpecialSlotsByLevel(int32 itemLevel);


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

extern bool isCollectionItem(uint32_t eqItemID);

extern uint32_t DBHelper_IsPetCollection(uint32_t eqItemID);
extern uint32_t DBHelper_GetPlayerReputation_Bits(Player * player);
extern uint32_t DBHelper_GetPlayerProfs_Bits(Player * player);

typedef struct _Fragment_Upgrade_t
{
	int				_id;
	int             multi;
	uint32			spellId;
	int             pointsNeed;
	int             pointsIncEachLevel;
	int             maxpoints;
	std::string		desc;
	std::string     unit;
}FragUpgrade_t;

#define PLAYER_ABILITIES_NUM_S8				14
extern const FragUpgrade_t Ability_MenuS8[PLAYER_ABILITIES_NUM_S8];
extern int DBHelper_get_upgrade_points(Player *player,  int ability_type, int curpoints);
extern int DBHelper_get_used_points(Player *player, int ability_type);
extern int DBHelper_get_bonus_points(Player *player, Item * pItem);

//point of intreset
typedef struct _teleport_point_t {
	uint32 id;		//point id, unique in map, also used as the index of the array, so it should be continuous, starting from 0.
	uint32 mapId;	//map id, the map id of the point of intreset.
	std::string name;	//name of the point of intreset.
	uint32 tele_mapid;  //map id of the teleport destination.
	float tele_x;		//x coordinate of the point of intreset.
	float tele_y;		//y coordinate of the point of intreset.
	float tele_z;		//z coordinate of the point of intreset.
	float tele_o;		//orientation of the point of intreset.
	uint32 npc_list[10];
}Teleport_Point_t;

extern const Teleport_Point_t TP_Dungeons[18];
extern const Teleport_Point_t TP_Raids[7];

extern uint32_t DBHelper_Get_Farm_Times(Player *player, uint32_t maptype, uint32_t mapid);
extern uint32_t DBHelper_Inc_Farm_Times(Player *player, uint32_t maptype, uint32_t mapid);

#endif	//_QZQSTAR_DB_H

