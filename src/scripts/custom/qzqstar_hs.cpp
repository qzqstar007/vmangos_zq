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

#include "Group.h"

#include "qzqstar_hs.h"
#include "QzqstarAchievements.h"
#include "qzqstar_id.h"
#include "qzqstar_rune_str.h"
#include "qzqstar_helper.h"
#include "qzqstar_custom.h"
#include "qzqstar_db.h"

#define	__MENU_NONE						0
#define	__MENU_MAIN						1
#define __MENU_SIZE						999
#define __MENU_MODE_MAIN				1000
#define __MENU_CITIES_MAIN				2000
#define __MENU_FRAG_MAIN				3000  //not used in S8
#define __MENU_ACHIEVE_MAIN				3000
#define __MENU_RUNE_MAIN				4000
#define __MENU_SUISHEN_MAIN				5000
#define __MENU_SKILL_STOLE				6000
#define __MENU_ZITIAO_MAIN				7000
#define __MENU_VIP_TELEPORT_MAIN		10000
#define __MENU_END						20000

#define	__STR(x)		((std::string)(x)).c_str()
#define	__NSTR(x)		(std::to_string(x))
	
#define	__BLUE(x)		"|cff002fa7"##x##"|r"
#define	__GREEN(x)		"|cff00b72f"##x##"|r"
#define	__ORANGE(x)		"|cffe85827"##x##"|r"
#define __RED(x)		"|cfff00019"##x##"|r"
#define __YELLOW(x)		"|cfff9dc24"##x##"|r"


void _Main_Menus(Player *player)
{
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);

	if (player->GetLevel() == 1)
	{
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("☆☆　挑战模式（一级可选）☆☆　")), GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
	}
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　主城传送　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);

	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　随身功能　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);

	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　成就系统　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);


	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　符文系统　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);

	if(false)
	{
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　成就提升　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　字条法宝　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_ZITIAO_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　技能盗窃　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SKILL_STOLE);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
	}
	
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　副本内飞　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_VIP_TELEPORT_MAIN);


	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, player->GetGUID());
}



#pragma region MENU RUNE

#define __MENU_RUNE_SLOT_LEARN		(100)	//1100-1199 learn slot spell
#define __MENU_RUNE_SLOT_FORGET		(200)	//2100-2199 forget slot spell
#define __MENU_RUNE_SLOT_BUY		(800)	//buy rune slot

#define	__RUNE_UPGRADE_ITEM_ALL			(ZQ_ITEM_RUNE_STONE)	//ALL

void _copy_rune(const Rune_Spell_Menu_t *from, Rune_Spell_Menu_t *to, uint32 size)
{
	/*	uint32			flags;
	uint32			spell_id;
	std::string		text; 
	std::string		desc; */
	//memcpy(to, from, sizeof(Rune_Spell_Menu_t)*size);
	for (uint32 i = 0; i < size; i++)
	{
		to[i].flags = 0;
		to[i].spell_id = from[i].spell_id;
		to[i].text = from[i].text;
		to[i].desc = from[i].desc;
	}
}

uint32 __rune_slot_numbers(Player *player)
{
	//will have one slot every level/15
	uint32 _nums = sQZAchievements.GetRuneSlots(player);

	//Maybe bonus later
	return _nums;
}

uint32 __rune_need_num(Player *player)
{
	auto needNum = 0;
	auto freeNum = 2;
	
	if (player->GetLevel()==60 && ( (player->M_Challenge_Mode & CHALLENGING_MODE_DONE_MANUFACT) == CHALLENGING_MODE_DONE_MANUFACT)) freeNum += 1;

	int32 curslots = __rune_slot_numbers(player);

	if (curslots < freeNum) needNum = 0;
	else
	{
		needNum = 10 * (curslots - freeNum + 1) * (curslots - freeNum + 1);
	}
	return needNum;
}

#pragma region MENU RUNE MAIN
bool Menus_Rune_Main(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
	if (!player || !_Creature) return false;

	uint32 _MAX_RUNE_SLOT = 0;
	uint32 __RUNE_UPGRADE_ITEM = 0;
	uint32 __RUNE_UPGRADE_ITEM_BASIC = 10;
	uint32 _CLASS_DESC = DEFAULT_GOSSIP_MESSAGE;

	//All player enter this menu, and then got the class menu belongs to him
	Rune_Spell_Menu_t _LocalMenus[RUNE_SPELLS_NUM];

	if (player->GetClass() == CLASS_WARRIOR) { _copy_rune(_Spells_Menu_Warrior, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16030; }
	else if (player->GetClass() == CLASS_MAGE) { _copy_rune(_Spells_Menu_Mage, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16030; }
	else if (player->GetClass() == CLASS_ROGUE) { _copy_rune(_Spells_Menu_Rogue, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16030; }
	else if (player->GetClass() == CLASS_PALADIN) { _copy_rune(_Spells_Menu_Paladin, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16030; }
	else if (player->GetClass() == CLASS_DRUID) { _copy_rune(_Spells_Menu_Druid, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16030; }
	else if (player->GetClass() == CLASS_HUNTER) { _copy_rune(_Spells_Menu_Hunter, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16030; }
	else if (player->GetClass() == CLASS_PRIEST) { _copy_rune(_Spells_Menu_Priest, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16030; }
	else if (player->GetClass() == CLASS_WARLOCK) { _copy_rune(_Spells_Menu_Warlock, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16030; }
	else if (player->GetClass() == CLASS_SHAMAN) { _copy_rune(_Spells_Menu_Shaman, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16030; }
	else
	{
		player->ADD_GOSSIP_ITEM(5, __STR("========|该职业暂时不支持...|========="), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->SEND_GOSSIP_MENU(_CLASS_DESC, _Creature->GetGUID());
		return true;
	}

	// Usually, _player_active_menu_num should be less than the Level/15, but for some reason
	// such as the reborn.. so set the max to be numbers that player holding.
	uint32 _player_learned_num = 0;

	//check if player has the spell, and set the flags
	for (size_t i = 0; i < RUNE_SPELLS_NUM; i++)
	{
		if (player->HasSpell(_LocalMenus[i].spell_id))
		{
			_LocalMenus[i].flags = 0x01;
			_player_learned_num ++;
		}
	}

	//set to be max of level or active nums
	//_MAX_RUNE_SLOT = player->GetLevel() / 15;
	_MAX_RUNE_SLOT = __rune_slot_numbers(player);

	bool _can_learn_more = (_player_learned_num < _MAX_RUNE_SLOT);

	std::string text = "";
	
	if (action == __MENU_RUNE_MAIN || action == __MENU_RUNE_MAIN + 1)
	{
		//player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("==========|符文槽状态|=========")), GOSSIP_SENDER_MAIN, __MENU_NONE);
		text = __STR(__BLUE("=== 符文槽数量： "));
		text.append(" |cffdd0000 " + __NSTR(_player_learned_num) + " / "+ __NSTR(_MAX_RUNE_SLOT) + "  |r ");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);
		text = __STR(__BLUE("===>> 点击开启额外符文槽 <<===== "));
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(text), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN + __MENU_RUNE_SLOT_BUY);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("==========|可用符文列表|========")), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN);

		//add to the gossip item, split to two pages.
		#define RUNE_SLOT_PAGE_ONE		8
		auto _start_id = 0;
		auto _end_id = RUNE_SLOT_PAGE_ONE;
		if(action == __MENU_RUNE_MAIN) 
		{
			_start_id = 0;
			_end_id = RUNE_SLOT_PAGE_ONE; //not include the RUNE_SLOT_PAGE_ONE
		}
		else if (action == __MENU_RUNE_MAIN + 1)
		{
			_start_id = RUNE_SLOT_PAGE_ONE;
			_end_id = RUNE_SPELLS_NUM;
		}

		for (size_t i = _start_id; i < _end_id; i++)
		{
			text = __BLUE("[符文槽]-");
			text.append(__NSTR(i+1));	
			text.append(":");
			text.append(__STR(_LocalMenus[i].text));
			//text.append("-");
			//text.append(__STR(_LocalMenus[i].desc));
			text.append(":");
			if (_LocalMenus[i].flags)
			{
				//alreay learned
				text.append(__GREEN("［已启－点击遗忘］ "));
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN + __MENU_RUNE_SLOT_FORGET + i);
			}
			else
			{
				text.append(__BLUE("［未启－点击学习］ "));
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN + __MENU_RUNE_SLOT_LEARN + i);
			}
		}

		//player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN);
		//add the next page button
		if (action == __MENU_RUNE_MAIN)
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("＝＝＝＝＝＝丨下一页｜＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN + 1);	
		}else
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("＝＝＝＝＝＝丨上一页｜＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN);	
		}
	}

	#define RUNE_CONFIRM_OFFSET		50
	//1120-1129, learn spell
	else if ( (action >= __MENU_RUNE_MAIN + __MENU_RUNE_SLOT_LEARN) && ( action < __MENU_RUNE_MAIN + __MENU_RUNE_SLOT_LEARN + RUNE_CONFIRM_OFFSET))
	{
		uint32 spell_to_learn_slot = action - (__MENU_RUNE_MAIN + __MENU_RUNE_SLOT_LEARN );

		//display the infos of the spell to learn, and then ask for the confirm from the player, and then learn the spell to him.
		text = "|cff002fa7符文：　";
		text.append(_LocalMenus[spell_to_learn_slot].text);
		text.append("　|r");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);

		text = "|cff002fa7描述：　";
		text.append(_LocalMenus[spell_to_learn_slot].desc);
		text.append("　|r");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);

		player->ADD_GOSSIP_ITEM(5, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);

		if (!_can_learn_more)
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__RED("符文已满, 请返回并删除部分符文再来哦.")), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN);
		}
		else
		{
			
			player->ADD_GOSSIP_ITEM(5, __STR(__BLUE("=====｜确定并学习｜=====")), GOSSIP_SENDER_MAIN, action + RUNE_CONFIRM_OFFSET);
			player->ADD_GOSSIP_ITEM(5, __STR(__BLUE("=====｜取消、返回｜=====")), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN);
		}
	}

	//confirm the spell
	else if ((action >= __MENU_RUNE_MAIN + __MENU_RUNE_SLOT_LEARN + RUNE_CONFIRM_OFFSET) && (action <= __MENU_RUNE_MAIN + __MENU_RUNE_SLOT_LEARN + 99))
	{
		uint32 spell_to_learn_slot = action - (__MENU_RUNE_MAIN + __MENU_RUNE_SLOT_LEARN + RUNE_CONFIRM_OFFSET);

		if (!_can_learn_more)
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__RED("符文已满, 请返回并删除部分符文再来哦.")), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN);
		}
		else
		{
			player->LearnSpell(_LocalMenus[spell_to_learn_slot].spell_id, false);
			player->ADD_GOSSIP_ITEM(5, __STR(__RED("====| 学习成功! |=====")), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN);
		}

	}
	//1130-1139, forget spell
	else if ((action >= __MENU_RUNE_MAIN + __MENU_RUNE_SLOT_FORGET) && (action <= __MENU_RUNE_MAIN + __MENU_RUNE_SLOT_FORGET + 99))
	{
		uint32 spell_to_delete_slot = action - (__MENU_RUNE_MAIN + __MENU_RUNE_SLOT_FORGET);

		player->RemoveSpell(_LocalMenus[spell_to_delete_slot].spell_id);
		player->ADD_GOSSIP_ITEM(5, __STR(__GREEN("====已经遗忘, |返回|=====")), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN);
	}
	else if ((action >= __MENU_RUNE_MAIN + __MENU_RUNE_SLOT_BUY) && (action <= __MENU_RUNE_MAIN + __MENU_RUNE_SLOT_BUY + 99))
	{
		uint32 _realAction = action - (__MENU_RUNE_MAIN + __MENU_RUNE_SLOT_BUY);

		//get the rune stones that player need to buy
		uint32 _rune_stones_need = 0;
		_rune_stones_need = __rune_need_num(player);
		//slog
		sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "Rune Stones Need: %d", _rune_stones_need);

		if (_realAction == 0)
		{
			//make up the text
			text = __STR(__BLUE("购买符文槽, 需要花费 "));
			text.append(__NSTR(_rune_stones_need));
			text.append(__BLUE(" 个符文石."));
			player->ADD_GOSSIP_ITEM(5, __BLUE(" ========================= "), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(5, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(5, __BLUE(" ========================= "), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("====| 确定 | 返回 | =====")), GOSSIP_SENDER_MAIN, action + 1);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("====| 取消 | 返回 |=====")), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN);
		}
		else if (_realAction == 1)
		{
			//check if has enough stones
			if (player->HasItemCount(__RUNE_UPGRADE_ITEM, _rune_stones_need))
			{
				//delete the item count
				player->DestroyItemCount(__RUNE_UPGRADE_ITEM, _rune_stones_need, true);

				//add the rune slot
				sQZAchievements.SetRuneSlots(player, sQZAchievements.GetRuneSlots(player) + 1);	

				//add the rune slot to the player
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__GREEN("====| 购买成功! |=====")), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN);

			}else
			{
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__RED("====| 材料不够 | 返回 |=====")), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN);	
			}
		}
	}


	player->SEND_GOSSIP_MENU(_CLASS_DESC, _Creature->GetGUID());

	return true;
}


