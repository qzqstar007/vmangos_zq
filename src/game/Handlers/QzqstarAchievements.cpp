#include "Database/DatabaseEnv.h"
#include "World.h"
#include "Log.h"
#include "ProgressBar.h"
#include "Policies/SingletonImp.h"
#include "Util.h"

#include "QzqstarAchievements.h"

INSTANTIATE_SINGLETON_1(QzqstarAchievements);

QzqstarAchievements::QzqstarAchievements()
{
	_constInterval = sWorld.getConfig(CONFIG_UINT32_AUTOBROADCAST_INTERVAL);
	_current = 0;
}

QzqstarAchievements::~QzqstarAchievements()
{
	entries.clear();
}

void QzqstarAchievements::Load()
{
	entries.clear();												//1			2		3			 4		5		6		7		 8		9
	std::unique_ptr<QueryResult> result(WorldDatabase.Query("SELECT `entry`, `guid`,  `type`, `subType`,`data1`,`data2`,`data3`,`data4`, `note`, FROM `a_achivements`"));

	if (!result)
	{
		BarGoLink bar(1);
		bar.step();

		sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "");
		sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, ">> Loaded 0 Achievements records.");
		return;
	}

	uint32 count = 0;
	BarGoLink bar(result->GetRowCount());

	Field* fields;
	do
	{
		bar.step();
		AchievementsEntry e;
		fields = result->Fetch();

		e.entry = fields[0].GetInt32();
		e.guid = fields[1].GetInt32();
		e.type = fields[2].GetInt32();
		e.subType = fields[3].GetInt32();
		e.data1 = fields[4].GetInt32();
		e.data2 = fields[5].GetInt32();
		e.data3 = fields[6].GetInt32();
		e.data4 = fields[7].GetInt32();
		e.note = fields[8].GetCppString();

		entries.push_back(e);
		++count;
	} while (result->NextRow());

	sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "");
	sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, ">> Loaded %u Achievements records.", count);

}

void QzqstarAchievements::Update(uint32 diff)
{
	if (entries.empty())
		return;

	_current += diff;

	if (_current >= _constInterval)
	{
		//AutoBroadCastEntry entry = SelectRandomContainerElement(entries);
		//sWorld.SendWorldText(entry.stringId);
		_current = 0;
	}
}
