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

#include "QzqstarAchievements.h"
#include "qzqstar_kelala.h"
#include "qzqstar_db.h"
#include "qzqstar_helper.h"
#include "Chat.h"
#include "qzqstar_id.h"
#include "qzqstar_rune_str.h"
#include "qzqstar_teleport.h"

#define	__MENU_NONE						0
#define	__MENU_SIZE						999

#define	__MENU_KELALA_NEWBIE			 (100)
#define __MENU_KELALA_MAIN				 1000
#define __MENU_KELALA_LOGIN				 2000
#define __MENU_KELALA_TASK				 3000
#define __MENU_KELALA_MODE				 4000
#define __MENU_KELALA_SHOP				 5000
#define __MENU_KELALA_SOCIAL			 6000
#define __MENU_KELALA_REP				 7000
#define __MENU_KELALA_EQUIP_COLLECTS     10000
#define __MENU_KELALA_EQUIP_COLLECTS_WORLD				11000
#define __MENU_KELALA_EQUIP_COLLECTS_DUNGEON 			12000
#define __MENU_KELALA_EQUIP_COLLECTS_RAID	 			16000
#define __MENU_KELALA_EQUIP_COLLECTS_PROFESSIONAL	 	18000
#define __MENU_KELALA_EQUIP_COLLECTS_END 19999


#define	__STR(x)		((std::string)(x)).c_str()
#define	__NSTR(x)		(std::to_string(x))

#define	__BLUE(x)		"|cff002fa7"##x##"|r"
#define	__GREEN(x)		"|cff00b72f"##x##"|r"
#define	__ORANGE(x)		"|cffe85827"##x##"|r"
#define __RED(x)		"|cfff00019"##x##"|r"
#define __YELLOW(x)		"|cfff9dc24"##x##"|r"

#define __KELALA_LOGIN_GOSSIPID			     	(ZQ_GOSSIP_KELALA_VIP)
#define __LOGIN_REWARD_QUEST_ID			 	 	(ZQ_QUEST_LOGON_IND)
#define __SUBMENU_LOGIN_MAIN					 0
#define __SUBMENU_LOGIN_VIP_LEVELUP				 800	//Normal Levelup using Gold
#define __SUBMENU_LOGIN_VIP_LEVELUP_STONE		 900	//Levelup using VIP Stone
#define __SUBMENU_LOGIN_VIP_ITEMID				 (ZQ_ITEM_VIP_UPGRADE_ITEM)
bool Menus_Kelala_Login(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
	//check if player is null and go is null
	if (!player ||!_Creature) return false;

	//get the abs action
	auto abs_action = action - __MENU_KELALA_LOGIN;

	//get player's vip level
	uint32 vip_level = sQZAchievements.GetVIPLevel(player);
	uint32 vip_gold  = vip_level * vip_level * 1000;
	std::string text = "";

	if (abs_action == __SUBMENU_LOGIN_MAIN)
	{
		//check if player has finished the quest of login reward
		//login reward quest id is __LOGIN_REWARD_QUEST_ID, 
		Quest const* pQuest = sObjectMgr.GetQuestTemplate(__LOGIN_REWARD_QUEST_ID);
		if(pQuest && player->CanTakeQuest(pQuest, false))
		{
			player->RewardQuest(pQuest, 0, player, false);

			//add items upon player's vip level
			if (vip_level == 1)
			{
				player->AddItem(ZQ_ITEM_VOUCHER,  5); player->AddItem(ZQ_ITEM_BUFF, 1); 
			}
			else if (vip_level == 2)
			{
				player->AddItem(ZQ_ITEM_VOUCHER, 10); player->AddItem(ZQ_ITEM_BUFF, 2); player->AddItem(ZQ_ITEM_FRAGMENTS, 2);	player->AddItem(ZQ_ITEM_PET_FOOD, 2);
			}
			else if (vip_level == 3)
			{
				player->AddItem(ZQ_ITEM_VOUCHER, 30); player->AddItem(ZQ_ITEM_FRAGMENTS, 5); player->AddItem(ZQ_ITEM_PET_FOOD, 5); player->AddItem(ZQ_ITEM_RUNE_STONE, 1);
			}
			else if (vip_level == 4)
			{
				player->AddItem(ZQ_ITEM_VOUCHER, 100);  player->AddItem(ZQ_ITEM_FRAGMENTS, 10); player->AddItem(ZQ_ITEM_PET_FOOD, 10); player->AddItem(ZQ_ITEM_RUNE_STONE, 2);//Pearls
			}
			else if (vip_level == 5)
			{
				player->AddItem(ZQ_ITEM_VOUCHER, 200);  player->AddItem(ZQ_ITEM_FRAGMENTS, 20); player->AddItem(ZQ_ITEM_PET_FOOD, 20); player->AddItem(ZQ_ITEM_RUNE_STONE, 5);//Pearls
			}

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＞　登陆奖励领取成功　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
		}
		else
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝奖励已领取，或等级１０级以上。＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);

		}

		//display the player's vip level
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);

		//todo, get the level if has vouchers/stones etc...
		for (uint32 i = 0; i < 4; i++)
		{
			//check if player has the item of vip level i
			if (player->HasItemCount(__SUBMENU_LOGIN_VIP_ITEMID + i, 1))
			{
				text = __STR(__BLUE("＝＝＝＞兑换VIP等级：VIP-"));
				text.append(__STR(__NSTR(i+2)));
				text.append(__STR(" "));
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, text.c_str(), GOSSIP_SENDER_MAIN, __MENU_KELALA_LOGIN + __SUBMENU_LOGIN_VIP_LEVELUP_STONE + i);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);
			}
		}

		//make up the vip level text
		text = __STR(__BLUE("＝＝＝＞　当前VIP等级：　"));
		text.append(__STR(__NSTR(vip_level)));
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, text.c_str(), GOSSIP_SENDER_MAIN, __MENU_NONE);
		//display next player's vip level
		if(vip_level < 5)
		{
			text = __STR(__BLUE("＝＝＝＞　下等级需要金币：　"));
			text.append(__STR(__NSTR(vip_gold)));
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, text.c_str(), GOSSIP_SENDER_MAIN, __MENU_NONE);
		
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);

			text = __STR(__BLUE("＝＝＝＞　点击升级 ＜＝＝　"));
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, __STR(text), GOSSIP_SENDER_MAIN, __MENU_KELALA_LOGIN + __SUBMENU_LOGIN_VIP_LEVELUP);
		}

	}
	else if (abs_action == __SUBMENU_LOGIN_VIP_LEVELUP)
	{
		//check if player has enough gold to level up vip level
		if(player->GetMoney() >= vip_gold * 10000)
		{
			sQZAchievements.SetVIPLevel(player, sQZAchievements.GetVIPLevel(player) + 1);
			player->ModifyMoney(0 - vip_gold * 10000);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　VIP等级升级成功　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
			
			//return to main menu
		}else
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　金币不足，无法升级　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
		}

		//add item to main menu
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("＝＝＝＞　返回　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_KELALA_LOGIN + __SUBMENU_LOGIN_MAIN);
	}
	else if (abs_action >= __SUBMENU_LOGIN_VIP_LEVELUP_STONE && abs_action <= __SUBMENU_LOGIN_VIP_LEVELUP_STONE + 3)
	{
		//check if player has the item of vip level i
		auto _next_viplevel = abs_action - __SUBMENU_LOGIN_VIP_LEVELUP_STONE;
		if (player->HasItemCount(__SUBMENU_LOGIN_VIP_ITEMID + _next_viplevel, 1))
		{
			//remove the item of vip level i
			player->DestroyItemCount(__SUBMENU_LOGIN_VIP_ITEMID + _next_viplevel, 1, true);
			sQZAchievements.SetVIPLevel(player, _next_viplevel + 2);	

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　VIP等级升级成功　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
		}else
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　道具不足，无法升级　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);	
		}

		//add item to main menu
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("＝＝＝＞　返回　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_KELALA_LOGIN + __SUBMENU_LOGIN_MAIN);

	}

	player->SEND_GOSSIP_MENU(__KELALA_LOGIN_GOSSIPID, _Creature->GetGUID());

	return true;
}

#pragma region Dynamic Task_Generate

#define __SUBMENU_TASK_MAIN							 (0)
#define __MENU_TASK_MAIN						    (__MENU_KELALA_TASK + __SUBMENU_TASK_MAIN)	
#define	__MENU_TASK_ACT_ACCEPT						(100)
#define	__MENU_TASK_ACT_COMPLET						(200)
#define	__MENU_TASK_ACT_COMPLET_BY_COST				(220)
#define	__MENU_TASK_ACT_REGET						(300)
#define __MENU_TASK_MODIFY_APSP						(500)