#pragma endregion


#pragma region MENU ACHIEVEMENT SPENT


void Menus_Achieve_Main(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
	if (!player ||!_Creature) return;	
	Player *pPlayer = player;

	int32_t abs_action = action - __MENU_ACHIEVE_MAIN;
	std::string text = "";
	int _ability_values[PLAYER_ABILITIES_NUM_S8] = {0};

	//step 1, get all of the spells of player
	_ability_values[0] = pPlayer->M_Achiv_Player_Chenyi;
	_ability_values[1] = pPlayer->M_Achiv_Player_Zhanpao;
	_ability_values[2] = pPlayer->M_Achiv_Player_NumsTalent;
	_ability_values[3] = pPlayer->M_Achiv_Player_LevelWeapon;
	_ability_values[4] = pPlayer->M_Achiv_Player_LevelPet;
	_ability_values[5] = pPlayer->M_Achiv_Player_NumsKang;
	_ability_values[6] = pPlayer->M_Achiv_Player_NumsStrength;
	_ability_values[7] = pPlayer->M_Achiv_Player_NumsAgility;
	_ability_values[8] = pPlayer->M_Achiv_Player_NumsStamina;
	_ability_values[9] = pPlayer->M_Achiv_Player_NumsIntellect;
	_ability_values[10] = pPlayer->M_Achiv_Player_NumsSpirit;
	_ability_values[11] = pPlayer->M_Achiv_Player_NumsSP;
	_ability_values[12] = pPlayer->M_Achiv_Player_NumsAP;
	_ability_values[13] = pPlayer->M_Achiv_Player_DungeonTimes;

	sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "PLAYER:[%u][%s] === chenyi:%d, zhanpao:%d, talent:%d, weapon:%d, pet:%d, kang:%d, strength:%d, agility:%d, stamina:%d, intellect:%d, spirit:%d, sp:%d, ap:%d, dungeonTimes:%d",
		pPlayer->GetGUID(), pPlayer->GetName(),
		_ability_values[0], _ability_values[1], _ability_values[2], _ability_values[3], _ability_values[4],
		_ability_values[5], _ability_values[6], _ability_values[7], _ability_values[8], _ability_values[9],
		_ability_values[10], _ability_values[11], _ability_values[12], _ability_values[13]);

	//Main action, display the menus
	if(abs_action == 0)
	{
		//display the menus
		//1. 查看点数 
		//2. 分配点数
		//3. 重置点数
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(Helper_MakeString(COLOR_BLUE, "＝　账号成就点数：　%u　　", player->M_Achiv_Account_Sum)), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(Helper_MakeString(COLOR_BLUE, "＝　当前角色消耗：　%u　　", player->M_Achiv_Player_Used)), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR((" ")), GOSSIP_SENDER_MAIN, __MENU_NONE);

		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("＝＞　账号成就　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN + 10);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("＝＞　兑换点数　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN + 14);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("＝＞　分配点数　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN + 20);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("＝＞　重置点数　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN + 30);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,       __STR(" "), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,       __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1,  __STR(__RED("＝＝　领取成就称号　＝＝　")), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN + 40);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,       __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, 		 __STR(" "), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("＝＝＝＝　返回　＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MAIN);
		pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		return;
	}
	else if (abs_action == 10)
	{
		//display the menus
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(Helper_MakeString(COLOR_BLUE, "【任务】完成数量：　%u　", pPlayer->M_Achiv_Account_Task)), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(Helper_MakeString(COLOR_BLUE, "【探索】发现区域：　%u　", pPlayer->M_Achiv_Account_Explore)), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(Helper_MakeString(COLOR_BLUE, "【宠物】收集数量：　%u　", COUNT_ONES(pPlayer->M_Achiv_Account_Pet_Collection))), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(Helper_MakeString(COLOR_BLUE, "【声望】崇拜数量：　%u　", COUNT_ONES(pPlayer->M_Achiv_Account_Reputation_List))), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(Helper_MakeString(COLOR_BLUE, "【专业】大师数量：　%u　", COUNT_ONES(pPlayer->M_Achiv_Account_Profession_Skill))), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(Helper_MakeString(COLOR_BLUE, "【击杀】怪物数量：　%u　", pPlayer->M_Achiv_Account_Killing_Nums)), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(Helper_MakeString(COLOR_BLUE, "【PVP】玩家数量：　%u　", pPlayer->M_Achiv_Account_PVP_Nums)), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(Helper_MakeString(COLOR_BLUE, "【金币】收集数量：　%u　", pPlayer->M_Achiv_Account_Gold_Collect)), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(Helper_MakeString(COLOR_BLUE, "【装备】收集数量：　%u　", pPlayer->M_Achiv_Account_EQ_Nums)), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(Helper_MakeString(COLOR_BLUE, "【成就奖章】数量：　%u　", pPlayer->M_Achiv_Account_Bonus_Nums)), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(Helper_MakeString(COLOR_BLUE, "【英雄副本】通关次数（查询）　")), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN + 13);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("＝＝＝＝　【返回】　＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);

		pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		return;
	}
	else if (abs_action == 13)
	{
		int32_t dg1 = pPlayer->M_Achiv_Account_Dungeon_NUMS1;
		int32_t dg2 = pPlayer->M_Achiv_Account_Dungeon_NUMS2;
		
		//the id is same with TP_Dungeons[], range from 0-17
		//each int32_t holds nine digits.
		int i = 0;
		uint8_t _dgTimes[18] = {0};
		for(i=0; i < 9; i++)
		{
			_dgTimes[i] = dg1 % 10;
			dg1 /= 10;
		}
		for(i=0; i < 9; i++)
		{
			_dgTimes[i+9] = dg2 % 10;
			dg2 /= 10;
		}

		//makeup string first
		std::string _txt = "";
		for(i=0; i < 18; i++)
		{
			_txt += TP_Dungeons[i].name;
			_txt += ":" + std::to_string(_dgTimes[i]);
			_txt += ", ";
		}
		Helper_Chat(pPlayer, __STR(Helper_MakeString(" ", "【英雄副本】通关次数：　%s", _txt.c_str())));
	}
	else if (abs_action >= 14 && abs_action < 20)
	{
		//now open a new menu page to display the items in bag
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝　将物品放在行囊前四个格子里　＝　")), GOSSIP_SENDER_MAIN, action);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝　例如宠物、成就奖章等　＝　")), GOSSIP_SENDER_MAIN, action);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, action);

		//offset is 15
		if(abs_action >= 15)
		{
			uint32 _pos = (abs_action - 15);
			if(_pos >= 4) return;

			//check the bag slot
			//if(!pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, _pos)) return;
			auto pItem = pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START + _pos);
			if(pItem)
			{
				if (int _bonus = DBHelper_get_bonus_points(pPlayer, pItem))
				{
					//else
					//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(Helper_MakeString(COLOR_BLUE, "<%s> ＝＞　%d 点成就。 ", pItem->GetProto()->Name1, _bonus).c_str()), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN + 15 + i);
					//__LOG("Add gossip item: %s", Helper_MakeString(" ", "【%s】可获得成就：%d", pItem->GetProto()->Name1, pItem->GetProto()->SellPrice * pItem->GetCount()).c_str());
					int _type = _bonus / 1000;
					int _realBonus = _bonus % 1000;
					bool _notice = false;
					switch (_type)
					{
						case ACHIEVEMENT_ACCOUNT_BONUS_NUMS:
						{
							//check the cuurrent achive bonus nums
							if (_realBonus && (_realBonus + pPlayer->M_Achiv_Account_Bonus_Nums <= 500))
							{
								player->DestroyItem(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START + _pos, true);
								pPlayer->M_Achiv_Account_Bonus_Nums += _realBonus;
								_notice = true;
							}else
							{
								pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("注意：材料已满或者有误，请再试。＝　")), GOSSIP_SENDER_MAIN, action);
							}
							break;
						}
						case ACHIEVEMENT_ACCOUNT_EQ_NUMS:
						{
							if (_realBonus)
							{
								player->M_Achiv_Account_EQ_Nums += _realBonus;
								player->DestroyItem(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START + _pos, true);
								_notice = true;
							}
							else
							{
								pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("注意：装备有误或成就已满，请再试。＝　")), GOSSIP_SENDER_MAIN, action);
							}

							break;
						}
						case ACHIEVEMENT_ACCOUNT_PET_COLLECTION:
						{
							int index = DBHelper_IsPetCollection(pItem->GetEntry());
							if (index && _realBonus && ((player->M_Achiv_Account_Pet_Collection & (1 << index)) == 0))
							{
								player->M_Achiv_Account_Pet_Collection |= 1 << index;
								_notice = true;
								player->DestroyItem(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START + _pos, true);
							}
							else
							{
								Helper_Chat(player, ">>> 你已拥有此宠物： %s, 出错请跟GM联系。 ", pItem->GetProto()->Name1);
							}
							break;
						}
					}

					if(_notice)
					{
						player->M_Achiv_Account_Sum += _realBonus;   ///Add bonus
						Helper_Chat(player, ">>> 你消耗物品: %s, 获得成就点： %u, 现成就总数： %u.  ", pItem->GetProto()->Name1, _realBonus, player->M_Achiv_Account_Sum);
					}
				}
			}

		}


		//list the four slots in bags
		for(uint32 i = 0; i < 4; i++)
		{
			auto pItem = pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START + i);
			if(pItem)
			{
				//check the pItemlevel,	range 10~ 80 etc ... max achivpoints is 500, 400 is 80 * 4 = 320 * 1.5 ... 
				if (int _bonus = DBHelper_get_bonus_points(pPlayer, pItem))
				{
					//else
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(Helper_MakeString(COLOR_BLUE, "<%s> ＝＞　%d 点成就。 ", pItem->GetProto()->Name1, _bonus%1000).c_str()), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN + 15 + i);
					//__LOG("Add gossip item: %s", Helper_MakeString(" ", "【%s】可获得成就：%d", pItem->GetProto()->Name1, pItem->GetProto()->SellPrice * pItem->GetCount()).c_str());
				}
			}
		}


		//add return menu
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝　返回　＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);
		pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		return;
	}
	else if (abs_action == 20)
	{
		//display the menus
		//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝　点击字条、分配点数　＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
		for (size_t i = 0; i < PLAYER_ABILITIES_NUM_S8; i++)
		{
			text = __STR(Ability_MenuS8[i].desc);
			text.append(__STR("|cff002fa7当前：　"));
			text.append(__NSTR(_ability_values[i] * Ability_MenuS8[i].multi));
			text.append(__STR(Ability_MenuS8[i].unit));
			text.append(__STR("|r"));
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN + i*10 + 100);	
		}
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝　返回　＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);

		pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		return;
	}
	else if (abs_action == 30)
	{
		//display the menus
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝　重置点数　＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);
		//caculate how much gold should be used.

		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("＝＝＝＝＝　返回　＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);

		pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		return;
	}
	else if (abs_action == 31)
	{

	}


	else if (abs_action >= 100 && abs_action < 300)
	{
		uint32 _realAction = (abs_action - 100) / 10;
		if (_realAction >= PLAYER_ABILITIES_NUM_S8) return;

		sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "[qzqstar_hs] ERROR Player:[%s] === Aciton:%d, Upgrade ability: %d", pPlayer->GetName(), action, _realAction);

		//check if abs_action is ended by zero
		//100 -> means the menu display,
		//101 -> add 1
		//102 -> add 10 etc
		int _achieve_num = DBHelper_get_upgrade_points(pPlayer, _realAction, _ability_values[_realAction]);

		if (abs_action % 10 == 0)
		{
			//make up the text
			text = __STR(Ability_MenuS8[_realAction].desc);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);

			text = "";
			text.append(__STR("|cff002fa7当前：　"));
			text.append(__NSTR(_ability_values[_realAction] * Ability_MenuS8[_realAction].multi));
			text.append(__STR(Ability_MenuS8[_realAction].unit));
			text.append(__STR("|r"));
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);

			text = "";
			text.append(__STR("|cff002fa7升级后：　"));
			text.append(__NSTR((_ability_values[_realAction] + 1) * Ability_MenuS8[_realAction].multi));
			text.append(__STR(Ability_MenuS8[_realAction].unit));
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);

			text = "";
			text = __STR("|cff002fa7需要消耗成就：　");
			text.append(__NSTR(_achieve_num));
			text.append(__STR("　|r"));
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);

			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);

			//check if has enough fragments
			if (_achieve_num <= 0)
			{
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, __STR(__RED("＝＝该字条已经阶段满级＝＝　")), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);
			}
			else if (pPlayer->M_Achiv_Account_Sum > pPlayer->M_Achiv_Player_Used && pPlayer->M_Achiv_Account_Sum - pPlayer->M_Achiv_Player_Used >= _achieve_num)
			{
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, __STR(__BLUE("＝＝＝＝　升级一次　＝＝＝＝　")), GOSSIP_SENDER_MAIN, action + 1);
				if(Ability_MenuS8[_realAction].pointsNeed <= 2)
				{
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, __STR(__BLUE("＝＝＝＝　升级十次　＝＝＝＝　")), GOSSIP_SENDER_MAIN, action + 2);
				}
			}	
			else
			{
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, __STR(__RED("＝＝＝＝　成就不足，返回　＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);
			}
				
			pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
		}
		else if ( (abs_action %10 == 1) || (abs_action %10 == 2) )
		{
			//check if has enough achievement and not exceed the max points
			uint32_t _add_points = (abs_action %10 == 1) ? 1 : 10;

			if(_ability_values[_realAction]  + _add_points > Ability_MenuS8[_realAction].maxpoints)
			{
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, __STR(__RED("＝＝　升级点数过大，返回　＝＝　")), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
				return;
			}
			if(pPlayer->M_Achiv_Player_Used + _achieve_num * _add_points > pPlayer->M_Achiv_Account_Sum)
			{
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, __STR(__RED("＝＝　剩余成就不足，返回　＝＝　")), GOSSIP_SENDER_MAIN, __MENU_ACHIEVE_MAIN);
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
				return;
			}
			//upgrade the ability
			_ability_values[_realAction] += _add_points;

			//mostly, the add points is 1.
			pPlayer->M_Achiv_Player_Used += _achieve_num * _add_points;

			//save to pPlayer->M_Achiv * back
			pPlayer->M_Achiv_Player_Chenyi = _ability_values[0];
			pPlayer->M_Achiv_Player_Zhanpao = _ability_values[1];
			pPlayer->M_Achiv_Player_NumsTalent = _ability_values[2];
			pPlayer->M_Achiv_Player_LevelWeapon = _ability_values[3];
			pPlayer->M_Achiv_Player_LevelPet = _ability_values[4];
			pPlayer->M_Achiv_Player_NumsKang = _ability_values[5];
			pPlayer->M_Achiv_Player_NumsStrength = _ability_values[6];
			pPlayer->M_Achiv_Player_NumsAgility = _ability_values[7];
			pPlayer->M_Achiv_Player_NumsStamina = _ability_values[8];
			pPlayer->M_Achiv_Player_NumsIntellect = _ability_values[9];
			pPlayer->M_Achiv_Player_NumsSpirit = _ability_values[10];
			pPlayer->M_Achiv_Player_NumsSP = _ability_values[11];
			pPlayer->M_Achiv_Player_NumsAP = _ability_values[12];		
			pPlayer->M_Achiv_Player_DungeonTimes = _ability_values[13];

			//check real action if chenyi and zhanpao
			if(_realAction == 0) 		pPlayer->AddItem(ZQ_ITEM_CHENYI + pPlayer->M_Achiv_Player_Chenyi);
			else if(_realAction == 1) 	pPlayer->AddItem(ZQ_ITEM_ZHANPAO + pPlayer->M_Achiv_Player_Zhanpao);

			//__LOG("Player %s upgrade ability %d, need %d achievements", pPlayer->GetName(), _realAction, _achieve_num);
			Helper_Chat(pPlayer, Helper_MakeString(" ", ">>> 你升级 %u 倍消耗成就点数： %u 剩余点数： %u ", abs_action %10 == 1?1:10,  _achieve_num * _add_points, pPlayer->M_Achiv_Account_Sum - pPlayer->M_Achiv_Player_Used).c_str());
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG,  __STR(__RED("＝＝　重复操作（注意点数）＝＝　")), GOSSIP_SENDER_MAIN, action);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, __STR(__BLUE("＝＝＝＝＝　返回　＝＝＝＝　")), GOSSIP_SENDER_MAIN, action /10 * 10);
			
			pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
		}
	}
		
}
#pragma endregion


