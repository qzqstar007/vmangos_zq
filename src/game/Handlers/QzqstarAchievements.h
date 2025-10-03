#ifndef _QZQSTAR_ACHIEVEMENTS_MGR_H
#define _QZQSTAR_ACHIEVEMENTS_MGR_H

#include "Policies/Singleton.h"
#include "Platform/Define.h"

#include <vector>




#define ACHIEVEMENT_TYPE_PLAYER		(0)
#define ACHIEVEMENT_TYPE_ACCOUNT	(1)

enum Achievement_t
{
	ACHIEVEMENT_ACCOUNT			= 	11,	//We use this as Account Achievement, shared the characters in this account
	ACHIEVEMENT_PLAYER_DATA 	= 	12,	//Player spend list, should be caculated each time log in



	//Older Type
	ACHIEVEMENT_VIP 		  	= 	66,
	ACHIEVEMENT_RUNE 		  	= 	88,
	ACHIEVEMENT_CUSTOM_QUEST 	= 	100,
	ACHIEVEMENT_SOCIAL_POINTS 	= 	200,
	ACHIEVEMENT_PETS 			= 	300,
	ACHIEVEMENTS_DUNGEONS 		= 	400,
	ACHIEVEMENTS_RAIDS 			= 	401,
	ACHIEVEMENTS_ZITIAO 		= 	500,
	ACHIEVEMENTS_COLLECTIONS 	= 	600,
	ACHIEVEMENTS_COLLECTIONS_WORLD	= 	600,	//collect the items in the dungeon, subtype is the dungeon id, 
	ACHIEVEMENTS_COLLECTIONS_DUNGEONS_1 	= 	601,	//collect the items in the dungeon, subtype is the dungeon id, 
	ACHIEVEMENTS_COLLECTIONS_DUNGEONS_2		= 	602,	//collect the items in the dungeon, subtype is the dungeon id, 
	ACHIEVEMENTS_COLLECTIONS_RAID			= 	603,	//collect the items in the dungeon, subtype is the dungeon id, 
	ACHIEVEMENTS_COLLECTIONS_PROFESSION		= 	604,	//collect the items of professions,
	ACHIEVEMENTS_COLLECTIONS_SKILLS			= 	620,	//collect the skills of monters
	ACHIEVEMENT_COUNTERS		=	1000,	//For combine and refreshing...
};

//S8 -
//Achievement Data
#define ACHIEVEMENT_ACCOUNT_VIP					(0)
#define ACHIEVEMENT_ACCOUNT_TASK				(1)	 //Explore the world, and get the data in data1
#define ACHIEVEMENT_ACCOUNT_EXPLORE				(2)
#define ACHIEVEMENT_ACCOUNT_PET_COLLECTION		(3)
#define ACHIEVEMENT_ACCOUNT_REPUTATION_LIST		(4)
#define ACHIEVEMENT_ACCOUNT_PROFESSION_SKILL	(5)	
#define ACHIEVEMENT_ACCOUNT_KILLING_NUMS		(6)	
#define ACHIEVEMENT_ACCOUNT_PVP_NUMS			(7)
#define ACHIEVEMENT_ACCOUNT_GOLD_COLLECT		(8)
#define ACHIEVEMENT_ACCOUNT_DUNGEON_NUMS1		(9)	
#define ACHIEVEMENT_ACCOUNT_DUNGEON_NUMS2		(10)	
#define ACHIEVEMENT_ACCOUNT_MATS_NUMS			(11)		
#define ACHIEVEMENT_ACCOUNT_EQ_NUMS				(12)	
#define ACHIEVEMENT_ACCOUNT_BONUS_NUMS			(14)
#define ACHIEVEMENT_ACCOUNT_REWARD				(15)

