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



/* ================================================================================================================== */
/* ========================= VIP system  ============================================================================ */
/* ================================================================================================================== */
#pragma region  VIP system

//__init_VIP_Entry
void QzqstarAchievements::__init_VIP_Entry(Player * _player)
{
	//check _player if none
	if (!_player)
		return;

	//add one VIP level to the _playerAchievements vector map of this player if not found
	AchievementsEntry e;
	e.guid = _player->GetGUID();
	e.type = ACHIEVEMENT_VIP;
	e.subType = 0;
	e.data1 = 1;
	e.data2 = 0;
	e.data3 = 0;
	e.data4 = 0;
	e.note = "";
	e.data5 = 0;	//reuse for Social Points (967 for faction old version)
	e.data6 = 0;
	e.data7 = 0;
	e.data8 = 0;
	_playerAchievements[_player->GetGUID()].push_back(e);
	sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "[QzqstarAchievements::GetVIPLevel] Not found but init one VIP Entry: %u", _player->GetGUID());
}

/**
 * @brief get the player's VIP level, if the player has not got any VIP level, return 1.
 * @param _player the player to check
 * @return the player's VIP level, if the player has not got any VIP level, return 1.
 */
uint32 QzqstarAchievements::GetVIPLevel(Player * _player)
{
	//check _player if none
	if (!_player)
		return 0;

	//iterate the _playerAchievements vector map of this player to find the VIP level the player has got
	for (auto it = _playerAchievements[_player->GetGUID()].begin(); it!= _playerAchievements[_player->GetGUID()].end(); ++it)
	{
		AchievementsEntry e = *it;
		if (e.type == ACHIEVEMENT_VIP)
		{
			sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "[QzqstarAchievements::GetVIPLevel] Player:%s GetVIPLevel: %u", _player->GetName(), e.data1);
			return e.data1;
		}	
	}


	//add one VIP level to the _playerAchievements vector map of this player if not found
	__init_VIP_Entry(_player);
	return 1;
}

/**
 * @brief set the player's VIP level and add the VIP level to the player's achievements vector
 * @param _player the player to check
 * @param level the VIP level to set, if the level is 0, the VIP level will be set to 1.
 * @return true if the VIP level is set successfully, false if the VIP level is not set successfully.
 */
bool QzqstarAchievements::SetVIPLevel(Player * _player, uint32 level)
{
	//check _player if none
	if (!_player)
		return false;

	//check if the level is 0, if so, set it to 1
	if (level == 0)
		level = 1;

	//iterate the _playerAchievements vector map of this player to find the VIP level the player has got
	for (auto it = _playerAchievements[_player->GetGUID()].begin(); it!= _playerAchievements[_player->GetGUID()].end(); ++it)	
	{
		AchievementsEntry& e = *it;
		if (e.type == ACHIEVEMENT_VIP)
		{
			//slog out with player info
			sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "[QzqstarAchievements::SetVIPLevel] Player:%s SetVIPLevel: %u", _player->GetName(), level);
			e.data1 = level;
			break;
		}	
	}

	return true;
}


//VIP enabled features: 1-16, each bit represents a feature, 1 means enabled, 0 means disabled
//get the player's VIP enabled features, if the player has not got any VIP enabled features, return 0.
uint32 QzqstarAchievements::GetVIPFeatures(Player * _player)
{
	//check _player if none
	if (!_player) return 0;	

	//iterate the _playerAchievements vector map of this player to find the VIP level the player has got
	for (auto it = _playerAchievements[_player->GetGUID()].begin(); it!= _playerAchievements[_player->GetGUID()].end(); ++it)
	{
		AchievementsEntry e = *it;
		if (e.type == ACHIEVEMENT_VIP)
		{
			return e.data2;	
		}	
	}

	//add one VIP level to the _playerAchievements vector map of this player if not found
	__init_VIP_Entry(_player);
	return 0;
}

//set the player's VIP enabled features and add the VIP enabled features to the player's achievements vector
bool QzqstarAchievements::SetVIPFeatures(Player * _player, uint32 features)
{
	//check _player if none
	if (!_player) return false;

	//iterate the _playerAchievements vector map of this player to find the VIP level the player has got
	for (auto it = _playerAchievements[_player->GetGUID()].begin(); it!= _playerAchievements[_player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == ACHIEVEMENT_VIP)
		{
			e.data2 |= features;
			break;
		}	
	}
	
	return true;
}