#pragma region MENU FRAGMENTS
const FragUpgrade_t _Ability_Menu[_ABILITIES_NUM]=
{
	{0, 2, 0,0,0, ZQ_SPELL_PROMOTION_STAT,   __STR("|cff002fa7【属性】最高１０％　|r"), __STR("％　")},
	{1, 2, 0,0,0, ZQ_SPELL_PROMOTION_DAMAGE, __STR("|cff002fa7【伤害】最高１０％　|r"), __STR("％　")},
	{2, 4, 0,0,0, ZQ_SPELL_PROMOTION_CRITDM, __STR("|cff002fa7【暴伤】最高２０％　|r"), __STR("％　")},
	{3, 2, 0,0,0, ZQ_SPELL_PROMOTION_VOIDAM,  __STR("|cff002fa7【免伤】最高１０％　|r"), __STR("％　")},
	{4, 4, 0,0,0, ZQ_SPELL_PROMOTION_HASTE,  __STR("|cff002fa7【急速】最高２０％　|r"), __STR("％　")},
	{5, 10, 0, 0,0,0, __STR("|cff002fa7【移速】最高５０％　|r"), __STR("％　")},
	{6, 1, 0, 0,0,0, __STR("|cff002fa7【天赋】最高５点　|r"), __STR("点　")},
	{7, 2, 0, 0,0,0, __STR("|cff002fa7【物理吸血】最高１０％　|r"), __STR("％　")},
	{8, 2, 0, 0,0,0, __STR("|cff002fa7【法术吸血】最高１０％　|r"), __STR("％　")},
	{9, 2, 0, 0,0,0, __STR("|cff002fa7【武器技能】最高１０点　|r"), __STR("点　")}
};
const int __PLAYER_NUM[5] = {1, 15, 25, 45, 60};