//Player Used Data
//need caculate and compare with account data
#define PLAYER_USED_LEVEL_CHENYI				(0)
#define PLAYER_USED_LEVEL_ZHANPAO				(1)
#define PLAYER_USED_NUMS_TALENT					(2)
#define PLAYER_USED_LEVEL_WEAPON				(3)
#define PLAYER_USED_LEVEL_PET					(4)
#define PLAYER_USED_NUMS_KANG					(5)
#define PLAYER_USED_NUMS_STRENGTH				(6)
#define PLAYER_USED_NUMS_AGILITY				(7)
#define PLAYER_USED_NUMS_STAMINA				(8)
#define PLAYER_USED_NUMS_INTELLECT				(9)
#define PLAYER_USED_NUMS_SPIRIT					(10)
#define PLAYER_USED_NUMS_SP						(11)
#define PLAYER_USED_NUMS_AP						(12)
#define PLAYER_USED_NUMS_DUNGEON_TIMES			(13)


//don't need caculate
#define PLAYER_USED_CHALLGE_MODE				(90)


//VIP Features
#define VIP_SUISHEN_ROBOT	(0x00000001)
#define VIP_SUISHEN_BANK	(0x00000002)
#define VIP_SUISHEN_AH		(0x00000004)
#define VIP_SUISHEN_STABLE	(0x00000008)
#define VIP_TEAM_SUMMON		(0x00000010)
#define VIP_TEAM_REVIVE		(0x00000020)
#define VIP_TEAM_FULLFILL	(0x00000040)
#define VIP_ONEKEY_PICK		(0x00000100)
#define VIP_SUISHEN_BUFF	(0x00000200)
#define VIP_LEVEL_MASK		(0x000F0000)
#define VIP_SPEICIAL_MASK	(0x00F00000)

#define VIP_SPECIAL_FREE_STOLE	 (0x01)		//stored in data8
#define VIP_SPECIAL_FREE_ENCHANT (0x02)		//stored in data8

//Challenging Mode
#define CHALLENGING_MODE_ONELIFE		(0x01)
#define CHALLENGING_MODE_MANUFACT		(0x02)
#define CHALLENGING_MODE_EQUIPMENT		(0x04)
#define CHALLENGING_MODE_TASK			(0x08)
#define CHALLENGING_MODE_RICH			(0x10)
#define CHALLENGING_MODE_KILLER_HUMAN	(0x20)
#define CHALLENGING_MODE_KILLER_BEAST	(0x40)
#define CHALLENGING_MODE_KILLER_UNDEAD	(0x80)
#define CHALLENGING_MODE_KILLER_MASK    (0x20 | 0x40 | 0x80)
#define CHALLENGING_MODE_DONE_ONELIFE	(0x0100)
#define CHALLENGING_MODE_DONE_MANUFACT	(0x0200)
#define CHALLENGING_MODE_DONE_EQUIPMENT	(0x0400)
#define CHALLENGING_MODE_DONE_TASK		(0x0800)
#define CHALLENGING_MODE_DONE_RICH		(0x1000)
#define CHALLENGING_MODE_DONE_KILLER	(0x2000)

//Itemset type(edada1.2.3) for achievements
#define COLLECTIONS_TYPE_ITEMSET_DUNGEONS			(1)	
#define COLLECTIONS_TYPE_ITEMSET_RAID_LOW			(2)
#define COLLECTIONS_TYPE_ITEMSET_RAID_HIGH			(3)
#define COLLECTIONS_TYPE_PROFESSIONS				(4)
#define COLLECTIONS_TYPE_WEAPONS_0					(5)	//normal difficutly dungeon weapons, each bit means a dungeon set.
#define COLLECTIONS_TYPE_WEAPONS_1					(6)
#define COLLECTIONS_TYPE_WEAPONS_2					(7)
#define COLLECTIONS_TYPE_WEAPONS_3					(8)

//ACHIEVEMENT_COUNTERS subtype
#define	ACHIEVEMENT_COUNTERS_COMBINE	(10)
#define	ACHIEVEMENT_COUNTERS_REFRESH	(20)

class QzqstarAchievements
{
private:
	//create a vector map to store all the player's achievements
	//key is player guid, value is a vector of AchievementsEntry
	//move to player's achievements
	std::map<uint32, std::vector<AchievementsEntry> > _playerAchievements;	//holds both player and account achievements

public:
	QzqstarAchievements();
	~QzqstarAchievements();

	void Init();
	void Update(uint32 diff);

	//SQL helper to save player's status
	void Save(Player *);

