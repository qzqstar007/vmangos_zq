#include "Database/DatabaseEnv.h"
#include "World.h"
#include "Log.h"
#include "ProgressBar.h"
#include "Policies/SingletonImp.h"
#include "Util.h"

#include "QzqstarAchievements.h"
#include "custom/qzqstar_db.h"
#include "custom/qzqstar_custom.h"

INSTANTIATE_SINGLETON_1(QzqstarAchievements);

#define DATA_MAX		(2000000000)

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

static void _zeroData(AchievementsEntry * e)
{
	if(!e) return;
	e->subType = 0;	e->data0 = 0;
	e->data1 = 0; 	e->data2 = 0; 	e->data3 = 0;	e->data4 = 0;
	e->data5 = 0; 	e->data6 = 0; 	e->data7 = 0;	e->data8 = 0;
	e->data9 = 0; 	e->data10 = 0; 	e->data11 = 0;	e->data12 = 0;
	e->data13 = 0; 	e->data14 = 0; 	e->data15 = 0;
}

//load player's achievements from database a_achievements
int32 QzqstarAchievements::Load(Player * _player)
{
	if(!_player)  return -1;

	//set the counter
	bool  _account_found = false;
	int32 counter = 0;
	Field* fields;

	uint32 guid = _player->GetGUID();
	uint32 auid = _player->M_AccountID;

	//clear the _player->M_Achievements vector map of this player first
	_player->M_Achievements.clear();

	std::unique_ptr<QueryResult> result(CharacterDatabase.PQuery("SELECT `auid`, `guid`, `type`, `subType`, `data0`, `data1`,`data2`,`data3`,`data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15` FROM `a_achievements` WHERE (`guid` = '%u' or `guid` = '0') and `auid` = '%u'", guid, auid));
	if (result)
	{
		do
		{
			fields = result->Fetch();

			//save fields to struct AchievementsEntry
			AchievementsEntry e;
			e.auid = fields[0].GetInt32();			e.guid = fields[1].GetInt32();			e.type = fields[2].GetInt32();			e.subType = fields[3].GetInt32();
			e.data0 = fields[4].GetInt32();			e.data1 = fields[5].GetInt32();			e.data2 = fields[6].GetInt32();			e.data3 = fields[7].GetInt32();
			e.data4 = fields[8].GetInt32();			e.data5 = fields[9].GetInt32();			e.data6 = fields[10].GetInt32();			e.data7 = fields[11].GetInt32();
			e.data8 = fields[12].GetInt32();			e.data9 = fields[13].GetInt32();			e.data10 = fields[14].GetInt32();			e.data11 = fields[15].GetInt32();
			e.data12 = fields[16].GetInt32();			e.data13 = fields[17].GetInt32();			e.data14 = fields[18].GetInt32();			e.data15 = fields[19].GetInt32();

			if (e.type == ACHIEVEMENT_ACCOUNT) _account_found = true;
			_player->M_Achievements[e.type] = e;
			counter ++;
			
		} while (result->NextRow());
	}

	if (!_account_found)
	{
		AchievementsEntry e;
		e.auid = _player->M_AccountID; e.guid = 0;	e.type = ACHIEVEMENT_ACCOUNT;
		_zeroData(&e);
		_player->M_Achievements[e.type] = e;
		sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "[QzqstarAchievements::__init_Account_Entry] Not found but init one Player:%s (AccountID:%u) type:%u, data0=%u, data1=%u",
			_player->GetName(), _player->M_AccountID, e.type, e.data0, e.data1);
	}
	return counter;
}

//Create the SQL and save to database a_achievements
void QzqstarAchievements::Save(Player * _player)
{
	//check _player if none
	if(!_player) return;
	
	//iterate the _player->M_Achievements vector map of this player to save to database a_achievements
	for (auto it = _player->M_Achievements.begin(); it != _player->M_Achievements.end(); ++it)
	{
		AchievementsEntry e = it->second;
		CharacterDatabase.PExecute("Replace into `a_achievements` ( `auid`, `guid`, `type`, `subType`,`data0`,`data1`,`data2`,`data3`,`data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`) VALUES('%u', '%u', '%u', '%u', '%u', '%u', '%u', '%u', '%u', '%u', '%u', '%u', '%u', '%u', '%u', '%u', '%u', '%u', '%u', '%u')",
			e.auid,				e.guid,			e.type,			e.subType,			
			e.data0,			e.data1,			e.data2,			e.data3,			e.data4, 			e.data5,			e.data6,			e.data7,
			e.data8,			e.data9,			e.data10,			e.data11,			e.data12,			e.data13,			e.data14,			e.data15
			);

		//__LOG("Saving player:%s, type:%d, data9=%u, data15=%u.", _player->GetName(), e.type, e.data9, e.data15);
	}
}


/* ================================================================================================================== */
/* ========================= S8 new caculate system  ================================================================ */
/* ================================================================================================================== */
#pragma region S8 new caculate system
static AchievementsEntry& __init_Achivement_Entry(Player * _player, uint32 type)
{
	AchievementsEntry e;
	e.auid = _player->M_AccountID; e.guid = 0;	e.type = type;
	_zeroData(&e);
	_player->M_Achievements[e.type] = e;
	return _player->M_Achievements[e.type];
}


// ----------------------------- Account -----------------------------------------
static void __init_Account_Entry(Player *_player)
{
	if (_player->M_Achievements.find(ACHIEVEMENT_ACCOUNT) == _player->M_Achievements.end())
	{
		AchievementsEntry& e = __init_Achivement_Entry(_player, ACHIEVEMENT_ACCOUNT);
	}
}

