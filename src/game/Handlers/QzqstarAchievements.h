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




class QzqstarAchievements
{
public:
	QzqstarAchievements();
	~QzqstarAchievements();

	void Load();
	void Update(uint32 diff);

protected:
	std::vector<AchievementsEntry> entries;
	time_t _constInterval;
	time_t _current;
};

#define sQzqstarAchievements MaNGOS::Singleton<QzqstarAchievements>::Instance()


#endif
#pragma once