static int32 _Quest_Counter = 12001;
bool Menus_Kelala_Task(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
	std::string text = "";
	uint32 __menu_nums = 0;

	auto	_currentQuestID = sQZAchievements.GetCustomQuestID(player);
	uint32	__reset_gold = player->GetLevel() * player->GetLevel() / 610 + 1;		//TODO NEXT SEASON

	//get custom settings
	uint32 __custom_settings = sQZAchievements.GetCustomSettings(player);
	uint32 __apsp_method = __custom_settings & 0x03;

	uint32 _doneCounter = sQZAchievements.GetQuestDoneCounters(player);
	uint32 _apBonus = PAIR32_HIPART(_doneCounter)/2;
	uint32 _spBonus = PAIR32_HIPART(_doneCounter)/4;
	//if(_apBonus > 5000) _apBonus = 5000;
	//if(_spBonus > 2500) _spBonus = 2500;

	if(__apsp_method == 1)  { _apBonus = _apBonus + _spBonus; _spBonus = 0; }
	else if(__apsp_method == 2) { _spBonus = _spBonus + _apBonus/2; _apBonus = 0; }

	if(action >= __MENU_TASK_MAIN && action < __MENU_TASK_MAIN + __MENU_TASK_MODIFY_APSP)
	{	
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝万环任务系统＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
		text = __STR("｜　　今日完成：|cff007733");
		text.append(__NSTR(PAIR32_LOPART(_doneCounter)));
		text.append(__STR(" |r / 100 "));
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);
		text = __STR("｜　　总共完成：|cff007733");
		text.append(__NSTR(PAIR32_HIPART(_doneCounter)));
		text.append(__STR(" |r / 10000 "));
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);
		text = __STR("｜　　攻强增加：|cff007733");
		text.append(__NSTR(_apBonus));
		text.append(__STR(" |r 点 "));
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);
		text = __STR("｜　　法伤增加：|cff007733");
		text.append(__NSTR(_spBonus));
		text.append(__STR(" |r 点 "));
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＝【切换加成方式】＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN + __MENU_TASK_MODIFY_APSP);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
	}


	
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
			__statusStr.append(__qStatus == QUEST_STATUS_COMPLETE ? __STR(__GREEN(" ＝【已完成】＝＝ ")) : __STR(__RED(" ＝【未完成】＝＝ ")));


			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__statusStr), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			if (__qStatus == QUEST_STATUS_COMPLETE)	
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__GREEN("== 领取奖励 == ")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN + __MENU_TASK_ACT_COMPLET);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__GREEN("＝＝【返回】＝＝＝ ")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("　"), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);


			if ( (__qStatus != QUEST_STATUS_COMPLETE) && (player->GetLevel() > 10))	 
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__RED("== 消耗５点券直接完成任务 == ")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN + __MENU_TASK_ACT_COMPLET_BY_COST);
			

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
	else if (action == __MENU_TASK_MAIN + __MENU_TASK_ACT_ACCEPT)
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
			// if 967 is less than 10, pick random quest otherwise, pick the quest
			auto _testQuestID = player->GetReputationMgr().GetReputation(966);
			if (_testQuestID > 10) _origQuestID = _testQuestID;
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
		newQuest->Objectives.append(__STR(" $B$B |cffff0000【注意】任务显示完成后直接回加基森交任务。如果任务显示不对，需要清除WDB文件夹。|r "));

		//check if any killed or creature needed
		/**/
		if ((newQuest->ReqCreatureOrGOId[0] == 0 && newQuest->ReqItemId[0] == 0) || (newQuest->SrcItemId != 0 && newQuest->ReqItemId[0] == newQuest->SrcItemId))
		{
			auto __item = DBHelper_GetItemMatsByLevel(player, 1);
			if (__item.itemID != 0)
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
			if ((player->GetLevel() < 11 || _qRank > 1) && newQuest->RewItemId[i] == 0)
			{
				// should pick up random rewards
				newQuest->RewItemId[i] = ZQ_ITEM_VOUCHER;
				newQuest->RewItemCount[i] = _qRank > 1 ? _qRank - 1 : 1;
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


#define _QUEST_NPC_ID (30000)
		sObjectMgr.GetCreatureInvolvedRelationsMap().insert(QuestRelationsMap::value_type(_QUEST_NPC_ID, _Quest_Counter));

		//auto _orig_quest = sObjectMgr.GetQuestTemplate(_origQuestID);
		//if(_orig_quest)	newQuest->SetSpecialFlag(_orig_quest->GetSpecialFlag());
		sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "[Task System] player:%s create questID:%u", player->GetName(), _Quest_Counter);

		auto& questMap = sObjectMgr.GetQuestTemplatesZQ();
		questMap[_Quest_Counter] = std::move(newQuest);

		//assign a new counter here
		auto _localQuestID = _Quest_Counter;
		_Quest_Counter++;

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
	else if (action == __MENU_TASK_MAIN + __MENU_TASK_ACT_COMPLET_BY_COST)
	{
		//check player has enough voucher
		if(!player->HasItemCount(ZQ_ITEM_VOUCHER, 5))
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__RED("＝＝你的点券不够，返回。＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
			return true;
		}

		player->DestroyItemCount(ZQ_ITEM_VOUCHER, 5, true);
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
			player->SetMoney(player->GetMoney() - __reset_gold * 10000);

			sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "[Task System] player:%s reset questID: %u", player->GetName(), _currentQuestID);

			//finish the quest, but not increase counter
			sQZAchievements.FinishCustomQuest(player, false);

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__GREEN("＝＝＝重置成功，重新获取任务。＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		return true;
	}

	else if (action >= __MENU_TASK_MAIN + __MENU_TASK_MODIFY_APSP)
	{
		uint32 _absAction = action - __MENU_TASK_MAIN - __MENU_TASK_MODIFY_APSP;

		if(_absAction == 0)
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝【当前加成方式】＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			if (__apsp_method == 0) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝　１．平均转化攻强和法伤　")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			else if (__apsp_method == 1) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝　２．全部转化为攻强　")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			else if (__apsp_method == 2) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝　３．全部转化为法伤　")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("点击下方更换计算方式，小退生效。　")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("－＞　１．平均转化攻强和法伤　")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN + __MENU_TASK_MODIFY_APSP + 1);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("－＞　２．全部转化为攻强　")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN + __MENU_TASK_MODIFY_APSP + 2);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("－＞　３．全部转化为法伤　")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN + __MENU_TASK_MODIFY_APSP + 3);

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝　返回　＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
		}
		else if (_absAction >= 1 && _absAction <= 3)
		{
			__custom_settings = __custom_settings & 0xFFFFFFFC;
			__custom_settings = __custom_settings | (_absAction - 1);
			sQZAchievements.SetCustomSettings(player, __custom_settings);	
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝加成方式已改变，小退生效。＝　")), GOSSIP_SENDER_MAIN, __MENU_TASK_MAIN);
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
	}

	return true;
}

#pragma endregion

#pragma region Shop System

#define __MENU_SHOP_MAIN						(__MENU_KELALA_SHOP + 0)

bool Menus_Kelala_Shop(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
	//check if player is null and go is null
	if (!player ||!_Creature) return false;

	// Main menu
	player->GetSession()->SendListInventory(_Creature->GetObjectGuid());

	return true;
}
#pragma endregion


#pragma region Mode System
#define	__GOSSIP_MODE_MSG						(ZQ_GOSSIP_KELALA_MODE_FUNC)
#define __MENU_MODE_MAIN						(__MENU_KELALA_MODE + 0)
#define __MENU_MODE_SUB_1						(__MENU_MODE_MAIN + 10)
#define	__MENU_MODE_SUB_1_NAME					"[退出一命模式，领取奖励]"
#define __MENU_MODE_SUB_1_ACT_1					(__MENU_MODE_SUB_1 + 1)
#define __MENU_MODE_SUB_1_SPELL					(CHALLENGING_MODE_ONELIFE)

#define __MENU_MODE_SUB_2						(__MENU_MODE_SUB_1 + 10)
#define	__MENU_MODE_SUB_2_NAME					"[退出自强模式，领取奖励]"
#define __MENU_MODE_SUB_2_ACT_1					(__MENU_MODE_SUB_2 + 1)
#define __MENU_MODE_SUB_2_SPELL					(CHALLENGING_MODE_TASK)

#define __MENU_MODE_SUB_3						(__MENU_MODE_SUB_2 + 10)
#define	__MENU_MODE_SUB_3_NAME					"[退出装等模式，领取奖励]"
#define __MENU_MODE_SUB_3_ACT_1					(__MENU_MODE_SUB_3 + 1)
#define __MENU_MODE_SUB_3_SPELL					(30845)

#define __MENU_MODE_SUB_4						(__MENU_MODE_SUB_3 + 10)
#define	__MENU_MODE_SUB_4_NAME					"[退出任务模式，领取奖励]"
#define __MENU_MODE_SUB_4_ACT_1					(__MENU_MODE_SUB_4 + 1)
#define __MENU_MODE_SUB_4_SPELL					(30847)

#define __MENU_MODE_SUB_5						(__MENU_MODE_SUB_4 + 10)
#define	__MENU_MODE_SUB_5_NAME					"[退出杀手模式，领取奖励]"
#define __MENU_MODE_SUB_5_ACT_1					(__MENU_MODE_SUB_5 + 1)
#define __MENU_MODE_SUB_5_SPELL					(30849)

#define __MENU_MODE_SUB_6						(__MENU_MODE_SUB_5 + 10)

//Mode function
#define __MENU_MODE_SUB_31						(__MENU_MODE_MAIN + 100)
#define	__MENU_MODE_SUB_31_NAME					"[装等模式：查看装等，突破等级]"
#define __MENU_MODE_SUB_31_ACT_1				(__MENU_MODE_SUB_31 + 1)

#define __MENU_MODE_SUB_32						(__MENU_MODE_MAIN + 110)
#define	__MENU_MODE_SUB_32_NAME					"[装等巅峰：查看装等，获取突破装等巅峰|]"
#define __MENU_MODE_SUB_32_ACT_1				(__MENU_MODE_SUB_32 + 1)

#define __MENU_MODE_SUB_51						(__MENU_MODE_MAIN + 150)
#define	__MENU_MODE_SUB_51_NAME					"[杀手模式：隐姓埋名，更改名字]"
#define __MENU_MODE_SUB_51_ACT_1				(__MENU_MODE_SUB_51 + 1)


#define __MENU_MODE_BREAK_THROUGH_DUMMY_SPELL		(31290)	//Dummy indicators
#define __MENU_MODE_BREAK_THROUGH_CAST_SPELL		(31295)	//Really Apply Auras

static Top10Ranking	__Rankings;