	//SQL helper to load player's status
	//return boolean to indicate if the player has any achievements in the database
	int32 Load(Player *);

	//add an achievement to the player's vector
	void AddAchievement(Player *player, AchievementsEntry entry);

	//get the player's achievements vector
	std::vector<AchievementsEntry> GetAchievements(Player *player);

	/*****************************************************
	 *
	 *		Account Achievement Functions
	 *
	 *****************************************************/
	uint32 QzqstarAchievements::GetAccountSum(Player * _player);
	uint32 QzqstarAchievements::GetAccAchieveData(Player * _player, uint32 type);
	void QzqstarAchievements::IncAccAchieveData(Player * _player, uint32 type);
	void QzqstarAchievements::SetAccAchieveData(Player * _player, uint32 type, uint32 data);

	uint32 QzqstarAchievements::GetPlayerData(Player * _player, uint32 type);
	uint32 QzqstarAchievements::GetPlayerSum(Player * _player);
	void QzqstarAchievements::SetPlayerData(Player * _player, uint32 type, uint32 data);


	/*****************************************************
	 *
	 *		VIP Systems Functions
	 *
	 *****************************************************/
	// note on data arrays
	// data1: vip level
	// data2: vip enabled features, each bit represents a feature, 1 means enabled, 0 means disabled

	void __init_VIP_Entry(Player *player);	//init the vip entry for the player

	// Get the player's VIP level, data1
	uint32 GetVIPLevel(Player *player);
	bool SetVIPLevel(Player *player, uint32 level);

	// Get the player's VIP enabled features, data2
	uint32 	GetVIPFeatures(Player *player);
	bool 	SetVIPFeatures(Player *player, uint32 features);

	// Get & Set player's challenging mode, using data3 to store the challenging mode, data3
	uint32 	GetChallengeMode(Player *player);
	void 	SetChallengeMode(Player *player, uint32 mode);

	// Get & Set player's custom settings using data4
	uint32 GetCustomSettings(Player * _player);
	void   SetCustomSettings(Player * _player, uint32 _settings);

	// Social points system functions, data5
	uint32 	GetSocialPoints(Player * _player);
	void 	SetSocialPoints(Player * _player, uint32 _points);

	// Promotions for the player, data6
	uint32  GetPromotions(Player * _player);
	void    SetPromotions(Player * _player, uint32 _promotions);

	// VIP special features, data8, bit 0 means free stole etc
	uint32  GetVIPSpecialFeatures(Player * _player);

	/*****************************************************
	 *
	 *		Rune Systems Functions
	 *
	 *****************************************************/
	// note on data arrays
	// data1: rune slots

	// Get the player's rune slots
	uint32 GetRuneSlots(Player *player);

	// Set the player's rune slots and add the rune slots to the player's achievements vector
	bool SetRuneSlots(Player *player, uint32 slots);

	/*****************************************************
	 *
	 *		Custom Quests Functions
	 *
	 *****************************************************/
	// note on data arrays
	// data1: current quest
	// data2: today quest
	// data3: total qeust
	// data4: normal quest done.
	
	//check if the player has accepted the custom quest
	// 0:  means the player has not accepted the quest yet, 
	// >0: means the player has accepted the quest and return the quest id
	int32 GetCustomQuestID(Player *player); 

	// set the custom id of the player's quest and add the quest to the player's achievements vector
	void SetCustomQuestID(Player *player, int32 questId);

	//finish the custom quest and add the quest to the player's achievements vector
	void FinishCustomQuest(Player *, bool);

	//get the quest done counters
	uint32 GetQuestDoneCounters(Player * _player);

	//set the normal quest done counters
	void   SetNormalQuestDoneNum(Player * _player, uint32 _counters);


	/*****************************************************
	 *
	 *		Social Points Functions
	 *
	 *****************************************************/
	 //data5, pvp killer counts
	 void 	  __init_SocialPoints_Entry(Player * _player);
	 uint32_t GetSocialPointsPVP(Player * _player);
	 void     SetSocialPointsPVP(Player * _player, uint32_t _points);
	