uint32 QzqstarAchievements::GetAccAchieveData(Player * _player, uint32 type)
{
	if (!_player) return 0;
	__init_Account_Entry(_player);

	//return the player's achievement data
	uint32 _data = 0;
	switch(type)
	{
		case ACHIEVEMENT_ACCOUNT_VIP:				_data = _player->M_Achievements[ACHIEVEMENT_ACCOUNT].data0; break;
		case ACHIEVEMENT_ACCOUNT_TASK:				_data = _player->M_Achievements[ACHIEVEMENT_ACCOUNT].data1 / 1000; break;
		case ACHIEVEMENT_ACCOUNT_EXPLORE:			_data = _player->M_Achievements[ACHIEVEMENT_ACCOUNT].data1 % 1000; break;   //data 2, spared. 20251206
		case ACHIEVEMENT_ACCOUNT_TASK_CUSTOM_MAXID: _data = _player->M_Achievements[ACHIEVEMENT_ACCOUNT].data2 % 10000; break;
		case ACHIEVEMENT_ACCOUNT_PET_COLLECTION:	_data = _player->M_Achievements[ACHIEVEMENT_ACCOUNT].data3; break;
		case ACHIEVEMENT_ACCOUNT_REPUTATION_LIST:	_data = _player->M_Achievements[ACHIEVEMENT_ACCOUNT].data4; break;
		case ACHIEVEMENT_ACCOUNT_PROFESSION_SKILL:	_data = _player->M_Achievements[ACHIEVEMENT_ACCOUNT].data5; break;
		case ACHIEVEMENT_ACCOUNT_KILLING_NUMS:		_data = _player->M_Achievements[ACHIEVEMENT_ACCOUNT].data6; break;
		case ACHIEVEMENT_ACCOUNT_PVP_NUMS:			_data = _player->M_Achievements[ACHIEVEMENT_ACCOUNT].data7; break;
		case ACHIEVEMENT_ACCOUNT_GOLD_COLLECT:		_data = _player->M_Achievements[ACHIEVEMENT_ACCOUNT].data8; break;
		case ACHIEVEMENT_ACCOUNT_DUNGEON_NUMS1:		_data = _player->M_Achievements[ACHIEVEMENT_ACCOUNT].data9; break;
		case ACHIEVEMENT_ACCOUNT_DUNGEON_NUMS2:		_data = _player->M_Achievements[ACHIEVEMENT_ACCOUNT].data10; break;
		case ACHIEVEMENT_ACCOUNT_MAX_LEVEL:			_data = _player->M_Achievements[ACHIEVEMENT_ACCOUNT].data11/1000000; break; //combine the eq, bonus as well
		case ACHIEVEMENT_ACCOUNT_EQ_NUMS:			_data = (_player->M_Achievements[ACHIEVEMENT_ACCOUNT].data11/1000)%1000; break;
		case ACHIEVEMENT_ACCOUNT_BONUS_NUMS:		_data = _player->M_Achievements[ACHIEVEMENT_ACCOUNT].data11%1000; break;   //data12/13/14 spared.
		case ACHIEVEMENT_ACCOUNT_DAILY_REWARD:		_data = _player->M_Achievements[ACHIEVEMENT_ACCOUNT].data15; break;
		default:  
			__LOG("[QzqstarAchievements::GetAccAchieveData] Not found data for player:%s, type:%d", _player->GetName(), type);
			break;
	}
	return _data;
}


//set the type and data1 of the achievement
void QzqstarAchievements::SetAccAchieveData(Player * _player, uint32 type, uint32 data)
{
	if (!_player) return;

	//check the player's achievement map if none
	__init_Account_Entry(_player);

	AchievementsEntry& e = _player->M_Achievements[ACHIEVEMENT_ACCOUNT];
	switch(type)
	{
		case ACHIEVEMENT_ACCOUNT_VIP:			e.data0 = data; break;
		case ACHIEVEMENT_ACCOUNT_TASK:				e.data1 = data * 1000 + (e.data1 % 1000); break;
		case ACHIEVEMENT_ACCOUNT_EXPLORE:			e.data1 = (e.data1 / 1000) * 1000 + data; break;   //data 2, spared. 20251206
		case ACHIEVEMENT_ACCOUNT_TASK_CUSTOM_MAXID: e.data2 = e.data2/10000 * 10000 + data; break;	//data2, lowest 9999 for custom task maxid
		case ACHIEVEMENT_ACCOUNT_PET_COLLECTION:	e.data3 = data; break;
		case ACHIEVEMENT_ACCOUNT_REPUTATION_LIST:	e.data4 = data; break;
		case ACHIEVEMENT_ACCOUNT_PROFESSION_SKILL:	e.data5 = data; break;
		case ACHIEVEMENT_ACCOUNT_KILLING_NUMS:		e.data6 = data; break;
		case ACHIEVEMENT_ACCOUNT_PVP_NUMS:		e.data7 = data; break;
		case ACHIEVEMENT_ACCOUNT_GOLD_COLLECT:	e.data8 = data; break;
		case ACHIEVEMENT_ACCOUNT_DUNGEON_NUMS1:	e.data9 = data; break;
		case ACHIEVEMENT_ACCOUNT_DUNGEON_NUMS2:	e.data10 = data; break;
		case ACHIEVEMENT_ACCOUNT_MAX_LEVEL:				e.data11 = data*1000000 + e.data11%1000000; break; //combine the eq, bonus as well
		case ACHIEVEMENT_ACCOUNT_EQ_NUMS:				e.data11 = (e.data11/1000000)*1000000 + data * 1000 + e.data11%1000; break;
		case ACHIEVEMENT_ACCOUNT_BONUS_NUMS:			e.data11 = (e.data11/1000)*1000 + data; break; 
		case ACHIEVEMENT_ACCOUNT_DAILY_REWARD:			e.data15 = data; break;
		default:  
			__LOG("[QzqstarAchievements::SetAccAchieveData] Not found data for player:%s, type:%d", _player->GetName(), type);
			break;
	}

	//__LOG("[QzqstarAchievements::SetAccAchieveData] set data for player:%s, type:%d, data:%d", _player->GetName(), type, data);
}