static int __get_fragupgrade_num(int playerlevel, int ability_level)
{
	//ability upgrade should also depend on the player level
	//player level [1, 15, 25, 45, 60]
	//ablity level [0, 1, 2, 3, 4]

	if(ability_level > 4) return -1;
	
	if(playerlevel < __PLAYER_NUM[ability_level]) return -1;

	return (ability_level * ability_level + 1)* 10;
}

void Menus_Frag_Main(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
	if (!player ||!_Creature) return;	
	Player *pPlayer = player;

	std::string text = "";
	int _ability_values[_ABILITIES_NUM] = {0};

	//step 1, get all of the spells of player
	//1.1 find the haste spells, support max 5 spells (_PEAK_MAX_RANK)
	for (size_t i = 0; i < _PEAK_MAX_RANK; i++)
	{
		if (pPlayer->HasSpell(ZQ_SPELL_PROMOTION_STAT 		+ i))	_ability_values[0]++;
		if (pPlayer->HasSpell(ZQ_SPELL_PROMOTION_DAMAGE 	+ i))	_ability_values[1]++;
		if (pPlayer->HasSpell(ZQ_SPELL_PROMOTION_CRITDM 	+ i))	_ability_values[2]++;
		if (pPlayer->HasSpell(ZQ_SPELL_PROMOTION_VOIDAM 	+ i))	_ability_values[3]++;
		if (pPlayer->HasSpell(ZQ_SPELL_PROMOTION_HASTE 		+ i))	_ability_values[4]++;
	}

	//1.2 find the talent points
	_ability_values[5] = pPlayer->M_Speed;
	_ability_values[6] = pPlayer->M_TalentPoints;
	_ability_values[7] = pPlayer->M_Leech_Phy;
	_ability_values[8] = pPlayer->M_Leech_Spell;
	_ability_values[9] = pPlayer->M_WeaponSkill;


	sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "PLAYER:[%u][%s] === Stat: %d, dmg %d, crigdmg %d, defend %d, haste:%d. speed:%d, talent: %d, leechphysical: %d, leechspell: %d, weaponskill: %d",
		pPlayer->GetGUID(), pPlayer->GetName(),
		_ability_values[0], _ability_values[1], _ability_values[2], _ability_values[3], _ability_values[4],
		_ability_values[5], _ability_values[6], _ability_values[7], _ability_values[8], _ability_values[9]);
		
	//step 2, display the menus
	if(action == __MENU_FRAG_MAIN)
	{
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝　能力提升系统　＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);	

		for (size_t i = 0; i < _ABILITIES_NUM; i++)
		{
			text = __STR(_Ability_Menu[i].desc);
			text.append(__STR("|cff002fa7当前：　"));
			text.append(__NSTR(_ability_values[i] * _Ability_Menu[i].multi));
			text.append(__STR(_Ability_Menu[i].unit));
			text.append(__STR("|r"));
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN + i + 100);	
		}

		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);	
		pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
	}

	//upgrade the ability using fragments
	else if (action >= __MENU_FRAG_MAIN + 100 && action <= __MENU_FRAG_MAIN + 300)
	{
		//get the ability index
		uint32 _realAction = action % 100;

		//Check if the ability is valid
		if(_realAction >= _ABILITIES_NUM) 
		{
			sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "[qzqstar_hs] ERROR Player:[%s] === Aciton:%d, Upgrade ability: %d", pPlayer->GetName(), action, _realAction);
			return;
		}

		int _frag_num = __get_fragupgrade_num(pPlayer->GetLevel(), _ability_values[_realAction]);

		//slog out
		sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "Player:[%s] === Aciton:%d, Upgrade ability: %d, need %d fragments", pPlayer->GetName(), action, _realAction, _frag_num);
		
		//check if the player has enough fragments
		if(action < __MENU_FRAG_MAIN + 200)
		{
			//make up the text
			text = __STR(_Ability_Menu[_realAction].desc);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);	

			text = "";
			text.append(__STR("|cff002fa7当前：　"));
			text.append(__NSTR(_ability_values[_realAction] * _Ability_Menu[_realAction].multi));
			text.append(__STR(_Ability_Menu[_realAction].unit));
			text.append(__STR("|r"));
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);	

			text = "";
			text.append(__STR("|cff002fa7升级后：　"));
			text.append(__NSTR((_ability_values[_realAction] + 1) * _Ability_Menu[_realAction].multi));
			text.append(__STR(_Ability_Menu[_realAction].unit));
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);	
			
			text = "";
			text = __STR("|cff002fa7需要提升碎片：　");
			text.append(__NSTR(_frag_num));
			text.append(__STR("　|r"));
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);

			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);
			
			//check if has enough fragments
			if(_frag_num <= 0)
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, __STR(__RED("＝＝该字条已经阶段满级＝＝　")), GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);
			else if (pPlayer->HasItemCount(ZQ_ITEM_FRAGMENTS, _frag_num))
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, __STR(__BLUE("＝＝＝＝确定升级＝＝＝＝　")), GOSSIP_SENDER_MAIN, action + 100);
			else
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG,  __STR(__RED("＝＝＝＝碎片不足＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);

			pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
		}

		else if(action < __MENU_FRAG_MAIN + 300)
		{
			if(_frag_num <= 0)
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, __STR(__RED("＝＝该字条已经阶段满级＝＝　")), GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);
			//upgrade the ability
			else if (pPlayer->HasItemCount(ZQ_ITEM_FRAGMENTS, _frag_num))
			{
				//delete the item count
				pPlayer->DestroyItemCount(ZQ_ITEM_FRAGMENTS, _frag_num, true);

				//upgrade the ability
				if(_realAction < 5)
				{
					pPlayer->LearnSpell(_Ability_Menu[_realAction].spellId + _ability_values[_realAction], false);
				}
				else 
				{
					/*    	uint32 _Promotions = sQZAchievements.GetPromotions(this);
							M_Leech_Phy = _Promotions % 10;           		//7, 1
							M_Leech_Spell = (_Promotions / 10)%10;			//8, 10
							M_TalentPoints = (_Promotions / 100)%10;		//6, 100
							M_Speed = (_Promotions / 1000)%10;				//5, 1000
							M_WeaponSkill = (_Promotions / 10000)%10;		//9, 10000
					*/
					uint32 _Promotions = sQZAchievements.GetPromotions(pPlayer);
					if(_realAction == 5) 		{ pPlayer->M_Speed ++;  _Promotions = _Promotions + 1000;}
					else if(_realAction == 6) 	{ pPlayer->M_TalentPoints ++;  _Promotions = _Promotions + 100;}
					else if(_realAction == 7) 	{ pPlayer->M_Leech_Phy ++;  _Promotions = _Promotions + 1;}
					else if(_realAction == 8) 	{ pPlayer->M_Leech_Spell ++;  _Promotions = _Promotions + 10;}
					else if(_realAction == 9) 	{ pPlayer->M_WeaponSkill ++; _Promotions = _Promotions + 10000;}

					sQZAchievements.SetPromotions(pPlayer, _Promotions);
				}

				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, __STR(__GREEN("＝＝升级成功，小退生效，返回＝＝　")), GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);

			}

			else
			{
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, __STR(__RED("＝＝＝碎片不足，返回＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);
			}


			pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
		}

	}
}
#pragma endregion

#pragma region VIP teleport 

void Menus_Vip_Teleport_Main(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
	if (!player ||!_Creature) return;

	std::string text = "";
		uint32 __menu_nums = 0;

	if (action == __MENU_VIP_TELEPORT_MAIN)
	{
		//Find all of the mapid from _TELEPORT_Locs
		for (size_t i = 0; i < sizeof(_TELEPORT_Locs) / sizeof(_TELEPORT_Locs[0]); ++i) {

			if (player->GetMapId() == _TELEPORT_Locs[i].map_id)
			{
				//add to menu.
				++ __menu_nums;
				uint32 vip_level = player->M_Achiv_Account_VIP % 10;

				if( (player->HasSpell(ZQ_SPELL_HS_VIP)) || (_TELEPORT_Locs[i].vip_ind == 0) || (player->IsGameMaster()) )
					player->ADD_GOSSIP_ITEM(5, __STR(_TELEPORT_Locs[i].text), GOSSIP_SENDER_MAIN, __MENU_VIP_TELEPORT_MAIN + _TELEPORT_Locs[i].action_id);
				else if ( (vip_level>=4) && ((i+1)%3 == 0))
					player->ADD_GOSSIP_ITEM(5, __STR(_TELEPORT_Locs[i].text), GOSSIP_SENDER_MAIN, __MENU_VIP_TELEPORT_MAIN + _TELEPORT_Locs[i].action_id);

			}
		}

		if (__menu_nums == 0)
		{
			player->ADD_GOSSIP_ITEM(5, __STR(__RED(" ==|　只能在副本使用　|== ")), GOSSIP_SENDER_MAIN, __MENU_NONE);
		}

		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
	}
	else
	{
		auto absAction = action - __MENU_VIP_TELEPORT_MAIN;
		//Find all of the mapid from _TELEPORT_Locs
		for (size_t i = 0; i < sizeof(_TELEPORT_Locs) / sizeof(_TELEPORT_Locs[0]); ++i) {

			if (absAction == _TELEPORT_Locs[i].action_id)
			{
				//just teleport to the position.
				player->CLOSE_GOSSIP_MENU();
				player->TeleportTo(_TELEPORT_Locs[i].map_id, _TELEPORT_Locs[i].pos[0], _TELEPORT_Locs[i].pos[1], _TELEPORT_Locs[i].pos[2], _TELEPORT_Locs[i].pos[3]);
			}
		}
	}

}
#pragma endregion