//get the player's challenging mode, if the player has not got any challenging mode, return 0.
uint32 QzqstarAchievements::GetChallengeMode(Player * _player)
{
	//check _player if none
	if (!_player) return 0;	
	//iterate the _playerAchievements vector map of this player to find the VIP level the player has got
	for (auto it = _playerAchievements[_player->GetGUID()].begin(); it!= _playerAchievements[_player->GetGUID()].end(); ++it)
	{
		AchievementsEntry e = *it;
		if (e.type == ACHIEVEMENT_VIP)
		{
			return e.data3;
		}
	}
	//add one VIP level to the _playerAchievements vector map of this player if not found
	__init_VIP_Entry(_player);
	return 0;
}


//set the player's challenging mode and add the challenging mode to the player's achievements vector
void QzqstarAchievements::SetChallengeMode(Player * _player, uint32 mode)
{
	//check _player if none
	if (!_player) return;

	//iterate the _playerAchievements vector map of this player to find the VIP level the player has got
	for (auto it = _playerAchievements[_player->GetGUID()].begin(); it!= _playerAchievements[_player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == ACHIEVEMENT_VIP)
		{
			e.data3 = mode;
			break;
		}	
	}	
}

//get set social points for player, if the player has not got any social points, return 0.
uint32 QzqstarAchievements::GetSocialPoints(Player * _player)
{
	//check _player if none
	if (!_player) return 0;

	//iterate the _playerAchievements vector map of this player to find the VIP level the player has got
	for (auto it = _playerAchievements[_player->GetGUID()].begin(); it!= _playerAchievements[_player->GetGUID()].end(); ++it)
	{
		AchievementsEntry e = *it;
		if (e.type == ACHIEVEMENT_VIP)
		{
			return e.data5;
		}	
	}

	//add one VIP level to the _playerAchievements vector map of this player if not found
	__init_VIP_Entry(_player);
	return 0;
}

//set social points for player and add the social points to the player's achievements vector
void QzqstarAchievements::SetSocialPoints(Player * _player, uint32 points)
{
	//check _player if none
	if (!_player) return;

	//iterate the _playerAchievements vector map of this player to find the VIP level the player has got
	for (auto it = _playerAchievements[_player->GetGUID()].begin(); it!= _playerAchievements[_player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == ACHIEVEMENT_VIP)
		{
			e.data5 = points;
			break;
		}	
	}	
}


//get the player's promotions, if the player has not got any promotions, return 0.
uint32 QzqstarAchievements::GetPromotions(Player * _player)
{
	//check _player if none
	if (!_player) return 0;

	//iterate the _playerAchievements vector map of this player to find the VIP level the player has got
	for (auto it = _playerAchievements[_player->GetGUID()].begin(); it!= _playerAchievements[_player->GetGUID()].end(); ++it)
	{
		AchievementsEntry e = *it;	
		if (e.type == ACHIEVEMENT_VIP)
		{
			return e.data6;
		}	
	}	

	//add one VIP level to the _playerAchievements vector map of this player if not found
	__init_VIP_Entry(_player);
	return 0;
}

//set the player's promotions and add the promotions to the player's achievements vector
void QzqstarAchievements::SetPromotions(Player * _player, uint32 promotions)
{
	//check _player if none
	if (!_player) return;

	//iterate the _playerAchievements vector map of this player to find the VIP level the player has got
	for (auto it = _playerAchievements[_player->GetGUID()].begin(); it!= _playerAchievements[_player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == ACHIEVEMENT_VIP)
		{
			e.data6 = promotions;
			break;
		}	
	}	
}

#pragma endregion


/* ================================================================================================================== */
/* ========================= Rune system  ============================================================================ */
/* ================================================================================================================== */
#pragma region  Rune system
/**
 * @brief get the player's Rune Slots, if the player has not got any Rune Slots, return 2.
 * @param _player the player to check
 * @return get the player's Rune Slots, if the player has not got any Rune Slots, return 2.
 */