bool Menus_Kelala_Mode(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
	//check if player is null and go is null
	if (!player ||!_Creature) return false;

	// Main menu
	std::string text = "";
	auto pLevel = player->GetLevel();

	switch (action)
	{
		case __MENU_MODE_MAIN:
		{
			player->ADD_GOSSIP_ITEM(5, __STR("＝＝满级退出挑战，领取奖励＝＝"), GOSSIP_SENDER_MAIN, __MENU_NONE);
			sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "[Mode main] player:%s, M_Mode:0x%X", player->GetName(), player->M_Challenge_Mode);

			//Five Modes Exit
			/*					case 1:	 _Mode |= CHALLENGING_MODE_ONELIFE; break;
					case 2:	 _Mode |= CHALLENGING_MODE_MANUFACT; break;
					case 3:	 _Mode |= CHALLENGING_MODE_TASK; break;
					case 4:	 _Mode |= CHALLENGING_MODE_EQUIPMENT; break;
					case 5:	 _Mode |= CHALLENGING_MODE_RICH; break;*/
			if (player->GetLevel() == 60 && (player->M_Challenge_Mode & CHALLENGING_MODE_ONELIFE) && ( (player->M_Challenge_Mode & CHALLENGING_MODE_DONE_ONELIFE) == 0) ) 
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__BLUE("＝＞　退出一命模式，领取奖励　＜＝")), GOSSIP_SENDER_MAIN, __MENU_MODE_SUB_1);
			if (player->GetLevel() == 60 && (player->M_Challenge_Mode & CHALLENGING_MODE_MANUFACT) && ( (player->M_Challenge_Mode & CHALLENGING_MODE_DONE_MANUFACT) == 0) ) 
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__BLUE("＝＞　退出工匠（保留减半），领取奖励　＜＝")), GOSSIP_SENDER_MAIN, __MENU_MODE_SUB_2);
			if (player->GetLevel() == 60 && (player->M_Challenge_Mode & CHALLENGING_MODE_TASK) && ((player->M_Challenge_Mode & CHALLENGING_MODE_DONE_TASK) == 0) )
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__BLUE("＝＞　退出任务模式，领取奖励　＜＝")), GOSSIP_SENDER_MAIN, __MENU_MODE_SUB_3);
			if (player->GetLevel() == 60 && (player->M_Challenge_Mode & CHALLENGING_MODE_EQUIPMENT) && ((player->M_Challenge_Mode & CHALLENGING_MODE_DONE_EQUIPMENT) == 0) )
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__BLUE("＝＞　退出装等模式，领取奖励　＜＝")), GOSSIP_SENDER_MAIN, __MENU_MODE_SUB_4);
			if (player->GetLevel() == 60 && (player->M_Challenge_Mode & CHALLENGING_MODE_RICH) && ((player->M_Challenge_Mode & CHALLENGING_MODE_DONE_RICH) == 0) )
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__BLUE("＝＞　退出富豪（退后无属性加成、考虑好）　＜＝")), GOSSIP_SENDER_MAIN, __MENU_MODE_SUB_5);
			if (player->GetLevel() == 60 
				&& ((player->M_Challenge_Mode & CHALLENGING_MODE_KILLER_MASK) != 0)
				&& ((player->M_Challenge_Mode & CHALLENGING_MODE_DONE_KILLER) == 0) )
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__BLUE("＝＞　领取杀手模式奖励　＜＝")), GOSSIP_SENDER_MAIN, __MENU_MODE_SUB_6);
			
			player->ADD_GOSSIP_ITEM(5, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(5, __STR("＝＝＝＝模式功能＝＝＝＝"), GOSSIP_SENDER_MAIN, __MENU_NONE);
			
			// change the player's level to 25/35/45/55 etc..
			if ( (player->M_Challenge_Mode & CHALLENGING_MODE_EQUIPMENT) && (pLevel==25 || pLevel == 35 || pLevel == 45 || pLevel == 58)) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__BLUE(__MENU_MODE_SUB_31_NAME)), GOSSIP_SENDER_MAIN, __MENU_MODE_SUB_31);
			if (pLevel > 58) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__BLUE(__MENU_MODE_SUB_32_NAME)), GOSSIP_SENDER_MAIN, __MENU_MODE_SUB_32);


			player->ADD_GOSSIP_ITEM(5, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(5, __STR("<===返回===="), GOSSIP_SENDER_MAIN, __MENU_KELALA_MAIN);

			player->SEND_GOSSIP_MENU(__GOSSIP_MODE_MSG, _Creature->GetGUID());
			break;
		}

		case __MENU_MODE_SUB_1: 
		{	
			//One Life mode
			player->M_Challenge_Mode |= CHALLENGING_MODE_DONE_ONELIFE;
			player->M_Challenge_Mode &= ~CHALLENGING_MODE_ONELIFE;
			sQZAchievements.SetChallengeMode(player, player->M_Challenge_Mode);
			player->AddItem(ZQ_ITEM_TURTLE);
			player->AddItem(ZQ_ITEM_VOUCHER, 600);
			player->ADD_GOSSIP_ITEM(5, "<==成功退出一命模式，返回首页===", GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN); 
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID()); 
			break;
		}

		case __MENU_MODE_SUB_2: //manufact
		{	
			player->M_Challenge_Mode |= CHALLENGING_MODE_DONE_MANUFACT;
			sQZAchievements.SetChallengeMode(player, player->M_Challenge_Mode);
			player->AddItem(ZQ_ITEM_VOUCHER, 300);
			player->ADD_GOSSIP_ITEM(5, "<==成功获取工匠奖励，返回首页===", GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN); 
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID()); 
			break;
		}

		case __MENU_MODE_SUB_3: //Task
		{	
			player->M_Challenge_Mode |= CHALLENGING_MODE_DONE_TASK;
			sQZAchievements.SetChallengeMode(player, player->M_Challenge_Mode);
			player->AddItem(ZQ_ITEM_VOUCHER, 300);
			player->ADD_GOSSIP_ITEM(5, "<==成功获取任务奖励，返回首页===", GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN); 
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID()); 
			break;
		}
		case __MENU_MODE_SUB_4: //Equipment
		{	
			player->M_Challenge_Mode |= CHALLENGING_MODE_DONE_EQUIPMENT;
			sQZAchievements.SetChallengeMode(player, player->M_Challenge_Mode);
			player->AddItem(ZQ_ITEM_VOUCHER, 300);
			player->ADD_GOSSIP_ITEM(5, "<==成功获取装等奖励，返回首页===", GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN); 
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID()); 
			break;
		}
		
		case __MENU_MODE_SUB_5:
		{	
			player->M_Challenge_Mode |= CHALLENGING_MODE_DONE_RICH;
			player->M_Challenge_Mode &= ~CHALLENGING_MODE_RICH;
			sQZAchievements.SetChallengeMode(player, player->M_Challenge_Mode);
			player->AddItem(ZQ_ITEM_GOLD_BAR, 5);
			player->ADD_GOSSIP_ITEM(5, "<==成功获取富豪奖励，返回首页===", GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN); 
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID()); 
			break;
		}

		case __MENU_MODE_SUB_6:
		{	
			player->M_Challenge_Mode |= CHALLENGING_MODE_DONE_KILLER;
			sQZAchievements.SetChallengeMode(player, player->M_Challenge_Mode);
			player->AddItem(ZQ_ITEM_VOUCHER, 300);
			player->ADD_GOSSIP_ITEM(5, "<==成功获取杀手奖励，返回首页===", GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN); 
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID()); 
			break;
		}

		//Advanced the level
		case __MENU_MODE_SUB_31:
		case __MENU_MODE_SUB_31_ACT_1:
		{	
			//get the all levels, and expected level
			//directly assign the eqlevel
			//const int _eqLevelEach[] = {25, 40, 55, 70}; // First stage
			//const int _eqLevelEach[] = { 20, 40, 55, 70 };	//Second Stage
			//const int _eqLevelEach[] = { 18, 35, 60, 70 };	//3rd Stage
			//const int _eqLevelEach[] = { 15, 30, 52, 80 };		//4th Stage
			const int _eqLevelEach[] = { 10, 20, 30, 40 };		//4th Stage
			
			//pick the _eqLevelEach for different level of player
			auto __pick = pLevel < 26 ? 0 : pLevel < 36 ? 1 : pLevel < 46 ? 2 : 3;
			int32 _needEQLevel = 20 * _eqLevelEach[__pick];

			if ((pLevel < 46) && (player->GetClass() == CLASS_DRUID || player->GetClass() == CLASS_PALADIN || player->GetClass() == CLASS_SHAMAN))
			{
				_needEQLevel = _needEQLevel * 19 / 20;
			}

			int32 _curEQLevel = 0;
			for (int i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i)
			{
				if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
				{
					_curEQLevel += pItem->GetProto()->ItemLevel;
					if(pItem->GetProto()->InventoryType == INVTYPE_2HWEAPON) _curEQLevel += pItem->GetProto()->ItemLevel;

					if(pItem->GetEntry() == ZQ_ITEM_CHENYI) _curEQLevel += player->GetLevel();
					else if(pItem->GetEntry() == ZQ_ITEM_ZHANPAO) _curEQLevel += player->GetLevel();

					//check the random properties
					if(pItem->GetItemRandomPropertyId() > 3300 && pItem->GetItemRandomPropertyId() < 3321)
					{
						auto difficutly = (pItem->GetItemRandomPropertyId() -1 ) % 5;
						_curEQLevel += (difficutly) * pItem->GetProto()->ItemLevel / 10;
					}
				}
			}

			if(action == __MENU_MODE_SUB_31_ACT_1)
			{
				//check if the player has enough eqlevel to break through the level
				if (_curEQLevel >= _needEQLevel && pLevel < 60)
				{
					player->GiveLevel(pLevel + 1);

					//give the vouchers
					if(_curEQLevel > _needEQLevel)
					{
						player->AddItem(ZQ_ITEM_VOUCHER, (_curEQLevel - _needEQLevel) );
					}
					
					//Announce the player
					auto const& sessions = sWorld.GetAllSessions();
					for (const auto& itr : sessions)
					{
						if (WorldSession* session = itr.second)
						{
							Player* _onlineplayer = session->GetPlayer();
							if (_onlineplayer && _onlineplayer->IsInWorld() && player->IsAlive())
							{
								//9021 |cff0000bb[等级突破]|r 恭喜玩家：|cff0000bb[%s]|r  成功突破等级限制，装等：%u，原等级：%u，现在等级：%u。
								ChatHandler(_onlineplayer).PSendSysMessage(ZQ_MANGOS_STRING_CHALLENGE_BREAKTHROUGH, player->GetName(), _curEQLevel, pLevel, pLevel+1);
							}
						}
					}
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "<==突破成功，返回首页===", GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN);
				}	
				else
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "<==|未达到要求，返回首页|===", GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN);
				}

				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
				return true;
			}

			// ----- region -------------- display the rankings
			//Add self rankings
			__Rankings.addOrUpdate(_curEQLevel, player->GetName());

			//Display the Rankings
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("　　＝＝装等排行榜＝＝　　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
			for (size_t i = 0; i < 5; i++)
			{
				text = (i==0 ? "玩家①： |cff0829C9 "
					: i == 1 ? "玩家②： |cff0829C9 "
					: i == 2 ? "玩家③： |cff0829C9 "
					: i == 3 ? "玩家④： |cff0829C9 "
					:"玩家⑤： |cff0829C9 "
					);
				auto entry = __Rankings.getEntryByRank(i + 1);
				text.append(entry.second);
				text.append("|r, 总装等： |cff0829C9 ");
				text.append(__NSTR(entry.first));
				text.append("|r ");

				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);
			}
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);
			//--------end of display rankings

			text = "|需要装等: ";
			text.append(__NSTR(_needEQLevel));
			text.append(" , 当前装等:");
			text.append(__NSTR(_curEQLevel));
			text.append(" .");

			sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "[COLLECT] player:%s, level:%d, EQ Total:%d, need EQ:%d", player->GetName(), pLevel, _curEQLevel, _needEQLevel);

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);
			if (_curEQLevel >= _needEQLevel && pLevel<60 )
			{
				
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝满足装等要求，可以突破等级＝＝ ")), GOSSIP_SENDER_MAIN, __MENU_NONE);
				text = "|cff0829C9 并获得点券数量：";
				text.append(__NSTR(_curEQLevel - _needEQLevel));
				text.append("。|r");
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__RED("==|确定突破|===")), GOSSIP_SENDER_MAIN, __MENU_MODE_SUB_31_ACT_1);
			}
			else player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__RED("==装等不够,,等会再来===")), GOSSIP_SENDER_MAIN, __MENU_NONE);

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "<==返回首页===", GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
			break;
		}

		//BreakThrough Top ranking
		case __MENU_MODE_SUB_32:
		{

			#define _BREAK_THROUGH_RANKING_SCORE_	(1600)		//+100 each

			//1. check the spells that player owned
			auto _Ranks_Now = 0;
			for (size_t i = 0; i < 5; i++)
			{
				if (player->HasSpell(__MENU_MODE_BREAK_THROUGH_DUMMY_SPELL + i))	_Ranks_Now++;
			}

			int32 _needEQLevel = _BREAK_THROUGH_RANKING_SCORE_ + _Ranks_Now * 100;
			int32 _curEQLevel = 0;
			for (int i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i)
			{
				if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
				{
					_curEQLevel += pItem->GetProto()->ItemLevel;
					if (pItem->GetProto()->InventoryType == INVTYPE_2HWEAPON) _curEQLevel += pItem->GetProto()->ItemLevel;
				}
			}
			
			// ----- region -------------- display the rankings
			//Add self rankings
			__Rankings.addOrUpdate(_curEQLevel, player->GetName());

			//Display the Rankings
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("　　＝＝装等排行榜＝＝　　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
			for (size_t i = 0; i < 5; i++)
			{
				text = (i == 0 ? "玩家①： |cff0829C9 "
					: i == 1 ? "玩家②： |cff0829C9 "
					: i == 2 ? "玩家③： |cff0829C9 "
					: i == 3 ? "玩家④： |cff0829C9 "
					: "玩家⑤： |cff0829C9 "
					);
				auto entry = __Rankings.getEntryByRank(i + 1);
				text.append(entry.second);
				text.append("|r, 总装等： |cff0829C9 ");
				text.append(__NSTR(entry.first));
				text.append("|r ");

				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);
			}
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);
			//--------end of display rankings

			text = "当前装等巅峰： |cffff0000 ";
			text.append(_Ranks_Now == 0? " ０－无称号 |r"
				: _Ranks_Now == 1 ? " １－青铜 |r"
				: _Ranks_Now == 2 ? " ２－白银 |r"
				: _Ranks_Now == 3 ? " ３－黄金 |r"
				: _Ranks_Now == 4 ? " ４－钻石 |r"
				: " ５－王者 |r"
			);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);

			text = "";
			if (_Ranks_Now < 5)
			{
				text = "|下阶段需要装等: ";
				text.append(__NSTR(_needEQLevel));
			}
			text.append(" , 当前装等:");
			text.append(__NSTR(_curEQLevel));
			text.append(" .");
			 player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);

			sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "[COLLECT] player:%s, BT Ranks:%d, EQ Total:%d, need EQ:%d", player->GetName(), _Ranks_Now, _curEQLevel, _needEQLevel);
			
			if (_Ranks_Now < 5 && _curEQLevel >= _needEQLevel && pLevel>58)
			{
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__BLUE("===|满足装等要求，已提升至新等级，小退生效。|===")), GOSSIP_SENDER_MAIN, __MENU_NONE);

				//learn spell
				player->LearnSpell(__MENU_MODE_BREAK_THROUGH_DUMMY_SPELL + _Ranks_Now, false);

				_Ranks_Now++;
				//Announce the player
				auto const& sessions = sWorld.GetAllSessions();
				for (const auto& itr : sessions)
				{
					if (WorldSession* session = itr.second)
					{
						Player* _onlineplayer = session->GetPlayer();
						if (_onlineplayer && _onlineplayer->IsInWorld() && player->IsAlive())
						{
							//|cff0000bb[装备巅峰]|r 恭喜玩家：|cff0000bb[%s]|r  成功装备突破新等级，总装等：%u，称号：%s。
							ChatHandler(_onlineplayer).PSendSysMessage(9052, player->GetName(), _curEQLevel, 
								_Ranks_Now == 1 ? __STR(" １－青铜 ")
								: _Ranks_Now == 2 ? __STR(" ２－白银 ")
								: _Ranks_Now == 3 ? __STR(" ３－黄金 ")
								: _Ranks_Now == 4 ? __STR(" ４－钻石 ")
								: __STR(" ５－王者 ")
								);
						}
					}
				}

			}
			else player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__RED("==装等不够，等会再来===")), GOSSIP_SENDER_MAIN, __MENU_NONE);

			player->ADD_GOSSIP_ITEM(5, "<==返回首页===", GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
			break;
		}


		case __MENU_MODE_SUB_51:
		{
			if (player->HasSpell(__MENU_MODE_SUB_5_SPELL)) player->SetName("1");
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__GREEN("==姓名已重置，请小退后再上===")), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
			break;
		}

	}

	return true;
}