uint32 QzqstarAchievements::GetAccountSum(Player * _player)
{
	if (!_player) return 0;
	__init_Account_Entry(_player);

	uint32 sum = 0;
	//data0 - VIP
	if(_player->M_Achiv_Account_VIP & VIP_MOUNT_LAND) sum += 10;
	if(_player->M_Achiv_Account_VIP & VIP_MOUNT_FLY) sum += 50;
	if(_player->M_Achiv_Account_VIP & VIP_MOUNT_TIGER) sum += 300;
	if(_player->M_Achiv_Account_VIP & VIP_MOUNT_GRIPH) sum += 300;

	//data1 - task
	if (_player->M_Achiv_Account_Task < 500) sum += _player->M_Achiv_Account_Task; else sum += 500;

	//data2 - explore
	if (_player->M_Achiv_Account_Explore < 100) sum += _player->M_Achiv_Account_Explore*2; else sum += 200;

	//data3 - pet collects
	uint32 _pet = _player->M_Achiv_Account_Pet_Collection;
	uint32 _index = 0;
	while(_pet > 0)
	{
		if(_pet & 1) { sum += 10; if(_index >= 12) sum += 20; }
		_index ++;
		_pet >>= 1;
	}

	//data4 - reputation
	sum += COUNT_ONES(_player->M_Achiv_Account_Reputation_List) * 50;

	//data5 - profession
	sum += COUNT_ONES(_player->M_Achiv_Account_Profession_Skill) * 50;
	
	//data6 - killing nums
	if(_player->M_Achiv_Account_Killing_Nums > 1000 * 100) sum += 1000;
	else sum += _player->M_Achiv_Account_Killing_Nums /100;

	//data7 - pvp nums
	if(_player->M_Achiv_Account_PVP_Nums > 1000 * 100) sum += 1000;
	else sum += _player->M_Achiv_Account_PVP_Nums /100;

	//data8 - gold collect, every 20g gives 1 point
	if(_player->M_Achiv_Account_Gold_Collect > 20000 * 10000) sum += 1000;
	else sum += _player->M_Achiv_Account_Gold_Collect/200000;

	//data9 - dungeon nums 1
	_pet = _player->M_Achiv_Account_Dungeon_NUMS1; //reuse the _pet
	while (_pet > 0)
	{
		sum += (_pet % 10) * 2;
		_pet /= 10;
	}

	//data10 - dungeon nums 2
	_pet = _player->M_Achiv_Account_Dungeon_NUMS2; //reuse the _pet
	while (_pet > 0)
	{
		sum += (_pet % 10) * 2;
		_pet /= 10;
	}

	//data11 - max levels 
	//if(_player->M_Achiv_Account_Max_Level > 500) sum += 500;
	//else sum += _player->M_Achiv_Account_Max_Level;		
	if(_player->M_Achiv_Account_Task_Custom_MaxID > 100)  sum += 500;
	else sum += _player->M_Achiv_Account_Task_Custom_MaxID * 5;
	
	//data12 - equipments
	if(_player->M_Achiv_Account_EQ_Nums > 500) sum += 500;
	else sum += _player->M_Achiv_Account_EQ_Nums;

	//data14 - extra points for some vip
	if(_player->M_Achiv_Account_Bonus_Nums > 1000) sum += 1000;
	else sum += _player->M_Achiv_Account_Bonus_Nums;

	return sum;
}


// ----------------------------- Player -----------------------------------------
static void __init_Player_Entry(Player *_player)
{
	if (_player->M_Achievements.find(ACHIEVEMENT_PLAYER_DATA) == _player->M_Achievements.end())
	{
		AchievementsEntry& e = __init_Achivement_Entry(_player, ACHIEVEMENT_PLAYER_DATA);
		e.guid = _player->GetGUID();
	}
}

int32  QzqstarAchievements::InitPlayerData(Player * _player)
{
	__init_Player_Entry(_player);

	//Max data = 15
	_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data0 = 0;
	_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data1 = 0;
	_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data2 = 0;
	_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data3 = 0;
	_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data4 = 0;
	_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data5 = 0;
	_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data6 = 0;
	_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data7 = 0;
	_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data8 = 0;
	_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data9 = 0;
	_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data10 = 0;
	_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data11 = 0;
	_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data12 = 0;
	_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data13 = 0;
	_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data14 = 0;
	_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data15 = 0;

    _player->M_Achiv_Player_Chenyi = 0;
    _player->M_Achiv_Player_Zhanpao = 0;
    _player->M_Achiv_Player_NumsTalent = 0;
    _player->M_Achiv_Player_LevelWeapon = 0;
    _player->M_Achiv_Player_LevelPet = 0;
    _player->M_Achiv_Player_NumsResistance = 0;
    _player->M_Achiv_Player_NumsStrength = 0;
    _player->M_Achiv_Player_NumsAgility = 0;
    _player->M_Achiv_Player_NumsStamina = 0;
    _player->M_Achiv_Player_NumsIntellect = 0;
    _player->M_Achiv_Player_NumsSpirit = 0;
    _player->M_Achiv_Player_NumsAP = 0;
    _player->M_Achiv_Player_NumsSP = 0;
    _player->M_Achiv_Player_DungeonTimes = 0;
	_player->M_Achiv_Player_DungeonTelePoints = 0;
    _player->M_Achiv_Player_RacialSpell_Passive = 0;
    _player->M_Achiv_Player_RacialSpell_Active = 0;
	_player->M_Achiv_Player_Custom_TaskID = 0;
	_player->M_Challenge_Mode = 0;
	_player->M_Dungeon_Difficulty = 0;

	__LOG("[QzqstarAchievements::Init New Player Data] Name:%s GUID:%u", _player->GetName(), _player->GetGUID());

	return 0;
}

