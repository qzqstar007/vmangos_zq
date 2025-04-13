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

#define	__STR(x)		((std::string)(x)).c_str()
#define	__NSTR(x)		(std::to_string(x))

#define	__BLUE(x)		"|cff002fa7"##x##"|r"
#define	__GREEN(x)		"|cff00871f"##x##"|r"
#define	__ORANGE(x)		"|cffe85827"##x##"|r"
#define __RED(x)		"|cfff00019"##x##"|r"
#define __YELLOW(x)		"|cfff9dc24"##x##"|r"


#pragma region __BattleGround___

#define __MENU_BG_MAIN					(1000)
#define	__MENU_BG_RW_OFFSET				(100)
#define __MENU_BG_MAIN_WS				(__MENU_BG_MAIN	+	10)
#define __MENU_BG_MAIN_AB				(__MENU_BG_MAIN	+	20)
#define __MENU_BG_MAIN_AV				(__MENU_BG_MAIN	+	30)

bool BG_Menus(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
	std::string text = "";
	uint32 __menu_nums = 0;

	if (action == __MENU_BG_MAIN)
	{
		//Add to join the battlegrounds
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("＝＝＝＝选择战场＝＝＝＝　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN);

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR("①战歌峡谷　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN_WS);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR("②阿拉希盆地　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN_AB);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR("③奥特兰克山谷　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN_AV);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("＝＝＝＝奖章兑换＝＝＝＝　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR("①战歌奖章　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN_WS + __MENU_BG_RW_OFFSET);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR("②阿拉希奖章　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN_AB + __MENU_BG_RW_OFFSET);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR("③奥特兰克奖章　"), GOSSIP_SENDER_MAIN, __MENU_BG_MAIN_AV + __MENU_BG_RW_OFFSET);


		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		return true;
	}
	else if (action > __MENU_BG_MAIN && action<__MENU_BG_MAIN + __MENU_BG_RW_OFFSET)
	{
		auto bgType = action == __MENU_BG_MAIN_WS ? BATTLEGROUND_WS : action == __MENU_BG_MAIN_AB ? BATTLEGROUND_AB : BATTLEGROUND_AV;

		player->InterruptSpellsWithChannelFlags(AURA_INTERRUPT_INTERACTING_CANCELS);
		player->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_INTERACTING_CANCELS);
		player->GetSession()->SendBattleGroundList(_Creature->GetGUID(), bgType);
	}
	else if (action > __MENU_BG_MAIN + __MENU_BG_RW_OFFSET)
	{
		action -= __MENU_BG_RW_OFFSET;
		auto bgType = action == __MENU_BG_MAIN_WS ? BATTLEGROUND_WS : action == __MENU_BG_MAIN_AB ? BATTLEGROUND_AB : BATTLEGROUND_AV;

		//if (player);
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

	auto _currentQuestID = sQZAchievements.GetCustomQuestID(player);

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
			if(__qStatus == QUEST_STATUS_COMPLETE)	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__GREEN("== 领取奖励 ==")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN + __MENU_TASK_ACT_COMPLET);
			else player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__GREEN("== 返回 ==")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__RED("== 点击重新获取任务，需要花费金币 ==")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN + __MENU_TASK_ACT_REGET);
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
		if (_qRank > 95) _qRank = 3;
		else if (_qRank > 75) _qRank = 2;
		else _qRank = 1;
				
		auto _qEntity = DBHelper_GetQuestByLevel(player);

		//test stub
		//__qIDBegin = 6502;
		_origQuestID = _qEntity.questID;
		/*
		//                                                           0        1         2             3           4             5       6                  7                8                9
		std::unique_ptr<QueryResult> result(WorldDatabase.PQuery("SELECT `entry`, `Method`, `ZoneOrSort`, `MinLevel`, `QuestLevel`, `Type`, `RequiredClasses`, `RequiredRaces`, `RequiredSkill`, `RequiredSkillValue`,"
			//                      10                     11                   12                       13                     14                       15                     16                  17
			"`RepObjectiveFaction`, `RepObjectiveValue`, `RequiredMinRepFaction`, `RequiredMinRepValue`, `RequiredMaxRepFaction`, `RequiredMaxRepValue`, `SuggestedPlayers`, `LimitTime`,"
			//                      18            19              20             21             22                23                  24           25              26
			"`QuestFlags`, `SpecialFlags`, `PrevQuestId`, `NextQuestId`, `ExclusiveGroup`, `NextQuestInChain`, `SrcItemId`, `SrcItemCount`, `SrcSpell`,"
			//                      27       28         29            30                 31                  32         33                34                35                36
			"`Title`, `Details`, `Objectives`, `OfferRewardText`, `RequestItemsText`, `EndText`, `ObjectiveText1`, `ObjectiveText2`, `ObjectiveText3`, `ObjectiveText4`,"
			//                      37            38            39            40            41               42               43               44
			"`ReqItemId1`, `ReqItemId2`, `ReqItemId3`, `ReqItemId4`, `ReqItemCount1`, `ReqItemCount2`, `ReqItemCount3`, `ReqItemCount4`,"
			//                      45              46              47              48              49                 50                 51                 52
			"`ReqSourceId1`, `ReqSourceId2`, `ReqSourceId3`, `ReqSourceId4`, `ReqSourceCount1`, `ReqSourceCount2`, `ReqSourceCount3`, `ReqSourceCount4`,"
			//                      53                    54                    55                    56                    57                       58                       59                       60
			"`ReqCreatureOrGOId1`, `ReqCreatureOrGOId2`, `ReqCreatureOrGOId3`, `ReqCreatureOrGOId4`, `ReqCreatureOrGOCount1`, `ReqCreatureOrGOCount2`, `ReqCreatureOrGOCount3`, `ReqCreatureOrGOCount4`,"
			//                      61               62               63               64
			"`ReqSpellCast1`, `ReqSpellCast2`, `ReqSpellCast3`, `ReqSpellCast4`,"
			//                      65                  66                  67                  68                  69                  70
			"`RewChoiceItemId1`, `RewChoiceItemId2`, `RewChoiceItemId3`, `RewChoiceItemId4`, `RewChoiceItemId5`, `RewChoiceItemId6`,"
			//                      71                     72                     73                     74                     75                     76
			"`RewChoiceItemCount1`, `RewChoiceItemCount2`, `RewChoiceItemCount3`, `RewChoiceItemCount4`, `RewChoiceItemCount5`, `RewChoiceItemCount6`,"
			//                      77            78            79            80            81               82               83               84
			"`RewItemId1`, `RewItemId2`, `RewItemId3`, `RewItemId4`, `RewItemCount1`, `RewItemCount2`, `RewItemCount3`, `RewItemCount4`,"
			//                      85                86                87                88                89                90              91              92              93              94
			"`RewRepFaction1`, `RewRepFaction2`, `RewRepFaction3`, `RewRepFaction4`, `RewRepFaction5`, `RewRepValue1`, `RewRepValue2`, `RewRepValue3`, `RewRepValue4`, `RewRepValue5`,"
			//                      95               96                  97          98              99                   100                 101           102       103       104
			"`RewOrReqMoney`, `RewMoneyMaxLevel`, `RewSpell`, `RewSpellCast`, `RewMailTemplateId`, `RewMailDelaySecs`, `PointMapId`, `PointX`, `PointY`, `PointOpt`,"
			//                      105              106              107              108              109                   110                   111                   112
			"`DetailsEmote1`, `DetailsEmote2`, `DetailsEmote3`, `DetailsEmote4`, `DetailsEmoteDelay1`, `DetailsEmoteDelay2`, `DetailsEmoteDelay3`, `DetailsEmoteDelay4`,"
			//                      113                114              115                  116                  117                  118
			"`IncompleteEmote`, `CompleteEmote`, `OfferRewardEmote1`, `OfferRewardEmote2`, `OfferRewardEmote3`, `OfferRewardEmote4`,"
			//                      119                       120                       121                       122
			"`OfferRewardEmoteDelay1`, `OfferRewardEmoteDelay2`, `OfferRewardEmoteDelay3`, `OfferRewardEmoteDelay4`,"
			//                      123            124               125         126             127      128                  129                     130
			"`StartScript`, `CompleteScript`, `MaxLevel`, `RewMailMoney`, `RewXP`, `RequiredCondition`, `BreadcrumbForQuestId`, `RewRepSpilloverMask`"
			" FROM `quest_template` where `entry` > '%u' and `entry` < '%u' and `QuestLevel`<='%u' and `RequiredClasses`=0 and `RequiredCondition`=0 and `MinLevel` > 1 and QuestFlags<64 and SpecialFlags=0; ",
			__qIDBegin, __qIDBegin + 100, player->GetLevel()));

		if (!result)
		{
			sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "[Task System Error, Null template] player:%s.", player->GetName());

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__RED("＝＝任务生成错误，点击重新获取。＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN+__MENU_TASK_ACT_ACCEPT);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
			return true;
		}

		Field* fields = result->Fetch();
		std::unique_ptr<Quest> newQuest = std::make_unique<Quest>(fields);
		*/
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
			auto __item = DBHelper_GetItemMatsByLevel(player, _qRank);
			if(__item.itemID != 0)
			{
				newQuest->ReqItemId[3] = __item.itemID;
				newQuest->ReqItemCount[3] = 10 - _qRank * 3;
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
				newQuest->RewItemId[i] = 30523;
				newQuest->RewItemCount[i] = _qRank > 1? _qRank-1 : 1;
				break;
			}
		}

		//sObjectMgr.GetCreatureQuestRelationsMap().insert(QuestRelationsMap::value_type(30003, _Quest_Counter));
		sObjectMgr.GetCreatureInvolvedRelationsMap().insert(QuestRelationsMap::value_type(30003, _Quest_Counter));

		auto _orig_quest = sObjectMgr.GetQuestTemplate(_origQuestID);
		if(_orig_quest)	newQuest->SetSpecialFlag(_orig_quest->GetSpecialFlag());
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
		if (player->CanCompleteQuest(_localQuestID)) player->FullQuestComplete(_localQuestID);

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__GREEN("＝＝＝成功接到任务，返回。＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);

		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		return true;
	}
	else if (action == __MENU_TASK_MAIN + __MENU_TASK_ACT_COMPLET)
	{
		if (_currentQuestID)
		{
			Quest const* pQuest = sObjectMgr.GetQuestTemplate(_currentQuestID);
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
		if (player->GetQuestStatus(9813) != QUEST_STATUS_NONE )//&& player->GetQuestStatus(9813) == QUEST_STATUS_COMPLETE)
		{
			Quest const* pQuest = sObjectMgr.GetQuestTemplate(9813);
			player->RewardQuest(pQuest, 0, player, false);
			sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "[Task System] player:%s complete questID status:%u", player->GetName(), player->GetQuestStatus(9813));
		}
		else sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "[Task System] player:%s error questID status:%u", player->GetName(), player->GetQuestStatus(9813));

		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		return true;
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


	//Add battle ground npc
	newscript = new Script;
	newscript->Name = "qzqstar_dynamictask";
	newscript->pGossipHello = [](Player *p, Creature *c) -> bool { return Task_Menus(p, c, 0, __MENU_TASK_MAIN); };
	newscript->pGossipSelect = &Task_Menus;
	newscript->RegisterSelf(false);

}