#pragma endregion


#pragma region social system
#define __MENU_SOCIAL_MAIN						(__MENU_KELALA_SOCIAL + 0)
#define	__MENU_SOCIAL_SELL						(__MENU_SOCIAL_MAIN + 100)
#define	__MENU_SOCIAL_BUY						(__MENU_SOCIAL_MAIN + 200)

uint32 __getPoints(Item* pItem, uint32 __socialPoints)
{
	uint32 points = 0;

	//max div will be 3000, so factor should be 3100~3700
	__socialPoints = __socialPoints / 10;

	if (pItem)
	{
		//formula is 
		//points = item_level ^ 2 / 100 / __rank;
		points = pItem->GetProto()->Quality * pItem->GetProto()->Quality * pItem->GetProto()->ItemLevel * pItem->GetProto()->ItemLevel / (__socialPoints + 100);

		if (points > 100) points = 100;
	}

	return points;
}

static const int __item_list[] = {
	//level 25 - about 52 items
#define	_ITEM_BUY_RANGE_1	(52)
	7682,6220,6904,9453,5201,6631,6318,1155,5196,5426,
	1292,6692,5423,5194,6687,6905,6469,6696,9456,3078,
	7230,9449,7723,9413,5197,6472,2816,9457,6909,6641,
	7689,10758,5198,8071,7001,16789,13245,4278,9458,6694,
	6449,5193,6340,6314,5191,5192,11121,6633,1156,6463,
	6321,1076,


#define	_ITEM_BUY_RANGE_2 (_ITEM_BUY_RANGE_1 + 39)	
	//level 45 - about 39
	7714,10761,2164,7713,9408,21452,9478,11920,871,9459,
	7717,2291,9412,18323,2824,17766,11803,2915,7721,9639,
	21715,11786,9372,21492,21801,17745,11748,10767,7726,21485,
	6901,10776,17739,9446,17719,17705,9447,9461,11118,

#define	_ITEM_BUY_RANGE_3 (_ITEM_BUY_RANGE_2 + 74)	
	//level 55 - about 74
	12709,14024,18392,13937,22335,22394,13408,13952,18737,11816,
	13285,11931,18738,18680,13380,12969,21806,14531,12791,21498,
	13368,22208,22333,13167,17780,13964,18372,14487,18376,11684,
	18520,13163,14541,13938,22408,13396, 1168,12602,22336,22315,
	18321,21466,14528,18485,13529,11623,13376,13386,18389,11626,
	13340,18328,18689,18396,22332,22383,13361,13246,15806,12939,
	12940,19168,11824,22433,22339,22257,13178,18395,22255,13373,
	13098,10795,11669,22331,

#define	_ITEM_BUY_RANGE_4 (_ITEM_BUY_RANGE_3 + 59)	
	//level 60 - orange 1 - 59 items
	17071,18805,18608,20581,19909,19852,17068,18538,17104,20038,
	17072,19918,17073,12590,19859,18842,19884,17113,18803,18878,
	19903,17112,20580,17076,19854,17077,19861,19862,17066,17105,
	19890,17106,19915,17078,19857,18811,18208,17102,18541,17107,
	12905,19864,17103,17075,18832,18348,19867,22721,19147,20632,
	19140,19138,18543,17063,22722,18821,18879,18813,20624,


#define	_ITEM_BUY_RANGE_5 (_ITEM_BUY_RANGE_4 + 58)
	//level 60 - orange 2 - 58 items
	19346,20578,21275,21128,19362,19363,19353,19354,19350,19368,
	19358,19357,21244,18816,19356,21273,21703,19347,21268,19335,
	19364,21679,19367,21603,21269,19349,21839,19360,21610,19348,
	22731,19378,21697,19430,20579,19398,21701,19436,21621,19386,
	21622,21521,19352,21650,19351,20577,21707,19403,19397,21620,
	19382,21681,21596,19376,21677,21695,21601,19432,

#define	_ITEM_BUY_RANGE_6 (_ITEM_BUY_RANGE_5 + 47)
	//level 60 - orange 3 - 47 items
	23044,21126,22801,22816,21242,22815,21134,21616,21272,22798,
	22802,22804,22799,22800,22809,22803,22808,22988,22813,22691,
	22821,22820,23043,22818,23056,22942,22819,23075,23050,23017,
	21583,22960,23030,21710,22938,22807,23014,23054,23577,22806,
	23025,23031,22939,23037,22961,23038,23018,
};

