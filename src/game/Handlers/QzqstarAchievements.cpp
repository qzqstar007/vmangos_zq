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
	std::unique_ptr<QueryResult> result(CharacterDatabase.PQuery("SELECT `guid`, `type`, `subType`,`data1`,`data2`,`data3`,`data4`, `note`, `data5`, `data6`, `data7`, `data8` FROM `a_achievements` WHERE `guid` = '%u'", _player->GetGUID()));
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
		e.data5 = fields[8].GetInt32();
		e.data6 = fields[9].GetInt32();
		e.data7 = fields[10].GetInt32();
		e.data8 = fields[11].GetInt32();
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
		CharacterDatabase.PExecute("Replace into `a_achievements` (`guid`, `type`, `subType`,`data1`,`data2`,`data3`,`data4`, `note`, `data5`, `data6`, `data7`, `data8`) VALUES('%u', '%u', '%u', '%u', '%u', '%u', '%u', '%s', '%u', '%u', '%u', '%u')",
			e.guid,
			e.type,
			e.subType,
			e.data1,
			e.data2,
			e.data3,
			e.data4,
			e.note.c_str(),
			e.data5,
			e.data6,
			e.data7,
			e.data8
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
			sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Player:%s GetCustomQuestID: %u", _player->GetName(), e.data1);

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
int32 QzqstarAchievements::GetActivePetInfo(Player * _player)
{
	//check _player if none
	if (!_player)
		return 0;

	//iterate the _playerAchievements vector map of this player to find the pet information
	for (auto it = _playerAchievements[_player->GetGUID()].begin(); it!= _playerAchievements[_player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == ACHIEVEMENT_PETS)
		{
			//if found, return the miscValue data
			sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Player:%s Loaded Pet: %u", _player->GetName(), e.subType);

			switch(e.subType)
			{	
				case 1: return (e.data1) + e.subType * 100000;
				case 2: return (e.data2) + e.subType * 100000;
				case 3: return (e.data3) + e.subType * 100000;
				case 4: return (e.data4) + e.subType * 100000;
				case 5: return (e.data5) + e.subType * 100000;
				case 6: return (e.data6) + e.subType * 100000;
				case 7: return (e.data7) + e.subType * 100000;
				case 8: return (e.data8) + e.subType * 100000;
			}
		}
	}

	//if not found, create one and return it
	AchievementsEntry e;
	e.guid = _player->GetGUID();
	e.type = ACHIEVEMENT_PETS;
	e.subType = 1;
	e.data1 = 10000;	//pet level, happiness level, relationship level
	e.data2 = 10000;	//pet happiness level
	e.data3 = 10000;	//pet max level
	e.data4 = 10000;	//pet type, active pet
	e.note = "";
	e.data5 = 10000;	
	e.data6 = 10000;
	e.data7 = 10000;
	e.data8 = 10000;
	_playerAchievements[_player->GetGUID()].push_back(e);

	sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Player:%s Init Pet: %u", _player->GetName(), e.subType);

	//default return code, means level=1, happy and relation to be zero.
	return 110000;
}

void QzqstarAchievements::SetActivePetInfo(Player *player, int32 miscValue)
{
	//check _player if none
	if (!player)
		return;

	if(miscValue > 100000)
	{
		//get the real pet info
		miscValue = miscValue % 100000;
	}
	//iterate the _playerAchievements vector map of this player to find the pet information
	for (auto it = _playerAchievements[player->GetGUID()].begin(); it != _playerAchievements[player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == ACHIEVEMENT_PETS)
		{
			//if found, set the miscValue data
			switch(e.subType)
			{
				case 1: e.data1 = miscValue; break;
				case 2: e.data2 = miscValue; break;
				case 3: e.data3 = miscValue; break;
				case 4: e.data4 = miscValue; break;
				case 5: e.data5 = miscValue; break;
				case 6: e.data6 = miscValue; break;
				case 7: e.data7 = miscValue; break;
				case 8: e.data8 = miscValue; break;
			}
		}
	}
}

//change active pet, range 1-7
void QzqstarAchievements::ChangeActivePet(Player *player, int32 petType)
{
	//check _player if none
	if (!player)
		return;

	//iterate the _playerAchievements vector map of this player to find the pet information
	for (auto it = _playerAchievements[player->GetGUID()].begin(); it!= _playerAchievements[player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == ACHIEVEMENT_PETS)
		{
			if(e.subType != petType) 
			{
				e.subType=petType;
				// despawn old pet before summon new
				if (player->GetMiniPet())
					player->RemoveMiniPet();
			}
		}
	}
}

//update pet points every some time
void QzqstarAchievements::UpdatePetPoints(Player *player)
{
	//check _player if none
	if (!player)
		return;

	//iterate the _playerAchievements vector map of this player to find the pet information
	for (auto it = _playerAchievements[player->GetGUID()].begin(); it!= _playerAchievements[player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == ACHIEVEMENT_PETS)
		{
			//decrease the relation ship for all
			if(e.data1%100 > 0) e.data1 = e.data1 - 1;	//relation level for pet 1
			if(e.data2%100 > 0) e.data2 = e.data2 - 1;	//relation level for pet 2
			if(e.data3%100 > 0) e.data3 = e.data3 - 1;	//relation level for pet 3
			if(e.data4%100 > 0) e.data4 = e.data4 - 1;	//relation level for pet 4
			if(e.data5%100 > 0) e.data5 = e.data5 - 1;	//relation level for pet 5
			if(e.data6%100 > 0) e.data6 = e.data6 - 1;	//relation level for pet 6
			if(e.data7%100 > 0) e.data7 = e.data7 - 1;	//relation level for pet 7
			if(e.data8%100 > 0) e.data8 = e.data8 - 1;	//relation level for pet 8
			
			switch(e.subType)
			{
				case 1: if(e.data1%100<98) e.data1 += 2; if(e.data1%10000 > 100) e.data1 -= 100; break;	//happy and relation for pet 1
				case 2: if(e.data2%100<98) e.data2 += 2; if(e.data2%10000 > 100) e.data2 -= 100; break;	//happy and relation for pet 2
				case 3: if(e.data3%100<98) e.data3 += 2; if(e.data3%10000 > 100) e.data3 -= 100; break;	//happy and relation for pet 3
				case 4: if(e.data4%100<98) e.data4 += 2; if(e.data4%10000 > 100) e.data4 -= 100; break;	//happy and relation for pet 4
				case 5: if(e.data5%100<98) e.data5 += 2; if(e.data5%10000 > 100) e.data5 -= 100; break;	//happy and relation for pet 5
				case 6: if(e.data6%100<98) e.data6 += 2; if(e.data6%10000 > 100) e.data6 -= 100; break;	//happy and relation for pet 6
				case 7: if(e.data7%100<98) e.data7 += 2; if(e.data7%10000 > 100) e.data7 -= 100; break;	//happy and relation for pet 7
				case 8: if(e.data8%100<98) e.data8 += 2; if(e.data8%10000 > 100) e.data8 -= 100; break;	//happy and relation for pet 8
			}
		}	
	}
}