uint32 QzqstarAchievements::GetRuneSlots(Player * _player)
{
	//check _player if none
	if (!_player)
		return 0;

	//iterate the _playerAchievements vector map of this player to find the VIP level the player has got
	for (auto it = _playerAchievements[_player->GetGUID()].begin(); it!= _playerAchievements[_player->GetGUID()].end(); ++it)
	{
		AchievementsEntry e = *it;
		if (e.type == ACHIEVEMENT_RUNE)
		{
			sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "[QzqstarAchievements::GetRuneSlots] Player:%s GetRuneSlots: %u", _player->GetName(), e.data1);
			return e.data1;
		}	
	}

	//add one VIP level to the _playerAchievements vector map of this player if not found
	AchievementsEntry e;
	e.guid = _player->GetGUID();
	e.type = ACHIEVEMENT_RUNE;
	e.subType = 0;
	e.data1 = 2;
	e.data2 = 0;
	e.data3 = 0;
	e.data4 = 0;
	e.note = "";
	_playerAchievements[_player->GetGUID()].push_back(e);

	sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "[QzqstarAchievements::GetRuneSlots] Not found but init one Rune Entry: %u", _player->GetGUID());
	return 2;
}

/**
 * @brief set the player's Rune Slots and add the Rune Slots to the player's achievements vector
 * @param _player the player to check
 * @param numbers the Rune Slots to set, if the numbers is 0, the Rune Slots will be set to 2.
 * @return true if the Rune Slots is set successfully, false if the Rune Slots is not set successfully.
 */
bool QzqstarAchievements::SetRuneSlots(Player * _player, uint32 numbers)
{
	//check _player if none
	if (!_player)
		return false;

	//check if the numbers is 0, if so, set it to 2
	if (numbers < 2)
		numbers = 2;

	//iterate the _playerAchievements vector map of this player to find the VIP level the player has got
	for (auto it = _playerAchievements[_player->GetGUID()].begin(); it!= _playerAchievements[_player->GetGUID()].end(); ++it)	
	{
		AchievementsEntry& e = *it;
		if (e.type == ACHIEVEMENT_RUNE)
		{
			//slog out with player info
			sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "[QzqstarAchievements::SetRuneSlots] Player:%s SetRuneSlots: %u", _player->GetName(), numbers);
			e.data1 = numbers;
			break;
		}	
	}

	return true;
}

#pragma endregion



/* ================================================================================================================== */
/* ======================= Quest system  ============================================================================ */
/* ================================================================================================================== */

#pragma region  Quest system
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

#pragma endregion

/* ================================================================================================================== */
/* ========================= Pet system  ============================================================================ */
/* ================================================================================================================== */

#pragma region  Pet system

//get the pet entry
AchievementsEntry QzqstarAchievements::GetPetEntry(Player * _player)
{
	//check _player if none
	if (!_player)
		return AchievementsEntry();

	//iterate the _playerAchievements vector map of this player to find the pet information
	for (auto it = _playerAchievements[_player->GetGUID()].begin(); it!= _playerAchievements[_player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == ACHIEVEMENT_PETS)
		{
			return e;
		}	
	}
	//add one pet to the _playerAchievements vector map of this player if not found
	AchievementsEntry e;
	e.guid = _player->GetGUID();
	e.type = ACHIEVEMENT_PETS;
	e.subType = 1;
	e.data1 = 10000;	//pet level, happiness level, relationship level
	e.data2 = 0;
	e.data3 = 0;
	e.data4 = 0;
	e.note = "";
	e.data5 = 0;	//pet level, happiness level, relationship level
	e.data6 = 0;
	e.data7 = 0;
	e.data8 = 0;
	_playerAchievements[_player->GetGUID()].push_back(e);

	// sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Not found but init one Pet Entry: %u", _player->GetGUID());
	return e;
}

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
	e.data2 = 0;	
	e.data3 = 0;
	e.data4 = 0;	
	e.note = "";
	e.data5 = 0;	
	e.data6 = 0;
	e.data7 = 0;
	e.data8 = 0;
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

				//summon new pet, and enable the pet if not enabled yet
				switch(petType)
				{
					case 1: if(e.data1 < 10000) e.data1=10000; break;	//summon pet 1	
					case 2: if(e.data2 < 10000) e.data2=10000; break;	//summon pet 2
					case 3: if(e.data3 < 10000) e.data3=10000; break;	//summon pet 3
					case 4: if(e.data4 < 10000) e.data4=10000; break;	//summon pet 4
					case 5: if(e.data5 < 10000) e.data5=10000; break;	//summon pet 5
					case 6: if(e.data6 < 10000) e.data6=10000; break;	//summon pet 6
					case 7: if(e.data7 < 10000) e.data7=10000; break;	//summon pet 7
				}

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



