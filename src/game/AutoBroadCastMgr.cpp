#include "Database/DatabaseEnv.h"
#include "World.h"
#include "Log.h"
#include "ProgressBar.h"
#include "Policies/SingletonImp.h"
#include "Util.h"

#include "AutoBroadCastMgr.h"

#include "QzqstarAchievements.h"

INSTANTIATE_SINGLETON_1(AutoBroadCastMgr);

AutoBroadCastMgr::AutoBroadCastMgr()
{
    _constInterval = sWorld.getConfig(CONFIG_UINT32_AUTOBROADCAST_INTERVAL);
    _current = 0;
	_currentMinute = 0;
}

AutoBroadCastMgr::~AutoBroadCastMgr()
{
    entries.clear();
}

void AutoBroadCastMgr::Load()
{
    //init _lastBroadcast
	//_currentMinute = time(NULL);
    _lastBroadCast1Min = time(NULL);
	_lastBroadCast10Min = time(NULL);
	_lastBroadCast60Min = time(NULL);

    entries.clear();
    std::unique_ptr<QueryResult> result(WorldDatabase.Query("SELECT `string_id` FROM `autobroadcast`"));

    if (!result)
    {
        BarGoLink bar(1);
        bar.step();

        sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "");
        sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, ">> Loaded 0 AutoBroadCast message");
        return;
    }

    uint32 count = 0;
    BarGoLink bar(result->GetRowCount());

    Field* fields;
    do
    {
        bar.step();
        AutoBroadCastEntry e;
        fields = result->Fetch();

        e.stringId = fields[0].GetInt32();

        entries.push_back(e);
        ++count;
    }
    while (result->NextRow());

    sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "");
    sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, ">> Loaded %u AutoBroadCast messages", count);

}


#pragma region Update for the QzqstarAchievements

void AutoBroadCastMgr::UpdatePetStatus()
{
    //get the player online list from sWorld
    auto const& sessions = sWorld.GetAllSessions();

    sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "[AutoBroadCastMgr::UpdatePetStatus] triggered.");


    for (const auto& itr : sessions)
    {
        if (WorldSession* session = itr.second)
        {
            Player* _pl = session->GetPlayer();
            if (_pl && _pl->IsInWorld())
            {
                //get the player's pet info from sQZAchievementsMgr, and update the player's pet info to the database, and add the pet to the player's achievements vector, if the pet info has changed.
				sQZAchievements.UpdatePetPoints(_pl);
            }
        }
    }

}

#pragma endregion


void AutoBroadCastMgr::Update(uint32 diff)
{
    /*
    if (entries.empty())
        return;

    _current += diff;

    if (_current >= _constInterval)
    {
        AutoBroadCastEntry entry = SelectRandomContainerElement(entries);
        sWorld.SendWorldText(entry.stringId);
        _current = 0;
    } */

    //qzqstar, 250423, add custom broadcast message
	_current += diff;

	if (_current >= 60000)
	{
		_current = 0;
		_currentMinute++;
	}

    if (_lastBroadCast1Min != _currentMinute)
    {
		_lastBroadCast1Min = _currentMinute;
        //slog
        sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "[AutoBroadCastMgr::Update] 1 Min: %u.", _lastBroadCast1Min);

    }

    //10min section
    if(_lastBroadCast10Min != _currentMinute /10)
    {
		_lastBroadCast10Min = _currentMinute /10;
        //slog
        sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "[AutoBroadCastMgr::Update] 10 Min: %u", _lastBroadCast10Min);


		//1. update pets
		UpdatePetStatus();
    }

    //60min section
    if(_lastBroadCast60Min!= _currentMinute /60)
    {
        _lastBroadCast60Min = _currentMinute /60;
        //slog
        sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "[AutoBroadCastMgr::Update] 60 Min: %u", _lastBroadCast60Min); 
    }

}