#pragma endregion
#define __TEAM_SUMMON_ACTION		10
#define __TEAM_REVIVE_ACTION		20
#define __TEAM_FULLHP_ACTION		30
#define __TEAM_SYNC_LEVEL_ACTION	40
bool static __localHandleGroupCommand(Player *pPlayer, uint32 action)
{
    //Player* pPlayer = m_session->GetPlayer();
	if(action == __TEAM_SYNC_LEVEL_ACTION)
	{
		if(pPlayer->M_Achiv_Account_Max_Level < pPlayer->GetLevel())
		{
			pPlayer->M_Achiv_Account_Max_Level = pPlayer->GetLevel();
			Helper_Chat(pPlayer, Helper_MakeString(" ", "该角色等级已保存为账号最高等级: %u 级，其他同账号低该等级的角色可以直升至当前等级。 ", pPlayer->M_Achiv_Account_Max_Level).c_str());
		}
		else if (pPlayer->M_Achiv_Account_Max_Level > pPlayer->GetLevel())
		{
			pPlayer->GiveLevel(pPlayer->M_Achiv_Account_Max_Level);
			Helper_Chat(pPlayer, Helper_MakeString(" ", "该角色等级已经同步至账号最高等级: %u 级。 ", pPlayer->M_Achiv_Account_Max_Level).c_str());
		}
		else
		{
			Helper_Chat(pPlayer, Helper_MakeString(" ", "该角色等级已经是账号最高级。 ").c_str());
		}
		return true;
	}

    Group* pGroup = pPlayer->GetGroup();
    if (!pGroup)
    {
		pPlayer->PSendSysMessage(__STR("该功能只能组队使用。 "));
        return false;
    }

	if(pPlayer->IsInCombat())
	{
		pPlayer->PSendSysMessage(__STR("该功能不能在战斗中使用。 "));
        return false;
	}

    for (GroupReference* itr = pGroup->GetFirstMember(); itr != nullptr; itr = itr->next())
    {
        if (Player* pMember = itr->getSource())
        {
            if (pMember == pPlayer)
                continue;

			//check the action
			switch(action)
			{
				case __TEAM_SUMMON_ACTION:
				{
					//summon request
					pMember->SendSummonRequest(pPlayer->GetObjectGuid(), pPlayer->GetMapId(), pPlayer->GetZoneId(), pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ());
					break;
				}

				case __TEAM_REVIVE_ACTION:
				{
					//revive request
					if (pMember->IsDead())
					{
						pMember->ResurrectPlayer(0.2f);
						pMember->SpawnCorpseBones();
					}
					break;
				}

				case __TEAM_FULLHP_ACTION:
				{
					//fufill request
					if (pMember->IsAlive())
					{

						if(pMember->HasAura(15007))
						{
							pMember->RemoveAurasDueToSpell(15007);
						}

						pMember->SetHealth(pMember->GetMaxHealth());
						if (pMember->GetPowerType() == POWER_MANA)
							pMember->SetPower(POWER_MANA, pMember->GetMaxPower(POWER_MANA));
					}

					break;
				}
			}
		}
    }

	pPlayer->PSendSysMessage(__STR("团队功能已成功释放。 "));
    return true;
}