uint32 QzqstarAchievements::GetPlayerData(Player * _player, uint32 type)
{
	if (!_player) return 0;

	//check the player's achievement map if none
	__init_Player_Entry(_player);
	//__LOG("[QzqstarAchievements::GetPlayerData] Get Data for player:%s, type:%d, and PET:%d, PASSIV:%d", _player->GetName(), type, PLAYER_USED_LEVEL_PET,PLAYER_USED_NUMS_RACIAL_SKILL_ACTIVE);

	uint32 _data = 0;
	switch(type)
	{
		case PLAYER_USED_NUMS_DUNGEON_TIMES:	_data = (_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data0 / 10000000)%10; break;
		case PLAYER_USED_LEVEL_CHENYI:			_data = (_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data0 / 1000000)%10; break;
		case PLAYER_USED_LEVEL_ZHANPAO:			_data = (_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data0 / 100000)%10; break;
		case PLAYER_USED_NUMS_TALENT:			_data = (_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data0 / 10000)%10; break;	
		case PLAYER_USED_LEVEL_WEAPON:			_data = (_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data0 / 1000)%10; break;
		case PLAYER_USED_LEVEL_PET:				_data = (_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data0 / 100)%10; break;
		case PLAYER_USED_NUMS_RESISTANCE:		_data = (_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data0)%100; break;
		case PLAYER_USED_NUMS_STRENGTH:			_data = (_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data1 / 1000000)%1000; break;
		case PLAYER_USED_NUMS_AGILITY:			_data = (_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data1 / 1000)%1000; break;
		case PLAYER_USED_NUMS_STAMINA:			_data = (_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data1 )%1000; break;
		case PLAYER_USED_NUMS_INTELLECT:		_data = (_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data2 / 1000)%1000; break;
		case PLAYER_USED_NUMS_SPIRIT:			_data = (_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data2 )%1000; break;
		case PLAYER_USED_NUMS_SP:				_data = (_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data3 / 1000)%1000; break;
		case PLAYER_USED_NUMS_AP:				_data = (_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data3 )%1000; break;
		case PLAYER_USED_NUMS_DUNGEON_TELEPORT:     _data = (_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data4 / 100)%10; break;
		case PLAYER_USED_NUMS_RACIAL_SKILL_PASSIVE:	_data = (_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data4 / 10)%10; break; //use the least two digits
		case PLAYER_USED_NUMS_RACIAL_SKILL_ACTIVE:	_data = (_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data4 )%10; break;

		case PLAYER_USED_CHALLGE_MODE:			_data = _player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data9; break;
		case PLAYER_USED_CUSTOM_TASKID:			_data = _player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data10; break; 
		case PLAYER_USED_DUNGEON_DIFFICULTY:	_data = _player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data11; break;
		default:  
			__LOG("[QzqstarAchievements::GetPlayerData] Not found data for player:%s, type:%d", _player->GetName(), type);
			break;
	}

	return _data;
}

void QzqstarAchievements::SetPlayerData(Player * _player, uint32 type, uint32 data)
{
	if (!_player) return;

	//check the player's achievement map if none
	__init_Player_Entry(_player);

	switch(type)
	{
		case PLAYER_USED_LEVEL_CHENYI:			
		case PLAYER_USED_LEVEL_ZHANPAO:	
		case PLAYER_USED_NUMS_TALENT:
		case PLAYER_USED_LEVEL_WEAPON:	
		case PLAYER_USED_LEVEL_PET:	
		case PLAYER_USED_NUMS_RESISTANCE:
		case PLAYER_USED_NUMS_DUNGEON_TIMES:
		{
			uint32 _data = _player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data0;
			uint32 _dg_times = (_data / 10000000) % 10;
			uint32 _chenyi = (_data / 1000000) % 10;
			uint32 _zhanpao = (_data / 100000) % 10;
			uint32 _talent = (_data / 10000) % 10;
			uint32 _weapon = (_data / 1000) % 10;
			uint32 _pet = (_data / 100) % 10;
			uint32 _kang = (_data ) % 100;

			switch(type)
			{
				case PLAYER_USED_NUMS_DUNGEON_TIMES:	_dg_times = data; break;
				case PLAYER_USED_LEVEL_CHENYI:		_chenyi = data; break;
				case PLAYER_USED_LEVEL_ZHANPAO:		_zhanpao = data; break;
				case PLAYER_USED_NUMS_TALENT:		_talent = data; break;
				case PLAYER_USED_LEVEL_WEAPON:		_weapon = data; break;
				case PLAYER_USED_LEVEL_PET:			_pet = data; break;
				case PLAYER_USED_NUMS_RESISTANCE:			_kang = data; break;
			}
			_data = _dg_times * 10000000 + _chenyi * 1000000 + _zhanpao * 100000 + _talent * 10000 + _weapon * 1000 + _pet * 100 + _kang;
			_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data0 = _data;
			break;
		}

		case PLAYER_USED_NUMS_STRENGTH:
		case PLAYER_USED_NUMS_AGILITY:
		case PLAYER_USED_NUMS_STAMINA:
		{
			uint32 _data = _player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data1;
			uint32 _strength = _data / 1000000;
			uint32 _agility = (_data / 1000) % 1000;
			uint32 _stamina = (_data ) % 1000;
			switch(type)
			{
				case PLAYER_USED_NUMS_STRENGTH:		_strength = data; break;
				case PLAYER_USED_NUMS_AGILITY:		_agility = data; break;
				case PLAYER_USED_NUMS_STAMINA:		_stamina = data; break;
			}
			_data = _strength * 1000000 + _agility * 1000 + _stamina;
			_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data1 = _data;
			break;
		}

		case PLAYER_USED_NUMS_INTELLECT:
		case PLAYER_USED_NUMS_SPIRIT:
		{
			uint32 _data = _player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data2;
			uint32 _intellect = _data / 1000;
			uint32 _spirit = (_data ) % 1000;
			switch(type)
			{
				case PLAYER_USED_NUMS_INTELLECT:		_intellect = data; break;
				case PLAYER_USED_NUMS_SPIRIT:		_spirit = data; break;
			}
			_data = _intellect * 1000 + _spirit;
			_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data2 = _data;
			break;
		}

		case PLAYER_USED_NUMS_AP:
		case PLAYER_USED_NUMS_SP:
		{
			uint32 _data = _player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data3;
			uint32 _sp = _data / 1000;
			uint32 _ap = (_data ) % 1000;
			switch(type)
			{
				case PLAYER_USED_NUMS_SP:		_sp = data; break;
				case PLAYER_USED_NUMS_AP:		_ap = data; break;
			}
			_data = _sp * 1000 + _ap;
			_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data3 = _data;
			break;
		}

		case PLAYER_USED_NUMS_DUNGEON_TELEPORT:
		case PLAYER_USED_NUMS_RACIAL_SKILL_PASSIVE:
		case PLAYER_USED_NUMS_RACIAL_SKILL_ACTIVE:
		{
			uint32 _data = _player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data4;
			uint32 _teleport = (_data / 100) % 10;
			uint32 _passive = (_data / 10) % 10;
			uint32 _active = (_data ) % 10;
			switch(type)
			{
				case PLAYER_USED_NUMS_DUNGEON_TELEPORT:	_teleport = data; break;
				case PLAYER_USED_NUMS_RACIAL_SKILL_PASSIVE:	_passive = data; break;
				case PLAYER_USED_NUMS_RACIAL_SKILL_ACTIVE:	_active = data; break;
			}
			_data = _teleport * 100 + _passive * 10 + _active;
			_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data4 = _data;
			break;
		}

		case PLAYER_USED_CHALLGE_MODE:
		{
			_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data9 = data; 
			break;
		}
		case PLAYER_USED_CUSTOM_TASKID:
		{
			_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data10 = data; 
			break;
		}
		case PLAYER_USED_DUNGEON_DIFFICULTY:
		{
			_player->M_Achievements[ACHIEVEMENT_PLAYER_DATA].data11 = data; 
			break;
		}
		
		default:  
			__LOG("[QzqstarAchievements::SetPlayerData] Not found data for player:%s, type:%d", _player->GetName(), type);
			break;
	}
}

