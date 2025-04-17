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
};

enum Achievement_t
{
	ACHIEVEMENT_CUSTOM_QUEST = 100,
	ACHIEVEMENT_SOCIAL_POINTS = 200,
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
	
protected:
	std::vector<AchievementsEntry> entries;
	time_t _constInterval;
	time_t _current;
};

#define sQZAchievements MaNGOS::Singleton<QzqstarAchievements>::Instance()


#endif
#pragma once
