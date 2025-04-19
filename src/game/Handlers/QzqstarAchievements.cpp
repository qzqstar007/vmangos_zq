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

void QzqstarAchievements::Init()
{
	entries.clear();												//1			2		3			 4		5		6		7		 8		9
	CharacterDatabase.PExecute("update `a_achievements` set data1=0, data2=0 where type=100");
	
	sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "");
	sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, ">> Reset a_achievements records.");

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

//load player's achievements from database a_achievements
int32 QzqstarAchievements::Load(Player * _player)
{
	std::unique_ptr<QueryResult> result(CharacterDatabase.PQuery("SELECT `guid`, `type`, `subType`,`data1`,`data2`,`data3`,`data4`, `note` FROM `a_achievements` WHERE `guid` = '%u'", _player->GetGUID()));
	if (!result)
	{
		return 0;
	}

	//clear the _playerAchievements vector map of this player first
	if(_player)
		_playerAchievements.erase(_player->GetGUID());

	//set the counter
	int32 counter = 0;
	Field* fields;
	do
	{
		fields = result->Fetch();

		//save fields to struct AchievementsEntry
		AchievementsEntry e;
		e.guid = fields[0].GetInt32();
		e.type = fields[1].GetInt32();
		e.subType = fields[2].GetInt32();
		e.data1 = fields[3].GetInt32();
		e.data2 = fields[4].GetInt32();
		e.data3 = fields[5].GetInt32();
		e.data4 = fields[6].GetInt32();
		e.note = fields[7].GetCppString();
		//save to vector maps _playerAchievements regardless of type
		_playerAchievements[_player->GetGUID()].push_back(e);
		
		counter ++;
	} while (result->NextRow());

	return counter;
}

//Create the SQL and save to database a_achievements
void QzqstarAchievements::Save(Player * _player)
{
	//check _player if none
	if (!_player)
		return;

	//iterate the _playerAchievements vector map of this player to save to database a_achievements
	for (auto it = _playerAchievements[_player->GetGUID()].begin(); it != _playerAchievements[_player->GetGUID()].end(); ++it)
	{
		AchievementsEntry e = *it;
		CharacterDatabase.PExecute("Replace into `a_achievements` (`guid`, `type`, `subType`,`data1`,`data2`,`data3`,`data4`, `note`) VALUES('%u', '%u', '%u', '%u', '%u', '%u', '%u', '%s')",
			e.guid,
			e.type,
			e.subType,
			e.data1,
			e.data2,
			e.data3,
			e.data4,
			e.note.c_str()
			);
	}
}


/**
 * @brief get the custom quest id the player has accepted 
 * @param _player the player to check
 * @return the custom quest id the player has accepted, 
 *		0 if the player has not accepted any custom quest yet.
 */
int32 QzqstarAchievements::GetCustomQuestID(Player * _player)
{
	bool found = false;

	//check _player if none
	if (!_player)
		return 0;

	//iterate the _playerAchievements vector map of this player to find the custom quest id the player has accepted
	for (auto it = _playerAchievements[_player->GetGUID()].begin(); it!= _playerAchievements[_player->GetGUID()].end(); ++it)
	{
		AchievementsEntry e = *it;
		if (e.type == ACHIEVEMENT_CUSTOM_QUEST)
		{
			sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "GetCustomQuestID: %u", e.data1);

			found = true;
			return e.data1;
		}
	}

	if (!found)
	{
		//add one custom quest to the _playerAchievements vector map of this player
		AchievementsEntry e;
		e.guid = _player->GetGUID();
		e.type = ACHIEVEMENT_CUSTOM_QUEST;
		e.subType = 0;
		e.data1 = 0;
		e.data2 = 0;
		e.data3 = 0;
		e.data4 = 0;
		e.note = "";
		_playerAchievements[_player->GetGUID()].push_back(e);

		sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Not found but init one Quest Entry: %u", _player->GetGUID());
	}

	return 0;
}

/**
 * @brief save the custom quest id the player has accepted 
 * @param _player the player to check
 * @param the custom quest id the player has accepted
 */
 void QzqstarAchievements::SetCustomQuestID(Player * _player, int32 questID)
 {
	 //check _player if none
	 if (!_player)
		 return;

	 //iterate the _playerAchievements vector map of this player to find the custom quest id the player has accepted
	 for (auto it = _playerAchievements[_player->GetGUID()].begin(); it!= _playerAchievements[_player->GetGUID()].end(); ++it)
	 {
		 AchievementsEntry& e = *it;
		 if (e.type == ACHIEVEMENT_CUSTOM_QUEST)
		 {
			sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "SetCustomQuestID: %u", questID);
			e.data1 = questID;
			break;
		 }	
	 }
 }
 

/**
 * @brief finish custome quest that the player has got
 * @param _player the player to check
 * @return none.
 */
void QzqstarAchievements::FinishCustomQuest(Player * _player, bool _increaseCounter)
{
	//check _player if none
	if (!_player)
		return;

	//iterate the _playerAchievements vector map of this player to find the custom quest id the player has accepted
	for (auto it = _playerAchievements[_player->GetGUID()].begin(); it!= _playerAchievements[_player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == ACHIEVEMENT_CUSTOM_QUEST)
		{
			e.data1 = 0;
			if (_increaseCounter)
			{
				e.data2++;	//TODAY
				e.data3++;	//TOTAL
			}
			break;
		}
	}
}

/**
* @brief finish custome quest that the player has got
* @param _player the player to check
* @return none.
*/
uint32 QzqstarAchievements::GetQuestDoneCounters(Player * _player)
{
	//check _player if none
	if (!_player)
		return 0;

	//iterate the _playerAchievements vector map of this player to find the custom quest id the player has accepted
	for (auto it = _playerAchievements[_player->GetGUID()].begin(); it != _playerAchievements[_player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == ACHIEVEMENT_CUSTOM_QUEST)
		{
			//TODAY, TOTAL
			return MAKE_PAIR32(e.data2, e.data3);
		}
	}

	return 0;
}


/* ================================================================================================================== */
/* ========================= Pet system  ============================================================================ */
/* ================================================================================================================== */
//get the player's pet information, return a vector of AchievementsEntry
AchievementsEntry* QzqstarAchievements::LoadPetInfo(Player * _player)
{
	//check _player if none
	if (!_player)
		return nullptr;

	//iterate the _playerAchievements vector map of this player to find the pet information
	for (auto it = _playerAchievements[_player->GetGUID()].begin(); it!= _playerAchievements[_player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == ACHIEVEMENT_PETS)
		{
			return &e;
		}
	}

	//if not found, create one and return it
	AchievementsEntry e;
	e.guid = _player->GetGUID();
	e.type = ACHIEVEMENT_PETS;
	e.subType = 0;
	e.data1 = 0;	//pet level
	e.data2 = 0;	//pet happiness level
	e.data3 = 0;	//pet max level
	e.data4 = 0;	//pet type, active pet
	e.note = "";
	_playerAchievements[_player->GetGUID()].push_back(e);

	return &e;
}