uint32 QzqstarAchievements::GetPlayerSum(Player * _player)
{
	if (!_player) return 0;
	__init_Player_Entry(_player);

	uint32 sum = 0;

	//calculate the chenyi, zhanpao needs ....
	sum += DBHelper_get_used_points(_player, PLAYER_USED_LEVEL_CHENYI);
	sum += DBHelper_get_used_points(_player, PLAYER_USED_LEVEL_ZHANPAO);
	sum += DBHelper_get_used_points(_player, PLAYER_USED_NUMS_TALENT);
	sum += DBHelper_get_used_points(_player, PLAYER_USED_LEVEL_WEAPON);
	sum += DBHelper_get_used_points(_player, PLAYER_USED_LEVEL_PET);
	sum += DBHelper_get_used_points(_player, PLAYER_USED_NUMS_RESISTANCE);
	sum += DBHelper_get_used_points(_player, PLAYER_USED_NUMS_STRENGTH);
	sum += DBHelper_get_used_points(_player, PLAYER_USED_NUMS_AGILITY);
	sum += DBHelper_get_used_points(_player, PLAYER_USED_NUMS_STAMINA);
	sum += DBHelper_get_used_points(_player, PLAYER_USED_NUMS_INTELLECT);
	sum += DBHelper_get_used_points(_player, PLAYER_USED_NUMS_SPIRIT);
	sum += DBHelper_get_used_points(_player, PLAYER_USED_NUMS_AP);
	sum += DBHelper_get_used_points(_player, PLAYER_USED_NUMS_SP);
	sum += DBHelper_get_used_points(_player, PLAYER_USED_NUMS_DUNGEON_TIMES);
	sum += DBHelper_get_used_points(_player, PLAYER_USED_NUMS_RACIAL_SKILL_PASSIVE);
	sum += DBHelper_get_used_points(_player, PLAYER_USED_NUMS_RACIAL_SKILL_ACTIVE);
	sum += DBHelper_get_used_points(_player, PLAYER_USED_NUMS_DUNGEON_TELEPORT);

	return sum;
}




#pragma endregion

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
	//e.note = "";
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
			//sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "[QzqstarAchievements::GetVIPLevel] Player:%s GetVIPLevel: %u", _player->GetName(), e.data1);
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

//e.data4 used as player's custom settings, bit 0-1 as APSP caclation method
uint32 QzqstarAchievements::GetCustomSettings(Player * _player)
{
	//check _player if none
	if (!_player) return 0;

	//iterate the _playerAchievements vector map of this player to find the VIP level the player has got
	for (auto it = _playerAchievements[_player->GetGUID()].begin(); it!= _playerAchievements[_player->GetGUID()].end(); ++it)
	{
		AchievementsEntry e = *it;
		if (e.type == ACHIEVEMENT_VIP)
		{
			return e.data4;
		}	
	}
	//add one VIP level to the _playerAchievements vector map of this player if not found
	__init_VIP_Entry(_player);
	return 0;
}
//set the player's custom settings and add the custom settings to the player's achievements vector
void QzqstarAchievements::SetCustomSettings(Player * _player, uint32 settings)
{
	//check _player if none
	if (!_player) return;

	//iterate the _playerAchievements vector map of this player to find the VIP level the player has got
	for (auto it = _playerAchievements[_player->GetGUID()].begin(); it!= _playerAchievements[_player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == ACHIEVEMENT_VIP)
		{
			e.data4 = settings;
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




//vip special features, in data8
uint32  QzqstarAchievements::GetVIPSpecialFeatures(Player * _player)
{
	//check _player if none
	if (!_player) return 0;

	//iterate the _playerAchievements vector map of this player to find the VIP level the player has got
	for (auto it = _playerAchievements[_player->GetGUID()].begin(); it!= _playerAchievements[_player->GetGUID()].end(); ++it)
	{
		AchievementsEntry e = *it;
		if (e.type == ACHIEVEMENT_VIP)
		{
			return e.data8;
		}	
	}

	return 0;
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
	e.data1 = 2; 	e.data2 = 0;	e.data3 = 0;	e.data4 = 0;	//e.note = "";
	e.data5 = 0;	e.data6 = 0;	e.data7 = 0;	e.data8 = 0;   //becareful, data 5 - 8 is not inited before.
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
			//sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Player:%s GetCustomQuestID: %u", _player->GetName(), e.data1);

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
		//e.note = "";
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
			return MAKE_PAIR32(e.data2, e.data3 + e.data4);
		}
	}

	return 0;
}

void QzqstarAchievements::SetNormalQuestDoneNum(Player * _player, uint32 counter)
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
			e.data4 = counter;
			break;
		}
	}
}

