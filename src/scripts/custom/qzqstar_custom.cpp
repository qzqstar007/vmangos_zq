/* Copyright (C) 2009 - 2010 ScriptDevZero <http://github.com/scriptdevzero/scriptdevzero>
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "scriptPCH.h"
#include "custom.h"
#include "ScriptedAI.h"
#include <ctime>

#include "qzqstar_custom.h"
#include "QzqstarAchievements.h"
#include "Qzqstar_db.h"
#include "Chat.h"
#include "qzqstar_id.h"

#define	__STR(x)		((std::string)(x)).c_str()
#define	__NSTR(x)		(std::to_string(x))

#define	__BLUE(x)		"|cff002fa7"##x##"|r"
#define	__GREEN(x)		"|cff00671f"##x##"|r"
#define	__ORANGE(x)		"|cffe85827"##x##"|r"
#define __RED(x)		"|cfff00019"##x##"|r"
#define __YELLOW(x)		"|cfff9dc24"##x##"|r"

#define	__MENU_NONE					0


#pragma region __BattleGround___

#define __MENU_BG_MAIN					(1000)

#define __MENU_BG_MAIN_WS				 (10)
#define __MENU_BG_MAIN_AB				 (20)
#define __MENU_BG_MAIN_AV				 (30)
#define	__MENU_BG_RW_OFFSET				(100)

bool BG_Menus(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
	std::string text = "";
	uint32 __menu_nums = 0;

	if (action == __MENU_BG_MAIN)
	{
		//Add to join the battlegrounds
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝选择战场＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN);

		if (player->GetLevel() >= 10)
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR("①－战歌峡谷　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN + __MENU_BG_MAIN_WS);
		}
		else
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR(__RED("①－战歌１０级可用。　")), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN);
		//check player level > 30 to enter Arthas Basin
		if (player->GetLevel() >= 30)
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR("②－阿拉希盆地　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN + __MENU_BG_MAIN_AB);
		else 
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR(__RED("②－阿拉希３０级可用。　")), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN);

		//check player level > 50 to enter Alaric's Vale
		if (player->GetLevel() >= 50)
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR("③－奥特兰克山谷　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN + __MENU_BG_MAIN_AV);
		else
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR(__RED("③－奥山５０级可用。　")), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝奖章兑换＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR("①－战歌奖章　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN + __MENU_BG_MAIN_WS + __MENU_BG_RW_OFFSET);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR("②－阿拉希奖章　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN + __MENU_BG_MAIN_AB + __MENU_BG_RW_OFFSET);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR("③－奥特兰克奖章　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN + __MENU_BG_MAIN_AV + __MENU_BG_RW_OFFSET);


		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		return true;
	}
	else if (action > __MENU_BG_MAIN && action<__MENU_BG_MAIN + __MENU_BG_RW_OFFSET)
	{
		action -= __MENU_BG_MAIN;
		auto bgType = (action == __MENU_BG_MAIN_WS ? BATTLEGROUND_WS 
					 : action == __MENU_BG_MAIN_AB ? BATTLEGROUND_AB 
					 : BATTLEGROUND_AV);

		if (player)
			sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Player %s is trying to join BG %u", player->GetName(), bgType);

		player->InterruptSpellsWithChannelFlags(AURA_INTERRUPT_INTERACTING_CANCELS);
		player->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_INTERACTING_CANCELS);
		player->GetSession()->SendBattleGroundList(_Creature->GetGUID(), bgType);
	}
	else if (action >= __MENU_BG_MAIN + __MENU_BG_RW_OFFSET)
	{
		action -= (__MENU_BG_MAIN + __MENU_BG_RW_OFFSET);
		auto bgType = action == __MENU_BG_MAIN_WS ? BATTLEGROUND_WS : action == __MENU_BG_MAIN_AB ? BATTLEGROUND_AB : BATTLEGROUND_AV;

		if (player)
			sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "Player %s is trying to RW BG %u", player->GetName(), bgType);
		
		//each Mark of Honor can be used to reward 100 reputaion

	}

	return true;
}


#pragma endregion


#pragma region Dynamic Task_Generate

#define __MENU_TASK_MAIN						(1000)
#define	__MENU_TASK_ACT_ACCEPT					(100)
#define	__MENU_TASK_ACT_COMPLET					(200)
#define	__MENU_TASK_ACT_REGET					(300)

static int32 _Quest_Counter = 12001;
bool Task_Menus(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
	std::string text = "";
	uint32 __menu_nums = 0;

	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝万环任务系统＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
	uint32 _doneCounter = sQZAchievements.GetQuestDoneCounters(player);
	text = __STR("｜　　今日完成：|cff007733");
	text.append(__NSTR(PAIR32_LOPART(_doneCounter)));
	text.append(__STR(" |r / 100 "));
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
	text = __STR("｜　　总共完成：|cff007733");
	text.append(__NSTR(PAIR32_HIPART(_doneCounter)));
	text.append(__STR(" |r / 10000 "));
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);

	auto	_currentQuestID = sQZAchievements.GetCustomQuestID(player);
	uint32	__reset_gold = player->GetLevel() * player->GetLevel() / 610 + 1;		//TODO NEXT SEASON

	if (action == __MENU_TASK_MAIN)
	{

		//When User interact with the NPC
		//should display the following screen:
		//  ==== 随机任务系统 ====
		//  如果已经有任务
		// 1. 当前任务状态：【完成】【未完成】
		// 2. 重新获取新任务
		if (_currentQuestID)
		{
			// alread has a quest, display the current quest status
			// 1. 当前任务状态：【完成】【未完成】	
			std::string __statusStr = __STR("当前已有任务：状态　");
			QuestStatus	__qStatus = player->GetQuestStatus(_currentQuestID);
			__statusStr.append( __qStatus == QUEST_STATUS_COMPLETE? __STR(__GREEN(" ＝【已完成】＝＝ ")) : __STR(__RED(" ＝【未完成】＝＝ ")) );


			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__statusStr) , GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			if(__qStatus == QUEST_STATUS_COMPLETE)	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__GREEN("== 领取奖励 == ")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN + __MENU_TASK_ACT_COMPLET);
			else player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__GREEN("＝＝【返回】＝＝＝ ")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			

			__statusStr = __STR(("|cffbb0000 ＝＝重新获取任务，花费金币： "));
			__statusStr.append(__NSTR(__reset_gold));
			__statusStr.append(__STR("G === |r"));

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__statusStr), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN + __MENU_TASK_ACT_REGET);
		}
		else
		{
			// no quest, display the new quest button
			// 2. 重新获取新任务
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("  当前没有任务，点击确定获取新任务  "), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("＝＝＝＝【确定】＝＝＝＝ ")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN + __MENU_TASK_ACT_ACCEPT);
		}		

		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		return true;
	}
	else if (action  == __MENU_TASK_MAIN + __MENU_TASK_ACT_ACCEPT)
	{
		int32 _origQuestID = 0;
		int32 _qRank = 1;
		
		_qRank = urand(0, 100);
		if (_qRank > 90 && player->GetLevel() >= 45) _qRank = 3;
		else if (_qRank > 70 && player->GetLevel() >= 25) _qRank = 2;
		else _qRank = 1;

		auto _qEntity = DBHelper_GetQuestByLevel(player);

		//test stub
		//__qIDBegin = 6502;
		_origQuestID = _qEntity.questID;

		if (player->IsGameMaster())
		{
			_origQuestID = player->GetReputationMgr().GetReputation(967);
			if(_origQuestID > 9300) _origQuestID = 5083;
		}

		ObjectMgr::QuestMap const& qTemplates = sObjectMgr.GetQuestTemplates();

		if (_origQuestID == 0 || qTemplates.find(_origQuestID) == qTemplates.end())
		{
			//should do the wrong things
			sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "[Task System Error, Null template] player:%s.", player->GetName());
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__RED("＝＝任务生成错误，点击重新获取。＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN + __MENU_TASK_ACT_ACCEPT);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
			return true;
		}

		std::unique_ptr<Quest> newQuest = std::make_unique<Quest>(*qTemplates.find(_origQuestID)->second);

		int loc_idx = player->GetSession()->GetSessionDbLocaleIndex();
		if (loc_idx >= 0)
		{
			QuestLocale const* il = sObjectMgr.GetQuestLocale(_origQuestID);
			if (il)
			{
				if ((int32)il->Objectives.size() > loc_idx && !il->Objectives[loc_idx].empty())
				{
					newQuest->Objectives = il->Objectives[loc_idx];

				}
				if ((int32)il->Details.size() > loc_idx && !il->Details[loc_idx].empty())
				{
					newQuest->Details = il->Details[loc_idx];
				}

			}
		}
		
		//set new quest id
		newQuest->QuestId = _Quest_Counter;
		newQuest->QuestLevel = player->GetLevel();
		newQuest->m_QuestFlags = 0;
		newQuest->m_SpecialFlags = 0;
		std::string _title = "";
		_title.append(player->GetName());
		_title.append(__STR("的专属任务　"));
		//Rare quest if possible
		newQuest->Title = _title;
		newQuest->Objectives.append(__STR(" $B$B |cffff0000【注意】任务显示完成后直接回加基森交任务。|r "));
		
		//check if any killed or creature needed
		/**/
		if ( (newQuest->ReqCreatureOrGOId[0] == 0 && newQuest->ReqItemId[0] == 0) || (newQuest->SrcItemId != 0 && newQuest->ReqItemId[0] == newQuest->SrcItemId))
		{
			auto __item = DBHelper_GetItemMatsByLevel(player, 1);
			if(__item.itemID != 0)
			{
				newQuest->ReqItemId[1] = __item.itemID;
				newQuest->ReqItemCount[1] = 5;
			}
		}

		//if Quest rank is 2 or 3
		if (_qRank  > 1)
		{
			auto __item = DBHelper_GetItemMatsByLevel(player, 2);
			if (__item.itemID != 0)
			{
				newQuest->ReqItemId[2] = __item.itemID;
				newQuest->ReqItemCount[2] = 3;
			}
		}
		
		if (_qRank > 2)
		{
			auto __item = DBHelper_GetItemMatsByLevel(player, 3);
			if (__item.itemID != 0)
			{
				newQuest->ReqItemId[3] = __item.itemID;
				newQuest->ReqItemCount[3] = 2;
			}
		}

		for (size_t i = 0; i < 4; i++)
		{
			if (newQuest->RewItemId[i] == 0)
			{
				auto item = DBHelper_GetItemEqByLevel(player, _qRank);
				if (item.itemID)
				{
					newQuest->RewItemId[i] = item.itemID;
					newQuest->RewItemCount[i] = 1;
					break;
				}
			}
		}

		for (size_t i = 0; i < 4; i++)
		{
			if ( ( player->GetLevel() < 11 || _qRank > 1) && newQuest->RewItemId[i] == 0)
			{
				// should pick up random rewards
				newQuest->RewItemId[i] = ZQ_ITEM_VOUCHER;
				newQuest->RewItemCount[i] = _qRank > 1? _qRank-1 : 1;
				break;
			}
		}


		for (int j = 0; j < QUEST_ITEM_OBJECTIVES_COUNT; ++j)
		{
			if (uint32 id = newQuest->ReqItemId[j])
			{
				newQuest->SetSpecialFlag(QUEST_SPECIAL_FLAG_DELIVER);
			}
		}


		for (int j = 0; j < QUEST_OBJECTIVES_COUNT; ++j)
		{
			if (newQuest->ReqCreatureOrGOId[j] != 0)
			{
				newQuest->SetSpecialFlag(QuestSpecialFlags(QUEST_SPECIAL_FLAG_KILL_OR_CAST));
			}
		}


