#ifndef _QZQSTAR_ACHIEVEMENTS_MGR_H
#define _QZQSTAR_ACHIEVEMENTS_MGR_H

#include "Policies/Singleton.h"
#include "Platform/Define.h"

#include <vector>

struct AchievementsEntry
{
	int32 entry;
	int32 guid;
	int32 type;
	int32 subType;
	int32 data1;	
	int32 data2;	
	int32 data3;
	int32 data4;
	std::string note;
	int32 data5;
	int32 data6;
	int32 data7;
	int32 data8;
};

enum Achievement_t
{
	ACHIEVEMENT_VIP 		  	= 	66,
	ACHIEVEMENT_RUNE 		  	= 	88,
	ACHIEVEMENT_CUSTOM_QUEST 	= 	100,
	ACHIEVEMENT_SOCIAL_POINTS 	= 	200,
	ACHIEVEMENT_PETS 			= 	300,
	ACHIEVEMENTS_DUNGEONS 		= 	400,
	ACHIEVEMENTS_ZITIAO 		= 	500,
	ACHIEVEMENT_COUNTERS		=	1000,	//For combine and refreshing...
};

//VIP Features
#define VIP_SUISHEN_ROBOT	(0x01)
#define VIP_SUISHEN_BANK	(0x02)
#define VIP_SUISHEN_AH		(0x04)
#define VIP_SUISHEN_STABLE	(0x08)
#define VIP_TEAM_SUMMON		(0x10)
#define VIP_TEAM_REVIVE		(0x20)
#define VIP_TEAM_FULLFILL	(0x40)

//Challenging Mode
#define CHALLENGING_MODE_ONELIFE		(0x01)
#define CHALLENGING_MODE_MANUFACT		(0x02)
#define CHALLENGING_MODE_COLLECT		(0x04)
#define CHALLENGING_MODE_TASK			(0x08)
#define CHALLENGING_MODE_RICH			(0x10)
#define CHALLENGING_MODE_KILLER_HUMAN	(0x20)
#define CHALLENGING_MODE_KILLER_BEAST	(0x40)
#define CHALLENGING_MODE_KILLER_UNDEAD	(0x80)


//ACHIEVEMENT_COUNTERS subtype
#define	ACHIEVEMENT_COUNTERS_COMBINE	(10)
#define	ACHIEVEMENT_COUNTERS_REFRESH	(20)

class QzqstarAchievements
{
private:
	//create a vector map to store all the player's achievements
	//key is player guid, value is a vector of AchievementsEntry
	std::map<uint32, std::vector<AchievementsEntry> > _playerAchievements;

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
	 *		VIP Systems Functions
	 *
	 *****************************************************/
	// note on data arrays
	// data1: vip level
	// data2: vip enabled features, each bit represents a feature, 1 means enabled, 0 means disabled

	void __init_VIP_Entry(Player *player);	//init the vip entry for the player

	// Get the player's VIP level
	uint32 GetVIPLevel(Player *player);
	bool SetVIPLevel(Player *player, uint32 level);

	// Get the player's VIP enabled features
	uint32 	GetVIPFeatures(Player *player);
	bool 	SetVIPFeatures(Player *player, uint32 features);

	// Get & Set player's challenging mode, using data3 to store the challenging mode
	uint32 	GetChallengeMode(Player *player);
	void 	SetChallengeMode(Player *player, uint32 mode);

	// Social points system functions
	uint32 	GetSocialPoints(Player * _player);
	void 	SetSocialPoints(Player * _player, uint32 _points);

	// Promotions for the player
	uint32  GetPromotions(Player * _player);
	void    SetPromotions(Player * _player, uint32 _promotions);

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
	// data1: quest id
	// data2: quest complete count (how many times the quest has been completed)
	
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
	uint32 GetDungeonsInfo(Player *player, uint32 ac_mapId /* should be mapped to 0-15 */);

	//save the player's dungeons information, add the dungeons to the player's achievements vector
	void SetDungeonsInfo(Player *player, uint32 ac_mapId /* should be mapped to 0-15 */, uint32 value);



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



protected:
	std::vector<AchievementsEntry> entries;
	time_t _constInterval;
	time_t _current;
};

#define sQZAchievements MaNGOS::Singleton<QzqstarAchievements>::Instance()


#endif
#pragma once