/* ================================================================================================================== */
/* ========================= Dungeons system  ======================================================================= */
/* ================================================================================================================== */
uint32 QzqstarAchievements::GetDungeonsInfo(Achievement_t _mapType, Player *player, uint32 ac_mapId /* should be mapped to 0-15 */)
{
	//check _player if none
	if (!player || ac_mapId > 19)
		return 0;

	if(_mapType !=ACHIEVEMENTS_DUNGEONS && _mapType!=ACHIEVEMENTS_RAIDS)
		return 0;

	//iterate the _playerAchievements vector map of this player to find the pet information
	for (auto it = _playerAchievements[player->GetGUID()].begin(); it!= _playerAchievements[player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == _mapType)
		{
			//if found, return the miscValue data
			sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Player:%s Loaded Dungeons, _ac ID: %u", player->GetName(), ac_mapId);

			//get the ac_mapId, 0-15, and return the data1, data2, data3, data4, data5, data6, data7, data8
			switch(ac_mapId)
			{
				case 0: return (e.data1) & 0xF;
				case 1: return (e.data1 >> 4) & 0xF;
				case 2: return (e.data1 >> 8) & 0xF;
				case 3: return (e.data1 >> 12) & 0xF;
				case 4: return (e.data2) & 0xF;
				case 5: return (e.data2 >> 4) & 0xF;
				case 6: return (e.data2 >> 8) & 0xF;
				case 7: return (e.data2 >> 12) & 0xF;
				case 8: return (e.data3) & 0xF;
				case 9: return (e.data3 >> 4) & 0xF;
				case 10: return (e.data3 >> 8) & 0xF;
				case 11: return (e.data3 >> 12) & 0xF;
				case 12: return (e.data4) & 0xF;
				case 13: return (e.data4 >> 4) & 0xF;
				case 14: return (e.data4 >> 8) & 0xF;
				case 15: return (e.data4 >> 12) & 0xF;
				case 16: return (e.data5) & 0xF;
				case 17: return (e.data5 >> 4) & 0xF;
				case 18: return (e.data5 >> 8) & 0xF;
				case 19: return (e.data5 >> 12) & 0xF;
				default: return 0;
			}	
		}	
	}

	//if not found, create one and return it
	AchievementsEntry e;
	e.guid = player->GetGUID();
	e.type = _mapType;
	e.subType = 0;
	e.data1 = 0;
	e.data2 = 0;
	e.data3 = 0;
	e.data4 = 0;
	e.note = "";
	e.data5 = 0;
	e.data6 = 0;
	e.data7 = 0;
	e.data8 = 0;
	_playerAchievements[player->GetGUID()].push_back(e);

	sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Player:%s Init Dungeons: %u", player->GetName(), e.subType);

	//default return code, means level=1, happy and relation to be zero.
	return 0;
}