uint32 __getSocialPointsBySeq(int seq)
{
	if (seq < _ITEM_BUY_RANGE_1) return  500;
	else if (seq < _ITEM_BUY_RANGE_2) return 1000;
	else if (seq < _ITEM_BUY_RANGE_3) return 2000;
	else if (seq < _ITEM_BUY_RANGE_4) return 3000;
	else if (seq < _ITEM_BUY_RANGE_5) return 4000;
	else							  return 5000;
}

bool Menus_Kelala_Social(Player *player, Creature *_Creature, uint32 action)
{
	int _socialPoints = 0;
	std::string text;
	auto localIdx = player->GetSession()->GetSessionDbLocaleIndex();
	//_socialPoints = player->GetReputationMgr().GetReputation(967);
	int ITEM_COUNTS = sizeof(__item_list) / sizeof(__item_list[0]);

	_socialPoints = sQZAchievements.GetSocialPoints(player);

	if (action == __MENU_SOCIAL_MAIN)
	{
		//Add SubAction 1 - Upload more items
		text = __STR(__BLUE("你目前的贡献值为==> "));
		text.append("|cffdd2222 " + std::to_string(_socialPoints) + " |r");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_SOCIAL_MAIN);

		text = __STR(__BLUE("你目前的称号是：")); 	text.append("|cff003333 ");
		if (_socialPoints > 30000)	text.append(__STR(" 【领袖】】  "));
		else if (_socialPoints > 20000)	text.append(__STR(" 【精英】】  "));
		else if (_socialPoints > 10000)	text.append(__STR(" 【勇者】】  "));
		else if (_socialPoints >  5000)	text.append(__STR(" 【新秀】】  "));
		else if (_socialPoints >  1000)	text.append(__STR(" 【见习】】  "));
		else text.append(__STR(" 【入门】】  "));
		text.append(" |r");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_SOCIAL_MAIN);

		if (_socialPoints <= 30000)
		{
			text = __STR(__BLUE("    下一等级贡献点 ==> "));
			text.append("|cff001166 " + std::to_string(
				_socialPoints > 20000 ? 30000
				: _socialPoints > 10000 ? 20000
				: _socialPoints >  5000 ? 10000
				: _socialPoints >  1000 ? 5000
				: 1000
			) + " |r");
		}
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_SOCIAL_MAIN);

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_SOCIAL_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_SOCIAL_MAIN);
		text = __STR(__BLUE(">>> 回收装备，提升贡献。 <<<"));
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, __STR(text), GOSSIP_SENDER_MAIN, __MENU_SOCIAL_SELL);
		text = __STR(__BLUE(">>> 贡献兑换物品（每日三次） <<<"));
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, __STR(text), GOSSIP_SENDER_MAIN, __MENU_SOCIAL_BUY);

		player->SEND_GOSSIP_MENU(16301, _Creature->GetGUID());

		return true;
	}
	else if (action >= __MENU_SOCIAL_SELL && action < __MENU_SOCIAL_BUY)
	{
		if (action == __MENU_SOCIAL_SELL)
		{
			text = __STR(__BLUE("目前贡献值为==> "));
			text.append("|cffdd2222 " + std::to_string(_socialPoints) + " |r");
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_SOCIAL_MAIN);

			//Now Display all of the things can be sold.
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " === 系统行囊物品列表 === ", GOSSIP_SENDER_MAIN, __MENU_SOCIAL_MAIN);

			for (int i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_START + 10; ++i)
			{
				Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i);

				/*Weapon(pItem->GetProto()->Class == ITEM_CLASS_WEAPON )
				|| (pItem->GetProto()->Class == ITEM_CLASS_ARMOR) */
				if ((pItem) && pItem->GetProto()->Quality>1 && pItem->GetProto()->DisenchantID > 0)
				{
					auto item_destroy_local = sObjectMgr.GetItemLocale(pItem->GetProto()->ItemId);
					auto item_destroy_text = (item_destroy_local == nullptr ? pItem->GetProto()->Name1 : item_destroy_local->Name[localIdx]);

					text = "";
					text.append(item_destroy_text);
					text.append(__STR(" ==> 回收贡献： |cffee0000 "));
					text.append(std::to_string(__getPoints(pItem, _socialPoints)));

					text.append(" |r");

					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, __STR(text), GOSSIP_SENDER_MAIN, __MENU_SOCIAL_SELL + i);
				}
			}

			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());

			return true;
		}
		else
		{
			//Got the item to sell
			auto __slot = action - __MENU_SOCIAL_SELL;

			if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, __slot))
			{

				if ((pItem) && pItem->GetProto()->Quality>1 && pItem->GetProto()->DisenchantID > 0)
				{

					auto _addPoints = __getPoints(pItem, _socialPoints);
					auto _pItemID = pItem->GetProto()->ItemId;

					//remove the item
					player->DestroyItem(INVENTORY_SLOT_BAG_0, __slot, true);

					//add the rep points

					/*
					if (FactionEntry const* factionEntry = sObjectMgr.GetFactionEntry(967))
						player->GetReputationMgr().ModifyReputation(factionEntry, _addPoints);
					*/
					_socialPoints += _addPoints;
					sQZAchievements.SetSocialPoints(player, _socialPoints);

					sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "PLAYER:[%u][%s] === Destroy:%d BeforePoints:%d AddPoints:%d ", player->GetGUID(), player->GetName(), _pItemID, _socialPoints, _addPoints);

					text.append(__STR(" ==> 回收成功，贡献： |cffee0000 "));
					text.append(std::to_string(_addPoints));
					text.append(__STR(" |r 返回 <==="));

					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(text), GOSSIP_SENDER_MAIN, __MENU_SOCIAL_SELL);
				}
				else
				{
					text.append(__STR(" ==> 物品有误，返回 <==="));
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(text), GOSSIP_SENDER_MAIN, __MENU_SOCIAL_SELL);
				}
			}
			else
			{
				text.append(__STR(" ==> 物品有误，返回 <==="));
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(text), GOSSIP_SENDER_MAIN, __MENU_SOCIAL_SELL);
			}

			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
			return true;
		}

	}

	else if (action >= __MENU_SOCIAL_BUY)
	{
		//the listed items should be upon player's level, socialpoints etc..
		//1. first check if chances runs out
		auto chances = 3;

		while ((chances > 0) && (action == __MENU_SOCIAL_BUY))
		{
			auto pQuest = sObjectMgr.GetQuestTemplate(ZQ_QUEST_SOCIAL_IND - chances);
			if (player->CanTakeQuest(pQuest, false))
			{
				player->RewardQuest(pQuest, 0, player, false);
				break;
			}

			chances--;
		}

		if (chances == 0)
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR("==== 今日次数耗尽，明天再来 ==="), GOSSIP_SENDER_MAIN, __MENU_SOCIAL_SELL);
		else
		{

			auto __canSeeRange = 3;
			if (_socialPoints < 5000 || player->GetLevel() < 42) __canSeeRange = _ITEM_BUY_RANGE_1;
			else if (_socialPoints < 10000 || player->GetLevel() < 50) __canSeeRange = _ITEM_BUY_RANGE_2;
			else if (_socialPoints < 15000 || player->GetLevel() < 60) __canSeeRange = _ITEM_BUY_RANGE_3;
			else if (_socialPoints < 20000) __canSeeRange = _ITEM_BUY_RANGE_4;
			else if (_socialPoints < 30000)  __canSeeRange = _ITEM_BUY_RANGE_5;
			else __canSeeRange = _ITEM_BUY_RANGE_6;

			//add constrains for level 60
			//ZUG
			if (__canSeeRange >= _ITEM_BUY_RANGE_4) __canSeeRange = _ITEM_BUY_RANGE_3;

			//MC Range
			//if (__canSeeRange >= _ITEM_BUY_RANGE_5) __canSeeRange = _ITEM_BUY_RANGE_4;

			//BWL&TAQ Range
			//if (__canSeeRange >= _ITEM_BUY_RANGE_6) __canSeeRange = _ITEM_BUY_RANGE_5;

			//check the actions
			if (action == __MENU_SOCIAL_BUY)
			{
				text = __STR("==== 今日次数剩余: |cffee0000 ");
				text.append(" " + __NSTR(chances) + " |r ====");
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(text), GOSSIP_SENDER_MAIN, __MENU_SOCIAL_SELL);

				//list the available things
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_SOCIAL_MAIN);

				//define the different levels item, five levels...
				//stage 1 = lvl 25,  stage 2 = lvl 45, statge 3 = lvl 55, 4/5/6 orange 1/2/3
				if (player->GetLevel() < 20 || _socialPoints < 1000)
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" ===　玩家等级＞２０，称号见习才能使用 ==== "), GOSSIP_SENDER_MAIN, __MENU_SOCIAL_MAIN);
				}
				else
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" ======= 可购随机物品列表  ====== "), GOSSIP_SENDER_MAIN, __MENU_SOCIAL_MAIN);



					sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "Player:%s Item counts:%u, see range:%u", player->GetName(), ITEM_COUNTS, __canSeeRange);

					if (__canSeeRange >= ITEM_COUNTS) __canSeeRange = ITEM_COUNTS - 1;

					for (size_t i = 0; i < 4; i++)
					{
						auto _buy_item_seq = urand(0, __canSeeRange);
						auto _itemId = __item_list[_buy_item_seq];

						if (const ItemPrototype *pItem = sObjectMgr.GetItemPrototype(_itemId))
						{
							auto item_local = sObjectMgr.GetItemLocale(pItem->ItemId);
							auto item_text = (item_local == nullptr ? pItem->Name1 : item_local->Name[localIdx]);

							text = __STR("购买【【 ");
							text.append(__STR(item_text));
							text.append(__STR(" 】需要花费：|cffdd0000 "));
							text.append("" + __NSTR(__getSocialPointsBySeq(_buy_item_seq)) + " |r 点贡献|   ");

							player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_SOCIAL_BUY + 10 + _buy_item_seq);
						}
					}

					// end of send items that player can buy

				}

			}
			else
			{
				//Buy something
				int32 _buy_item_seq = action - __MENU_SOCIAL_BUY - 10;

				if (_buy_item_seq >= 0 && _buy_item_seq < __canSeeRange && _socialPoints > __getSocialPointsBySeq(_buy_item_seq))
				{
					//add the item and delete rep
					//delete the rep points
					_socialPoints -= __getSocialPointsBySeq(_buy_item_seq);
					sQZAchievements.SetSocialPoints(player, _socialPoints);
					player->AddItem(__item_list[_buy_item_seq]);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __BLUE(" ===> 已经购买成功，返回 <==== "), GOSSIP_SENDER_MAIN, __MENU_SOCIAL_BUY);

					/*
					if (FactionEntry const* factionEntry = sObjectMgr.GetFactionEntry(967))
					{
						player->GetReputationMgr().ModifyReputation(factionEntry, 0 - __getSocialPointsBySeq(_buy_item_seq));
						player->AddItem(__item_list[_buy_item_seq]);
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __BLUE(" ===> 已经购买成功，返回 <==== "), GOSSIP_SENDER_MAIN, __MENU_SOCIAL_BUY);
					}*/
				}
				else
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __RED(" ===> 物品选择有误，返回 <==== "), GOSSIP_SENDER_MAIN, __MENU_SOCIAL_BUY);
				}
			}

		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		return true;
	}

	return true;
}

