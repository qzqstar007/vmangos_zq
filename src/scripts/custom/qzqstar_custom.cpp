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

#define	__STR(x)		((std::string)(x)).c_str()
#define	__NSTR(x)		(std::to_string(x))

#define	__BLUE(x)		"|cff002fa7"##x##"|r"
#define	__GREEN(x)		"|cff00b72f"##x##"|r"
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
#define	__MENU_TASK_OFFSET_ACT1					(100)
#define	__MENU_TASK_OFFSET_ACT2					(200)
#define	__MENU_TASK_OFFSET_ACT3					(300)

static int32 _Quest_Counter = 12001;
bool Task_Menus(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
	std::string text = "";
	uint32 __menu_nums = 0;

	if (action == __MENU_TASK_MAIN)
	{
		//First check if player already has a quest
		//1. if no, then create the quest from template.
		if (true)
		{
			//                                                                0        1         2             3           4             5       6                  7                8                9
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
				" FROM `quest_template` where `entry` = 12000; "));

			if (!result)
			{
				sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "[Task System Error, Null template] player:%s.", player->GetName());
				return true;
			}

			Field* fields = result->Fetch();
			std::unique_ptr<Quest> newQuest = std::make_unique<Quest>(fields);

			//set new quest id
			newQuest->QuestId = _Quest_Counter;
			newQuest->QuestLevel = player->GetLevel();
			std::string _title = "";
			_title.append(player->GetName());
			_title.append(__STR("的专属任务　"));
			//Rare quest if possible
			newQuest->Title = _title;
			newQuest->SetSpecialFlag(QuestSpecialFlags(QUEST_SPECIAL_FLAG_KILL_OR_CAST | QUEST_SPECIAL_FLAG_SPEAKTO));


			sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "[Task System] player:%s create questID:%u", player->GetName(), _Quest_Counter);

			auto& questMap = sObjectMgr.GetQuestTemplatesZQ();
			questMap[_Quest_Counter] = std::move(newQuest);

			//save to player's M_Spare_Data3, use as Quest_Counter, for CONFIRM action use!
			//pls don't delete other wise i'll kick u.
			player->M_Spare_Data3 = _Quest_Counter;
						
			_Quest_Counter++;


			//Assign the player task content upon player's levels

			//Add to join the battlegrounds
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("＝＝＝＝随机任务＝＝＝＝　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR("①战歌峡谷　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR("②阿拉希盆地　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR("③奥特兰克山谷　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("＝＝＝＝是否接受＝＝＝＝　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR("① 重新获取任务（需要）　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN + __MENU_TASK_OFFSET_ACT1);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR("② 接受　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN + __MENU_TASK_OFFSET_ACT2);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR("③ 退出　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN + __MENU_TASK_OFFSET_ACT3);
		}
		else
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("＝＝＝＝你今天已经接过任务。＝＝＝＝　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
		}

		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		return true;
	}
	else if (action > __MENU_TASK_MAIN && action<__MENU_TASK_MAIN + __MENU_TASK_OFFSET_ACT3)
	{
		if (player->M_Spare_Data3)
		{
			auto _quest = sObjectMgr.GetQuestTemplate(player->M_Spare_Data3);
			player->AddQuest(_quest, nullptr);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("＝＝＝＝成功接到任务。＝＝＝＝　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
		}

		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		return true;
	}
	else if (action >= __MENU_TASK_MAIN + __MENU_TASK_OFFSET_ACT3)
	{
		
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