#pragma endregion

/* ================================================================================================================== */
/* ========================= Social Points system  ============================================================================ */
/* ================================================================================================================== */
void QzqstarAchievements::__init_SocialPoints_Entry(Player * _player)
{
	//check _player if none
	if (!_player)
		return;

	//add one VIP level to the _playerAchievements vector map of this player if not found
	AchievementsEntry e;
	e.guid = _player->GetGUID();
	e.type = ACHIEVEMENT_SOCIAL_POINTS;
	e.subType = 0;
	e.data1 = 0;
	e.data2 = 0;
	e.data3 = 0;
	e.data4 = 0;
	//e.note = "";
	e.data5 = 0;	// for use of PVP killer points
	e.data6 = 0;
	e.data7 = 0;
	e.data8 = 0;
	_playerAchievements[_player->GetGUID()].push_back(e);
	sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "[QzqstarAchievements::GetVIPLevel] Not found but init one VIP Entry: %u", _player->GetGUID());
}

uint32_t QzqstarAchievements::GetSocialPointsPVP(Player * _player)
{
	//check _player if none
	if (!_player) return 0;
	//iterate the _playerAchievements vector map of this player to find the VIP level the player has got
	for (auto it = _playerAchievements[_player->GetGUID()].begin(); it!= _playerAchievements[_player->GetGUID()].end(); ++it)
	{
		AchievementsEntry e = *it;
		if (e.type == ACHIEVEMENT_SOCIAL_POINTS)
		{
			return e.data5; //we use the lower 9999 as weekly, and higher 99999 as total
		}
	}

	//non
	__init_SocialPoints_Entry(_player);
	return 0;
}
void     QzqstarAchievements::SetSocialPointsPVP(Player * _player, uint32_t _points)
{
	//check _player if none
	if (!_player) return;
	//iterate the _playerAchievements vector map of this player to find the VIP level the player has got
	for (auto it = _playerAchievements[_player->GetGUID()].begin(); it!= _playerAchievements[_player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == ACHIEVEMENT_SOCIAL_POINTS)
		{
			e.data5 = _points;
			return;
		}
	}
	//none	
	__init_SocialPoints_Entry(_player);
}


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
	//e.note = "";
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
			//sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Player:%s Loaded Pet: %u", _player->GetName(), e.subType);

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
	//e.note = "";
	e.data5 = 0;	
	e.data6 = 0;
	e.data7 = 0;
	e.data8 = 0;
	_playerAchievements[_player->GetGUID()].push_back(e);

	//sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Player:%s Init Pet: %u", _player->GetName(), e.subType);

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
			//sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Player:%s Loaded Dungeons, _ac ID: %u", player->GetName(), ac_mapId);

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
	//e.note = "";
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
	e.data1 = 0;
	e.data2 = 0;
	e.data3 = 0;
	e.data4 = 0;
	//e.note = "";
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
			//e.note = entry.note;
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
//	e.note = entry.note;
	e.data5 = entry.data5;
	e.data6 = entry.data6;
	e.data7 = entry.data7;
	e.data8 = entry.data8;
	_playerAchievements[player->GetGUID()].push_back(e);
	
}



/* ================================================================================================================== */
/* ========================= Collect system  ============================\============================================= */
/* ================================================================================================================== */
uint32_t QzqstarAchievements::GetCollectAchiveInfo(Player *player, uint32_t itemSetType)
{
	//check _player if none
	if (!player)
		return 0;

	//iterate the _playerAchievements vector map of this player to find the collect information
	for (auto it = _playerAchievements[player->GetGUID()].begin(); it!= _playerAchievements[player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == ACHIEVEMENTS_COLLECTIONS)
		{
			//if found, return the miscValue data
			switch(itemSetType)
			{
				case COLLECTIONS_TYPE_ITEMSET_DUNGEONS: 	return (e.data1);
				case COLLECTIONS_TYPE_ITEMSET_RAID_LOW: 	return (e.data2);
				case COLLECTIONS_TYPE_ITEMSET_RAID_HIGH: 	return (e.data3);
				case COLLECTIONS_TYPE_PROFESSIONS: 			return (e.data4);
				default: return 0;
			}	
		}	
	}

	//if not found, create one and return it
	AchievementsEntry e;
	e.guid = player->GetGUID();
	e.type = ACHIEVEMENTS_COLLECTIONS;
	e.subType = 0; 	e.data1 = 0;	e.data2 = 0;	e.data3 = 0;	e.data4 = 0;
	//e.note = "";	e.data5 = 0;	e.data6 = 0;	e.data7 = 0;	e.data8 = 0;
	_playerAchievements[player->GetGUID()].push_back(e);

	//sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Player:%s Init Collections: %u", player->GetName(), e.subType);

	return 0;
}
void QzqstarAchievements::SetCollectAchiveInfo(Player *player, uint32_t itemSetType, uint32_t value)
{
	//check _player if none
	if (!player) return;

	//iterate the _playerAchievements vector map of this player to find the collect information
	for (auto it = _playerAchievements[player->GetGUID()].begin(); it!= _playerAchievements[player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == ACHIEVEMENTS_COLLECTIONS)
		{
			//if found, set the miscValue data
			switch(itemSetType)
			{
				case COLLECTIONS_TYPE_ITEMSET_DUNGEONS: 	e.data1 = value; break;
				case COLLECTIONS_TYPE_ITEMSET_RAID_LOW: 	e.data2 = value; break;
				case COLLECTIONS_TYPE_ITEMSET_RAID_HIGH: 	e.data3 = value; break;
				case COLLECTIONS_TYPE_PROFESSIONS: 			e.data4 = value; break;
				default: break;
			}	
		}	
	}
	//if not found, create one and return it
	AchievementsEntry e;
	e.guid = player->GetGUID();
	e.type = ACHIEVEMENTS_COLLECTIONS;
	e.subType = 0; 	e.data1 = 0;	e.data2 = 0;	e.data3 = 0;	e.data4 = 0;
	//e.note = "";	e.data5 = 0;	e.data6 = 0;	e.data7 = 0;	e.data8 = 0;
	_playerAchievements[player->GetGUID()].push_back(e);
	//sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Player:%s Init Collections: %u", player->GetName(), e.subType);
}