#pragma endregion


#pragma region rep system
#define __MENU_REPU_MAIN		(__MENU_KELALA_REP)
#define __MENU_REPU_ACT_1		(10)
#define __ITEM_REPU				(ZQ_ITEM_FACTION_UPGRADE)
static uint32 __get_repu_by_player(Player *player, uint32 faction)
{
	const uint32 _repu_horde[] = {76,76, 889, 510, 729};
	const uint32 _repu_alliance[] = {72,72, 890, 509, 730};

	if (faction < 1000) return faction;
	else if(faction < 1005)
	{
		//get faction by player
		if (player->GetTeam() == HORDE)
		{
			return _repu_horde[faction-1000];
		}
		else if (player->GetTeam() == ALLIANCE)
		{
			return _repu_alliance[faction - 1000];
		}
	}

	return faction;
}
bool Menus_Kelala_Rep(Player *player, Creature *_Creature, uint32 action)
{
	if (!player || !_Creature) return false;

	//get the abs action
	auto abs_action = action - __MENU_REPU_MAIN;
	std::string text = "";

	//Main Menu
	if(abs_action == 0)
	{
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝兑换声望列表＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝需要消耗【声望碎片】＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
		//get the repu list -- _Rep_List
		//_Rep_List is const Rep_List_t _Rep_List[] 
		for (size_t i = 0; i < sizeof(_Rep_List) / sizeof(_Rep_List[0]); i++)
		{
			auto _faction = _Rep_List[i];
			//check if the player has the reputation
			if (player->GetLevel() >= _faction.rep_level)
			{
				text = __STR("声望::");
				text.append(_faction.rep_name);
				text.append("当前::");
				auto __real_rep_id = __get_repu_by_player(player, _faction.rep_id);
				text.append(__NSTR(player->GetReputationMgr().GetReputation(__real_rep_id)));
				if(player->HasItemCount(__ITEM_REPU, 1)) 
				{
					text.append("|cff002fa7 》点击提升《 |r");
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_REPU_MAIN + __MENU_REPU_ACT_1 + i);
				}
				else 
				{
					text.append(" 》碎片不够《 |r");
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);
				}
			}
		}

		//player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
	}
	else if(abs_action >= __MENU_REPU_ACT_1)
	{
		abs_action -= __MENU_REPU_ACT_1;

		//check if the player has the repu shards
		if (player->HasItemCount(__ITEM_REPU, 1))
		{
			//remove the repu shards and add the reputation
			player->DestroyItemCount(__ITEM_REPU, 1, true);

			auto __real_rep_id = __get_repu_by_player(player, _Rep_List[abs_action].rep_id);

			if (FactionEntry const* factionEntry = sObjectMgr.GetFactionEntry(__real_rep_id))
				player->GetReputationMgr().ModifyReputation(factionEntry, 100);


			//check more of the rep if CITY
			if (_Rep_List[abs_action].rep_id == 1001)
			{
				if (player->GetTeam() == HORDE)
				{
					//remained 3 cities
					player->GetReputationMgr().ModifyReputation(sObjectMgr.GetFactionEntry(68), 100);
					player->GetReputationMgr().ModifyReputation(sObjectMgr.GetFactionEntry(81), 100);
					player->GetReputationMgr().ModifyReputation(sObjectMgr.GetFactionEntry(530), 100);
				} 
				else if (player->GetTeam() == ALLIANCE)
				{
					player->GetReputationMgr().ModifyReputation(sObjectMgr.GetFactionEntry(47), 100);
					player->GetReputationMgr().ModifyReputation(sObjectMgr.GetFactionEntry(54), 100);
					player->GetReputationMgr().ModifyReputation(sObjectMgr.GetFactionEntry(69), 100);
				}

			}

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝提升成功，返回。＝＝＝ ")), GOSSIP_SENDER_MAIN, __MENU_REPU_MAIN);
		}
		else
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＝＝碎片不够，返回。＝＝＝＝ ")), GOSSIP_SENDER_MAIN, __MENU_REPU_MAIN);
		}
	}

	//gossip 16032 for repuation description
	player->SEND_GOSSIP_MENU(16032, _Creature->GetGUID());
	return true;
}
#pragma endregion


#pragma region Equipment Collects