	/*****************************************************
	 *
	 *		Pet Systems Functions
	 *
	 *****************************************************/
	// note on data arrays
	// subtype: active pet id, 1 - 7 means data1 to data4(types);
	// each data holds 10 digits
	// ----------------
	// A-BC-DE-F-GH-IJ
	// 0-12-23-5-67-89
	// ----------------
	// A/F -means pet level, from 1 to 9
	// BC.GH - means happiness points, from 0 to 99, interval can be 10minutes
	// DE.IJ - means relationship points, from 0 to 99
	AchievementsEntry QzqstarAchievements::GetPetEntry(Player * _player);
	
	//get the player's pet information, return a vector of AchievementsEntry
	int32 GetActivePetInfo(Player *player);

	//set the player's pet information, add the pet to the player's achievements vector
	void SetActivePetInfo(Player *player, int32 miscValue);

	//change active pet, range 1-7
	void ChangeActivePet(Player *player, int32 petType);

	//every 10 minutes, minus 1 happiness points for all players online
	//add 10 points to pet's relationship points if active
	void UpdatePetPoints(Player *player);


	/*****************************************************
	 *
	 *		Dungeons Systems Functions
	 *
	 *****************************************************/
	// note on data arrays
	// dataX: used as 2 dungeons status, each holding 8 bits
	// bit7-4, unused
	// bit3:2, used as dungeons difficulty achieved;
	// bit1:0, used as dungeons current difficulty;

	//get the player's dungeons information, return a byte
	uint32 GetDungeonsInfo(Achievement_t _mapType, Player *player, uint32 ac_mapId /* should be mapped to 0-15 */);

	//save the player's dungeons information, add the dungeons to the player's achievements vector
	void SetDungeonsInfo(Achievement_t _mapType, Player *player, uint32 ac_mapId /* should be mapped to 0-15 */, uint32 value);

	/*****************************************************
	 *
	 *		Zitiaos Systems Functions
	 *
	 *****************************************************/
	// note on data arrays
	// subType: used to store the spell numbers that player can have (4+)
	// dataX: used to store the extracted spell id(learnt spell)
	// all player have 4 slots free, data1-4
	// next 4 slots should be payment spells, data5-8

	//get the player's zitiaos information, return a vector of AchievementsEntry
	AchievementsEntry GetZitiaosInfo(Player *player);

	//save the player's zitiaos information, add the zitiaos to the player's achievements vector
	void SetZitiaosInfo(Player *player, AchievementsEntry entry);


	/*****************************************************
	 *
	 *		Collections Systems Functions
	 *
	 *****************************************************/
	// note on data arrays, each data can hold about 24 bits, 1 bit for each itemset id
	// dataX: used to store the itemset id player had achieved, 0 means not achieved, 1 means achieved
	// data1: stores the lower dungeon item set
	// data2: stores the lower raid item set, T0, ZUL and T0.5 and T2.5 etc
	// data3: stores the T1-T3 Item set list
	// data4: professional item set list
	// data5-8: stores the equipments collections
	uint32_t 	GetCollectAchiveInfo(Player *player, uint32_t itemSetType);
	void 		SetCollectAchiveInfo(Player *player, uint32_t itemSetType, uint32_t value);

	/*****************************************************
	 *		Collection of World Functions
	 *****************************************************/
	 uint32    			GetEQCollectBonus(Player *player);
	 AchievementsEntry 	GetCollectionEntry(Player *player, uint32_t collection_type);
	 uint32    			GetEquipCollectCommon(Player *player, uint32_t collection_type, uint32_t key);
	 void      			SetEquipCollectCommon(Player *player, uint32_t collection_type, uint32_t key, uint32_t value);

	/*****************************************************
	 *
	 *		Collection of Skills from monster
	 * 
	 *****************************************************/
	 AchievementsEntry GetSkillsCollectEntry(Player *player);
	 uint32    GetSkillsCollectActiveID(Player *player);
	 int32     GetSkillsCollectEmptySlot(Player *player);
	 void      SetSkillsCollectInfo(Player *player, uint32_t spellId, uint32_t pos);


protected:
	std::vector<AchievementsEntry> entries;
	time_t _constInterval;
	time_t _current;
};

#define sQZAchievements MaNGOS::Singleton<QzqstarAchievements>::Instance()


#endif
#pragma once
