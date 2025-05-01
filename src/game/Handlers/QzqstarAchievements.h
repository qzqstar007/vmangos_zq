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
	ACHIEVEMENT_VIP 		  	= 66,
	ACHIEVEMENT_RUNE 		  	= 88,
	ACHIEVEMENT_CUSTOM_QUEST 	= 100,
	ACHIEVEMENT_SOCIAL_POINTS 	= 200,
	ACHIEVEMENT_PETS = 300,
	ACHIEVEMENT_COUNTERS	=	1000,	//For combine and refreshing...
};


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

	// Get the player's VIP level
	uint32 GetVIPLevel(Player *player);

	// Set the player's VIP level and add the VIP level to the player's achievements vector
	bool SetVIPLevel(Player *player, uint32 level);


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

protected:
	std::vector<AchievementsEntry> entries;
	time_t _constInterval;
	time_t _current;
};

#define sQZAchievements MaNGOS::Singleton<QzqstarAchievements>::Instance()


#endif
#pragma once