struct CustomHSSpell : SpellScript
{
	void OnSuccessfulFinish(Spell* spell) const final
	{
		if(Player *player = (spell->m_casterUnit)->ToPlayer())
		{
			player->PlayerTalkClass->ClearMenus();
			player->M_Gossip_Spell_Ind = 1998;
			
			_Main_Menus(player);
		}

		else
		{
			sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "Error of Spell_HS, not a player??.");
		}
	}

	void OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action) const final
	{
		//sLog.Out(LOG_BASIC, LOG_LVL_ERROR, "test. %d %d", sender, action);
		pPlayer->CLOSE_GOSSIP_MENU();

		if(action == __MENU_MAIN)
		{
			_Main_Menus(pPlayer);	return;
		}
		else if (action >= __MENU_MODE_MAIN && action <= __MENU_MODE_MAIN + __MENU_SIZE)
		{
			// display the challenging mode
			uint32 _Mode = sQZAchievements.GetChallengeMode(pPlayer);
			if(action == __MENU_MODE_MAIN)
			{
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝　挑战模式状态　＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
				if(_Mode & CHALLENGING_MODE_ONELIFE) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　一命模式　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
				else pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＝＞　开启一命模式　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN + 1);

				if(_Mode & CHALLENGING_MODE_MANUFACT) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　工匠模式　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
				else pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＝＞　开启工匠模式　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN + 2);

				if(_Mode & CHALLENGING_MODE_TASK) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　任务模式　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
				else pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＝＞　开启任务模式　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN + 3);

				if(_Mode & CHALLENGING_MODE_EQUIPMENT) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　装等模式　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
				else pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＝＞　开启装等模式　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN + 4);

				if(_Mode & CHALLENGING_MODE_RICH) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　富豪模式　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
				else pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＝＞　开启富豪模式　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN + 5);

				if(_Mode >= CHALLENGING_MODE_KILLER_HUMAN) 
				{
					if(_Mode & CHALLENGING_MODE_KILLER_HUMAN)pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　杀手模式（人形）　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
					else if(_Mode & CHALLENGING_MODE_KILLER_BEAST) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　杀手模式（野兽）　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
					else if(_Mode & CHALLENGING_MODE_KILLER_UNDEAD) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　杀手模式（亡灵）　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
				}
				else
				{
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞　开启杀手模式（人形）　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN + 10);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞　开启杀手模式（野兽）　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN + 11);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞　开启杀手模式（亡灵）　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN + 12);
				}


				if(_Mode & CHALLENGING_MODE_DISCIPLINE) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　修行模式　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
				else pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＝＞　开启修行模式　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN + 6);

				if(_Mode & CHALLENGING_MODE_LEADER) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　领袖模式　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
				else pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＝＞　开启领袖模式　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN + 7);

			}

			else if(action >= __MENU_MODE_MAIN + 1 && action <= __MENU_MODE_MAIN + 19)
			{
				//change the challenging mode
				auto _realAction = action - __MENU_MODE_MAIN ;

				switch(_realAction)
				{
					case 1:	 
					{
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　一命模式介绍　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE(" ")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜１＞　死亡后等级降低２级　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜２＞　死亡掉落一半金币　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜３＞　满级奖励３００点券、乌龟坐骑　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						break;
					}
					case 2:	
					{
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　工匠模式介绍　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE(" ")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜１＞　可选专业＋２　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜２＞　制造专业所需材料减半　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜３＞　专业技能升级降为１点　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						break;	
					}
					case 3:	 
					{
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　任务模式介绍　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE(" ")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜１＞　杀怪经验降低　")), GOSSIP_SENDER_MAIN, __MENU_NONE);	
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜２＞　任务经验提升　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						break;	

					}
					case 4:	 
					{
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　装等模式介绍　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE(" ")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜１＞　２５＼３５＼４５等级时，需要突破装等才能继续升级；　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜２＞　突破等级可以获取点券奖励；　")), GOSSIP_SENDER_MAIN, __MENU_NONE);	
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜３＞　装等越高，获取奖励越多。　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						break;	
					}
					case 5:	
					{
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　富豪模式介绍　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE(" ")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜１＞　携带金币可以提升伤害、免伤　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜２＞　额外增加衬衣战袍的属性　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜３＞　死亡后掉落部分金币　")), GOSSIP_SENDER_MAIN, __MENU_NONE);	
						break;	
					}
					case 6:	
					{
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　修行模式介绍　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE(" ")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜１＞　坐骑速度提升５０％　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜２＞　伤害＋５％　")), GOSSIP_SENDER_MAIN, __MENU_NONE);	
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜３＞　无法使用世界传送　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						break;	
					}
					case 7:	
					{
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　领袖模式介绍　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE(" ")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜１＞　作为队长，增减伤５％　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜２＞　作为团长，增减伤１０％　")), GOSSIP_SENDER_MAIN, __MENU_NONE);	
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜３＞　非组队状态，受伤提升５％　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						break;	
					}
					case 10: 
					{
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　杀手模式（人形）介绍　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE(" ")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜１＞　击杀人形获得额外经验　")), GOSSIP_SENDER_MAIN, __MENU_NONE);	
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜２＞　击杀其他类型不获取经验　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜３＞　对人形伤害提升２０％　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						break;	
					}
					case 11: 
					{
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　杀手模式（野兽）介绍　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE(" ")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜１＞　击杀野兽获得额外经验　")), GOSSIP_SENDER_MAIN, __MENU_NONE);	
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜２＞　击杀其他类型不获取经验　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜３＞　对野兽伤害提升２０％　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						break;	
					}
					case 12: 
					{
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　杀手模式（亡灵）介绍　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE(" ")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜１＞　击杀亡灵获得额外经验　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜２＞　击杀其他类型不获取经验　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＜３＞　对亡灵伤害提升２０％　")), GOSSIP_SENDER_MAIN, __MENU_NONE);	
						break;	
					}
				}

				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE(" ")), GOSSIP_SENDER_MAIN, __MENU_NONE);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,  __STR(__RED("＝＝＝＞＞　确定开启　＜＜＝＝　")), GOSSIP_SENDER_MAIN, action + 20);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞＞　我再想想　＜＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN);
			}
			else if (action >= __MENU_MODE_MAIN + 20 && action <= __MENU_MODE_MAIN + 39)
			{
				//change the challenging mode
				auto _realAction = action - __MENU_MODE_MAIN - 20;
				switch(_realAction)
				{
					case 1:	 _Mode |= CHALLENGING_MODE_ONELIFE; break;
					case 2:	 _Mode |= CHALLENGING_MODE_MANUFACT; break;
					case 3:	 _Mode |= CHALLENGING_MODE_TASK; break;
					case 4:	 _Mode |= CHALLENGING_MODE_EQUIPMENT; break;
					case 5:	 _Mode |= CHALLENGING_MODE_RICH; break;
					case 6:	 _Mode |= CHALLENGING_MODE_DISCIPLINE; break;
					case 7:	 _Mode |= CHALLENGING_MODE_LEADER; break;
					case 10: _Mode |= CHALLENGING_MODE_KILLER_HUMAN; break;
					case 11: _Mode |= CHALLENGING_MODE_KILLER_BEAST; break;
					case 12: _Mode |= CHALLENGING_MODE_KILLER_UNDEAD; break;
				}
				sQZAchievements.SetChallengeMode(pPlayer, _Mode);
				pPlayer->M_Challenge_Mode = _Mode;
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　挑战模式已开启　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN);
			}

			pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
		}
		else if (action >= __MENU_CITIES_MAIN && action <= __MENU_CITIES_MAIN + __MENU_SIZE)
		{
			//display the cities
			if(action == __MENU_CITIES_MAIN)
			{
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＞加基森（世界主城）＜＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 9);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＝＞　使用炉石　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 8);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE(" ")), GOSSIP_SENDER_MAIN, __MENU_NONE);

				if (pPlayer->GetTeam() == ALLIANCE)
				{
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＝＞　暴风城　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 11);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＝＞　铁炉堡　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 12);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＝＞　达纳苏斯　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 13);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);

					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＞　人类出生地　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 14);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＞　矮人出生地　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 15);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＞　精灵出生地　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 16);
				}
				else //HORDE
				{

					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＝＞　奥格瑞玛　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 21);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＝＞　幽暗城　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 22);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＝＞　雷霆崖　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 23);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);

					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＞　兽人出生地　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 24);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＞　亡灵出生地　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 25);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＞　牛头出生地　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 26);

				}

				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＝＞　永望镇　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 31);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＝＞　藏宝海湾　＜＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 32);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, __STR(__BLUE("＝＝＞　棘齿城　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_CITIES_MAIN + 33);
				
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
				return;
			}

			else
			{
				uint32 _realAction = action - __MENU_CITIES_MAIN;
				switch(_realAction)
				{
				case 8:	pPlayer->TeleportToHomebind(); return;
				case 9:	pPlayer->CastSpell(pPlayer, 23441, true); return;

				case 11:	pPlayer->TeleportTo(MAP_EASTERN_KINGDOMS, -8828.231445f, 627.927490f, 94.055664f, 0.0f, TELE_TO_FORCE_MAP_CHANGE); return;
				case 12:	pPlayer->TeleportTo(MAP_EASTERN_KINGDOMS, -4917.0f, -955.0f, 502.0f, 0.0f, TELE_TO_FORCE_MAP_CHANGE); return;
				case 13:	pPlayer->TeleportTo(MAP_KALIMDOR, 9962.712891f, 2280.142822f, 1341.394409f, 0.0f, TELE_TO_FORCE_MAP_CHANGE); return;
				
				case 14:	pPlayer->TeleportTo(MAP_EASTERN_KINGDOMS, -8943.133789f, -132.934921f, 83.704269f, 0.0f, TELE_TO_FORCE_MAP_CHANGE); return;
				case 15:	pPlayer->TeleportTo(MAP_EASTERN_KINGDOMS, -6231.106445f, 332.270477f, 383.153931f, 0.0f, TELE_TO_FORCE_MAP_CHANGE); return;
				case 16:	pPlayer->TeleportTo(MAP_KALIMDOR, 10329.918945f, 833.500305f, 1326.260620f, 0.0f), TELE_TO_FORCE_MAP_CHANGE; return;

				case 21:	pPlayer->TeleportTo(MAP_KALIMDOR, 1437.0f, -4421.0f, 25.24f, 1.65f, TELE_TO_FORCE_MAP_CHANGE); return;
				case 22:	pPlayer->TeleportTo(MAP_EASTERN_KINGDOMS, 1822.0999f, 238.638855f, 60.694809f, 0.0f, TELE_TO_FORCE_MAP_CHANGE); return;
				case 23:	pPlayer->TeleportTo(MAP_KALIMDOR, -1272.703735f, 116.886490f, 131.016861f, 0.0f), TELE_TO_FORCE_MAP_CHANGE; return;

				case 24:	pPlayer->TeleportTo(MAP_KALIMDOR, -602.1253f, -4262.4208f, 38.956341f, 0.0f, TELE_TO_FORCE_MAP_CHANGE); return;
				case 25:	pPlayer->TeleportTo(MAP_EASTERN_KINGDOMS, 1663.517f, 1678.187744f, 120.5303f, 0.0f, TELE_TO_FORCE_MAP_CHANGE); return;
				case 26:	pPlayer->TeleportTo(MAP_KALIMDOR, -2914.16992f, -266.061798f, 53.658211f, 0.0f, TELE_TO_FORCE_MAP_CHANGE); return;

				case 31:	pPlayer->TeleportTo(MAP_KALIMDOR, 6755.33f, -4658.09f, 724.8f, 3.4049f, TELE_TO_FORCE_MAP_CHANGE); return;
				case 32:	pPlayer->TeleportTo(MAP_EASTERN_KINGDOMS, -14462.0f, 460.0f, 16.1f, 3.4049f, TELE_TO_FORCE_MAP_CHANGE); return; 
				case 33:	pPlayer->TeleportTo(MAP_KALIMDOR, -952.2f, -3746.3f, 5.66f, 1.1f, TELE_TO_FORCE_MAP_CHANGE); return;
				}
			}
		}

		else if(action >= __MENU_SUISHEN_MAIN && action <= __MENU_SUISHEN_MAIN + __MENU_SIZE)
		{
			uint32 _vipFeature = pPlayer->M_Achiv_Account_VIP;

			if(action == __MENU_SUISHEN_MAIN)
			{
				if (_vipFeature & 0x01) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　召唤维修机器人　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 10);
				if (_vipFeature & 0x02)	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　召唤移动银行　　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 20);
				if (_vipFeature & 0x04)	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　召唤中立拍卖师　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 30);
				if (_vipFeature & 0x08)	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　召唤猎人兽栏　　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 40);
				if (!(_vipFeature & 0x01)) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＞购买维修机器（１００点券）＜＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 10 + 100);
				if (!(_vipFeature & 0x02)) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＞购买移动银行（１００点券）＜＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 20 + 100);
				if (!(_vipFeature & 0x04)) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＞购买中立拍卖（１００点券）＜＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 30 + 100);
				if (!(_vipFeature & 0x08)) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＞购买猎人兽栏（１００点券）＜＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 40 + 100);
				
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);
				if((_vipFeature & 0x100)&&(!pPlayer->HasSpell(ZQ_SPELL_AUTOPICK)))	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＞账号共享一键拾取 （免费）＜＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 95 + 100);
				if((_vipFeature & 0x200)&&(!pPlayer->HasSpell(ZQ_SPELL_BUFF_ALL)))	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＞账号共享永久BUFF （免费）＜＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 96 + 100);
				if(!(_vipFeature & 0x100))	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＞购买一键拾取 （１００点券）＜＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 97 + 100);
				if(!(_vipFeature & 0x200))	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＞购买永久BUFF （２００点券）＜＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 98 + 100);
				

				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);
				if (_vipFeature & 0x10) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　队伍召唤　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 80);
				if (_vipFeature & 0x20)	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　全体复活　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 81);
				if (_vipFeature & 0x40)	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　全体恢复　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 82);
				if (!(_vipFeature & 0x10)) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞永久队伍召唤（500点券）＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 80 + 100);
				if (!(_vipFeature & 0x20)) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞永久全体复活（500点券）＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 81 + 100);
				if (!(_vipFeature & 0x40)) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞永久全体恢复（500点券）＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 82 + 100);

				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);
				if (_vipFeature & VIP_SYNC_LEVEL) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　同步角色等级　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 83);
				if (!(_vipFeature & VIP_SYNC_LEVEL)) pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＞永久账号等级同步（500点券）＜＝　")), GOSSIP_SENDER_MAIN, __MENU_SUISHEN_MAIN + 83 + 100);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);
				
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MAIN);

				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
			}

			else if (action>__MENU_SUISHEN_MAIN && action < __MENU_SUISHEN_MAIN + 99)
			{
				switch (action - __MENU_SUISHEN_MAIN)
				{
				case 10: pPlayer->CastSpell(pPlayer, ZQ_SPELL_SUMMON_ROBOT, true); return; //summon robot;
				case 20: pPlayer->CastSpell(pPlayer, ZQ_SPELL_SUMMON_BANK, true); return; //summon bank;
				case 30: pPlayer->CastSpell(pPlayer, ZQ_SPELL_SUMMON_AH, true); return; //summon ah;
				case 40: pPlayer->CastSpell(pPlayer, ZQ_SPELL_SUMMON_STABLE, true); return; //summon stable;

				case 80: __localHandleGroupCommand(pPlayer, __TEAM_SUMMON_ACTION); return; //summon request;
				case 81: __localHandleGroupCommand(pPlayer, __TEAM_REVIVE_ACTION); return; //revive request;
				case 82: __localHandleGroupCommand(pPlayer, __TEAM_FULLHP_ACTION); return; //full hp request;
				case 83: __localHandleGroupCommand(pPlayer, __TEAM_SYNC_LEVEL_ACTION); return; //sync level request;
				}
			}

			else if (action > __MENU_SUISHEN_MAIN + 100 && action < __MENU_SUISHEN_MAIN + 150)
			{
				//check if player has enough vouchers
				
				if(!pPlayer->HasItemCount(ZQ_ITEM_VOUCHER, 100))
				{
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞　点券不够，返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MAIN);
				}
				else
				{
					pPlayer->DestroyItemCount(ZQ_ITEM_VOUCHER, 100, true, true);
					switch (action - __MENU_SUISHEN_MAIN - 100)
					{
						case 10: pPlayer->M_Achiv_Account_VIP |= VIP_SUISHEN_ROBOT;  	break;
						case 20: pPlayer->M_Achiv_Account_VIP |= VIP_SUISHEN_BANK; 	break;
						case 30: pPlayer->M_Achiv_Account_VIP |= VIP_SUISHEN_AH; 		break;
						case 40: pPlayer->M_Achiv_Account_VIP |= VIP_SUISHEN_STABLE; 	break;
					}	
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　开通成功，返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MAIN);	
				}
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
				return;
			}
			else if (action >= __MENU_SUISHEN_MAIN + 180 && action < __MENU_SUISHEN_MAIN + 190)
			{
				//Team action buy
				//check if player has enough vouchers
				if(!pPlayer->HasItemCount(ZQ_ITEM_VOUCHER, 500))
				{
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞　点券不够，返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MAIN);	
				}
				else
				{
					pPlayer->DestroyItemCount(ZQ_ITEM_VOUCHER, 500, true, true);
					switch (action - __MENU_SUISHEN_MAIN - 180)
					{
						case 0: pPlayer->M_Achiv_Account_VIP |= VIP_TEAM_SUMMON;  		break;
						case 1: pPlayer->M_Achiv_Account_VIP |= VIP_TEAM_REVIVE; 		break;
						case 2: pPlayer->M_Achiv_Account_VIP |= VIP_TEAM_FULLFILL; 	break;
						case 3: pPlayer->M_Achiv_Account_VIP |= VIP_SYNC_LEVEL; 	break;
					}	
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　开通成功，返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MAIN);
				}

				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
				return;
			}
			//Auto pick all
			else if (action  == __MENU_SUISHEN_MAIN + 197)
			{
				if(!pPlayer->HasItemCount(ZQ_ITEM_VOUCHER, 100))
				{
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞　点券不够，返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MAIN);
				}
				else
				{
					pPlayer->DestroyItemCount(ZQ_ITEM_VOUCHER, 100, true, true);
					pPlayer->LearnSpell(ZQ_SPELL_AUTOPICK, false);
					sQZAchievements.SetAccAchieveData(pPlayer, ACHIEVEMENT_ACCOUNT_VIP, VIP_ONEKEY_PICK);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　成功学会一键拾取，返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MAIN);	
				}
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
				return;
			}
			else if (action  == __MENU_SUISHEN_MAIN + 195)
			{
				pPlayer->LearnSpell(ZQ_SPELL_AUTOPICK, false);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　账号共享一键拾取，返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MAIN);	
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
				return;
			}
			else if (action  == __MENU_SUISHEN_MAIN + 198)
			{
				if(!pPlayer->HasItemCount(ZQ_ITEM_VOUCHER, 200))
				{
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞　点券不够，返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MAIN);
				}
				else
				{
					pPlayer->DestroyItemCount(ZQ_ITEM_VOUCHER, 200, true, true);
					pPlayer->LearnSpell(ZQ_SPELL_BUFF_ALL, false);
					sQZAchievements.SetAccAchieveData(pPlayer, ACHIEVEMENT_ACCOUNT_VIP, VIP_SUISHEN_BUFF);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　成功学会一键BUFF，返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MAIN);	
				}
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
				return;
			}
			else if (action  == __MENU_SUISHEN_MAIN + 196)
			{
				pPlayer->LearnSpell(ZQ_SPELL_BUFF_ALL, false);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　账号共享一键BUFF，返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_MAIN);	
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
				return;
			}
		}

		else if(action >= __MENU_SKILL_STOLE && action <= __MENU_SKILL_STOLE + __MENU_SIZE)
		{
			AchievementsEntry e = sQZAchievements.GetSkillsCollectEntry(pPlayer);
			std::string text = "";

			//Stole the skill from other monsters
			if(action == __MENU_SKILL_STOLE)
			{
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);	
				text= "";
				text.append(__STR("|cff0000ff＝　当前激活技能：　"));
				if(e.subType == 0) text.append(__STR("无　|r"));
				else 
				{
					SpellEntry const* spellInfo = sSpellMgr.GetSpellEntry(e.subType);
					if(spellInfo)
					{
						std::string name = spellInfo->SpellName[LOCALE_zhCN];
						if(name.empty()) name = spellInfo->SpellName[LOCALE_enUS];
						text.append(name);
					}
					else
					{
						text.append(__STR("无　|r"));
					}
				}
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　已学技能列表　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);

				if(e.data8 < 2) e.data8 = 2;
				else if(e.data8 > 4) e.data8 = 4;

				for (size_t i = 0; i < 4; i++)
				{
					bool _foundSpell = false;
					text= "(";
					text.append(__NSTR(i));
					text.append(__STR(") 已收集技能：　"));

					uint32_t spell_id = (i==0)?e.data1:
										(i==1)?e.data2:
										(i==2)?e.data3:
											   e.data4;

					if(i < e.data8)
					{
						if(spell_id != 0)
						{
							SpellEntry const* spellInfo = sSpellMgr.GetSpellEntry(spell_id);
							if(spellInfo)
							{
								_foundSpell = true;

								std::string name = spellInfo->SpellName[LOCALE_zhCN];
								if(name.empty()) name = spellInfo->SpellName[LOCALE_enUS];
								text.append(name);
								text.append(__STR("（点击激活）　|r"));
								pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(text), GOSSIP_SENDER_MAIN, __MENU_SKILL_STOLE + i + 100);
								text = __STR(__RED("　　　　　　　　↑↑（点击遗忘）"));
								pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(text), GOSSIP_SENDER_MAIN, __MENU_SKILL_STOLE + i + 200);
							}
						}
						
						if(!_foundSpell) 
						{
							text.append(__STR("无　|r"));
							pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);
						}
					}

					else //i >= e.data8
					{
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("＝＝　消耗５００点券开启法术槽　＝＝")), GOSSIP_SENDER_MAIN, __MENU_SKILL_STOLE + 300);
					}


				}
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_SKILL_STOLE);

				uint32_t _vip_special = sQZAchievements.GetVIPSpecialFeatures(pPlayer);
				if(_vip_special & VIP_SPECIAL_FREE_STOLE)
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝　超级ＶＩＰ免费冷却技能　＝＝")), GOSSIP_SENDER_MAIN, __MENU_SKILL_STOLE + 401);
				else
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝　消耗２点券冷却偷取技能　＝＝")), GOSSIP_SENDER_MAIN, __MENU_SKILL_STOLE + 400);
			}

			else if(action >= __MENU_SKILL_STOLE + 100 && action <= __MENU_SKILL_STOLE + 103)
			{
				//activate the skill
				uint32_t absAction = action - __MENU_SKILL_STOLE - 100;

				uint32_t spell_id = (absAction==0)?e.data1:
									(absAction==1)?e.data2:
									(absAction==2)?e.data3:
										   e.data4;

				if(spell_id!= 0)
				{
					sQZAchievements.SetSkillsCollectInfo(pPlayer, spell_id, 0);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　成功激活技能，返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SKILL_STOLE);
				}
				else
				{
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞　技能错误，联系老G　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SKILL_STOLE);
				}
			}

			else if(action >= __MENU_SKILL_STOLE + 200 && action <= __MENU_SKILL_STOLE + 203)
			{
				//forget the skill
				uint32_t absAction = action - __MENU_SKILL_STOLE - 200;

				sQZAchievements.SetSkillsCollectInfo(pPlayer, 0, absAction + 1);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　成功忘记技能，返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SKILL_STOLE);
			}

			else if(action == __MENU_SKILL_STOLE + 300)
			{
				//open more slots, need 500 voucher
				if(!pPlayer->HasItemCount(ZQ_ITEM_VOUCHER, 500))
				{
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞　需要500点券，返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SKILL_STOLE);
				}
				else
				{
					pPlayer->DestroyItemCount(ZQ_ITEM_VOUCHER, 500, true, true);
					sQZAchievements.SetSkillsCollectInfo(pPlayer, e.data8+1, 8);	
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　成功开启技能槽，返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SKILL_STOLE);
				}
			}

			else if(action == __MENU_SKILL_STOLE + 400)
			{
				//cooldown, need 1 voucher
				if(!pPlayer->HasItemCount(ZQ_ITEM_VOUCHER, 2))	
				{
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞　需要２点券，返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SKILL_STOLE);	
				}
				else
				{
					pPlayer->DestroyItemCount(ZQ_ITEM_VOUCHER, 2, true, true);
					//todo 
					// immediately finishes cooldown of stole and cast
					auto cdCheck = [](SpellEntry const & spellEntry) -> bool
					{
						if ((spellEntry.Id == ZQ_SPELL_ACHIEVE_STOLE  || spellEntry.Id == ZQ_SPELL_ACHIEVE_CAST ) && spellEntry.GetRecoveryTime() > 0)
							return true;
						return false;
					};
					pPlayer->RemoveSomeCooldown(cdCheck);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　成功冷却技能，返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SKILL_STOLE);
				}
			}

			else if(action == __MENU_SKILL_STOLE + 401)
			{
				//cooldown, need 1 voucher
				auto cdCheck = [](SpellEntry const & spellEntry) -> bool
				{
					if ((spellEntry.Id == ZQ_SPELL_ACHIEVE_STOLE  || spellEntry.Id == ZQ_SPELL_ACHIEVE_CAST ) && spellEntry.GetRecoveryTime() > 0)
						return true;
					return false;
				};
				pPlayer->RemoveSomeCooldown(cdCheck);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　成功冷却技能，返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_SKILL_STOLE);
			}

			//add menu
			pPlayer->SEND_GOSSIP_MENU(ZQ_GOSSIP_SPELL_STOLE, pPlayer->GetGUID());
			return ;
		}

		else if(action >= __MENU_ZITIAO_MAIN && action <= __MENU_ZITIAO_MAIN + __MENU_SIZE)
		{
			AchievementsEntry _aEntry = sQZAchievements.GetZitiaosInfo(pPlayer);
			auto localIdx = pPlayer->GetSession()->GetSessionDbLocaleIndex();
			std::string _str = "";

			#define _FREE_SLOTS  		(4)
			#define _NEED_VOUCHER(X)   (200*((X) - _FREE_SLOTS + 1))

			uint32 spell_id[8];

			//init the spell_id
			spell_id[0] = _aEntry.data1; spell_id[1] = _aEntry.data2; spell_id[2] = _aEntry.data3; spell_id[3] = _aEntry.data4;
			spell_id[4] = _aEntry.data5; spell_id[5] = _aEntry.data6; spell_id[6] = _aEntry.data7; spell_id[7] = _aEntry.data8;

			//check how many spells player has
			uint32 _spells_num_have = 0;
			for (size_t i = 0; i < 8; i++)
			{
				if(spell_id[i]!= 0)
				{
					_spells_num_have++;
				}
			}

			//Zitiao means equip spell effect extract, player can extract the spell effect from equipment;
			if(action == __MENU_ZITIAO_MAIN)
			{
				//Display the spells that player has
				//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝　已提取特效列表　＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝　点击字条新增或替换　＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
				_str = __STR("|cff002fa7＝＝你总共： ");
				_str.append(__NSTR(_aEntry.subType));
				_str.append(__STR("条，已使用 "));
				_str.append(__NSTR(_spells_num_have));
				_str.append(__STR("条＝＝|r "));
				
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(_str), GOSSIP_SENDER_MAIN, __MENU_NONE);
				for (size_t i = 0; i < 8; i++)
				{
					if(spell_id[i] != 0)
					{
						for (size_t j = 0; j < sizeof(__Spells_DBC_Array) / sizeof(__Spells_DBC_Array[0]); j++)
						{
							if (spell_id[i] == __Spells_DBC_Array[j].spell_learn_id)
							{
								_str = __STR("|cff066e22字条[");
								_str.append(__NSTR(i+1));
								_str.append(__STR("]=> "));
								_str.append(__Spells_DBC_Array[j].text);
								_str.append(__STR("r"));
								pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(_str), GOSSIP_SENDER_MAIN, __MENU_ZITIAO_MAIN + (i+1));
								break;
							}
						}
					}
					else
					{
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("＝＝＞　空字条、点击添加　＜＝＝　"), GOSSIP_SENDER_MAIN, __MENU_ZITIAO_MAIN + (i + 1));
					}
				}

				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
				return;
			}

			else if (action >= __MENU_ZITIAO_MAIN + 1 && action <= __MENU_ZITIAO_MAIN + 8)
			{
				//Display the spells that player has in this slot
				uint32 _slot_selected = action - __MENU_ZITIAO_MAIN - 1;
				
				if(spell_id[_slot_selected] == 0)
				{
					//Means this is a empty slot, so player can add a spell to this slot

					if(_spells_num_have >= (uint32)_aEntry.subType)
					{
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞　已达到最大提取数量　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
						_str = __STR("|cfff00019点击开启额外空槽，花费点券： ");
						_str.append(__NSTR(_NEED_VOUCHER(_spells_num_have)));
						_str.append(__STR("|r"));
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(_str), GOSSIP_SENDER_MAIN, __MENU_ZITIAO_MAIN + 100);
						pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
						return;
					}
				}

				// now display the spells that player has in this slot, and find the first item in bag
				// to add to this slot.
				// 1. display the spells that player has in this slot
				//Now need to find the first bag
				auto pItem = pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START);

				if (!pItem || (pItem->GetProto()->Class != ITEM_CLASS_WEAPON && pItem->GetProto()->Class != ITEM_CLASS_ARMOR) || pItem->GetProto()->Spells[0].SpellId == 0
					|| pItem->GetProto()->Quality > 4
					)
				{
					std::string text = __BLUE("[|请将可提取绿字字条的装备放在角色行囊里第一个格子|]");
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_ZITIAO_MAIN);
					pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());

					return;
				}

				// 2. find the first item in bag to add to this slot.
				//should declare the 3 spell id
				uint32 spell_id[3];
				uint32 spell_dbc_id[3];
				std::string spell_text[3];

				for (size_t i = 0; i < 3; i++)
				{
					spell_id[i] = 0; spell_dbc_id[i] = 0;
					spell_text[i] = __STR("|该字条无法转移|");

					spell_id[i] = pItem->GetProto()->Spells[i].SpellId;

					//sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "Item spell: %d", spell_id[i]);
					if (spell_id[i] != 0)
					{
						for (size_t j = 0; j < sizeof(__Spells_DBC_Array) / sizeof(__Spells_DBC_Array[0]); j++)
						{
							if (spell_id[i] == __Spells_DBC_Array[j].spell_id)
							{
								spell_dbc_id[i] = __Spells_DBC_Array[j].dbc_id;
								spell_text[i] = __Spells_DBC_Array[j].text;
								break;
							}
						}
					}
				}

				// 3. add the item to this slot.
				std::string text = __RED("[待提取装备]");
				auto item_destroy_local = sObjectMgr.GetItemLocale(pItem->GetProto()->ItemId);
				auto item_destroy_text = (item_destroy_local == nullptr ? pItem->GetProto()->Name1 : item_destroy_local->Name[localIdx]);
				text.append(item_destroy_text);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);

				for (size_t i = 0; i < 3; i++)
				{
					if (spell_id[i])
					{
						text = __GREEN("提取字条＝＞");
						text.append(spell_text[i]);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(text), GOSSIP_SENDER_MAIN, __MENU_ZITIAO_MAIN + 200 + _slot_selected * 10 + i);
					}
				}
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
			}

			else if (action >= __MENU_ZITIAO_MAIN + 100 && action <= __MENU_ZITIAO_MAIN + 199)
			{
				if(action == __MENU_ZITIAO_MAIN + 100)
				{
					//Means player wants to add a empty slot to the zitiaos info
					if(!pPlayer->HasItemCount(ZQ_ITEM_VOUCHER, _NEED_VOUCHER(_spells_num_have)))	
					{
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝＞　点券不够，返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_ZITIAO_MAIN);
						pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
						return;	
					}

					else
					{
						pPlayer->DestroyItemCount(ZQ_ITEM_VOUCHER, _NEED_VOUCHER(_spells_num_have), true, true);
						_aEntry.subType++;
						sQZAchievements.SetZitiaosInfo(pPlayer, _aEntry);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＞　开通成功，返回　＜＝＝　")), GOSSIP_SENDER_MAIN, __MENU_ZITIAO_MAIN);
						pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
						return;
					}
				}
			}

			else if (action >= __MENU_ZITIAO_MAIN + 200 && action <= __MENU_ZITIAO_MAIN + 299)
			{
				//Means player wants to add a spell to this slot
				uint32 _fb_slot_selected = (action - __MENU_ZITIAO_MAIN - 200)/10;
				uint32 _item_spell_selected = (action - __MENU_ZITIAO_MAIN - 200)%10;
				
				uint32 _oldSpellId = spell_id[_fb_slot_selected];
				uint32 _newSpellId = 0;

				auto pItem = pPlayer->GetItemByPos(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START);
				if (!pItem || (pItem->GetProto()->Class != ITEM_CLASS_WEAPON && pItem->GetProto()->Class != ITEM_CLASS_ARMOR) || pItem->GetProto()->Spells[_item_spell_selected].SpellId == 0
					|| pItem->GetProto()->Quality > 4)
				{
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __RED("此装备错误！请联系GM。。 "), GOSSIP_SENDER_MAIN, __MENU_ZITIAO_MAIN);
					pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
					return;
				}

				//iterate the spelllist
				for (size_t j = 0; j < sizeof(__Spells_DBC_Array) / sizeof(__Spells_DBC_Array[0]); j++)
				{
					if (pItem->GetProto()->Spells[_item_spell_selected].SpellId == __Spells_DBC_Array[j].spell_id)
					{
						//found the spell id, now check if the spell is in the spelllist
						_newSpellId = __Spells_DBC_Array[j].spell_learn_id;
						break;
					}
				}

				if(!_newSpellId || _oldSpellId == _newSpellId || pPlayer->HasSpell(_newSpellId))
				{
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __RED("玩家已经有此字条，或装备错误！请联系GM。。 "), GOSSIP_SENDER_MAIN, __MENU_ZITIAO_MAIN);		
					pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
					return;
				}
				else
				{
					if(_oldSpellId != 0)
					{
						pPlayer->RemoveSpell(_oldSpellId);
					}

					pPlayer->LearnSpell(_newSpellId, false);

					spell_id[_fb_slot_selected] = _newSpellId;
					_aEntry.data1 = spell_id[0]; _aEntry.data2 = spell_id[1]; _aEntry.data3 = spell_id[2]; _aEntry.data4 = spell_id[3];
					_aEntry.data5 = spell_id[4]; _aEntry.data6 = spell_id[5]; _aEntry.data7 = spell_id[6]; _aEntry.data8 = spell_id[7];
					sQZAchievements.SetZitiaosInfo(pPlayer, _aEntry);

					//delete the item
					pPlayer->DestroyItem(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START, true);

					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __GREEN("字条提取成功，返回查看！"), GOSSIP_SENDER_MAIN, __MENU_ZITIAO_MAIN);
					pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pPlayer->GetGUID());
					return;
				}
			}
		}

		
		else if(action >= __MENU_RUNE_MAIN && action <= __MENU_RUNE_MAIN + __MENU_SIZE)
		{
			Menus_Rune_Main(pPlayer, pCreature, sender, action);
		}
		/*
		else if(action >= __MENU_FRAG_MAIN && action <= __MENU_FRAG_MAIN + __MENU_SIZE)
		{
			Menus_Frag_Main(pPlayer, pCreature, sender, action);
		}*/
		
		else if(action >= __MENU_ACHIEVE_MAIN && action <= __MENU_ACHIEVE_MAIN + __MENU_SIZE)
		{
			Menus_Achieve_Main(pPlayer, pCreature, sender, action);
		}

		else if(action >= __MENU_VIP_TELEPORT_MAIN && action <= __MENU_VIP_TELEPORT_MAIN + 10000)
		{
			Menus_Vip_Teleport_Main(pPlayer, pCreature, sender, action);
		}

	}
};

SpellScript* GetScript_CustomHSSpell(SpellEntry const*)
{
	return new CustomHSSpell();
}

void AddSC_qzqstar_spell_hs()
{
	Script* newscript2;
	newscript2 = new Script;
	newscript2->Name = "qzqstar_spell_hs";
	newscript2->GetSpellScript = &GetScript_CustomHSSpell;
	//newscript2->pGossipSelect = &GossipHello_HS;
	newscript2->RegisterSelf(false);
}