/* ================================================================================================================== */
/* ========================= Collections of Dungeon system  ==========================================================*/
/*=====================================================================================================================*/

AchievementsEntry QzqstarAchievements::GetCollectionEntry(Player *player, uint32_t collection_type)
{
	//check _player if none
	if (!player) return AchievementsEntry();

	//iterate the _playerAchievements vector map of this player to find the collect information
	for (auto it = _playerAchievements[player->GetGUID()].begin(); it!= _playerAchievements[player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == collection_type)
		{
			return e;
		}
	}

	//if not found, create one and return it
	AchievementsEntry e;
	e.guid = player->GetGUID();
	e.type = collection_type;
	e.subType = 0; 	e.data1 = 0;	e.data2 = 0;	e.data3 = 0;	e.data4 = 0;
	//e.note = "";	e.data5 = 0;	e.data6 = 0;	e.data7 = 0;	e.data8 = 0;
	_playerAchievements[player->GetGUID()].push_back(e);
	// sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Not found but init one Dungeon Entry: %u", player->GetGUID());
	return e;
}

uint32    QzqstarAchievements::GetEquipCollectCommon(Player *player, uint32_t key, uint32_t _realMapID)
{
	//check _player if none
	if (!player || _realMapID > 8) return 0;

	//iterate the _playerAchievements vector map of this player to find the collect information
	for (auto it = _playerAchievements[player->GetGUID()].begin(); it!= _playerAchievements[player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == key)
		{
			//if found, return the miscValue data
			switch(_realMapID)
			{
				case 0: return (e.subType);
				case 1: return (e.data1);
				case 2: return (e.data2);
				case 3: return (e.data3);
				case 4: return (e.data4);
				case 5: return (e.data5);
				case 6: return (e.data6);
				case 7: return (e.data7);
				case 8: return (e.data8);
				default: return 0;
			}
		}	
	}

	//if not found, create one and return it
	AchievementsEntry e;
	e.guid = player->GetGUID();
	e.type = key;
	e.subType = 0; 	e.data1 = 0;	e.data2 = 0;	e.data3 = 0;	e.data4 = 0;
	//e.note = "";	e.data5 = 0;	e.data6 = 0;	e.data7 = 0;	e.data8 = 0;
	_playerAchievements[player->GetGUID()].push_back(e);

	//sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Player:%s Init Dungeon Collect: %u", player->GetName(), ac_mapId);

	return 0;
}

void      QzqstarAchievements::SetEquipCollectCommon(Player *player, uint32_t collection_type, uint32_t key, uint32_t value)
{
	//check _player if none
	if (!player || key > 8) return;

	//iterate the _playerAchievements vector map of this player to find the collect information
	for (auto it = _playerAchievements[player->GetGUID()].begin(); it!= _playerAchievements[player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == collection_type)
		{
			//if found, set the miscValue data
			switch(key)
			{
				case 0: e.subType = value; break;
				case 1: e.data1 = value; break;
				case 2: e.data2 = value; break;
				case 3: e.data3 = value; break;
				case 4: e.data4 = value; break;
				case 5: e.data5 = value; break;
				case 6: e.data6 = value; break;
				case 7: e.data7 = value; break;
				case 8: e.data8 = value; break;
				default: break;	
			}
			
			return;
		}	
	}

	//if not found, create one and return it
	AchievementsEntry e;
	e.guid = player->GetGUID();
	e.type = key;
	e.subType = 0; 	e.data1 = 0;	e.data2 = 0;	e.data3 = 0;	e.data4 = 0;
	//e.note = "";	e.data5 = 0;	e.data6 = 0;	e.data7 = 0;	e.data8 = 0;
	_playerAchievements[player->GetGUID()].push_back(e);

	sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Player:%s Init Dungeon Collect: %u", player->GetName(), key);
}



static float __count_weight(uint32_t data)
{
	float bonus = 0;

	while (data > 0)
	{
		if (data & 0x01)	bonus += 1.0f;	//difficulty 0
		if (data & 0x02)	bonus += 1.2f;	//1
		if (data & 0x04)	bonus += 1.5f;	//2
		if (data & 0x08)	bonus += 1.8f;	//3

		data = data / 16;
	}

	return bonus;
}