void QzqstarAchievements::SetDungeonsInfo(Achievement_t _mapType, Player *player, uint32 ac_mapId /* should be mapped to 0-19 */, uint32 value)
{
	//check _player if none
	if (!player || value>15 || ac_mapId > 19) return;

	if(_mapType!=ACHIEVEMENTS_DUNGEONS && _mapType!=ACHIEVEMENTS_RAIDS) return;

	//iterate the _playerAchievements vector map of this player to find the pet information
	for (auto it = _playerAchievements[player->GetGUID()].begin(); it!= _playerAchievements[player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == _mapType)
		{
			//if found, set the miscValue data
			switch(ac_mapId)
			{
				case 0: e.data1 = (e.data1 & 0xFFF0) | (value); break;
				case 1: e.data1 = (e.data1 & 0xFF0F) | (value << 4); break;
				case 2: e.data1 = (e.data1 & 0xF0FF) | (value << 8); break;
				case 3: e.data1 = (e.data1 & 0x0FFF) | (value << 12); break;
				case 4: e.data2 = (e.data2 & 0xFFF0) | (value); break;
				case 5: e.data2 = (e.data2 & 0xFF0F) | (value << 4); break;
				case 6: e.data2 = (e.data2 & 0xF0FF) | (value << 8); break;
				case 7: e.data2 = (e.data2 & 0x0FFF) | (value << 12); break;
				case 8: e.data3 = (e.data3 & 0xFFF0) | (value); break;
				case 9: e.data3 = (e.data3 & 0xFF0F) | (value << 4); break;
				case 10: e.data3 = (e.data3 & 0xF0FF) | (value << 8); break;
				case 11: e.data3 = (e.data3 & 0x0FFF) | (value << 12); break;
				case 12: e.data4 = (e.data4 & 0xFFF0) | (value); break;
				case 13: e.data4 = (e.data4 & 0xFF0F) | (value << 4); break;
				case 14: e.data4 = (e.data4 & 0xF0FF) | (value << 8); break;
				case 15: e.data4 = (e.data4 & 0x0FFF) | (value << 12); break;
				case 16: e.data5 = (e.data5 & 0xFFF0) | (value); break;
				case 17: e.data5 = (e.data5 & 0xFF0F) | (value << 4); break;
				case 18: e.data5 = (e.data5 & 0xF0FF) | (value << 8); break;
				case 19: e.data5 = (e.data5 & 0x0FFF) | (value << 12); break;
				default: break;
			}	
		}	
	}
}


/* ================================================================================================================== */
/* ========================= Zitiao system  ========================================================================= */
/* ================================================================================================================== */
//get the player's zitiaos information, return a vector of AchievementsEntry
AchievementsEntry QzqstarAchievements::GetZitiaosInfo(Player *player)
{
	//check _player if none
	if (!player)
		return AchievementsEntry();

	//iterate the _playerAchievements vector map of this player to find the zitiaos information
	for (auto it = _playerAchievements[player->GetGUID()].begin(); it!= _playerAchievements[player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == ACHIEVEMENTS_ZITIAO)
		{
			return e;
		}	
	}

	//if not found, create one and return it
	AchievementsEntry e;
	e.guid = player->GetGUID();
	e.type = ACHIEVEMENTS_ZITIAO;
	e.subType = 4;
	e.data1 = 9140;
	e.data2 = 0;
	e.data3 = 0;
	e.data4 = 0;
	e.note = "";
	e.data5 = 0;
	e.data6 = 0;
	e.data7 = 0;
	e.data8 = 0;
	_playerAchievements[player->GetGUID()].push_back(e);

	// sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Not found but init one Zitiao Entry: %u", player->GetGUID());
	return e;
}

//save the player's zitiaos information, add the zitiaos to the player's achievements vector
void QzqstarAchievements::SetZitiaosInfo(Player *player, AchievementsEntry entry)
{
	//check _player if none
	if (!player)
		return;
	
	//iterate the _playerAchievements vector map of this player to find the zitiaos information
	for (auto it = _playerAchievements[player->GetGUID()].begin(); it!= _playerAchievements[player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == ACHIEVEMENTS_ZITIAO)
		{
			e.subType = entry.subType;
			e.data1 = entry.data1;
			e.data2 = entry.data2;
			e.data3 = entry.data3;
			e.data4 = entry.data4;
			e.note = entry.note;
			e.data5 = entry.data5;
			e.data6 = entry.data6;
			e.data7 = entry.data7;
			e.data8 = entry.data8;
			break;	
		}	
	}

	//if not found, create one and return it
	AchievementsEntry e;
	e.guid = player->GetGUID();
	e.type = ACHIEVEMENTS_ZITIAO;
	e.subType = entry.subType;
	e.data1 = entry.data1;
	e.data2 = entry.data2;
	e.data3 = entry.data3;
	e.data4 = entry.data4;
	e.note = entry.note;
	e.data5 = entry.data5;
	e.data6 = entry.data6;
	e.data7 = entry.data7;
	e.data8 = entry.data8;
	_playerAchievements[player->GetGUID()].push_back(e);
	
}

#pragma endregion