bool Menus_Kelala_EquipCollects(Player *player, Creature *_Creature, uint32 action)
{
	if (!player ||!_Creature) return false;
	
	if (action == __MENU_KELALA_EQUIP_COLLECTS)
	{
		uint32 _EQBonus = sQZAchievements.GetEQCollectBonus(player);
		uint32 __custom_settings = sQZAchievements.GetCustomSettings(player);
		uint32 __apsp_method = __custom_settings & 0x03;
		uint32 _apBonus = _EQBonus;
		uint32 _spBonus = _EQBonus/2;
		if(__apsp_method == 1)  { _apBonus = _apBonus + _spBonus; _spBonus = 0; }
		else if(__apsp_method == 2) { _spBonus = _spBonus + _apBonus/2; _apBonus = 0; }


		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　野外装备收集　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_KELALA_EQUIP_COLLECTS_WORLD);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　副本装备收集　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_KELALA_EQUIP_COLLECTS_DUNGEON);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　团本装备收集　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_KELALA_EQUIP_COLLECTS_RAID);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　专业制造收益　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_KELALA_EQUIP_COLLECTS_PROFESSIONAL);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
		



		std::string text = "";
		text.append(__STR("|cff0000ff＝＞　当前攻强加成：　"));
		text.append(__NSTR(_apBonus));
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_KELALA_MAIN);
		text = "";
		text.append(__STR("|cff0000ff＝＞　当前法伤加成：　"));
		text.append(__NSTR(_spBonus));
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_KELALA_MAIN);

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＞　返回　＜＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_KELALA_MAIN);

		player->SEND_GOSSIP_MENU(ZQ_GOSSIP_KELALA_EQ_COLLECT, _Creature->GetGUID());
		return true;	
	}

	else if (action >= __MENU_KELALA_EQUIP_COLLECTS_WORLD && action < __MENU_KELALA_EQUIP_COLLECTS_DUNGEON)
	{

		auto _absAction = action - __MENU_KELALA_EQUIP_COLLECTS_WORLD;

		//world equip collects
		//get the player achievement info and check if the player has the achievement
		if (_absAction == 0)
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝　点击各阶段查看更多　＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);	
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　阶段１　＜＝＝　")), GOSSIP_SENDER_MAIN, action + 100);	
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
			if(player->GetLevel() >= 15) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　阶段２　＜＝＝　")), GOSSIP_SENDER_MAIN, action + 200);	
			if(player->GetLevel() >= 15) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
			if(player->GetLevel() >= 25) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　阶段３　＜＝＝　")), GOSSIP_SENDER_MAIN, action + 300);	
			if(player->GetLevel() >= 25) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
			if(player->GetLevel() >= 35) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　阶段４　＜＝＝　")), GOSSIP_SENDER_MAIN, action + 400);	
			if(player->GetLevel() >= 35) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
			if(player->GetLevel() >= 45) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　阶段５　＜＝＝　")), GOSSIP_SENDER_MAIN, action + 500);	
			if(player->GetLevel() >= 45) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
			if(player->GetLevel() >= 55) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　阶段６　＜＝＝　")), GOSSIP_SENDER_MAIN, action + 600);
			if(player->GetLevel() >= 55) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝　返回主页　＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_KELALA_EQUIP_COLLECTS);	
		}
		else if(_absAction >= 100 && _absAction < 700)
		{
			//get the world level info
			uint32_t _worldlevel = _absAction / 100 ;
			auto _eq_entry = DBHelper_GetEQByWorldLevel(_worldlevel - 1); // pls note world level is 1 based, so we need to -1
			//get player's achievement info and check if the player has the achievement
			uint32_t _achievement = sQZAchievements.GetEquipCollectCommon(player, ACHIEVEMENTS_COLLECTIONS_WORLD, _worldlevel);

			if (_absAction % 100 == 0)
			{
				//get the stage info by _eqList
				std::string text = "";
				text.append(__STR("|cff0000ff＝收集装备增加 "));
				text.append(__NSTR(_worldlevel * 20));
				text.append("攻强");
				text.append(__NSTR(_worldlevel * 10));
				text.append("法伤＝ |r");
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);
				for (size_t i = 0; i < 12; i++)
				{
					if (_eq_entry->eq_list[i] > 0)
					{
						auto _itemId = _eq_entry->eq_list[i];
						if (const ItemPrototype *pItem = sObjectMgr.GetItemPrototype(_itemId))	
						{
							auto item_local = sObjectMgr.GetItemLocale(pItem->ItemId);
							auto item_text = (item_local == nullptr? pItem->Name1 : item_local->Name[LOCALE_deDE]);

							text = "|cff0000ff(";
							text.append(__NSTR(i+1));
							text.append(")　");
							text.append(item_text);

							//check if the player has the item
							if(_achievement & (1 << i))
							{
								text.append(__STR(" |r |cff00bb00＜已完成＞　|r "));
								player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);
							}
							else
							{
								text.append(__STR(" |r |cffbb0000＜未完成＞　|r "));
								player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(text), GOSSIP_SENDER_MAIN, action + i + 1);
							}
						}
					}
				}

				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("＝＝＝＝＝　返回主页　＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_KELALA_EQUIP_COLLECTS);	
			}

			else
			{
				//get the item info by _eqList
				auto _equipID = _eq_entry->eq_list[_absAction % 100 - 1];
				auto pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START);

				if( pItem && pItem->GetProto()->ItemId == _equipID ) 	
				{
					//take the item and add the achievement
					player->DestroyItem(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START, true);

					//set the achievement info
					_achievement |= (1 << (_absAction % 100 - 1));
					sQZAchievements.SetEquipCollectCommon(player, ACHIEVEMENTS_COLLECTIONS_WORLD, _worldlevel, _achievement);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　已获取此装备增益　＜＝＝＝＝　")), GOSSIP_SENDER_MAIN, action / 100 * 100);
				}
				else
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　请将装备放在行囊第一个格子　＜＝＝＝＝　")), GOSSIP_SENDER_MAIN, action / 100 * 100);
				}
			}
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
	}

	else if (action >= __MENU_KELALA_EQUIP_COLLECTS_DUNGEON && action < __MENU_KELALA_EQUIP_COLLECTS_RAID)
	{

		uint32_t _absAction = action - __MENU_KELALA_EQUIP_COLLECTS_DUNGEON;
		std::string text = "";

		//get the player achievement info and check if the player has the achievement
		//the action should be 0 to 2999 about
		//map id is multplied by 100, offset 1(which ragefire means 1)
		//so, mapid should be 1 to 18, and action is 100 to 1800
		//WE Reuse the TP_Dungeons for dislay info
		//we need two pages to display the dungeon info

		if(_absAction <= 1)
		{
			//sizeof(TP_Dungeons) / sizeof(TP_Dungeons[0] = 18
			uint32_t _startPos = _absAction * 9;
			uint32_t _endPos = _startPos + 9 ;

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝点击副本查看详细内容＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);

			for (size_t i = _startPos; i < _endPos; i++)
			{
				if (i >= sizeof(TP_Dungeons) / sizeof(TP_Dungeons[0])) break;

				auto _dungeon = TP_Dungeons[i];	

				//make up the text
				text = __STR("|cff0000ff＝＞　");
				text.append(_dungeon.name);
				//get the player achievement info and check if the player has the achievement
				uint32_t _achive_info = sQZAchievements.GetEquipCollectCommon(player, 
					(_absAction == 0)? ACHIEVEMENTS_COLLECTIONS_DUNGEONS_1:ACHIEVEMENTS_COLLECTIONS_DUNGEONS_2, 
					i - _startPos);
				if(_achive_info == 0xFFFFFF) text.append(__STR("|r ｜　|cff00bb00＜已完成＞　|r "));	
				else text.append(__STR("|r ｜　|cffbb0000＜未完成＞　|r "));	
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_KELALA_EQUIP_COLLECTS_DUNGEON + (i+1) * 100);	
			}

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);

			//add the next page button
			if(_absAction == 0)
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("＝＝＝＞　下一页　＜＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_KELALA_EQUIP_COLLECTS_DUNGEON + 1);
			else //upper page
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("＝＝＝＞　上一页　＜＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_KELALA_EQUIP_COLLECTS_DUNGEON);
		}

		else if(_absAction >= 100 && _absAction % 100 == 0)		//main menus of the map
		{
			std::string text = "";
			//the detail info of the dungeon
			//get the mapid
			//the action should be 100 to 1800
			//the mapid should be 1 to 18
			//so, the mapid is action / 100
			auto _dungeon = TP_Dungeons[(_absAction - 100) / 100];

			//display the detail equip info of the dungeon
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝点击装备查看更多＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);

			text = __STR("＝＝＞　|cff0000ff 地图：： ");
			text.append(_dungeon.name);
			text.append(__STR("|r　　"));
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);

			//achivement info
			uint32_t _achive_info = sQZAchievements.GetEquipCollectCommon(player, 
				_dungeon.id < 9 ?  ACHIEVEMENTS_COLLECTIONS_DUNGEONS_1:ACHIEVEMENTS_COLLECTIONS_DUNGEONS_2, 
				_dungeon.id % 9);

			auto _eqList = DBHelper_GetEQByDungeonID(_dungeon.id);
			if (_eqList == nullptr) return false;

			for (size_t i = 0; i < 6; i++)
			{
				auto item_1_local = sObjectMgr.GetItemLocale(_eqList->eq_list[i]);
				auto item_1_text = (item_1_local == nullptr ? __STR("未知装备 ") : item_1_local->Name[LOCALE_deDE]);

				text = __STR(("|cff0000ff＝＝＞　"));
				text.append(__STR(item_1_text));
				//text.append(j==0?__STR("（普通）　 "):j==1?__STR("（试炼）　 "):j==2?__STR("（地狱）　 "):__STR("（梦魇）　 "));
				if( (_achive_info & (3 << i)) == (3<<i)) text.append(__STR("|r　|cff00bb00＜已完成＞|r "));	
				else text.append(__STR("|r　|cffbb0000＜未完成＞|r "));
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(text), GOSSIP_SENDER_MAIN, __MENU_KELALA_EQUIP_COLLECTS_DUNGEON +  _absAction + (i+1)*10);
			}

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　返回　＜＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_KELALA_EQUIP_COLLECTS_DUNGEON);
		}

		else if(_absAction >= 101 && _absAction % 10 == 0)		//equip info of the dungeon
		{
			//the detail info of the dungeon
			auto _dungeon = TP_Dungeons[(_absAction - 100) / 100];
			auto _eqList = DBHelper_GetEQByDungeonID(_dungeon.id);
			if (_eqList == nullptr) return false;
			auto _equipID = _eqList->eq_list[(_absAction % 100) / 10 - 1];
			//achivement info
			uint32_t _achive_info = ( (sQZAchievements.GetEquipCollectCommon(player, 
				_dungeon.id < 9 ?  ACHIEVEMENTS_COLLECTIONS_DUNGEONS_1:ACHIEVEMENTS_COLLECTIONS_DUNGEONS_2, 
				_dungeon.id % 9)) >> (((_absAction % 100) / 10 - 1) * 4 ) ) & 0x0F;

			//display the detail equip info of the dungeon
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝将装备放在行囊第一个格子＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);

			text = __STR("|cff0000ff＝＝＞　装备：： ");
			auto item_1_local = sObjectMgr.GetItemLocale(_equipID);
			auto item_1_text = (item_1_local == nullptr? __STR("未知装备 ") : item_1_local->Name[LOCALE_deDE]);
			text.append(__STR(item_1_text));
			text.append(__STR("|r　　"));
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);

			//display the detail equip info of the dungeon
			for (size_t i = 0; i < 4; i++)
			{
				text = __STR("|cff0000ff＝＝＞　难度： ");
				text.append(i==0?__STR("（普通）　"):i==1?__STR("（试炼）　"):i==2?__STR("（地狱）　"):__STR("（梦魇）　"));
				if( (_achive_info & (1 << i)) == (1<<i)) text.append(__STR("|r　|cff00bb00＜已完成＞ |r"));
				else text.append(__STR("|r |cffbb0000＜未完成＞ |r"));
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(text), GOSSIP_SENDER_MAIN, __MENU_KELALA_EQUIP_COLLECTS_DUNGEON + _absAction + i + 1);
			}
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　返回　＜＝＝＝＝　")), GOSSIP_SENDER_MAIN, action / 100 * 100);
		}
		else if(_absAction % 10 > 0)		//equip info of the dungeon
		{
			auto _dungeon = TP_Dungeons[(_absAction - 100) / 100];
			auto _eqList = DBHelper_GetEQByDungeonID(_dungeon.id);
			if (_eqList == nullptr) return false;
			auto _equipID = _eqList->eq_list[(_absAction % 100) / 10 - 1];
			auto _difficulty = (_absAction % 10) - 1;
			//achivement info
			uint32_t _achive_dg_info = sQZAchievements.GetEquipCollectCommon(player, 
										_dungeon.id < 9 ?  ACHIEVEMENTS_COLLECTIONS_DUNGEONS_1:ACHIEVEMENTS_COLLECTIONS_DUNGEONS_2, 
										_dungeon.id % 9);
			uint32_t _achive_eq_info = (_achive_dg_info >> ((_absAction % 100) / 10 - 1) * 4 ) & 0x0F;

			if( (_achive_eq_info & (1 << _difficulty)) == (1<<_difficulty))
			{
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　已完成　＜＝＝＝＝　")), GOSSIP_SENDER_MAIN, action / 10 * 10);
			}
			else
			{
				//check the player if has this in bag slot 1
				
				auto pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START);

				//high level can be replaced by lower levels
				if( (pItem && pItem->GetProto()->ItemId == _equipID) && ( (_difficulty==0)||((pItem->GetItemRandomPropertyId()  >3300 + _difficulty * 5) && (pItem->GetItemRandomPropertyId() <3321) ) ))	
				{
					//take the item and add the achievement
					player->DestroyItem(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START, true);

					//set the achievement info
					_achive_dg_info |= ( (1 << _difficulty) << ((_absAction % 100) / 10 - 1) * 4 );  
					sQZAchievements.SetEquipCollectCommon(player, 
						_dungeon.id < 9 ?  ACHIEVEMENTS_COLLECTIONS_DUNGEONS_1:ACHIEVEMENTS_COLLECTIONS_DUNGEONS_2, 
						_dungeon.id % 9, 
						_achive_dg_info);

					
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　已获取此装备增益　＜＝＝＝＝　")), GOSSIP_SENDER_MAIN, action / 10 * 10);
				}
				else
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　请将装备放在行囊第一个格子　＜＝＝＝＝　")), GOSSIP_SENDER_MAIN, action / 10 * 10);
				}

			}
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		return true;
	}

	else if (action >= __MENU_KELALA_EQUIP_COLLECTS_RAID && action < __MENU_KELALA_EQUIP_COLLECTS_PROFESSIONAL)
	{
		std::string text = "";
		//raid equip collects
		//player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝尚未开放＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);	
		auto abs_action = action - __MENU_KELALA_EQUIP_COLLECTS_RAID;
		if(abs_action == 0)
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝团本所需均为【梦魇】＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);

			for (size_t i = 0; i < sizeof(TP_Raids) / sizeof(TP_Raids[0]); i++)
			{
				auto _raid = TP_Raids[i];	

				//make up the text
				text = __STR("|cff0000ff＝＞　");
				text.append(_raid.name);
				//get the player achievement info and check if the player has the achievement
				uint32_t _achive_info = sQZAchievements.GetEquipCollectCommon(player,  ACHIEVEMENTS_COLLECTIONS_RAID, i);
				if(_achive_info >= 0xFFF) text.append(__STR("|r ｜　|cff00bb00＜已完成＞　|r "));	
				else text.append(__STR("|r ｜　|cffbb0000＜未完成＞　|r "));	
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_KELALA_EQUIP_COLLECTS_RAID + (i+1) * 100);	
			}
		}
		else if (abs_action >100)
		{
			if(abs_action % 100 == 0)
			{
				//display the raid equip info of the raid
				auto _raid = TP_Raids[(abs_action / 100) - 1];

				text = __STR("＝＝＞　|cff0000ff 地图：： ");
				text.append(_raid.name);
				text.append(__STR("|r　　"));
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);

				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　　尚未开放　　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
			}
		}


		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		return true;
	}

	else if (action >= __MENU_KELALA_EQUIP_COLLECTS_PROFESSIONAL && action < __MENU_KELALA_EQUIP_COLLECTS_END)
	{
		//professional equip collects
		//get abs action
		auto abs_action = action - __MENU_KELALA_EQUIP_COLLECTS_PROFESSIONAL;
		if(abs_action  == 0)
		{
			//display the professional equip info of the dungeon
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝　点击专业查看更多　＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　　炼　金　　＜＝＝　")), GOSSIP_SENDER_MAIN, action + 100);	
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　　附　魔　　＜＝＝　")), GOSSIP_SENDER_MAIN, action + 200);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　　裁　缝　　＜＝＝　")), GOSSIP_SENDER_MAIN, action + 300);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　　制　皮　　＜＝＝　")), GOSSIP_SENDER_MAIN, action + 400);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　　锻　造　　＜＝＝　")), GOSSIP_SENDER_MAIN, action + 500);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　　工　程　　＜＝＝　")), GOSSIP_SENDER_MAIN, action + 600);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝　返回主页　＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_KELALA_EQUIP_COLLECTS);	

			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		}

		//similar with world menus
		else if(abs_action >= 100 && abs_action < 700)
		{
			//get the world level info
			uint32_t _profession = abs_action / 100 ;	
			auto _eq_entry = DBHelper_GetEQByProfession(_profession - 1); // pls note world level is 1 based, so we need to -1
			//get player's achievement info and check if the player has the achievement
			uint32_t _achievement = sQZAchievements.GetEquipCollectCommon(player, ACHIEVEMENTS_COLLECTIONS_PROFESSION, _profession);

			if (abs_action % 100 == 0)
			{
				//get the stage info by _eqList
				std::string text = "";
				text.append(__STR("|cff0000ff＝收集制作品增加 "));
				text.append(__NSTR(50));
				text.append("攻强");
				text.append(__NSTR(25));
				text.append("法伤＝ |r");
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);
				for (size_t i = 0; i < 6; i++)
				{
					if (_eq_entry->eq_list[i] > 0)
					{
						auto _itemId = _eq_entry->eq_list[i];
						if (const ItemPrototype *pItem = sObjectMgr.GetItemPrototype(_itemId))
						{
							auto item_local = sObjectMgr.GetItemLocale(pItem->ItemId);
							auto item_text = (item_local == nullptr? pItem->Name1 : item_local->Name[LOCALE_deDE]);

							text = "|cff0000ff(";
							text.append(__NSTR(i+1));
							text.append(")　");
							text.append(item_text);

							//check if the player has the item
							if(_achievement & (1 << i))
							{
								text.append(__STR(" |r |cff00bb00＜已完成＞　|r "));
								player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);	
							}	
							else
							{
								text.append(__STR(" |r |cffbb0000＜未完成＞　|r "));
								player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(text), GOSSIP_SENDER_MAIN, action + i + 1);
							}
						}

					}

				}

				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝　返回　＝＝＝＝　")), GOSSIP_SENDER_MAIN, action/100 * 100);

			}
			else
			{
				//get the item info by _eqList
				auto _equipID = _eq_entry->eq_list[abs_action % 100 - 1];
				auto pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START);

				if( pItem && pItem->GetProto()->ItemId == _equipID )
				{
					//take the item and add the achievement
					player->DestroyItem(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START, true);

					//set the achievement info
					_achievement |= (1 << (abs_action % 100 - 1));
					sQZAchievements.SetEquipCollectCommon(player, ACHIEVEMENTS_COLLECTIONS_PROFESSION, _profession, _achievement);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　已获取此装备增益　＜＝＝＝＝　")), GOSSIP_SENDER_MAIN, action / 100 * 100);	
				}	
				else
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　请将装备放在行囊第一个格子　＜＝＝＝＝　")), GOSSIP_SENDER_MAIN, action / 100 * 100);
				}

				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝　返回　＝＝＝＝　")), GOSSIP_SENDER_MAIN, action/100 * 100);
			}
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
	}



	return true;
}