uint32    QzqstarAchievements::GetEQCollectBonus(Player *player)
{
	uint32 bonus = 0;

	//achieve of equipment collection, add 10 AP and 5 SP for each equipment collected.
	AchievementsEntry ACHIVE_Entries[5]; 
	ACHIVE_Entries[0] = sQZAchievements.GetCollectionEntry(player, ACHIEVEMENTS_COLLECTIONS_WORLD);
	ACHIVE_Entries[1] = sQZAchievements.GetCollectionEntry(player, ACHIEVEMENTS_COLLECTIONS_DUNGEONS_1);
	ACHIVE_Entries[2] = sQZAchievements.GetCollectionEntry(player, ACHIEVEMENTS_COLLECTIONS_DUNGEONS_2);
	ACHIVE_Entries[3] = sQZAchievements.GetCollectionEntry(player, ACHIEVEMENTS_COLLECTIONS_RAID);
	ACHIVE_Entries[4] = sQZAchievements.GetCollectionEntry(player, ACHIEVEMENTS_COLLECTIONS_PROFESSION);
	
	/* Do World collections */
	bonus += COUNT_ONES(ACHIVE_Entries[0].data1) * 20;
	bonus += COUNT_ONES(ACHIVE_Entries[0].data2) * 40;
	bonus += COUNT_ONES(ACHIVE_Entries[0].data3) * 60;
	bonus += COUNT_ONES(ACHIVE_Entries[0].data4) * 80;
	bonus += COUNT_ONES(ACHIVE_Entries[0].data5) * 100;

	/* Do Dungeon collections */
	bonus += __count_weight(ACHIVE_Entries[1].subType) * 1.5f;
	bonus += __count_weight(ACHIVE_Entries[1].data1) * 1.8f;
	bonus += __count_weight(ACHIVE_Entries[1].data2) * 2.0f;
	bonus += __count_weight(ACHIVE_Entries[1].data3) * 2.2f;
	bonus += __count_weight(ACHIVE_Entries[1].data4) * 2.5f;
	bonus += __count_weight(ACHIVE_Entries[1].data5) * 2.7f;
	bonus += __count_weight(ACHIVE_Entries[1].data6) * 3.0f;
	bonus += __count_weight(ACHIVE_Entries[1].data7) * 3.5f;
	bonus += __count_weight(ACHIVE_Entries[1].data8) * 4.0f;

	bonus += __count_weight(ACHIVE_Entries[2].subType) * 4.2f;
	bonus += __count_weight(ACHIVE_Entries[2].data1) * 4.5f;
	bonus += __count_weight(ACHIVE_Entries[2].data2) * 4.7f;
	bonus += __count_weight(ACHIVE_Entries[2].data3) * 5.0f;
	bonus += __count_weight(ACHIVE_Entries[2].data4) * 5.5f;
	bonus += __count_weight(ACHIVE_Entries[2].data5) * 6.0f;
	bonus += __count_weight(ACHIVE_Entries[2].data6) * 6.5f;
	bonus += __count_weight(ACHIVE_Entries[2].data7) * 7.0f;
	bonus += __count_weight(ACHIVE_Entries[2].data8) * 7.5f;

	//raid bonus

	//profession bonus
	bonus += COUNT_ONES(ACHIVE_Entries[4].data1) * 50;
	bonus += COUNT_ONES(ACHIVE_Entries[4].data2) * 50;
	bonus += COUNT_ONES(ACHIVE_Entries[4].data3) * 50;
	bonus += COUNT_ONES(ACHIVE_Entries[4].data4) * 50;
	bonus += COUNT_ONES(ACHIVE_Entries[4].data5) * 50;
	bonus += COUNT_ONES(ACHIVE_Entries[4].data6) * 50;

	//Do Raid collections
	sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Player:%s GetEQCollectBonus: %u", player->GetName(), bonus);
	return bonus;
}



/* ================================================================================================================== */
/* ========================= Collections of Skills from monster  =====================================================*/
/*====================================================================================================================*/

AchievementsEntry QzqstarAchievements::GetSkillsCollectEntry(Player *player)
{
	//check _player if none
	if (!player) return AchievementsEntry();

	//iterate the _playerAchievements vector map of this player to find the collect information
	for (auto it = _playerAchievements[player->GetGUID()].begin(); it!= _playerAchievements[player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == ACHIEVEMENTS_COLLECTIONS_SKILLS)
		{
			return e;
		}	
	}

	//if not found, create one and return it
	AchievementsEntry e;
	e.guid = player->GetGUID();
	e.type = ACHIEVEMENTS_COLLECTIONS_SKILLS;
	e.subType = 0; 	e.data1 = 0;	e.data2 = 0;	e.data3 = 0;	e.data4 = 0;
	//e.note = "";	e.data5 = 0;	e.data6 = 0;	e.data7 = 0;	e.data8 = 2;
	_playerAchievements[player->GetGUID()].push_back(e);
	// sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Not found but init one Skills Entry: %u", player->GetGUID());
	return e;
}

uint32    QzqstarAchievements::GetSkillsCollectActiveID(Player *player)
{
	//check _player if none
	if (!player) return 0;

	//iterate the _playerAchievements vector map of this player to find the collect information
	for (auto it = _playerAchievements[player->GetGUID()].begin(); it!= _playerAchievements[player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == ACHIEVEMENTS_COLLECTIONS_SKILLS)
		{
			return (e.subType); //Active ID
		}	
	}

	return 0;
}
void      QzqstarAchievements::SetSkillsCollectInfo(Player *player, uint32_t spellId, uint32_t pos)
{
	//check _player if none
	if (!player) return;

	//iterate the _playerAchievements vector map of this player to find the collect information
	for (auto it = _playerAchievements[player->GetGUID()].begin(); it!= _playerAchievements[player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == ACHIEVEMENTS_COLLECTIONS_SKILLS)
		{
			//if found, set the miscValue data
			switch(pos)	
			{
				case 0: e.subType = spellId; break;
				case 1: e.data1 = spellId; break;
				case 2: e.data2 = spellId; break;
				case 3: e.data3 = spellId; break;
				case 4: e.data4 = spellId; break;
				case 8: e.data8 = spellId; break;	//e.data8 means total actived skills count
				default: break;
			}
		}	
	}
}


int32     QzqstarAchievements::GetSkillsCollectEmptySlot(Player *player)
{
	//check _player if none
	if (!player) return 0;

	//iterate the _playerAchievements vector map of this player to find the collect information
	for (auto it = _playerAchievements[player->GetGUID()].begin(); it!= _playerAchievements[player->GetGUID()].end(); ++it)
	{
		AchievementsEntry& e = *it;
		if (e.type == ACHIEVEMENTS_COLLECTIONS_SKILLS)
		{
			//if found, set the miscValue data
			if(e.data8 < 2) e.data8 = 2;
			else if(e.data8 > 4) e.data8 = 4;

			for(int i = 0; i < e.data8; i++)
			{
				if(i==0 && e.data1 == 0) return 1;
				else if(i==1 && e.data2 == 0) return 2;
				else if(i==2 && e.data3 == 0) return 3;
				else if(i==3 && e.data4 == 0) return 4;
			}
		}	
	}

	//not found
	return 0;
}





#pragma endregion