#define _QUEST_NPC_ID (30005)
		sObjectMgr.GetCreatureInvolvedRelationsMap().insert(QuestRelationsMap::value_type(_QUEST_NPC_ID, _Quest_Counter));

		//auto _orig_quest = sObjectMgr.GetQuestTemplate(_origQuestID);
		//if(_orig_quest)	newQuest->SetSpecialFlag(_orig_quest->GetSpecialFlag());
		sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "[Task System] player:%s create questID:%u", player->GetName(), _Quest_Counter);

		auto& questMap = sObjectMgr.GetQuestTemplatesZQ();
		questMap[_Quest_Counter] = std::move(newQuest);

		//assign a new counter here
		auto _localQuestID = _Quest_Counter;
		_Quest_Counter ++;

		//save to player's Achievements cache
		sQZAchievements.SetCustomQuestID(player, _localQuestID);		

		//Add to join the battlegrounds
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("＝＝＝＝已生成随机任务＝＝＝＝　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);

		auto _quest = sObjectMgr.GetQuestTemplate(_localQuestID);
		player->AddQuest(_quest, nullptr);
		//if (player->CanCompleteQuest(_localQuestID)) player->FullQuestComplete(_localQuestID);

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__GREEN("＝＝＝成功接到任务，返回。＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);

		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		return true;
	}
	else if (action == __MENU_TASK_MAIN + __MENU_TASK_ACT_COMPLET)
	{
		if (_currentQuestID)
		{
			Quest const* pQuest = sObjectMgr.GetQuestTemplate(_currentQuestID);

			// 删除并获取下一个迭代器
			auto _kpair = sObjectMgr.GetCreatureInvolvedRelationsMap().equal_range(_QUEST_NPC_ID);
			for (auto it = _kpair.first; it != _kpair.second; ) {
				if (it->second == _currentQuestID) {
					it = sObjectMgr.GetCreatureInvolvedRelationsMap().erase(it);  
				}
				else {
					++it;
				}
			}
			player->RewardQuest(pQuest, 0, player, false);
		}

		sQZAchievements.FinishCustomQuest(player, true);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__GREEN("＝＝＝成功完成任务，返回。＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		return true;
	}
	else if (action == __MENU_TASK_MAIN + __MENU_TASK_ACT_REGET)
	{

		if (player->GetMoney() < __reset_gold * 10000)
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__RED("＝＝＝金币不够，返回。＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
		}
		else
		{
			//remove the money
			player->SetMoney(player->GetMoney() - __reset_gold * 10000 );
			
			sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "[Task System] player:%s reset questID: %u", player->GetName(), _currentQuestID);

			//finish the quest, but not increase counter
			sQZAchievements.FinishCustomQuest(player, false);

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__GREEN("＝＝＝重置成功，重新获取任务。＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		return true;
	}

	return true;
}

#pragma endregion



#pragma region Pet development system

#define __MENU_PET_MAIN					(1000)
#define	__MENU_PET_ACT_FEED				(100)
#define	__MENU_PET_ACT_UPGRADE			(200)
#define	__MENU_PET_ACT_CHANGE			(300)
#define	__MENU_PET_ACT_BUY  			(400)

#define __PET_UPGRADE_ITEM				(ZQ_ITEM_PET_FOOD)
#define	__GOSSIP_PET_ID					(DEFAULT_GOSSIP_MESSAGE)
/**** Pet Development System 
 *
 * Pet using the table sAchievements to store information.
 * type:
 * subType:
 * value:

 * 1. Pet can be feed by meat, and the meat will be consumed.
 * 2. Pet can be upgrade by gold, and the gold will be consumed.
 * 3. Pet can be change by gold, and the gold will be consumed.
 *
 */

uint32 __getHappinessPoints(Item* pItem, uint32 petLevel,  uint32 counts)
{
	uint32 points = 0;

	if (pItem)
	{
		if (pItem->GetProto()->ItemLevel < petLevel * 5) 
			points = 0;
		else 
			points = (pItem->GetProto()->ItemLevel - petLevel * 5 + 2) * counts / 10;
	}

	return points;
}

bool Pet_Menus(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
	std::string text = "";
	uint32 __menu_nums = 0;

	//check if the player is the owner of the pet
	if(_Creature->GetOwnerGuid() != player->GetObjectGuid())
	{
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("　　你不是我的主人！　　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		return true;
	}

	//get the player's pet information from the database
	int32 _petInfo = sQZAchievements.GetActivePetInfo(player);
	int32 _nowPetLevel = _petInfo % 100000 / 10000;
	int32 _nowPetHappy = _petInfo % 10000 / 100;
	int32 _nowPetRelation = _petInfo % 100;


	//宠物当前等级：%u级，快乐点数：%u/100，忠诚点数：%u/100。
	ChatHandler(player).PSendSysMessage(9038, _nowPetLevel, _nowPetHappy, _nowPetRelation);

	if (action == __MENU_PET_MAIN)
	{
		//display the pet information
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝宠物养成系统＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);

		//set the pet information
		text = "";
		text.append(__STR("丨　等　级：　"));
		text.append(__NSTR(_nowPetLevel));
		text.append(__STR("　"));
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);

		text = "";
		text.append(__STR("丨　快乐度：　"));
		text.append(__STR(_nowPetHappy > 75? __GREEN("高兴　　") 
					: _nowPetHappy > 25? __ORANGE("一般　　") 
					: __RED("不开心　　")));
		text.append(__STR("　"));
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);
		
		text = "";
		text.append(__STR("丨　忠诚度：　"));
		text.append(__STR(_nowPetRelation > 80? __GREEN("★★★★★　 ")
					: _nowPetRelation > 60? __GREEN("★★★★　 ")
					: _nowPetRelation > 40? __ORANGE("★★★　 ")
					: _nowPetRelation > 20? __ORANGE("★★　 ")
					: __RED("★　 ")));
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);

		//can only use meat etc..
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("＝＝　喂　养　＝＝＝ ")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN + __MENU_PET_ACT_FEED);
		if(_nowPetLevel < 9) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("＝＝　升　级　＝＝＝ ")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN + __MENU_PET_ACT_UPGRADE);
		else player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("＝＝　宠物已满级　＝＝＝ ")), GOSSIP_SENDER_MAIN, __MENU_NONE);
		
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__RED("＝＝　更换宠物类型　＝＝＝ ")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN + __MENU_PET_ACT_CHANGE);
		
		player->SEND_GOSSIP_MENU(__GOSSIP_PET_ID, _Creature->GetGUID());
		return true;
	}
	else if (action > __MENU_PET_MAIN && action<__MENU_PET_MAIN + __MENU_PET_ACT_UPGRADE)
	{
		// here, just feed the pet, remove the meat from the bag, and add the happiness points
		if (action > __MENU_PET_MAIN + __MENU_PET_ACT_FEED)
		{
			//1. get the slot pos, and check again
			int32 __slotNum = action - __MENU_PET_MAIN - __MENU_PET_ACT_FEED;
			if (__slotNum >= INVENTORY_SLOT_ITEM_START && __slotNum < INVENTORY_SLOT_ITEM_START + 12)
			{
				//correct slot
				Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, __slotNum);
				if ((pItem) && (pItem->GetProto()->FoodType == PET_DIET_MEAT || pItem->GetProto()->FoodType == PET_DIET_FISH) )
				{
					//get the happiness points
					int32 __happyReward = __getHappinessPoints(pItem, (_petInfo % 100000 / 10000),  pItem->GetCount());

					//remove the item from the bag, and add the happiness points
					player->DestroyItem(INVENTORY_SLOT_BAG_0, __slotNum, true);
					
					//add the happiness points to the pet, 
					int32 _nowHappiness = _petInfo % 10000 / 100;
					_nowHappiness += __happyReward;
					if (_nowHappiness > 99) _nowHappiness = 99;

					// and update the pet information to the database, 
					sQZAchievements.SetActivePetInfo(player, _petInfo / 10000 * 10000 + _nowHappiness * 100 + _petInfo % 100);

				}
			}
		}
		auto localIdx = player->GetSession()->GetSessionDbLocaleIndex();

		//feed the pet, display the new menu, can only feed meat or fish
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("　请将食物（肉类、鱼类）放在行囊中　　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
		
		//check if the player has the meat or fish in the bag
		//find in the bag, from slot0 to slot12
		for (int i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_START + 12; ++i)
		{
			Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);

			if ((pItem) && (pItem->GetProto()->FoodType == PET_DIET_MEAT || pItem->GetProto()->FoodType == PET_DIET_FISH) )
			{
				auto item_destroy_local = sObjectMgr.GetItemLocale(pItem->GetProto()->ItemId);
				auto item_destroy_text = (item_destroy_local == nullptr ? pItem->GetProto()->Name1 : item_destroy_local->Name[localIdx]);

				text = "";
				text.append(item_destroy_text);
				text.append(__STR(" ==> 增加快乐点数：|cffee0000 "));
				text.append(std::to_string(__getHappinessPoints(pItem, _nowPetLevel,  pItem->GetCount())));
				text.append(" |r");

				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(text), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN + __MENU_PET_ACT_FEED + i);
			}
		}
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
		
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝【返回】＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN);

		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		return true;
	}
	else if (action >= __MENU_PET_MAIN + __MENU_PET_ACT_UPGRADE && action < __MENU_PET_MAIN + __MENU_PET_ACT_CHANGE)
	{
		int32 _needShardNumber = _nowPetLevel * _nowPetLevel * 2 + 4 * _nowPetLevel;

		// here, upgrade the pet, remove the MATs from the bag, and add the level
		if (action > __MENU_PET_MAIN + __MENU_PET_ACT_UPGRADE)
		{
			if ((player->HasItemCount(__PET_UPGRADE_ITEM, _needShardNumber))) 
			{
				player->DestroyItemCount(__PET_UPGRADE_ITEM, _needShardNumber, true);

				//level up the pet, and reset the happiness points to 0
				if(_nowPetLevel<9) _nowPetLevel++;

				//save to datebase
				sQZAchievements.SetActivePetInfo(player, _nowPetLevel * 10000 + _nowPetHappy * 100 + _nowPetRelation % 100);
				
			}
		}
		
		//refresh the shard number
		_needShardNumber = _nowPetLevel * _nowPetLevel * 2 + 4 * _nowPetLevel;
		auto localIdx = player->GetSession()->GetSessionDbLocaleIndex();

		//feed the pet, display the new menu, can only feed meat or fish
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN);

		//concate the text with _currentPetLevel
		text = "";
		text.append(__STR("　　现等级：　"));
		text.append(__NSTR(_nowPetLevel));
		text.append(__STR("级　"));
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN);

		if(_nowPetLevel<9) 
		{
			//show next petleve
			text = "";
			text.append(__STR("　　升级后：　"));
			text.append(__NSTR(_nowPetLevel + 1));
			text.append(__STR("级　"));
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN);

			//show need shard numbers
			text = "";
			text.append(__STR("　　需要宠物饲料：|cffbb1122　"));
			text.append(__NSTR(_needShardNumber));
			text.append(__STR("个　|r"));
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN);


			if ((player->HasItemCount(__PET_UPGRADE_ITEM, _needShardNumber)))
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("＝＝＝＝＝【确定升级】＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN + __MENU_PET_ACT_UPGRADE + 1);
			else
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__RED("＝＝＝＝【饲料不足，返回】＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN);
		}
		else
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("＝＝＝＝＝【宠物已满级，返回】＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN);	
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		return true;
	}
	else if (action >= __MENU_PET_MAIN + __MENU_PET_ACT_CHANGE)
	{

		// caculate how mnay active pets the player has
		int32 _activePetNumber = 0;

		if (action > __MENU_PET_MAIN + __MENU_PET_ACT_CHANGE && action < __MENU_PET_MAIN + __MENU_PET_ACT_CHANGE + 10)
		{
			//confirm to change the pet type,
			int32 _newPetType = action - __MENU_PET_MAIN - __MENU_PET_ACT_CHANGE;
			if (_newPetType >= 1 && _newPetType <= 7)
			{
				//change the pet type
				sQZAchievements.ChangeActivePet(player, _newPetType);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("　＝＞　宠物切换成功，返回　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN);	
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());

				return true;
			}
		}

		auto  _petEntry = sQZAchievements.GetPetEntry(player);

		if( action >= __MENU_PET_MAIN + __MENU_PET_ACT_BUY && action < __MENU_PET_MAIN + __MENU_PET_ACT_BUY + 20)
		{
			//check how many active pets the player has
			if(_petEntry.data1) _activePetNumber++;
			if(_petEntry.data2) _activePetNumber++;
			if(_petEntry.data3) _activePetNumber++;
			if(_petEntry.data4) _activePetNumber++;
			if(_petEntry.data5) _activePetNumber++;
			if(_petEntry.data6) _activePetNumber++;
			if(_petEntry.data7) _activePetNumber++;

			//Tell the player how much gold he should pay, around 10 - 2160 Gold
			int32 _needGold = _activePetNumber * _activePetNumber * _activePetNumber * 10;

			auto __actionBuy = action - __MENU_PET_MAIN - __MENU_PET_ACT_BUY;

			
			//Menus for display
			//__MENU_PET_MAIN + __MENU_PET_ACT_BUY + 0 ~10 range for display
			if(__actionBuy > 0 && __actionBuy < 10)
			{
				//display the pet type, and the cost to change the pet type
				//need to pay gold to active the pet, and icnreased the cost when more pets
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);

				text = "";
				text.append(__STR("　　已有宠物数量：　"));
				text.append(__NSTR(_activePetNumber));
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);

				text = "";
				text.append(__STR("　　新购花费金币：　"));
				text.append(__NSTR(_needGold));
				text.append(__STR(" G  "));
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);
				//need to pay gold to active the pet, and icnreased the cost when more pets

				//if player has engough gold
				if(player->GetMoney() > _needGold * 10000)
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("　＝＞　确定购买　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN + __MENU_PET_ACT_BUY + __actionBuy + 10);	
				}
				else 
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__RED("　＝＞　金币不够，返回　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN);	
				}

			}
			//Menus for confirm buy
			//__MENU_PET_MAIN + __MENU_PET_ACT_BUY + 10 ~ 20 range for confirm buy
			if(__actionBuy > 10 && __actionBuy < 20)
			{
				//check if the player has enough gold to buy the pet
				if(player->GetMoney() > _needGold)
				{
					//buy the pet, and add the pet to the player's achievements vector, and save to database
					player->ModifyMoney(-_needGold * 10000);

					//check which pet type the player wants to buy, and add the pet to the player's achievements vector, and save to database
					sQZAchievements.ChangeActivePet(player, __actionBuy - 10);

					//slog the player's pet information, and the pet type he bought, and the cost he paid, and the gold he has left
					sLog.Out(LOG_BASIC, LOG_LVL_MINIMAL, "[Pet System] player:%s buy pet type:%u, cost:%ug, gold left:%u", player->GetName(), __actionBuy - 10, _needGold, player->GetMoney());
				
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("　＝＞　宠物激活成功，返回　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN);	
				}	
				else
				{
					//tell the player that he has not enough gold to buy the pet, and return to the main menu
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__RED("　＝＞　金币不够，返回　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN);	
				}
			}

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
			return true;
		}


		sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "[Pet System] player:%s has %u active pets", player->GetName(), _activePetNumber);

		//show the pet type, and the cost to change the pet type
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("　　更改（开启）宠物类型：　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
		//need to pay gold to active the pet, and icnreased the cost when more pets
		if(_petEntry.data1) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("　＝＞　①：寒冰系－寒冰箭（可开启）　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN + __MENU_PET_ACT_CHANGE + 1);
		else player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__RED("　＝＞　①：寒冰系－寒冰箭（可购买）　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN + __MENU_PET_ACT_BUY + 1);
		
		if(_petEntry.data2) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("　＝＞　②：火焰系－火球术（可开启）　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN + __MENU_PET_ACT_CHANGE + 2);
		else player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__RED("　＝＞　②：火焰系－火球术（可购买）　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN + __MENU_PET_ACT_BUY + 2);
		
		if(_petEntry.data3) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("　＝＞　③：暗影系－暗影箭（可开启）　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN + __MENU_PET_ACT_CHANGE + 3);
		else player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__RED("　＝＞　③：暗影系－暗影箭（可购买）　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN + __MENU_PET_ACT_BUY + 3);
		
		if(_petEntry.data4) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("　＝＞　④：自然系－闪电链（可开启）　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN + __MENU_PET_ACT_CHANGE + 4);
		else player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__RED("　＝＞　④：自然系－闪电链（可购买）　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN + __MENU_PET_ACT_BUY + 4);
		
		if(_petEntry.data5) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("　＝＞　⑤：奥术系－星火术（可开启）　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN + __MENU_PET_ACT_CHANGE + 5);
		else player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__RED("　＝＞　⑤：奥术系－星火术（可购买）　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN + __MENU_PET_ACT_BUY + 5);

		if(_petEntry.data6) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("　＝＞　⑥：神圣系－治疗术（可开启）　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN + __MENU_PET_ACT_CHANGE + 6);
		else player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__RED("　＝＞　⑥：神圣系－治疗术（可购买）　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN + __MENU_PET_ACT_BUY + 6);

		if(_petEntry.data7) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("　＝＞　⑦：物理系－顺劈斩（可开启）　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN + __MENU_PET_ACT_CHANGE + 7);
		else player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__RED("　＝＞　⑦：物理系－顺劈斩（可购买）　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN + __MENU_PET_ACT_BUY + 7);

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR((" ")), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝【返回】＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_PET_MAIN);

		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
	}
	return true;
}

#pragma endregion


void AddSC_qzqstar_custom()
{
	Script* newscript;

	//Add battle ground npc
	newscript = new Script;
	newscript->Name = "qzqstar_battleground";
	newscript->pGossipHello = [](Player *p, Creature *c) -> bool { return BG_Menus(p, c, 0, __MENU_BG_MAIN); };
	newscript->pGossipSelect = &BG_Menus;
	newscript->RegisterSelf(false);


	//Add Dynanic quest script
	newscript = new Script;
	newscript->Name = "qzqstar_dynamictask";
	newscript->pGossipHello = [](Player *p, Creature *c) -> bool { return Task_Menus(p, c, 0, __MENU_TASK_MAIN); };
	newscript->pGossipSelect = &Task_Menus;
	newscript->RegisterSelf(false);


	//Add Pet Development script
	newscript = new Script;
	newscript->Name = "qzqstar_pet_system";
	newscript->pGossipHello = [](Player *p, Creature *c) -> bool { return Pet_Menus(p, c, 0, __MENU_PET_MAIN); };
	newscript->pGossipSelect = &Pet_Menus;
	newscript->RegisterSelf(false);

}