#pragma endregion

bool Menus_Kelala_Main(Player *player, Creature *_cr, uint32 sender, uint32 action)
{
	//check if player is null and go is null
	if (!player || !_cr) return false;

	// Main menu
	// Several Features:
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
	
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　登陆奖励　＜＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_KELALA_LOGIN);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
	
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　点券商城　＜＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_KELALA_SHOP);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);

	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　任务系统　＜＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_KELALA_TASK);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);

	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　装备收集　＜＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_KELALA_EQUIP_COLLECTS);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);

	//player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　社区贡献　＜＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_KELALA_SOCIAL);
	//player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);

	if (player->GetLevel() >= 25)
	{
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　挑战模式　＜＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_KELALA_MODE);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
	}

	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　声望奖励　＜＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_KELALA_REP);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);




	//player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＞　衬衣战袍背包升级　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_KELALA_SUIT);
	//player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->SEND_GOSSIP_MENU(9602, _cr->GetGUID());
	return true;
}

//newbie login menu, give player some items and rewards
#define __MENU_KELALA_NEWBIE_QUESTID 	(ZQ_QUEST_NEWBIE)
bool Menus_Kelala_Newbie(Player *player, Creature *_cr, uint32 sender, uint32 action)
{
	//check if player is null and go is null
	if (!player ||!_cr) return false;

	if (player->GetQuestStatus(__MENU_KELALA_NEWBIE_QUESTID) == QUEST_STATUS_NONE) return false;

	//check the action if __MENU_KELALA_NEWBIE
	if (action == __MENU_KELALA_NEWBIE)
	{
		//reward the player with some items and rewards
		Quest const* pQuest = sObjectMgr.GetQuestTemplate(__MENU_KELALA_NEWBIE_QUESTID);
		player->RewardQuest(pQuest, 0, player, false);

		//add item
		player->AddItem(30000, 200);

		//Cast the first spell
		player->CastSpell(player, ZQ_SPELL_BUFF_DRAGON_SLAYER, true);

		//tell the player that he has received the reward
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＞ 领取新人奖励完毕！　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_KELALA_MAIN);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _cr->GetGUID());
		return true;
	}
	
	//check if has the newbie's quest
	auto _qData = player->GetQuestStatusData(__MENU_KELALA_NEWBIE_QUESTID);
	if (_qData && _qData->m_rewarded == false)
	{
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＞　欢迎！领取新人奖励　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_KELALA_NEWBIE);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);

		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _cr->GetGUID());
		return true;
	}

	return false;
}

//define a wrapper function for the equip system menus
bool Kelala_Menus(Player *player, Creature *_cr, uint32 sender, uint32 action)
{
	//check if player is null and go is null
	if (!player || !_cr) return false;

	//sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "[Kelala Menus] PLAYER:[%u][%s] === action: %d", player->GetGUID(), player->GetName(), action);

	//just display the new bie
	if(Menus_Kelala_Newbie(player, _cr, sender, action)) return true;

	// Main menu
	if (action >= __MENU_KELALA_MAIN && action <= __MENU_KELALA_MAIN + __MENU_SIZE)
	{
		return Menus_Kelala_Main(player, _cr, sender, action);
	}
	// Login menu
	else if (action >= __MENU_KELALA_LOGIN && action <= __MENU_KELALA_LOGIN + __MENU_SIZE)
	{
		return Menus_Kelala_Login(player, _cr, sender, action);
	}
	// Task menu
	else if (action >= __MENU_KELALA_TASK && action <= __MENU_KELALA_TASK + __MENU_SIZE)
	{
		return Menus_Kelala_Task(player, _cr, sender, action);
	}
	// Mode menu
	else if (action >= __MENU_KELALA_MODE && action <= __MENU_KELALA_MODE + __MENU_SIZE)
	{
		return Menus_Kelala_Mode(player, _cr, sender, action);	
	}
	// Shop menu
	else if (action >= __MENU_KELALA_SHOP && action <= __MENU_KELALA_SHOP + __MENU_SIZE)
	{
		return Menus_Kelala_Shop(player, _cr, sender, action);
	}
	// Social menu
	else if (action >= __MENU_KELALA_SOCIAL && action <= __MENU_KELALA_SOCIAL + __MENU_SIZE)
	{
		return Menus_Kelala_Social(player, _cr, action);	
	}
	// Rep menu
	else if (action >= __MENU_KELALA_REP && action <= __MENU_KELALA_REP + __MENU_SIZE)
	{
		return Menus_Kelala_Rep(player, _cr, action);
	}

	//Collects menu
	else if (action >= __MENU_KELALA_EQUIP_COLLECTS && action <= __MENU_KELALA_EQUIP_COLLECTS_END)
	{
		return Menus_Kelala_EquipCollects(player, _cr, action);
	}


	return false;
}



void AddSC_qzqstar_kelala()
{
	Script* newscript;

	newscript = new Script;
	newscript->Name = "qzqstar_kelala";
	newscript->pGossipHello = [](Player *p, Creature *c) -> bool { return Kelala_Menus(p, c, 0, __MENU_KELALA_MAIN); };
	newscript->pGossipSelect = &Kelala_Menus;
	newscript->RegisterSelf(false);
}


