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

#include "qzqstar_rune_str.h"
#include "qzqstar_helper.h"

#include "chat.h"

#include "QzqstarAchievements.h"


// RUNE NPC
#define	__MENU_NONE					0
#define __MENU_SIZE					(999)
#define __MENU_TISH_MAIN			1000
#define __MENU_COMM_MAIN			2000
#define __MENU_RUNE_MAIN			3000
#define __MENU_FRAG_MAIN			4000
#define __MENU_REPU_MAIN			5000
#define __MENU_PEAK_MAIN			6000
#define __MENU_VOUCHER_MAIN			9000

#define	__STR(x)		((std::string)(x)).c_str()
#define	__NSTR(x)		(std::to_string(x))
	
#define	__BLUE(x)		"|cff002fa7"##x##"|r"
#define	__GREEN(x)		"|cff00b72f"##x##"|r"
#define	__ORANGE(x)		"|cffe85827"##x##"|r"
#define __RED(x)		"|cfff00019"##x##"|r"
#define __YELLOW(x)		"|cfff9dc24"##x##"|r"
#define __PINK(x)		"|cff800080"##x##"|r"



//Copy the content of const menus to player specific 
void X_copy_rune(const Rune_Spell_Menu_t *from, Rune_Spell_Menu_t *to, uint32 size)
{
	memcpy(to, from, sizeof(Rune_Spell_Menu_t)*size);
}

#pragma region Tisheng Menus
bool Menus_Tish_Main(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{

	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　通用符文　＜＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_COMM_MAIN);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　职业符文　＜＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　碎片提升　＜＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　兑换声望　＜＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_REPU_MAIN);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);

	if (player->GetLevel() >= 60) 
	{
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　巅峰等级　＜＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_PEAK_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);
	}
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
	//if (player->GetLevel() >= 60) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("==下赛季继承点券查询 | 不断完善中 | (赞助另算)=="), GOSSIP_SENDER_MAIN, __MENU_OTHER_MAIN);
	
	//if(player->GetLevel() >= 60 && player->HasSpell(32990) &&(!player->HasSpell(32988)) )	player->ADD_GOSSIP_ITEM(5, __STR(__BLUE("=== |继续 一命自强模式| ====")), GOSSIP_SENDER_MAIN, __MENU_ZQ_MAIN+100);

	player->SEND_GOSSIP_MENU(16001, _Creature->GetGUID());
	return true;
}
#pragma endregion

#pragma region MENU COMM RUNE

bool Menus_Comm_Main(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
	if (!player || !_Creature) return false;


	return true;
}
#pragma endregion

#pragma region MENU RUNE
#define __MENU_RUNE_SLOT_LEARN		(100)	//1100-1199 learn slot spell
#define __MENU_RUNE_SLOT_FORGET		(200)	//2100-2199 forget slot spell
#define __MENU_RUNE_SLOT_BUY		(800)	//buy rune slot

#define	__RUNE_UPGRADE_ITEM_ALL			(30136)	//ALL

#define __MODE_KILLER       		(30849)
#define __MODE_KILLER_REWARD       	(30852)
uint32 X__rune_slot_numbers(Player *player)
{
	//will have one slot every level/15
	uint32 _nums = sQZAchievements.GetRuneSlots(player);

	//check if has the __MODE_KILLER
	//if (player->HasSpell(__MODE_KILLER) || player->HasSpell(__MODE_KILLER_REWARD)) _nums += 1;

	//extra slots
	return _nums;
}

uint32 X__rune_need_num(Player *player)
{
	auto needNum = 0;
	auto freeNum = 2;
	//if (player->HasSpell(__MODE_KILLER) || player->HasSpell(__MODE_KILLER_REWARD)) freeNum += 1;

	int32 curslots = X__rune_slot_numbers(player);

	if (curslots < freeNum) needNum = 0;
	else
	{
		needNum = 10 * (curslots - freeNum + 1) * (curslots - freeNum + 1);
	}
	return needNum;
}

bool X_Menus_Rune_Main(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
	return true;
}
bool X_Menus_Rune_Main1(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
	if (!player || !_Creature) return false;

	uint32 _MAX_RUNE_SLOT = 0;
	uint32 __RUNE_UPGRADE_ITEM = 0;
	uint32 __RUNE_UPGRADE_ITEM_BASIC = 10;
	uint32 _CLASS_DESC = DEFAULT_GOSSIP_MESSAGE;

	//All player enter this menu, and then got the class menu belongs to him
	Rune_Spell_Menu_t _LocalMenus[RUNE_SPELLS_NUM];

	if (player->GetClass() == CLASS_WARRIOR) { X_copy_rune(_Spells_Menu_Warrior, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16030; }
	else if (player->GetClass() == CLASS_MAGE) { X_copy_rune(_Spells_Menu_Mage, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16030; }
	else if (player->GetClass() == CLASS_ROGUE) { X_copy_rune(_Spells_Menu_Rogue, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16030; }
	else if (player->GetClass() == CLASS_PALADIN) { X_copy_rune(_Spells_Menu_Paladin, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16030; }
	else if (player->GetClass() == CLASS_DRUID) { X_copy_rune(_Spells_Menu_Druid, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16030; }
	else if (player->GetClass() == CLASS_HUNTER) { X_copy_rune(_Spells_Menu_Hunter, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16030; }
	else if (player->GetClass() == CLASS_PRIEST) { X_copy_rune(_Spells_Menu_Priest, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16030; }
	else if (player->GetClass() == CLASS_WARLOCK) { X_copy_rune(_Spells_Menu_Warlock, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16030; }
	else if (player->GetClass() == CLASS_SHAMAN) { X_copy_rune(_Spells_Menu_Shaman, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16030; }
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
	_MAX_RUNE_SLOT = X__rune_slot_numbers(player);

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

		text = "|cff002fa7功能：　";
		text.append(__STR(_LocalMenus[spell_to_learn_slot].desc));
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
			/*
			player->LearnSpell(_LocalMenus[spell_to_learn_slot].spell_id, false);
			player->ADD_GOSSIP_ITEM(5, __STR(__RED("====| 学习成功! |=====")), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN);*/
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

		/*
		text = __RED("确定要遗忘此技能?-->");
		text.append(_LocalMenus[spell_to_delete_slot].text);
		text.append(__RED("<-- 返还符文石数量Ｘ "));
		text.append(__NSTR(X__rune_need_num(player, _player_learned_num - 1) / 2));

		player->ADD_GOSSIP_ITEM(5, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(5, __STR(__RED("====| 确定 | 返回 | =====")), GOSSIP_SENDER_MAIN, action + RUNE_CONFIRM_OFFSET);
		player->ADD_GOSSIP_ITEM(5, __STR(__GREEN("====| 取消 | 返回 |=====")), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN);
		*/
		player->RemoveSpell(_LocalMenus[spell_to_delete_slot].spell_id);
		player->ADD_GOSSIP_ITEM(5, __STR(__GREEN("====已经遗忘, |返回|=====")), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN);
	}
	//confirm the spell
	/*
	else if ((action >= __MENU_RUNE_MAIN + 130 + RUNE_CONFIRM_OFFSET) && (action <= __MENU_RUNE_MAIN + 139 + RUNE_CONFIRM_OFFSET))
	{
		uint32 spell_to_delete_slot = action - (__MENU_RUNE_MAIN + 130 + RUNE_CONFIRM_OFFSET);

		if (player && player->HasSpell(_LocalMenus[spell_to_delete_slot].spell_id))
		{
			player->RemoveSpell(_LocalMenus[spell_to_delete_slot].spell_id);

			//add runes back to players
			if((_player_learned_num > 0) && (X__rune_need_num(player, _player_learned_num-1) > 0))
				player->AddItem(__RUNE_UPGRADE_ITEM,  (X__rune_need_num(player, _player_learned_num-1)) / 2 );

			player->ADD_GOSSIP_ITEM(5, __STR(__GREEN("====已经遗忘, |返回|=====")), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN);
		}
		else
			player->ADD_GOSSIP_ITEM(5, __STR(__RED("====出错, |返回|=====")), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN);
	}
	*/
	else if ((action >= __MENU_RUNE_MAIN + __MENU_RUNE_SLOT_BUY) && (action <= __MENU_RUNE_MAIN + __MENU_RUNE_SLOT_BUY + 99))
	{
		uint32 _realAction = action - (__MENU_RUNE_MAIN + __MENU_RUNE_SLOT_BUY);

		//get the rune stones that player need to buy
		uint32 _rune_stones_need = 0;
		_rune_stones_need = X__rune_need_num(player);
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

#pragma region MENU FRAG



#define	__FRAG_MAX_RANK							(3)					//Fragments only 3 ranks
#define	__FRAG_ITEM_NEED(val)					(20 * (val + 1))	//20, 40, 60... frags


#define	__MENU_FRAG_KILLER_ITEM					(30746)
//Killer 0, 人形、巨人
#define __MENU_FRAG_SUB_0						(__MENU_FRAG_MAIN + 10)
#define	__MENU_FRAG_SUB_0_NAME					"[杀手||人形,巨人 (每级30点伤害)]"
#define __MENU_FRAG_SUB_0_ACT_1					(__MENU_FRAG_SUB_0 + 1)
#define __MENU_FRAG_SUB_0_SPELL					(32457)
//Killer 1, 野兽、龙类
#define __MENU_FRAG_SUB_1						(__MENU_FRAG_MAIN + 20)
#define	__MENU_FRAG_SUB_1_NAME					"[杀手||野兽,龙类 (每级30点伤害)]"
#define __MENU_FRAG_SUB_1_ACT_1					(__MENU_FRAG_SUB_1 + 1)
#define __MENU_FRAG_SUB_1_SPELL					(__MENU_FRAG_SUB_0_SPELL + 3)
//Killer 2, 亡灵、恶魔
#define __MENU_FRAG_SUB_2						(__MENU_FRAG_MAIN + 30)
#define	__MENU_FRAG_SUB_2_NAME					"[杀手||亡灵,恶魔 (每级30点伤害)]"
#define __MENU_FRAG_SUB_2_ACT_1					(__MENU_FRAG_SUB_2 + 1)
#define __MENU_FRAG_SUB_2_SPELL					(__MENU_FRAG_SUB_1_SPELL + 3)

//攻防碎片
#define	__MENU_FRAG_GF_ITEM						(30746)
#define __MENU_FRAG_SUB_3						(__MENU_FRAG_MAIN + 40)
#define	__MENU_FRAG_SUB_3_NAME					"[物理攻防 (提升/减免物理5%伤害)]"
#define __MENU_FRAG_SUB_3_ACT_1					(32466)
#define __MENU_FRAG_SUB_3_SPELL					(__MENU_FRAG_SUB_2_SPELL + 3)

#define __MENU_FRAG_SUB_4						(__MENU_FRAG_MAIN + 50)
#define	__MENU_FRAG_SUB_4_NAME					"[火焰|暗影攻防 (提升/减免该系5%伤害)]"
#define __MENU_FRAG_SUB_4_ACT_1					(__MENU_FRAG_SUB_4 + 1)
#define __MENU_FRAG_SUB_4_SPELL					(__MENU_FRAG_SUB_3_SPELL + 3)

#define __MENU_FRAG_SUB_5						(__MENU_FRAG_MAIN + 60)
#define	__MENU_FRAG_SUB_5_NAME					"[冰霜|奥术攻防 (提升/减免该系5%伤害)]"
#define __MENU_FRAG_SUB_5_ACT_1					(__MENU_FRAG_SUB_5 + 1)
#define __MENU_FRAG_SUB_5_SPELL					(__MENU_FRAG_SUB_4_SPELL + 3)

#define __MENU_FRAG_SUB_6						(__MENU_FRAG_MAIN + 70)
#define	__MENU_FRAG_SUB_6_NAME					"[神圣|自然攻防 (提升/减免该系5%伤害)]"
#define __MENU_FRAG_SUB_6_ACT_1					(__MENU_FRAG_SUB_6 + 1)
#define __MENU_FRAG_SUB_6_SPELL					(__MENU_FRAG_SUB_5_SPELL + 3)

//杀手
#define __MENU_FRAG_SUB_70								(__MENU_FRAG_MAIN + 100)
#define	__MENU_FRAG_SUB_70_NAME							"[杀手碎片兑换]"
#define __MENU_FRAG_SUB_70_ACT_1						(__MENU_FRAG_SUB_70 + 1)
#define __MENU_FRAG_SUB_70_ACT_2						(__MENU_FRAG_SUB_70 + 2)
#define __MENU_FRAG_SUB_70_ACT_3						(__MENU_FRAG_SUB_70 + 3)
#define __MENU_FRAG_SUB_70_ACT_1_SPELL					(32457)
#define __MENU_FRAG_SUB_70_ACT_2_SPELL					(__MENU_FRAG_SUB_70_ACT_1_SPELL+3)
#define __MENU_FRAG_SUB_70_ACT_3_SPELL					(__MENU_FRAG_SUB_70_ACT_2_SPELL+3)

//攻防
#define __MENU_FRAG_SUB_80								(__MENU_FRAG_MAIN + 150)
#define	__MENU_FRAG_SUB_80_NAME							"[攻防碎片兑换]"
#define __MENU_FRAG_SUB_80_ACT_1						(__MENU_FRAG_SUB_80 + 1)
#define __MENU_FRAG_SUB_80_ACT_2						(__MENU_FRAG_SUB_80 + 2)
#define __MENU_FRAG_SUB_80_ACT_3						(__MENU_FRAG_SUB_80 + 3)
#define __MENU_FRAG_SUB_80_ACT_4						(__MENU_FRAG_SUB_80 + 4)
#define __MENU_FRAG_SUB_80_ACT_1_SPELL					(32466)
#define __MENU_FRAG_SUB_80_ACT_2_SPELL					(__MENU_FRAG_SUB_80_ACT_1_SPELL+3)
#define __MENU_FRAG_SUB_80_ACT_3_SPELL					(__MENU_FRAG_SUB_80_ACT_2_SPELL+3)
#define __MENU_FRAG_SUB_80_ACT_4_SPELL					(__MENU_FRAG_SUB_80_ACT_3_SPELL+3)

//宠物， 施法， 武器大师， 只有一个等级， 需要30点。
//武器大师
#define	__MENU_FRAG_WEAPON_ITEM							(30746)
#define __MENU_FRAG_SUB_90								(__MENU_FRAG_MAIN + 200)
#define	__MENU_FRAG_SUB_90_NAME							"[武器大师||施法加速||宠物提升]"
#define __MENU_FRAG_SUB_90_ACT_1						(__MENU_FRAG_SUB_90 + 1)
#define __MENU_FRAG_SUB_90_ACT_2						(__MENU_FRAG_SUB_90 + 2)
#define __MENU_FRAG_SUB_90_ACT_3						(__MENU_FRAG_SUB_90 + 3)
#define __MENU_FRAG_SUB_90_ACT_4						(__MENU_FRAG_SUB_90 + 4)
#define __MENU_FRAG_SUB_90_ACT_5						(__MENU_FRAG_SUB_90 + 5)
#define __MENU_FRAG_SUB_90_ACT_6						(__MENU_FRAG_SUB_90 + 6)
#define __MENU_FRAG_SUB_90_ACT_7						(__MENU_FRAG_SUB_90 + 7)

#define __MENU_FRAG_SUB_90_ACT_1_SPELL					(32410)
#define __MENU_FRAG_SUB_90_ACT_2_SPELL					(32411)
#define __MENU_FRAG_SUB_90_ACT_3_SPELL					(32412)
#define __MENU_FRAG_SUB_90_ACT_4_SPELL					(32413)
#define __MENU_FRAG_SUB_90_ACT_5_SPELL					(32414)
#define __MENU_FRAG_SUB_90_ACT_6_SPELL					(32456)	//SF
#define __MENU_FRAG_SUB_90_ACT_7_SPELL					(32455)	//Pet


bool Menus_Frag_Main(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{

	if (!player || !_Creature) return false;

	std::string text = "";
	//RANK 3
	int val_killer_0 = 0;
	int val_killer_1 = 0;
	int val_killer_2 = 0;
	int val_gf_0	= 0;
	int val_gf_1	= 0;
	int val_gf_2	= 0;
	int val_gf_3	= 0;
	//Rank ONE
	int val_pet		 = 0;
	int val_sf		 = 0;
	int val_weapon_0 = 0;
	int val_weapon_1 = 0;
	int val_weapon_2 = 0;
	int val_weapon_3 = 0;
	int val_weapon_4 = 0;

	//step 1, get all of the spells of player
	//1.1 find the frag spells, support max 3 spells (MAX_RANK)
	for (size_t i = 0; i < __FRAG_MAX_RANK; i++)
	{
		if (player->HasSpell(__MENU_FRAG_SUB_70_ACT_1_SPELL + i))	val_killer_0++;
		if (player->HasSpell(__MENU_FRAG_SUB_70_ACT_2_SPELL + i))	val_killer_1++;
		if (player->HasSpell(__MENU_FRAG_SUB_70_ACT_3_SPELL + i))	val_killer_2++;
		if (player->HasSpell(__MENU_FRAG_SUB_80_ACT_1_SPELL + i))	val_gf_0++;
		if (player->HasSpell(__MENU_FRAG_SUB_80_ACT_2_SPELL + i))	val_gf_1++;
		if (player->HasSpell(__MENU_FRAG_SUB_80_ACT_3_SPELL + i))	val_gf_2++;
		if (player->HasSpell(__MENU_FRAG_SUB_80_ACT_4_SPELL + i))	val_gf_3++;
	}

	//1.2 rank one
	if (player->HasSpell(__MENU_FRAG_SUB_90_ACT_7_SPELL))	val_pet ++;
	if (player->HasSpell(__MENU_FRAG_SUB_90_ACT_6_SPELL))	val_sf ++;
	if (player->HasSpell(__MENU_FRAG_SUB_90_ACT_1_SPELL))	val_weapon_0++;
	if (player->HasSpell(__MENU_FRAG_SUB_90_ACT_2_SPELL))	val_weapon_1++;
	if (player->HasSpell(__MENU_FRAG_SUB_90_ACT_3_SPELL))	val_weapon_2++;
	if (player->HasSpell(__MENU_FRAG_SUB_90_ACT_4_SPELL))	val_weapon_3++;
	if (player->HasSpell(__MENU_FRAG_SUB_90_ACT_5_SPELL))	val_weapon_4++;
	
	sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "PLAYER:[%u][%s] = Kill: %d, %d, %d. GF %d, %d, %d, %d. Pet:%d, SF:%d, WP: %d, %d, %d, %d, %d.",
		player->GetGUID(), player->GetName(),
		val_killer_0, val_killer_1, val_killer_2,
		val_gf_0, val_gf_1, val_gf_2, val_gf_3,
		val_pet, val_sf,
		val_weapon_0, val_weapon_1, val_weapon_2, val_weapon_3, val_weapon_4);

	switch (action)
	{
	case __MENU_FRAG_MAIN: 
		
		player->ADD_GOSSIP_ITEM(5, __STR("=======碎片兑换系统========"), GOSSIP_SENDER_MAIN, __MENU_NONE);
		//Killer
		text = __BLUE(__MENU_FRAG_SUB_70_NAME);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_FRAG_SUB_70);
		//GF
		text = __BLUE(__MENU_FRAG_SUB_80_NAME);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_FRAG_SUB_80);
		//Weapons
		text = __BLUE(__MENU_FRAG_SUB_90_NAME);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_FRAG_SUB_90);

		player->ADD_GOSSIP_ITEM(5, "============================", GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(5, __STR("<===返回===="), GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);

		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		break;

#pragma region __MENU_KILLERS

	//-------------- KILLER 杀手 ---------------------------
	case __MENU_FRAG_SUB_70: // KILLERS
	{
		auto __val = 0;

		//killer 0
		text = __BLUE("[杀手|| 人形,巨人]"); __val = val_killer_0;
		if (__val<__FRAG_MAX_RANK) {
			text.append("当前伤害:+ " + __NSTR(__val * 30) + " ，最高９０伤害，升级需要碎片数量： 　" + __NSTR(__FRAG_ITEM_NEED(__val)));
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_FRAG_SUB_70_ACT_1);}
		else { text.append(__GREEN("伤害+90.(完成)")); player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE); }

		//killer 1
		text = __BLUE("[杀手|| 野兽,龙类]"); __val = val_killer_1;
		if (__val<__FRAG_MAX_RANK) {
			text.append("当前伤害:+ " + __NSTR(__val * 30) + " ，最高９０伤害，升级需要碎片数量： 　" + __NSTR(__FRAG_ITEM_NEED(__val)));
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_FRAG_SUB_70_ACT_2);}
		else { text.append(__GREEN("伤害+90.(完成)")); player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE); }

		//killer 2
		text = __BLUE("[杀手|| 亡灵,恶魔,元素]"); __val = val_killer_2;
		if (__val<__FRAG_MAX_RANK) {
			text.append("当前伤害:+ " + __NSTR(__val * 30) + " ，最高９０伤害，升级需要碎片数量： 　" + __NSTR(__FRAG_ITEM_NEED(__val)));
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_FRAG_SUB_70_ACT_3);}
		else { text.append(__GREEN("伤害+90.(完成)")); player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE); }

		player->ADD_GOSSIP_ITEM(5, "<==取消===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		break;
	}
	case __MENU_FRAG_SUB_70_ACT_1: //Confirm and check  ...
	{	auto __val = val_killer_0;
		if ( (player->HasItemCount(__MENU_FRAG_KILLER_ITEM, __FRAG_ITEM_NEED(__val)))){   
			player->DestroyItemCount(__MENU_FRAG_KILLER_ITEM, __FRAG_ITEM_NEED(__val), true);
			player->LearnSpell(__MENU_FRAG_SUB_70_ACT_1_SPELL + __val, false); //false should be ?
			player->SaveToDB();	player->ADD_GOSSIP_ITEM(5, "<== 提升成功，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);}
		else player->ADD_GOSSIP_ITEM(5, "<== 碎片不够，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		break;
	}
	case __MENU_FRAG_SUB_70_ACT_2: //Confirm and check  ...
	{	auto __val = val_killer_1;
		if ( (player->HasItemCount(__MENU_FRAG_KILLER_ITEM, __FRAG_ITEM_NEED(__val)))){   
			player->DestroyItemCount(__MENU_FRAG_KILLER_ITEM, __FRAG_ITEM_NEED(__val), true);
			player->LearnSpell(__MENU_FRAG_SUB_70_ACT_2_SPELL + __val, false); //false should be ?
			player->SaveToDB();	player->ADD_GOSSIP_ITEM(5, "<== 提升成功，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);}
		else player->ADD_GOSSIP_ITEM(5, "<== 碎片不够，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		break;
	}
	case __MENU_FRAG_SUB_70_ACT_3: //Confirm and check  ...
	{	auto __val = val_killer_2;
		if ( (player->HasItemCount(__MENU_FRAG_KILLER_ITEM, __FRAG_ITEM_NEED(__val)))){   
			player->DestroyItemCount(__MENU_FRAG_KILLER_ITEM, __FRAG_ITEM_NEED(__val), true);
			player->LearnSpell(__MENU_FRAG_SUB_70_ACT_3_SPELL + __val, false); //false should be ?
			player->SaveToDB();	player->ADD_GOSSIP_ITEM(5, "<== 提升成功，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);}
		else player->ADD_GOSSIP_ITEM(5, "<== 碎片不够，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		break;
	}

#pragma endregion


#pragma region __MENU__GF

	//-------------- 攻防 ---------------------------
	case __MENU_FRAG_SUB_80: // GF
	{
		auto __val = 0;

		//gf 0
		text = __BLUE("[攻防|| 物理]"); __val = val_gf_0;
		if (__val<__FRAG_MAX_RANK) {
			text.append("该系伤害提升、减免，当前： +" + __NSTR(__val * 5) + " ％（最高１５％）升级需要碎片数量: " + __NSTR(__FRAG_ITEM_NEED(__val)));
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_FRAG_SUB_80_ACT_1);
		}
		else { text.append(__GREEN("伤害提升,减免:+15%.(完成)")); player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE); }

		//gf 1
		text = __BLUE("[攻防|| 火焰,暗影]"); __val = val_gf_1;
		if (__val<__FRAG_MAX_RANK) {
			text.append("该系伤害提升、减免，当前： +" + __NSTR(__val * 5) + " ％（最高１５％）升级需要碎片数量: " + __NSTR(__FRAG_ITEM_NEED(__val)));
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_FRAG_SUB_80_ACT_2);
		}
		else { text.append(__GREEN("伤害提升,减免:+15%.(完成)")); player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE); }

		//gf 2
		text = __BLUE("[攻防|| 冰霜,奥术]"); __val = val_gf_2;
		if (__val<__FRAG_MAX_RANK) {
			text.append("该系伤害提升、减免，当前： +" + __NSTR(__val * 5) + " ％（最高１５％）升级需要碎片数量: " + __NSTR(__FRAG_ITEM_NEED(__val)));
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_FRAG_SUB_80_ACT_3);
		}
		else { text.append(__GREEN("伤害提升,减免:+15%.(完成)")); player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE); }
		
		//gf 3
		text = __BLUE("[攻防|| 神圣,自然]"); __val = val_gf_3;
		if (__val<__FRAG_MAX_RANK) {
			text.append("该系伤害提升、减免，当前： +" + __NSTR(__val * 5) + " ％（最高１５％）升级需要碎片数量: " + __NSTR(__FRAG_ITEM_NEED(__val)));
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_FRAG_SUB_80_ACT_4);
		}
		else { text.append(__GREEN("伤害提升,减免:+15%.(完成)")); player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE); }

		player->ADD_GOSSIP_ITEM(5, "<==取消===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		break;
	}
	case __MENU_FRAG_SUB_80_ACT_1: //Confirm and check  ...
	{	auto __val = val_gf_0;
		if ((player->HasItemCount(__MENU_FRAG_GF_ITEM, __FRAG_ITEM_NEED(__val)))) {
			player->DestroyItemCount(__MENU_FRAG_GF_ITEM, __FRAG_ITEM_NEED(__val), true);
			player->LearnSpell(__MENU_FRAG_SUB_80_ACT_1_SPELL + __val, false); //false should be ?
			player->SaveToDB();	player->ADD_GOSSIP_ITEM(5, "<== 碎片学习成功，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);}
		else player->ADD_GOSSIP_ITEM(5, "<== 提升碎片不够，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		break;
	}
	case __MENU_FRAG_SUB_80_ACT_2: //Confirm and check  ...
	{	auto __val = val_gf_1;
		if ((player->HasItemCount(__MENU_FRAG_GF_ITEM, __FRAG_ITEM_NEED(__val)))) {
			player->DestroyItemCount(__MENU_FRAG_GF_ITEM, __FRAG_ITEM_NEED(__val), true);
			player->LearnSpell(__MENU_FRAG_SUB_80_ACT_2_SPELL + __val, false); //false should be ?
			player->SaveToDB();	player->ADD_GOSSIP_ITEM(5, "<== 碎片学习成功，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);}
		else player->ADD_GOSSIP_ITEM(5, "<== 提升碎片不够，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		break;
	}
	case __MENU_FRAG_SUB_80_ACT_3: //Confirm and check  ...
	{	auto __val = val_gf_2;
		if ((player->HasItemCount(__MENU_FRAG_GF_ITEM, __FRAG_ITEM_NEED(__val)))) {
			player->DestroyItemCount(__MENU_FRAG_GF_ITEM, __FRAG_ITEM_NEED(__val), true);
			player->LearnSpell(__MENU_FRAG_SUB_80_ACT_3_SPELL + __val, false); //false should be ?
			player->SaveToDB();	player->ADD_GOSSIP_ITEM(5, "<== 碎片学习成功，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);}
		else player->ADD_GOSSIP_ITEM(5, "<== 提升碎片不够，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		break;
	}
	case __MENU_FRAG_SUB_80_ACT_4: //Confirm and check  ...
	{	auto __val = val_gf_3;
		if ((player->HasItemCount(__MENU_FRAG_GF_ITEM, __FRAG_ITEM_NEED(__val)))) {
			player->DestroyItemCount(__MENU_FRAG_GF_ITEM, __FRAG_ITEM_NEED(__val), true);
			player->LearnSpell(__MENU_FRAG_SUB_80_ACT_4_SPELL + __val, false); //false should be ?
			player->SaveToDB();	player->ADD_GOSSIP_ITEM(5, "<== 碎片学习成功，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);}
		else player->ADD_GOSSIP_ITEM(5, "<== 提升碎片不够，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		break;
	}


#pragma endregion


#pragma region __MENU__WEAPONS

	//Weapons
	//-------------- WEAPONS ---------------------------
	case __MENU_FRAG_SUB_90: //武器大师
	{
		text = __BLUE("[剑|双手剑|法杖]"); 
		if (!val_weapon_0) {
			text.append("技能提升需要３０个提升碎片。　");
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_FRAG_SUB_90_ACT_1);}
		else { text.append(__GREEN("武器技能 +8.(完成)")); player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE); }

		text = __BLUE("[锤|双手锤|魔杖]");
		if (!val_weapon_1) {
			text.append("技能提升需要３０个提升碎片。　");
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_FRAG_SUB_90_ACT_2);
		}
		else { text.append(__GREEN("武器技能＋８（完成）　")); player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE); }

		text = __BLUE("[斧|双手斧|长柄]");
		if (!val_weapon_2) {
			text.append("技能提升需要３０个提升碎片。　");
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_FRAG_SUB_90_ACT_3);
		}
		else { text.append(__GREEN("武器技能＋８（完成）　")); player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE); }

		text = __BLUE("[弓|弩|枪械]");
		if (!val_weapon_3) {
			text.append("技能提升需要３０个提升碎片。　");
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_FRAG_SUB_90_ACT_4);
		}
		else { text.append(__GREEN("武器技能＋８（完成）　")); player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE); }

		text = __BLUE("[匕首|拳套|投掷]");
		if (!val_weapon_4) {
			text.append("技能提升需要３０个提升碎片。　");
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_FRAG_SUB_90_ACT_5);
		}
		else { text.append(__GREEN("武器技能＋８（完成）　")); player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE); }

		text = __BLUE("[施法加速（提升施法速度１０％）]");
		if (!val_sf) {
			text.append("技能提升需要５０个提升碎片。　");
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_FRAG_SUB_90_ACT_6);
		}
		else { text.append(__GREEN("提升＋１０％（完成）　")); player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE); }

		text = __BLUE("[宠物增强（提升宠物３０％伤害，受到伤害降低３０％）]");
		if (!val_pet) {
			text.append("技能提升需要５０个提升碎片。　");
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_FRAG_SUB_90_ACT_7);
		}
		else { text.append(__GREEN("宠物增强＋３０％（完成）　")); player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE); }


		player->ADD_GOSSIP_ITEM(5, "<==取消===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		break;
	}
	case __MENU_FRAG_SUB_90_ACT_1: //Weapon 1
	{	//confirmed, just remove the pearls and then learn the spell, return to 
		if ((player->HasItemCount(__MENU_FRAG_WEAPON_ITEM, 30)))
		{	player->DestroyItemCount(__MENU_FRAG_WEAPON_ITEM, 30, true);
			player->LearnSpell(__MENU_FRAG_SUB_90_ACT_1_SPELL, false); 
			player->SaveToDB(); player->ADD_GOSSIP_ITEM(5, "<== |武器大师|学习成功，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);}
		else player->ADD_GOSSIP_ITEM(5, "<== 碎片不够，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID()); break;
	}
	case __MENU_FRAG_SUB_90_ACT_2: //Weapon 2
	{	//confirmed, just remove the pearls and then learn the spell, return to 
		if ((player->HasItemCount(__MENU_FRAG_WEAPON_ITEM, 30)))
		{	player->DestroyItemCount(__MENU_FRAG_WEAPON_ITEM, 30, true);
			player->LearnSpell(__MENU_FRAG_SUB_90_ACT_2_SPELL, false);
			player->SaveToDB(); player->ADD_GOSSIP_ITEM(5, "<== |武器大师|学习成功，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);}
		else player->ADD_GOSSIP_ITEM(5, "<== 碎片不够，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID()); break;
	}
	case __MENU_FRAG_SUB_90_ACT_3: //Weapon 3
	{	//confirmed, just remove the pearls and then learn the spell, return to 
		if ((player->HasItemCount(__MENU_FRAG_WEAPON_ITEM, 30)))
		{	player->DestroyItemCount(__MENU_FRAG_WEAPON_ITEM, 30, true);
			player->LearnSpell(__MENU_FRAG_SUB_90_ACT_3_SPELL, false);
			player->SaveToDB(); player->ADD_GOSSIP_ITEM(5, "<== |武器大师|学习成功，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);}
		else player->ADD_GOSSIP_ITEM(5, "<== 碎片不够，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID()); break;
	}
	case __MENU_FRAG_SUB_90_ACT_4: //Weapon 4
	{	//confirmed, just remove the pearls and then learn the spell, return to 
		if ((player->HasItemCount(__MENU_FRAG_WEAPON_ITEM, 30)))
		{	player->DestroyItemCount(__MENU_FRAG_WEAPON_ITEM, 30, true);
			player->LearnSpell(__MENU_FRAG_SUB_90_ACT_4_SPELL, false);
			player->SaveToDB(); player->ADD_GOSSIP_ITEM(5, "<== |武器大师|学习成功，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);}
		else player->ADD_GOSSIP_ITEM(5, "<== 武片不够，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID()); break;
	}
	case __MENU_FRAG_SUB_90_ACT_5: //Weapon 5
	{	//confirmed, just remove the pearls and then learn the spell, return to 
		if ((player->HasItemCount(__MENU_FRAG_WEAPON_ITEM, 30)))
		{	player->DestroyItemCount(__MENU_FRAG_WEAPON_ITEM, 30, true);
			player->LearnSpell(__MENU_FRAG_SUB_90_ACT_5_SPELL, false);
			player->SaveToDB(); player->ADD_GOSSIP_ITEM(5, "<== |武器大师|学习成功，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);}
		else player->ADD_GOSSIP_ITEM(5, "<== 武器碎片不够，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID()); break;
	}
	case __MENU_FRAG_SUB_90_ACT_6: //SF
	{	//confirmed, just remove the pearls and then learn the spell, return to 
		if ((player->HasItemCount(__MENU_FRAG_WEAPON_ITEM, 50)))
		{   player->DestroyItemCount(__MENU_FRAG_WEAPON_ITEM, 50, true);
			player->LearnSpell(__MENU_FRAG_SUB_90_ACT_6_SPELL, false);
			player->SaveToDB(); player->ADD_GOSSIP_ITEM(5, "<== |施法加速|学习成功，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);		}
		else player->ADD_GOSSIP_ITEM(5, "<== 碎片不够，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID()); break;
	}
	case __MENU_FRAG_SUB_90_ACT_7: //Pet
	{	//confirmed, just remove the pearls and then learn the spell, return to 
		if ((player->HasItemCount(__MENU_FRAG_WEAPON_ITEM, 50)))
		{   player->DestroyItemCount(__MENU_FRAG_WEAPON_ITEM, 50, true);
			player->LearnSpell(__MENU_FRAG_SUB_90_ACT_7_SPELL, false);
			player->SaveToDB(); player->ADD_GOSSIP_ITEM(5, "<== |宠物增强|学习成功，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);		}
		else player->ADD_GOSSIP_ITEM(5, "<== 碎片不够，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_FRAG_MAIN);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID()); break;
	}

#pragma endregion
	//end
	}

	return true;
}


#pragma endregion

#pragma region MENU PEAK

#define __MENU_PEAK_SUB_0			(__MENU_PEAK_MAIN + 10)
#define __MENU_PEAK_SUB_0_ACT_1			(__MENU_PEAK_SUB_0 + 1)
#define __MENU_PEAK_SUB_0_ACT_2			(__MENU_PEAK_SUB_0 + 2)
#define __MENU_PEAK_SUB_0_SPELL		(31260)

#define __MENU_PEAK_SUB_1			(__MENU_PEAK_MAIN + 20)
#define __MENU_PEAK_SUB_1_ACT_1			(__MENU_PEAK_SUB_1 + 1)
#define __MENU_PEAK_SUB_1_ACT_2			(__MENU_PEAK_SUB_1 + 2)
#define __MENU_PEAK_SUB_1_SPELL		(31265)

#define __MENU_PEAK_SUB_2			(__MENU_PEAK_MAIN + 30)
#define __MENU_PEAK_SUB_2_ACT_1			(__MENU_PEAK_SUB_2 + 1)
#define __MENU_PEAK_SUB_2_ACT_2			(__MENU_PEAK_SUB_2 + 2)
#define __MENU_PEAK_SUB_2_SPELL		(31270)

#define __MENU_PEAK_SUB_3			(__MENU_PEAK_MAIN + 40)
#define __MENU_PEAK_SUB_3_ACT_1			(__MENU_PEAK_SUB_3 + 1)
#define __MENU_PEAK_SUB_3_ACT_2			(__MENU_PEAK_SUB_3 + 2)
#define __MENU_PEAK_SUB_3_SPELL		(31275)

#define __MENU_PEAK_SUB_4			(__MENU_PEAK_MAIN + 50)
#define __MENU_PEAK_SUB_4_ACT_1			(__MENU_PEAK_SUB_4 + 1)
#define __MENU_PEAK_SUB_4_ACT_2			(__MENU_PEAK_SUB_4 + 2)
#define __MENU_PEAK_SUB_4_SPELL		(31280)

#define __MENU_PEAK_SUB_5			(__MENU_PEAK_MAIN + 60)
#define __MENU_PEAK_SUB_5_ACT_1			(__MENU_PEAK_SUB_5 + 1)
#define __MENU_PEAK_SUB_5_ACT_2			(__MENU_PEAK_SUB_5 + 2)
#define __MENU_PEAK_SUB_5_SPELL		(31285)

#define	_PEARL_ITEM					(30524)
#define	_PEARL_NUM_NEED(val)		(20 +  (val)*20)		//10 balls init, max 170 balls (10 +  (val)*(val)*10)	
#define	_MONEY_GOLD_NEED(val)		(300 +  (val)*300)		//300g init, max 5100g (300 +  (val)*(val)*300)
#define	_PEAK_MAX_RANK				(5)
bool Menus_Peak_Main(Player *player, Creature *_Creature, int32 sender, uint32 action)
{
	if (!player || !_Creature) return false;

	std::string text = "";
	int val_stat = 0;
	int val_haste = 0;
	int val_dmg = 0;
	int val_defend = 0;
	int val_critdmg = 0;
	int val_talent = 0;

	//step 1, get all of the spells of player
	//1.1 find the haste spells, support max 5 spells (_PEAK_MAX_RANK)
	for (size_t i = 0; i < _PEAK_MAX_RANK; i++)
	{
		if (player->HasSpell(__MENU_PEAK_SUB_0_SPELL + i))	val_stat++;
		if (player->HasSpell(__MENU_PEAK_SUB_1_SPELL + i))	val_haste++;
		if (player->HasSpell(__MENU_PEAK_SUB_2_SPELL + i))	val_dmg++;
		if (player->HasSpell(__MENU_PEAK_SUB_3_SPELL + i))	val_defend++;
		if (player->HasSpell(__MENU_PEAK_SUB_4_SPELL + i))	val_critdmg++;
		if (player->HasSpell(__MENU_PEAK_SUB_5_SPELL + i))	val_talent++;
	}

	sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "PLAYER:[%u][%s] === Stat: %d, haste:%d, dmg %d, defend %d, crigdmg %d, talent %d.",
		player->GetGUID(), player->GetName(),
		val_stat, val_haste, val_dmg, val_defend, val_critdmg, val_talent);

	switch (action)
	{
	case __MENU_PEAK_MAIN:

		player->ADD_GOSSIP_ITEM(5, __STR("=====巅峰系统======"), GOSSIP_SENDER_MAIN, __MENU_NONE);

		//stat
		text = __BLUE("[全属性性 (每级2%)]");
		text.append("当前:" + __NSTR(2 * val_stat) + "%, MAX(10%)");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, val_stat<_PEAK_MAX_RANK ? __MENU_PEAK_SUB_0 : __MENU_NONE);

		text = __BLUE("[急速速 (每级2%)]");
		text.append("当前:" + __NSTR(2 * val_haste) + "%, MAX(10%)");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, val_haste<_PEAK_MAX_RANK ? __MENU_PEAK_SUB_1 : __MENU_NONE);

		text = __BLUE("[伤害 (每级2%)]");
		text.append("当前:" + __NSTR(2 * val_dmg) + "%, MAX(10%)");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, val_dmg<_PEAK_MAX_RANK ? __MENU_PEAK_SUB_2 : __MENU_NONE);

		text = __BLUE("[免伤 (每级2%)]");
		text.append("当前:" + __NSTR(2 * val_defend) + "%, MAX(10%)");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, val_defend<_PEAK_MAX_RANK ? __MENU_PEAK_SUB_3 : __MENU_NONE);

		text = __BLUE("[爆伤 (每级5%)]");
		text.append("当前:" + __NSTR(5 * val_critdmg) + "%, MAX(25%)");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, val_critdmg<_PEAK_MAX_RANK ? __MENU_PEAK_SUB_4 : __MENU_NONE);

		text = __BLUE("[天赋 (每级1点)]");
		text.append("当前:" + __NSTR(val_talent) + " 点, MAX(5点)");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, val_talent<_PEAK_MAX_RANK ? __MENU_PEAK_SUB_5 : __MENU_NONE);

		player->ADD_GOSSIP_ITEM(5, "============================", GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(5, __STR("<===返回===="), GOSSIP_SENDER_MAIN, __MENU_PEAK_MAIN);

		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		break;

		//-------------- STAT ---------------------------
	case __MENU_PEAK_SUB_0: //stat
	{
#undef  __local_val 
#define	__local_val val_stat
#undef  __LOCAL_SPELL
#define	__LOCAL_SPELL __MENU_PEAK_SUB_0_SPELL
		text = __BLUE("[全属性性 (每级2%) ]");
		text.append("当前:");
		text.append(__NSTR(2 * __local_val) + "%. ");
		text.append("需要花费: ");
		text.append(__NSTR(_MONEY_GOLD_NEED(__local_val)) + "G, ");
		text.append(__NSTR(_PEARL_NUM_NEED(__local_val)) + "个宝石到");
		text.append("下一等级:: ");
		text.append(__NSTR(2 * __local_val + 2) + "%.");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_PEAK_SUB_0_ACT_1);
		player->ADD_GOSSIP_ITEM(5, "<==取消===", GOSSIP_SENDER_MAIN, __MENU_PEAK_MAIN);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		break;
	}
	case __MENU_PEAK_SUB_0_ACT_1: //confirm add more ...
	{
		//confirmed
		//just remove the pearls and then learn the spell, return to 
		if ((player->HasItemCount(_PEARL_ITEM, _PEARL_NUM_NEED(__local_val))) && ((player->GetMoney()) > (uint32)(_MONEY_GOLD_NEED(__local_val) * 10000)))
		{
			player->DestroyItemCount(_PEARL_ITEM, _PEARL_NUM_NEED(__local_val), true);
			player->SetMoney(player->GetMoney() - (_MONEY_GOLD_NEED(__local_val) * 10000));

			player->LearnSpell(__LOCAL_SPELL + __local_val, false); //false should be ?
			player->SaveToDB();

			player->ADD_GOSSIP_ITEM(5, "<== 修炼成功，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_PEAK_MAIN);
		}
		else
		{
			player->ADD_GOSSIP_ITEM(5, "<== 金币或者宝珠不够，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_PEAK_MAIN);
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		break;
	}



	//-------------- HASTE ---------------------------
	case __MENU_PEAK_SUB_1:
	{
#undef  __local_val 
#define	__local_val val_haste
#undef  __LOCAL_SPELL
#define	__LOCAL_SPELL __MENU_PEAK_SUB_1_SPELL
		text = __BLUE("[急速速 (每级2%) ]");
		text.append("当前:");
		text.append(__NSTR(2 * __local_val) + "%. ");
		text.append("需要花费: ");
		text.append(__NSTR(_MONEY_GOLD_NEED(__local_val)) + "G, ");
		text.append(__NSTR(_PEARL_NUM_NEED(__local_val)) + "个宝石到");
		text.append("下一等级:: ");
		text.append(__NSTR(2 * __local_val + 2) + "%.");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_PEAK_SUB_1_ACT_1);
		player->ADD_GOSSIP_ITEM(5, "<==取消===", GOSSIP_SENDER_MAIN, __MENU_PEAK_MAIN);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		break;
	}
	case __MENU_PEAK_SUB_1_ACT_1: //confirm add more ...
	{
		//confirmed
		//just remove the pearls and then learn the spell, return to 
		if ((player->HasItemCount(_PEARL_ITEM, _PEARL_NUM_NEED(__local_val))) && ((player->GetMoney()) > (uint32)(_MONEY_GOLD_NEED(__local_val) * 10000)))
		{
			player->DestroyItemCount(_PEARL_ITEM, _PEARL_NUM_NEED(__local_val), true);
			player->SetMoney(player->GetMoney() - (_MONEY_GOLD_NEED(__local_val) * 10000));
			player->LearnSpell(__LOCAL_SPELL + __local_val, false); //false should be ?
			player->SaveToDB();

			player->ADD_GOSSIP_ITEM(5, "<== 修炼成功，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_PEAK_MAIN);
		}
		else
		{
			player->ADD_GOSSIP_ITEM(5, "<== 金币或者宝珠不够，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_PEAK_MAIN);
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		break;
	}


	//-------------- DAMAGE ---------------------------
	case __MENU_PEAK_SUB_2:
	{
#undef  __local_val 
#define	__local_val val_dmg
#undef  __LOCAL_SPELL
#define	__LOCAL_SPELL __MENU_PEAK_SUB_2_SPELL
		text = __BLUE("[伤害 (每级2%) ]");
		text.append("当前:");
		text.append(__NSTR(2 * __local_val) + "%. ");
		text.append("需要花费: ");
		text.append(__NSTR(_MONEY_GOLD_NEED(__local_val)) + "G, ");
		text.append(__NSTR(_PEARL_NUM_NEED(__local_val)) + "个宝石到");
		text.append("下一等级:: ");
		text.append(__NSTR(2 * __local_val + 2) + "%.");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_PEAK_SUB_2_ACT_1);
		player->ADD_GOSSIP_ITEM(5, "<==取消===", GOSSIP_SENDER_MAIN, __MENU_PEAK_MAIN);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		break;
	}
	case __MENU_PEAK_SUB_2_ACT_1: //confirm add more ...
	{
		//confirmed
		//just remove the pearls and then learn the spell, return to 
		if ((player->HasItemCount(_PEARL_ITEM, _PEARL_NUM_NEED(__local_val))) && ((player->GetMoney()) > (uint32)(_MONEY_GOLD_NEED(__local_val) * 10000)))
		{
			player->DestroyItemCount(_PEARL_ITEM, _PEARL_NUM_NEED(__local_val), true);
			player->SetMoney(player->GetMoney() - (_MONEY_GOLD_NEED(__local_val) * 10000));
			player->LearnSpell(__LOCAL_SPELL + __local_val, false); //false should be ?
			player->SaveToDB();
			player->ADD_GOSSIP_ITEM(5, "<== 修炼成功，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_PEAK_MAIN);
		}
		else
		{
			player->ADD_GOSSIP_ITEM(5, "<== 金币或者宝珠不够，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_PEAK_MAIN);
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		break;
	}

	//-------------- Defence ---------------------------
	case __MENU_PEAK_SUB_3:
	{
#undef  __local_val 
#define	__local_val val_defend
#undef  __LOCAL_SPELL
#define	__LOCAL_SPELL __MENU_PEAK_SUB_3_SPELL
		text = __BLUE("[免伤 (每级2%) ]");
		text.append("当前:");
		text.append(__NSTR(2 * __local_val) + "%. ");
		text.append("需要花费: ");
		text.append(__NSTR(_MONEY_GOLD_NEED(__local_val)) + "G, ");
		text.append(__NSTR(_PEARL_NUM_NEED(__local_val)) + "个宝石到");
		text.append("下一等级:: ");
		text.append(__NSTR(2 * __local_val + 2) + "%.");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_PEAK_SUB_3_ACT_1);
		player->ADD_GOSSIP_ITEM(5, "<==取消===", GOSSIP_SENDER_MAIN, __MENU_PEAK_MAIN);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		break;
	}
	case __MENU_PEAK_SUB_3_ACT_1: //confirm add more ...
	{
		//confirmed
		//just remove the pearls and then learn the spell, return to 
		if ((player->HasItemCount(_PEARL_ITEM, _PEARL_NUM_NEED(__local_val))) && ((player->GetMoney()) > (uint32)(_MONEY_GOLD_NEED(__local_val) * 10000)))
		{
			player->DestroyItemCount(_PEARL_ITEM, _PEARL_NUM_NEED(__local_val), true);
			player->SetMoney(player->GetMoney() - (_MONEY_GOLD_NEED(__local_val) * 10000));
			player->LearnSpell(__LOCAL_SPELL + __local_val, false); //false should be ?
			player->SaveToDB();
			player->ADD_GOSSIP_ITEM(5, "<== 修炼成功，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_PEAK_MAIN);
		}
		else
		{
			player->ADD_GOSSIP_ITEM(5, "<== 金币或者宝珠不够，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_PEAK_MAIN);
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		break;
	}

	//-------------- CRIT DAMAGE ---------------------------
	case __MENU_PEAK_SUB_4:
	{
#undef  __local_val 
#define	__local_val val_critdmg
#undef  __LOCAL_SPELL
#define	__LOCAL_SPELL __MENU_PEAK_SUB_4_SPELL
		text = __BLUE("[爆伤 (每级5%) ]");
		text.append("当前:");
		text.append(__NSTR(5 * __local_val) + "%. ");
		text.append("需要花费: ");
		text.append(__NSTR(_MONEY_GOLD_NEED(__local_val)) + "G, ");
		text.append(__NSTR(_PEARL_NUM_NEED(__local_val)) + "个宝石到");
		text.append("下一等级:: ");
		text.append(__NSTR(5 * __local_val + 5) + "%.");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_PEAK_SUB_4_ACT_1);
		player->ADD_GOSSIP_ITEM(5, "<==取消===", GOSSIP_SENDER_MAIN, __MENU_PEAK_MAIN);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		break;
	}
	case __MENU_PEAK_SUB_4_ACT_1: //confirm add more ...
	{
		//confirmed
		//just remove the pearls and then learn the spell, return to 
		if ((player->HasItemCount(_PEARL_ITEM, _PEARL_NUM_NEED(__local_val))) && ((player->GetMoney()) > (uint32)(_MONEY_GOLD_NEED(__local_val) * 10000)))
		{
			player->DestroyItemCount(_PEARL_ITEM, _PEARL_NUM_NEED(__local_val), true);
			player->SetMoney(player->GetMoney() - (_MONEY_GOLD_NEED(__local_val) * 10000));
			player->LearnSpell(__LOCAL_SPELL + __local_val, false); //false should be ?
			player->SaveToDB();
			player->ADD_GOSSIP_ITEM(5, "<== 修炼成功，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_PEAK_MAIN);
		}
		else
		{
			player->ADD_GOSSIP_ITEM(5, "<== 金币或者宝珠不够，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_PEAK_MAIN);
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		break;
	}

	//-------------- TALENT ---------------------------
	case __MENU_PEAK_SUB_5:
	{
#undef  __local_val 
#define	__local_val val_talent
#undef  __LOCAL_SPELL
#define	__LOCAL_SPELL __MENU_PEAK_SUB_5_SPELL
		text = __BLUE("[天赋]");
		text.append("当前:");
		text.append(__NSTR(__local_val) + " 点天赋,");
		text.append("需要花费: ");
		text.append(__NSTR(_MONEY_GOLD_NEED(__local_val)) + "G, ");
		text.append(__NSTR(_PEARL_NUM_NEED(__local_val)) + "个宝石到");
		text.append("下一等级:");
		text.append(__NSTR(__local_val + 1) + " 点天赋.");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_PEAK_SUB_5_ACT_1);
		player->ADD_GOSSIP_ITEM(5, "<==取消===", GOSSIP_SENDER_MAIN, __MENU_PEAK_MAIN);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		break;
	}
	case __MENU_PEAK_SUB_5_ACT_1: //confirm add more ...
	{
		//confirmed
		//just remove the pearls and then learn the spell, return to 
		if ((player->HasItemCount(_PEARL_ITEM, _PEARL_NUM_NEED(__local_val))) && ((player->GetMoney()) > (uint32)(_MONEY_GOLD_NEED(__local_val) * 10000)))
		{
			player->DestroyItemCount(_PEARL_ITEM, _PEARL_NUM_NEED(__local_val), true);
			player->SetMoney(player->GetMoney() - (_MONEY_GOLD_NEED(__local_val) * 10000));
			player->LearnSpell(__LOCAL_SPELL + __local_val, false); //false should be ?
			player->SaveToDB();
			player->ADD_GOSSIP_ITEM(5, "<== 修炼成功，返回首页(天赋需要小退生效) ===", GOSSIP_SENDER_MAIN, __MENU_PEAK_MAIN);
		}
		else
		{
			player->ADD_GOSSIP_ITEM(5, "<== 金币或者宝珠不够，返回首页 ===", GOSSIP_SENDER_MAIN, __MENU_PEAK_MAIN);
		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		break;
	}


	}

	return true;
}

#pragma endregion

#pragma region MENU Reputation
#define __MENU_REPU_ACT_1	(10)
#define __ITEM_REPU			(30750)
uint32 __get_repu_by_player(Player *player, uint32 faction)
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
bool Menus_Repu_Main(Player *player, Creature *_Creature, uint32 sender, uint32 action)
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

#pragma region MENU VOUCHER SCORE

#define GET_ITEM_SCORE(p, item, score)		((p)->GetItemCount(item) * score)
#define GET_SPEL_SCORE(p, spell, score)		((p)->HasSpell(spell)? score : 0)
//Other actions
bool Menus_Voucher_Main(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
	//find the player's spell and items
	int score = 0;

	//update the honor
	auto __rankinfo = player->GetHonorMgr().GetHighestRank().rank;

	if (__rankinfo >= 17) score += 300;
	else if (__rankinfo == 16) score += 250;
	else if (__rankinfo == 15) score += 200;
	else if (__rankinfo == 14) score += 150;
	else if (__rankinfo == 13) score += 100;
	else if (__rankinfo >= 10) score += 50;

	//additional 

	//2阶橙武 
	//橙武+300,  风剑+500
	score += GET_ITEM_SCORE(player, 26005, 500);
	score += GET_ITEM_SCORE(player, 26004, 400);
	score += GET_ITEM_SCORE(player, 26008, 400);
	score += GET_ITEM_SCORE(player, 26007, 400);
	score += GET_ITEM_SCORE(player, 39992, 400);
	score += GET_ITEM_SCORE(player, 26010, 400);
	score += GET_ITEM_SCORE(player, 26011, 400);

	//橙武，提布+200,  风剑+300
	score += GET_ITEM_SCORE(player, 19019, 300);
	score += GET_ITEM_SCORE(player, 1728,   50);
	score += GET_ITEM_SCORE(player, 17182, 150);
	score += GET_ITEM_SCORE(player, 39991, 150);
	score += GET_ITEM_SCORE(player, 26002, 150);
	score += GET_ITEM_SCORE(player, 39901, 150);
	score += GET_ITEM_SCORE(player, 26003, 150);
	
	//橙色饰品，盾，魔杖 +20
	score += GET_ITEM_SCORE(player, 39801, 20);
	score += GET_ITEM_SCORE(player, 39802, 20);
	score += GET_ITEM_SCORE(player, 30006, 20);
	score += GET_ITEM_SCORE(player, 30007, 20);
	score += GET_ITEM_SCORE(player, 30008, 20);
	score += GET_ITEM_SCORE(player, 30009, 20);

	//橙色披风戒指二阶：100
	score += GET_ITEM_SCORE(player, 39817, 100);
	score += GET_ITEM_SCORE(player, 39818, 100);
	score += GET_ITEM_SCORE(player, 39819, 100);
	score += GET_ITEM_SCORE(player, 39820, 100);
	score += GET_ITEM_SCORE(player, 39854, 100);
	score += GET_ITEM_SCORE(player, 39858, 100);
	score += GET_ITEM_SCORE(player, 39862, 100);
	score += GET_ITEM_SCORE(player, 39866, 50);
	score += GET_ITEM_SCORE(player, 39796, 50);
	score += GET_ITEM_SCORE(player, 39792, 50);
	score += GET_ITEM_SCORE(player, 39788, 50);
	score += GET_ITEM_SCORE(player, 39784, 50);
	score += GET_ITEM_SCORE(player, 39772, 50);
	score += GET_ITEM_SCORE(player, 39768, 50);
	score += GET_ITEM_SCORE(player, 39764, 50);
	score += GET_ITEM_SCORE(player, 39752, 50);
	score += GET_ITEM_SCORE(player, 39748, 50);
	score += GET_ITEM_SCORE(player, 39744, 50);		
		
		

	//橙色披风戒指1阶：20
	score += GET_ITEM_SCORE(player, 39815, 20);
	score += GET_ITEM_SCORE(player, 39816, 20);
	score += GET_ITEM_SCORE(player, 39853, 20);
	score += GET_ITEM_SCORE(player, 39857, 20);
	score += GET_ITEM_SCORE(player, 39861, 20);
	score += GET_ITEM_SCORE(player, 39865, 20);
	score += GET_ITEM_SCORE(player, 39795, 10);
	score += GET_ITEM_SCORE(player, 39791, 10);
	score += GET_ITEM_SCORE(player, 39787, 10);
	score += GET_ITEM_SCORE(player, 39783, 10);
	score += GET_ITEM_SCORE(player, 39771, 10);
	score += GET_ITEM_SCORE(player, 39767, 10);
	score += GET_ITEM_SCORE(player, 39763, 10);
	score += GET_ITEM_SCORE(player, 39751, 10);
	score += GET_ITEM_SCORE(player, 39709, 10);
	score += GET_ITEM_SCORE(player, 39715, 10);


	//老兵3, 50
	score += GET_ITEM_SCORE(player, 30303, 50);


	//永久药剂，每个50
	score += GET_ITEM_SCORE(player, 39961, 50);
	score += GET_ITEM_SCORE(player, 39962, 50);
	score += GET_ITEM_SCORE(player, 39963, 50);
	score += GET_ITEM_SCORE(player, 39964, 50);

	//一键buff
	
	//碎片+5
	score += GET_ITEM_SCORE(player, 30522, 5);

	//中立声望，每个+20
	score += GET_SPEL_SCORE(player, 31931, 20);
	score += GET_SPEL_SCORE(player, 31932, 20);
	score += GET_SPEL_SCORE(player, 31933, 20);
	score += GET_SPEL_SCORE(player, 31934, 20);
	score += GET_SPEL_SCORE(player, 31935, 20);
	score += GET_SPEL_SCORE(player, 31936, 20);
	score += GET_SPEL_SCORE(player, 31937, 20);
	score += GET_SPEL_SCORE(player, 31938, 20);
	score += GET_SPEL_SCORE(player, 31939, 20);
	score += GET_SPEL_SCORE(player, 31940, 20);

	//三级宠物，30
	score += GET_SPEL_SCORE(player, 24696, 30);
	score += GET_SPEL_SCORE(player, 28871, 30);
	score += GET_SPEL_SCORE(player, 17709, 30);
	score += GET_SPEL_SCORE(player, 28738, 30);
	score += GET_SPEL_SCORE(player, 28739, 30);
	score += GET_SPEL_SCORE(player, 17707, 30);
	
	//三坐骑，黑虫子，每个+100
	score += GET_SPEL_SCORE(player, 32005, 100);
	score += GET_SPEL_SCORE(player, 32006, 100);
	score += GET_SPEL_SCORE(player, 32007, 100);
	score += GET_SPEL_SCORE(player, 32012, 100);

	//点券坐骑，乌龟等
	score += GET_SPEL_SCORE(player, 32084, 5);
	score += GET_SPEL_SCORE(player, 32086, 5);
	score += GET_SPEL_SCORE(player, 32088, 5);
	score += GET_SPEL_SCORE(player, 32093, 5);

	//ZG-T2.5技能，每个+15
	score += GET_SPEL_SCORE(player, 24431, 15);
	score += GET_SPEL_SCORE(player, 24460, 15);
	score += GET_SPEL_SCORE(player, 24467, 15);
	score += GET_SPEL_SCORE(player, 24471, 15);
	score += GET_SPEL_SCORE(player, 24483, 15);
	score += GET_SPEL_SCORE(player, 24461, 15);
	score += GET_SPEL_SCORE(player, 24491, 15);
	score += GET_SPEL_SCORE(player, 24486, 15);
	score += GET_SPEL_SCORE(player, 24480, 15);
	score += GET_SPEL_SCORE(player, 23563, 15);
	score += GET_SPEL_SCORE(player, 23591, 15);
	score += GET_SPEL_SCORE(player, 23559, 15);
	score += GET_SPEL_SCORE(player, 21881, 15);
	score += GET_SPEL_SCORE(player, 22010, 15);
	score += GET_SPEL_SCORE(player, 23570, 15);
	score += GET_SPEL_SCORE(player, 22007, 15);
	score += GET_SPEL_SCORE(player, 23599, 15);
	score += GET_SPEL_SCORE(player, 21872, 15);
	score += GET_SPEL_SCORE(player, 26110, 15);
	score += GET_SPEL_SCORE(player, 26130, 15);
	score += GET_SPEL_SCORE(player, 26173, 15);
	score += GET_SPEL_SCORE(player, 26113, 15);
	score += GET_SPEL_SCORE(player, 26171, 15);
	score += GET_SPEL_SCORE(player, 26119, 15);
	score += GET_SPEL_SCORE(player, 26128, 15);
	score += GET_SPEL_SCORE(player, 26116, 15);
	score += GET_SPEL_SCORE(player, 26106, 15);
	score += GET_SPEL_SCORE(player, 23548, 15);
	score += GET_SPEL_SCORE(player, 26135, 15);
	score += GET_SPEL_SCORE(player, 26176, 15);
	score += GET_SPEL_SCORE(player, 26114, 15);
	score += GET_SPEL_SCORE(player, 26172, 15);
	score += GET_SPEL_SCORE(player, 26123, 15);
	score += GET_SPEL_SCORE(player, 26131, 15);
	score += GET_SPEL_SCORE(player, 23555, 15);
	score += GET_SPEL_SCORE(player, 26153, 15);
	
	//T0-T1 兑换技能，每个+10
	score += GET_SPEL_SCORE(player, 27419, 10);
	score += GET_SPEL_SCORE(player, 27498, 10);
	score += GET_SPEL_SCORE(player, 27785, 10);
	score += GET_SPEL_SCORE(player, 27787, 10);
	score += GET_SPEL_SCORE(player, 27778, 10);
	score += GET_SPEL_SCORE(player, 27774, 10);
	score += GET_SPEL_SCORE(player, 27867, 10);
	score += GET_SPEL_SCORE(player, 27780, 10);
	score += GET_SPEL_SCORE(player, 27781, 10);
	score += GET_SPEL_SCORE(player, 32008, 10);
	score += GET_SPEL_SCORE(player, 23562, 10);
	score += GET_SPEL_SCORE(player, 21747, 10);
	score += GET_SPEL_SCORE(player, 23566, 10);
	score += GET_SPEL_SCORE(player, 21975, 10);
	score += GET_SPEL_SCORE(player, 21973, 10);
	score += GET_SPEL_SCORE(player, 32025, 10);
	score += GET_SPEL_SCORE(player, 25113, 10);
	score += GET_SPEL_SCORE(player, 21741, 10);
	score += GET_SPEL_SCORE(player, 23557, 10);

	//攻防、杀手、武器大师，每个20
	score += GET_SPEL_SCORE(player, 32441, 20);
	score += GET_SPEL_SCORE(player, 32442, 20);
	score += GET_SPEL_SCORE(player, 32443, 20);
	score += GET_SPEL_SCORE(player, 32444, 20);
	score += GET_SPEL_SCORE(player, 32445, 20);
	score += GET_SPEL_SCORE(player, 32446, 20);
	score += GET_SPEL_SCORE(player, 32447, 20);
	score += GET_SPEL_SCORE(player, 32448, 20);
	score += GET_SPEL_SCORE(player, 32449, 20);
	score += GET_SPEL_SCORE(player, 32450, 20);
	score += GET_SPEL_SCORE(player, 32451, 20);
	score += GET_SPEL_SCORE(player, 32452, 20);
	score += GET_SPEL_SCORE(player, 32453, 20);
	score += GET_SPEL_SCORE(player, 32454, 20);
	score += GET_SPEL_SCORE(player, 32455, 20);
	score += GET_SPEL_SCORE(player, 32456, 20);

	//巅峰 可兑换为：等级x10个
	score += GET_SPEL_SCORE(player, 31260, 2);
	score += GET_SPEL_SCORE(player, 31261, 5);
	score += GET_SPEL_SCORE(player, 31262, 10);
	score += GET_SPEL_SCORE(player, 31263, 20);
	score += GET_SPEL_SCORE(player, 31264, 40);
	score += GET_SPEL_SCORE(player, 31265, 2);
	score += GET_SPEL_SCORE(player, 31266, 5);
	score += GET_SPEL_SCORE(player, 31267, 10);
	score += GET_SPEL_SCORE(player, 31268, 20);
	score += GET_SPEL_SCORE(player, 31269, 40);
	score += GET_SPEL_SCORE(player, 31270, 2);
	score += GET_SPEL_SCORE(player, 31271, 5);
	score += GET_SPEL_SCORE(player, 31272, 10);
	score += GET_SPEL_SCORE(player, 31273, 20);
	score += GET_SPEL_SCORE(player, 31274, 40);
	score += GET_SPEL_SCORE(player, 31275, 2);
	score += GET_SPEL_SCORE(player, 31276, 5);
	score += GET_SPEL_SCORE(player, 31277, 10);
	score += GET_SPEL_SCORE(player, 31278, 20);
	score += GET_SPEL_SCORE(player, 31279, 40);
	score += GET_SPEL_SCORE(player, 31280, 2);
	score += GET_SPEL_SCORE(player, 31281, 5);
	score += GET_SPEL_SCORE(player, 31282, 10);
	score += GET_SPEL_SCORE(player, 31283, 20);
	score += GET_SPEL_SCORE(player, 31284, 40);
	score += GET_SPEL_SCORE(player, 31285, 2);
	score += GET_SPEL_SCORE(player, 31286, 5);
	score += GET_SPEL_SCORE(player, 31287, 10);
	score += GET_SPEL_SCORE(player, 31288, 20);
	score += GET_SPEL_SCORE(player, 31289, 40);

	std::string text = __BLUE("[姓名]");
	text.append(player->GetName());
	text.append(__GREEN(", [可以继承点券]"));
	text.append(__NSTR(score));

	sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "PLAYER:[%u][%s] === Vouchor: %d", player->GetGUID(), player->GetName(), score);

	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_VOUCHER_MAIN);
	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());

	return true;
}


#pragma endregion

#pragma region Rune Menus Wrapper
//define a wrapper function for the equip system menus
bool Rune_Menus(Player *player, Creature *_c, uint32 sender, uint32 action)
{
	//check if player is null and go is null
	if (!player || !_c) return false;

	sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "[Rune Menus] PLAYER:[%u][%s] === action: %d", player->GetGUID(), player->GetName(), action);

	// Main menu
	if (action >= __MENU_TISH_MAIN && action <= __MENU_TISH_MAIN + __MENU_SIZE)
	{
		return Menus_Tish_Main(player, _c, sender, action);
	}
	else if (action >= __MENU_COMM_MAIN && action <= __MENU_COMM_MAIN + __MENU_SIZE)
	{
		return Menus_Comm_Main(player, _c, sender, action);
	}
	else if (action >= __MENU_RUNE_MAIN && action <= __MENU_RUNE_MAIN + __MENU_SIZE)
	{
		return X_Menus_Rune_Main(player, _c, sender, action);
	}
	else if (action >= __MENU_FRAG_MAIN && action <= __MENU_FRAG_MAIN + __MENU_SIZE)
	{
		return Menus_Frag_Main(player, _c, sender, action);
	}
	else if (action >= __MENU_REPU_MAIN && action <= __MENU_REPU_MAIN + __MENU_SIZE)
	{
		return Menus_Repu_Main(player, _c, sender, action);
	}
	else if (action >= __MENU_PEAK_MAIN && action <= __MENU_PEAK_MAIN + __MENU_SIZE)
	{
		return Menus_Peak_Main(player, _c, sender, action);
	}
	else if (action >= __MENU_VOUCHER_MAIN && action <= __MENU_VOUCHER_MAIN + __MENU_SIZE)
	{
		return Menus_Voucher_Main(player, _c, sender, action);
	}
	return true;
}

#pragma endregion



#pragma region  GO Equip Menus
/*****************************************************************************************
 *  Equip system Menus for the game object.
 *  @param player: The player who interact with the game object.
 *  @param _go: The game object.
 *  @param sender: The sender of the menu.
 *  @param action: The action of the menu.
 ******************************************************************************************/
 #define __MENU_EQUIP_MAIN 				1000
 #define __MENU_EQUIP_SUB_CREATE 		2000
 #define __MENU_EQUIP_SUB_SLOT			3000
 #define __MENU_EQUIP_SUB_MODDISP 		4000
 #define __MENU_EQUIP_SUB_RESTORE	 	5000
#define	__GOSSIP_EQCREATE_DESC			(16501)
bool Menus_Equip_Main(Player *player, GameObject *_go, uint32 sender, uint32 action)
{
	//check if player is null and go is null
	if (!player || !_go) return false;

	// Main menu
	// Several Features:
	// 1. Equipment Creation: Create the equipment with the player's items.
	// 2. Equipment Upgrade: Upgrade the equipment with the player's items.
	// 3. Equipment Disenchant : Disenchant the equipment with the player's items.
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);	
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　装备创造　＜＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_EQUIP_SUB_CREATE);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);	
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　字条转移　＜＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_EQUIP_SUB_SLOT);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);		
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　装备幻化　＜＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_EQUIP_SUB_MODDISP);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);	
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　装备还原　＜＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_EQUIP_SUB_RESTORE);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _go->GetGUID());
	return true;
}
#pragma endregion

#pragma region 	Equipment Creation System

std::string __get_Item_Name_GO(uint32 Class, uint32 SubClass, uint32 Inv)
{
	switch (Class)
	{
	case ITEM_CLASS_WEAPON:
		switch (SubClass)
		{
		case ITEM_SUBCLASS_WEAPON_AXE: 			return  __STR("创造的单手斧·　");
		case ITEM_SUBCLASS_WEAPON_AXE2:			return  __STR("创造的双手斧·　");
		case ITEM_SUBCLASS_WEAPON_BOW:			return  __STR("创造的弓·　");
		case ITEM_SUBCLASS_WEAPON_GUN:			return  __STR("创造的枪·　");
		case ITEM_SUBCLASS_WEAPON_MACE:			return  __STR("创造的单手锤·　");
		case ITEM_SUBCLASS_WEAPON_MACE2:		return  __STR("创造的双手锤·　");
		case ITEM_SUBCLASS_WEAPON_POLEARM:		return  __STR("创造的长柄·　");
		case ITEM_SUBCLASS_WEAPON_SWORD:		return  __STR("创造的单手剑·　");
		case ITEM_SUBCLASS_WEAPON_SWORD2:		return  __STR("创造的双手剑·　");
		case ITEM_SUBCLASS_WEAPON_STAFF:		return  __STR("创造的法杖·　");
		case ITEM_SUBCLASS_WEAPON_DAGGER:		return  __STR("创造的匕首·　");
		case ITEM_SUBCLASS_WEAPON_THROWN:		return  __STR("创造的飞刀·　");
		case ITEM_SUBCLASS_WEAPON_SPEAR:		return  __STR("创造的长矛·　");
		case ITEM_SUBCLASS_WEAPON_CROSSBOW:		return  __STR("创造的弩·　");
		case ITEM_SUBCLASS_WEAPON_WAND:			return  __STR("创造的魔杖·　");
		}
		return  __STR("创造的未知武器·　");
	case ITEM_CLASS_ARMOR:
		switch (Inv)
		{
		case INVTYPE_HEAD:				return  __STR("创造的头饰·　");
		case INVTYPE_NECK:				return  __STR("创造的项链·　");
		case INVTYPE_SHOULDERS:			return  __STR("创造的护肩·　");
		case INVTYPE_BODY:				return  __STR("创造的衬衣·　");
		case INVTYPE_ROBE:				return  __STR("创造的长袍·　");
		case INVTYPE_CHEST:				return  __STR("创造的胸甲·　");
		case INVTYPE_WAIST:				return  __STR("创造的腰带·　");
		case INVTYPE_LEGS:				return  __STR("创造的护腿·　");
		case INVTYPE_FEET:				return  __STR("创造的靴子·　");
		case INVTYPE_WRISTS:			return  __STR("创造的护腕·　");
		case INVTYPE_HANDS:				return  __STR("创造的护手·　");
		case INVTYPE_FINGER:			return  __STR("创造的戒指·　");
		case INVTYPE_TRINKET:			return  __STR("创造的饰品·　");
		case INVTYPE_SHIELD:			return  __STR("创造的盾牌·　");
		case INVTYPE_CLOAK:				return  __STR("创造的披风·　");
		case INVTYPE_HOLDABLE:			return  __STR("创造的副手·　");
		case INVTYPE_RELIC:				return  __STR("创造的圣物·　");
		}
	}
	return  __STR("创造的未知装备·　");
}
//Menus for Equip Creation
#define	__MENU_CREATE_MAIN		__MENU_EQUIP_SUB_CREATE
#define __MENU_CREATE_ACT_1		(10)
#define __MENU_CREATE_ACT_2		(20)
#define __MENU_CREATE_ACT_3		(30)
bool Menus_Equip_Sub_Create(Player *player, GameObject *_go, uint32 sender, uint32 action)
{
	//Now need to find the first bag
	auto pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START);
	if (!pItem || (pItem->GetProto()->Class != ITEM_CLASS_WEAPON && pItem->GetProto()->Class != ITEM_CLASS_ARMOR) 
		|| pItem->GetProto()->DisenchantID == 0
		|| pItem->GetProto()->Quality > 4 || pItem->GetProto()->Quality < 2
		//|| pItem->GetProto()->ItemId > 38000, check the first equipment
		)
	{
		std::string text = __BLUE("[|请将不一样的装备放在角色行囊里前两个格子，（仅限绿、蓝、紫等可分解装备）|]");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_CREATE_MAIN);
		player->SEND_GOSSIP_MENU(__GOSSIP_EQCREATE_DESC, _go->GetGUID());

		return false;
	}

	auto pItem1Proto = pItem->GetProto();

	//if pItem Id is larger than 40000, means it's a invented item, we need to find the orignal item
	if (pItem1Proto->ItemId > 40000)
	{
		//find the original item
		auto pItemOrig = sObjectMgr.GetItemPrototype(pItem1Proto->BuyPrice);	
		if (pItemOrig)	pItem1Proto = pItemOrig; //should be OK for future steps
	}

	auto pItem2 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START + 1);
	if (!pItem2 || (pItem2->GetProto()->Class != ITEM_CLASS_WEAPON && pItem2->GetProto()->Class != ITEM_CLASS_ARMOR) 
		|| pItem2->GetProto()->DisenchantID == 0
		|| pItem2->GetProto()->Quality > 4 || pItem2->GetProto()->Quality < 2
		//|| pItem1Proto->SubClass != pItem2->GetProto()->SubClass	//Should be same type of equipment
		|| pItem2->GetProto()->ItemId > 38000
		|| pItem1Proto->ItemId == pItem2->GetProto()->ItemId
		)
	{
		std::string text = __BLUE("[|请将不一样的装备放在角色行囊里前两个格子，限可分解装备|]");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_CREATE_MAIN);
		player->SEND_GOSSIP_MENU(__GOSSIP_EQCREATE_DESC, _go->GetGUID());

		return false;
	}
	auto pItem2Proto = pItem2->GetProto();

	//fix the gun, crossbow and bow together， 15,26,26
	//relic 28
	if(  ( (pItem2Proto->InventoryType == 26) || (pItem2Proto->InventoryType == 15) || (pItem2Proto->InventoryType == 28))
		&&
		((pItem1Proto->InventoryType == 26) || (pItem1Proto->InventoryType == 15) || (pItem1Proto->InventoryType == 28) )
	   )
	{
		//can combine
	}
	else
	{
		if (   pItem2Proto->InventoryType != pItem1Proto->InventoryType
			|| ( ( pItem2Proto->SubClass != pItem1Proto->SubClass )&&(pItem2Proto->Quality + pItem1Proto->Quality>6))
			|| pItem2Proto->Class != pItem1Proto->Class
			)
		{
			std::string text = __BLUE("[|待合成装备类型必须一致，紫装不可跨甲！|]");
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_CREATE_MAIN);
			player->SEND_GOSSIP_MENU(__GOSSIP_EQCREATE_DESC, _go->GetGUID());

			return false;
		}
	}

	//now get the proto spell of old item (to be destroyed)
	auto localIdx = player->GetSession()->GetSessionDbLocaleIndex();

	auto item_1_local = sObjectMgr.GetItemLocale(pItem1Proto->ItemId);
	auto item_1_text = (item_1_local == nullptr ? pItem1Proto->Name1 : item_1_local->Name[localIdx]);

	auto item_2_local = sObjectMgr.GetItemLocale(pItem2Proto->ItemId);
	auto item_2_text = (item_2_local == nullptr ? pItem2Proto->Name1 : item_2_local->Name[localIdx]);

	auto _needGold = pItem1Proto->ItemLevel * pItem2Proto->ItemLevel / 300;
	if (_needGold < 1)  _needGold = 1;

	/*
	auto _needSuiliNum = 9999;
	if (pItem1Proto->Quality == 4 && pItem1Proto->ItemLevel > 60)
		_needSuiliNum = (pItem1Proto->ItemLevel - 55) *  (pItem1Proto->ItemLevel - 55) / 5;
	if (_needSuiliNum < 1) _needSuiliNum = 1;
	*/

	std::string item_new_text = "　";
	item_new_text.append(__STR(player->GetName()));
	//item_new_text.append(__STR("创造的"));
	item_new_text.append(__get_Item_Name_GO(pItem1Proto->Class, pItem1Proto->SubClass, pItem1Proto->InventoryType));

	std::string item_desc = "原合成物品：①　";
	item_desc.append(__STR(item_1_text));
	item_desc.append(__STR("、　②　"));
	item_desc.append(__STR(item_2_text));
	item_desc.append(__STR("。　\r\n"));
	item_desc.append(__STR(__GREEN("装备等级：　")));
	//item_desc.append(__STR(player->GetName()));

	//now get the proto of 
	switch (action)
	{
	case __MENU_EQUIP_SUB_CREATE:
	{

		std::string text = __RED("[合成材料①]");
		text.append(item_1_text);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_CREATE_MAIN);

		text = __RED("[合成材料②]");
		text.append(item_2_text);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_CREATE_MAIN);


		text = __GREEN(" 合成需要花费金：--->");
		text.append(__NSTR(_needGold));
		text.append(" G");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, __STR(text), GOSSIP_SENDER_MAIN, __MENU_CREATE_MAIN);

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "　　　　　　丨丨 ", GOSSIP_SENDER_MAIN, __MENU_CREATE_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "　　　　　　丨丨 ", GOSSIP_SENDER_MAIN, __MENU_CREATE_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "　　　　　　ｖｖ ", GOSSIP_SENDER_MAIN, __MENU_CREATE_MAIN);


		text = __RED("[自制装备]==>  ");
		text.append(item_new_text.substr(0, item_new_text.length() - 5));
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_CREATE_MAIN);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, __STR(__BLUE(" ===== [确定合成] ===== ")), GOSSIP_SENDER_MAIN, __MENU_CREATE_MAIN + __MENU_CREATE_ACT_1);

		/*
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, __STR(" === [普通合成，确定] ===== "), GOSSIP_SENDER_MAIN, __MENU_CREATE_MAIN + __MENU_SLOT_ACT_1);
		text = __STR(" === [必定升华，消耗【橙武碎粒】数量： |cffdd2200 ");
		
		text.append(__NSTR(_needSuiliNum));
		text.append(" |r ] =="); */
		//if(player->HasItemCount(30320)) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, __STR(__BLUE(" === [SVIP, 消耗一枚标记必定升华] === ")), GOSSIP_SENDER_MAIN, __MENU_CREATE_MAIN + __MENU_SLOT_ACT_2);
		if (player->HasItemCount(30320)) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK,  __STR(__BLUE(" == [使用垫刀石，失败不消耗装备] == ")), GOSSIP_SENDER_MAIN, __MENU_CREATE_MAIN + __MENU_CREATE_ACT_2);

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, __STR(" ======= [返回] ======= "), GOSSIP_SENDER_MAIN, __MENU_CREATE_MAIN);


		player->SEND_GOSSIP_MENU(__GOSSIP_EQCREATE_DESC, _go->GetGUID());

		break;
	}

	case __MENU_CREATE_MAIN + __MENU_CREATE_ACT_1:
	case __MENU_CREATE_MAIN + __MENU_CREATE_ACT_2:
	case __MENU_CREATE_MAIN + __MENU_CREATE_ACT_3:
	{

		if ( (player->GetMoney()) < (uint32)_needGold * 10000 )
		{
			player->ADD_GOSSIP_ITEM(5, __RED("<== |金币不够或者武器错误，返回首页| ==="), GOSSIP_SENDER_MAIN, __MENU_CREATE_MAIN);
		}
		//check if has enough suilis
		else if (action ==  (__MENU_EQUIP_SUB_CREATE + __MENU_CREATE_ACT_2) && (!player->HasItemCount(30320)) )
		{
			player->ADD_GOSSIP_ITEM(5, __RED("<== |垫刀石不够或者武器错误，返回首页| ==="), GOSSIP_SENDER_MAIN, __MENU_CREATE_MAIN);
		}
		else
		{
			//should apply the chance?
			auto _mustHit = false;

			//remove money
			player->ModifyMoney(0 - (_needGold * 10000));

			//must remove the second slot
			player->DestroyItem(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START + 1, true);

			//if weapon must up
			if (action == (__MENU_EQUIP_SUB_CREATE + __MENU_CREATE_ACT_2) && player->HasItemCount(30320))
			{
				player->DestroyItemCount(30320, 1, true);

				player->M_Luckydraw_Times++;

				//roll chance
				if (roll_chance_i(15 + player->M_Luckydraw_Times))
				{
					_mustHit = true;
				}
				else
				{
					player->ADD_GOSSIP_ITEM(5, __BLUE("<== |升华失败，不消耗装备。| ==="), GOSSIP_SENDER_MAIN, __MENU_CREATE_MAIN);
					player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _go->GetGUID());
					return false;
				}
				
			}

			//remove the item, remains if not success
			player->DestroyItem(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START, true);

			//create the new item
			if (_mustHit) player->M_Luckydraw_Times = 9999;
			auto newItem = sObjectMgr.DynamicGenerateItem(pItem1Proto, pItem2Proto, item_new_text, item_desc, player->M_Luckydraw_Times, (Classes)player->GetClass());

			//Set can be used by creator
			Item *ppItem = player->AddItem(newItem->ItemId);
			ppItem->SetGuidValue(ITEM_FIELD_CREATOR, player->GetObjectGuid());

			//Annouce to players online
			//【幸运者】
			if (newItem->Quality > pItem1Proto->Quality)
			{
				//reset the luck draw
				player->M_Luckydraw_Times = 0;

				auto const& sessions = sWorld.GetAllSessions();
				for (const auto& itr : sessions)
				{
					if (WorldSession* session = itr.second)
					{
						Player* __player = session->GetPlayer();
						if (__player && __player->IsInWorld() && __player->IsAlive())
						{
							//|cffD2691E[幸运公告]|r 幸运玩家：|cffD2691E[%s]|r  成功创造极品装备：%s，品质：%s，装等：%u，合成材料为：%s 、 %s。
							ChatHandler(__player).PSendSysMessage(9037, player->GetName(), newItem->Name1,
								newItem->Quality == 5 ? __ORANGE("传奇") : newItem->Quality == 4 ? __PINK("史诗") : __BLUE("精良"),
								newItem->ItemLevel, item_1_text, item_2_text);
						}
					}
				}
			}
			//not lucky
			else {
				player->M_Luckydraw_Times++;
			}

			//add new item
			player->ADD_GOSSIP_ITEM(5, __BLUE("<== |合成成功，请检查背包| ==="), GOSSIP_SENDER_MAIN, __MENU_CREATE_MAIN);
		}

		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _go->GetGUID());

		break;
	}


	default:
		break;
	}

	return true;
}

#pragma endregion

#pragma region  Equipment Slot System

#define	__GOSSIP_SLOT_DESC				(16101)

#define __MENU_SLOT_MAIN				__MENU_EQUIP_SUB_CREATE
#define __MENU_SLOT_ACT_1					(10)
#define __MENU_SLOT_ACT_2					(20)
#define __MENU_SLOT_ACT_3					(30)

bool Menus_Equip_Sub_Upgrade(Player *player, GameObject *_go, uint32 sender, uint32 action)
{
	//Now need to find the first bag
	auto pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START);


	if (!pItem || (pItem->GetProto()->Class != ITEM_CLASS_WEAPON && pItem->GetProto()->Class != ITEM_CLASS_ARMOR) || pItem->GetProto()->Spells[0].SpellId == 0
		|| pItem->GetProto()->Quality > 4
		)
	{
		std::string text = __BLUE("[|请将可提取绿字字条的装备放在角色行囊里第一个格子|]");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_SLOT_MAIN);
		player->SEND_GOSSIP_MENU(__GOSSIP_SLOT_DESC, _go->GetGUID());

		return true;
	}

	//get allowed slots for the equipment
	uint8 slots[4];
	pItem->GetProto()->GetAllowedEquipSlots(slots, player->GetClass(), false);
	auto pEquippedItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slots[0]);
	if (!pEquippedItem || pEquippedItem->GetProto()->InventoryType != pItem->GetProto()->InventoryType || pEquippedItem->GetEnchantmentId(PROP_ENCHANTMENT_SLOT_3)<3000
		|| pEquippedItem->GetProto()->SubClass != pItem->GetProto()->SubClass
		|| pEquippedItem->GetProto()->Class != pItem->GetProto()->Class
		)
	{
		std::string text = __BLUE("[|目标装备需要在角色身上，并有字条槽，保证装备类型一致|]");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_SLOT_MAIN);
		player->SEND_GOSSIP_MENU(__GOSSIP_SLOT_DESC, _go->GetGUID());

		return true; 
	}

	//now get the proto spell of old item (to be destroyed)
	auto localIdx = player->GetSession()->GetSessionDbLocaleIndex();

	auto item_destroy_local = sObjectMgr.GetItemLocale(pItem->GetProto()->ItemId);
	auto item_destroy_text = (item_destroy_local == nullptr ? pItem->GetProto()->Name1 : item_destroy_local->Name[localIdx]);

	auto item_equipped_local = sObjectMgr.GetItemLocale(pEquippedItem->GetProto()->ItemId);
	auto item_equipped_text = (item_equipped_local == nullptr ? pEquippedItem->GetProto()->Name1 : item_equipped_local->Name[localIdx]);

	auto _needGold = pEquippedItem->GetProto()->ItemLevel * pEquippedItem->GetProto()->ItemLevel / 100 + 1;

	//should declare the 3 spell id
	uint32 spell_id[3];
	uint32 spell_dbc_id[3];
	std::string spell_text[3];

	for (size_t i = 0; i < 3; i++)
	{
		spell_id[i] = 0; spell_dbc_id[i] = 0;
		spell_text[i] = "|该字条无法转移|";

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

	//now get the proto of 
	switch (action)
	{
	case __MENU_SLOT_MAIN:
	{
		if (spell_id[0])
		{
			std::string text = __RED("[待提取装备]");
			text.append(item_destroy_text);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_SLOT_MAIN);

			for (size_t i = 0; i < 3; i++)
			{
				if (spell_id[i])
				{
					text = __GREEN("[ ==转移==> |字条| ]");
					text.append(spell_text[i]);
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR(text), GOSSIP_SENDER_MAIN, __MENU_SLOT_MAIN + __MENU_SLOT_ACT_1 + i * 10);
				}
			}

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "　　　　　　丨丨 ", GOSSIP_SENDER_MAIN, __MENU_SLOT_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "　　　　　　丨丨 ", GOSSIP_SENDER_MAIN, __MENU_SLOT_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "　　　　　　ｖｖ ", GOSSIP_SENDER_MAIN, __MENU_SLOT_MAIN);

			text = __GREEN("[转移到装备]");
			text.append(item_equipped_text);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);
			text = __GREEN("      需要花费金：--->");
			text.append(__NSTR(_needGold));
			text.append(" G");
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);

			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _go->GetGUID());
		}


		break;
	}

	case __MENU_SLOT_MAIN + __MENU_SLOT_ACT_1:
	case __MENU_SLOT_MAIN + __MENU_SLOT_ACT_2:
	case __MENU_SLOT_MAIN + __MENU_SLOT_ACT_3:
	{
		uint32 _idToMove = (action - __MENU_SLOT_MAIN - __MENU_SLOT_ACT_1) / 10;

		if (_idToMove < 3)
		{
			uint32 _spellId = spell_id[_idToMove];
			uint32 _dbcId = spell_dbc_id[_idToMove];

			// Get player money 
			if (((player->GetMoney()) >(uint32)_needGold * 10000) && (_spellId > 0 && _dbcId > 0))
			{
				//set the target 
				pEquippedItem->SetEnchantment(PROP_ENCHANTMENT_SLOT_3, _dbcId, 0, 0);

				//remove the item
				player->DestroyItem(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START, true);

				//remove money
				player->SetMoney(player->GetMoney() - (_needGold * 10000));

				//save to db
				player->SaveToDB();

				player->ADD_GOSSIP_ITEM(5, __BLUE("<== 转移成功，装备需要重新使用才能生效 ==="), GOSSIP_SENDER_MAIN, __MENU_NONE);

			}
			else
			{
				player->ADD_GOSSIP_ITEM(5, __RED("<== 金币不够或者武器错误，返回首页 ==="), GOSSIP_SENDER_MAIN, __MENU_SLOT_MAIN);
			}
		};

		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _go->GetGUID());

		break;
	}


	default:
		break;
	}

	return true;
}
#pragma endregion

#pragma region  Equipment Display Modify System
#define	__MENU_DISP_MOD_MAIN				__MENU_EQUIP_SUB_MODDISP
#define __MENU_DISP_MOD_ACT_1					(10)
#define __GOSSIP_DISP_MOD_DESC					(16034)
//Menus for Equip Display Modify
bool Menus_Equip_Sub_DispMod(Player *player, GameObject *_go, uint32 sender, uint32 action)
{
	//check player and go
	if (!player ||!_go) return false;
	
	//Now need to find the first bag
	auto pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START);


	if (!pItem || (pItem->GetProto()->Class != ITEM_CLASS_WEAPON && pItem->GetProto()->Class != ITEM_CLASS_ARMOR) 
		)
	{
		std::string text = __BLUE("== |请将幻化样式放在行囊第一个格子| ==");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_DISP_MOD_MAIN);
		player->SEND_GOSSIP_MENU(__GOSSIP_DISP_MOD_DESC, _go->GetGUID());

		return true;
	}

	//get allowed slots for the equipment
	uint8 slots[4];
	pItem->GetProto()->GetAllowedEquipSlots(slots, player->GetClass(), false);
	auto pEquippedItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slots[0]);
	if (!pEquippedItem 
		|| pEquippedItem->GetProto()->ItemId < 40000
		|| pEquippedItem->GetProto()->InventoryType != pItem->GetProto()->InventoryType 
		|| pEquippedItem->GetProto()->SubClass != pItem->GetProto()->SubClass
		|| pEquippedItem->GetProto()->Class != pItem->GetProto()->Class
		)
	{
		std::string text = __BLUE("[|待幻化装备需要在角色身上，并且是自制装备，类型保持一致！|]");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_SLOT_MAIN);
		player->SEND_GOSSIP_MENU(__GOSSIP_DISP_MOD_DESC, _go->GetGUID());

		return true; 
	}

	//now get the proto spell of old item (to be targeted)
	auto localIdx = player->GetSession()->GetSessionDbLocaleIndex();

	auto item_target_local = sObjectMgr.GetItemLocale(pItem->GetProto()->ItemId);
	auto item_target_text = (item_target_local == nullptr ? pItem->GetProto()->Name1 : item_target_local->Name[localIdx]);

	auto item_equipped_local = sObjectMgr.GetItemLocale(pEquippedItem->GetProto()->ItemId);
	auto item_equipped_text = (item_equipped_local == nullptr ? pEquippedItem->GetProto()->Name1 : item_equipped_local->Name[localIdx]);

	auto _needGold = pEquippedItem->GetProto()->ItemLevel * pEquippedItem->GetProto()->ItemLevel / 60 + 1;

	//now get the proto of 
	switch (action)
	{
	case __MENU_DISP_MOD_MAIN:
	{
		if (true)
		{
			std::string text = __RED("[幻化样式]");
			text.append(item_target_text);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_DISP_MOD_MAIN);


			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "　　　　　　丨丨 ", GOSSIP_SENDER_MAIN, __MENU_DISP_MOD_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "　　　　　　丨丨 ", GOSSIP_SENDER_MAIN, __MENU_DISP_MOD_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "　　　　　　ｖｖ ", GOSSIP_SENDER_MAIN, __MENU_DISP_MOD_MAIN);

			text = __GREEN("[转移到装备]");
			text.append(item_equipped_text);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_DISP_MOD_MAIN);


			text = __GREEN("需要花费金币：--->");
			text.append(__NSTR(_needGold));
			text.append(" G");
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, __STR(text), GOSSIP_SENDER_MAIN, __MENU_DISP_MOD_MAIN);

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("======== | 确定转移 | =======")), GOSSIP_SENDER_MAIN, __MENU_DISP_MOD_MAIN + __MENU_DISP_MOD_ACT_1);
			player->SEND_GOSSIP_MENU(__GOSSIP_DISP_MOD_DESC, _go->GetGUID());
		}
		break;
	}

	case __MENU_DISP_MOD_MAIN + __MENU_DISP_MOD_ACT_1:
	{

		// Get player money 
		if ((player->GetMoney() > (uint32)_needGold * 10000) && (pEquippedItem->GetProto()->ItemId > 40000))
		{
			//remove money
			player->ModifyMoney(0 - (_needGold * 10000));

			//get the GetItemPrototypeMap
			ItemPrototypeMap& _item_Maps = sObjectMgr.GetItemPrototypeMapUnsafe();
			auto iter = _item_Maps.find(pEquippedItem->GetProto()->ItemId);
			if (iter == _item_Maps.end())
			{
				player->ADD_GOSSIP_ITEM(5, __RED("<== 金币不够或者武器错误，返回首页 ==="), GOSSIP_SENDER_MAIN, __MENU_DISP_MOD_MAIN);
				return true;
			}

			else
			{	
				sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "[ModDisp] Player:%s modify item display ID:%u from %u to %u", 
					player->GetName(),
					iter->second.ItemId,
					iter->second.DisplayInfoID,
					pItem->GetProto()->DisplayInfoID);
				
				//save to the display id.
				iter->second.DisplayInfoID = pItem->GetProto()->DisplayInfoID;

				//_item_Maps[pEquippedItem->GetProto()->ItemId].DisplayInfoID = pItem->GetProto()->DisplayInfoID;
			}


			//save to db
			static SqlStatementID updItem;

            SqlStatement stmt = WorldDatabase.CreateStatement(updItem, "UPDATE `item_template_custom` SET `display_id` = ? WHERE `entry` = ?");
            stmt.addUInt32(pItem->GetProto()->DisplayInfoID);
            stmt.addUInt32(pEquippedItem->GetProto()->ItemId);
            stmt.Execute();

			player->ADD_GOSSIP_ITEM(5, __BLUE("<== 转移成功，需要删除WDB才能生效 ==="), GOSSIP_SENDER_MAIN, __MENU_DISP_MOD_MAIN);

			pEquippedItem->MarkForClientUpdate();
			pEquippedItem->SendForcedObjectUpdate();
		}
		else
		{
			player->ADD_GOSSIP_ITEM(5, __RED("<== 金币不够或者武器错误，返回首页 ==="), GOSSIP_SENDER_MAIN, __MENU_DISP_MOD_MAIN);
		}

		player->SEND_GOSSIP_MENU(__GOSSIP_DISP_MOD_DESC, _go->GetGUID());

		break;
	}


	default:
		break;
	}
	return true;
}
#pragma endregion

#pragma region  Equipment Restore System
//Menus for restore equipment to orignal state
//located at the 1st slot of bag 0
#define __MENU_EQUIP_RESTORE_MAIN				__MENU_EQUIP_SUB_RESTORE
#define __MENU_EQUIP_RESTORE_ACT_1				(10)
#define __GOSSIP_EQUIP_RESTORE_DESC				(16033)
bool Menus_Equip_Sub_Restore(Player *player, GameObject *_go, uint32 sender, uint32 action)
{
	//check player and go
	if (!player ||!_go) return false;

	//get the first item in the bag 0
	auto pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START);
	auto localIdx = player->GetSession()->GetSessionDbLocaleIndex();

	if (pItem && pItem->GetProto()->ItemId > 42000)
	{
		//need extra mats, such as gold, suili, etc.
		auto item_1_local = sObjectMgr.GetItemLocale(pItem->GetProto()->ItemId);
		auto item_1_text = (item_1_local == nullptr ? pItem->GetProto()->Name1 : item_1_local->Name[localIdx]);

		auto item_2_proto = sObjectMgr.GetItemPrototype(pItem->GetProto()->BuyPrice);
		auto item_2_local = sObjectMgr.GetItemLocale(pItem->GetProto()->BuyPrice);
		//check null
		if (!item_2_proto || !item_2_local) 
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝恢复装备有误，请反馈GM＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);	
			player->SEND_GOSSIP_MENU(__GOSSIP_EQUIP_RESTORE_DESC, _go->GetGUID());
			return false;
		}
		auto item_2_text = (item_2_local == nullptr ? item_2_proto->Name1 : item_2_local->Name[localIdx]);

		//get the need gold
		auto _needSuiliNum = pItem->GetProto()->Quality < 4 ? 0 : (pItem->GetProto()->Quality - 3) * pItem->GetProto()->ItemLevel / 20;

		if (action == __MENU_EQUIP_RESTORE_MAIN)
		{
			std::string text = __STR(__RED("[摧毁装备]::"));
			text.append(item_1_text);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_EQUIP_RESTORE_MAIN);
			text = __STR(__RED("额外需要橙武碎粒：-> "));
			text.append(__NSTR(_needSuiliNum));
			text.append("  个  ");
			if(_needSuiliNum) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, __STR(text), GOSSIP_SENDER_MAIN, __MENU_EQUIP_RESTORE_MAIN);
		
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "　　　　　　丨丨 ", GOSSIP_SENDER_MAIN, __MENU_EQUIP_RESTORE_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "　　　　　　丨丨 ", GOSSIP_SENDER_MAIN, __MENU_EQUIP_RESTORE_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "　　　　　　ｖｖ ", GOSSIP_SENDER_MAIN, __MENU_EQUIP_RESTORE_MAIN);
			text = __STR(__BLUE("[原始装备]::"));
			text.append(item_2_text);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_EQUIP_RESTORE_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_EQUIP_RESTORE_MAIN);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("＝＝＝【确定恢复】＝＝＝＝ ")), GOSSIP_SENDER_MAIN, __MENU_EQUIP_RESTORE_MAIN + __MENU_EQUIP_RESTORE_ACT_1);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("＝＝＝＝【退出】＝＝＝＝ ")), GOSSIP_SENDER_MAIN, __MENU_EQUIP_RESTORE_MAIN);
		
		}
		else if (action == __MENU_EQUIP_RESTORE_MAIN + __MENU_EQUIP_RESTORE_ACT_1)
		{
			//check if player has enough suili
			if ((_needSuiliNum==0) || player->HasItemCount(30521, _needSuiliNum))
			{
				//remove the item
				player->DestroyItem(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START, true);

				//remove the suili
				if(_needSuiliNum) player->DestroyItemCount(30521, _needSuiliNum, true);

				//add the item
				player->AddItem(pItem->GetProto()->BuyPrice, 1);

				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __BLUE("＝＝＝＝【恢复成功】＝＝＝＝ "), GOSSIP_SENDER_MAIN, __MENU_EQUIP_RESTORE_MAIN);
			}
			else
			{
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __RED("＝＝＝【橙武碎片不够】＝＝＝ "), GOSSIP_SENDER_MAIN, __MENU_EQUIP_RESTORE_MAIN);
			}
		}
	}
	else
	{
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__RED("＝＝请将待还原装备放在第一个格子＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);	
	}

	player->SEND_GOSSIP_MENU(__GOSSIP_EQUIP_RESTORE_DESC, _go->GetGUID());
	return true;	
}
#pragma endregion


//define a wrapper function for the equip system menus
bool Equip_Menus(Player *player, GameObject *_go, uint32 sender, uint32 action)
{
	//check if player is null and go is null
	if (!player ||!_go) return false;

	sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "[Equip_Menus] PLAYER:[%u][%s] === action: %d", player->GetGUID(), player->GetName(), action);

	// Main menu
	if (action >= __MENU_EQUIP_MAIN && action <= __MENU_EQUIP_MAIN + __MENU_SIZE)
	{
		return Menus_Equip_Main(player, _go, sender, action);	
	}
	else if (action >= __MENU_EQUIP_SUB_CREATE && action <= __MENU_EQUIP_SUB_CREATE + __MENU_SIZE)
	{
		return Menus_Equip_Sub_Create(player, _go, sender, action);
	}
	else if (action >= __MENU_EQUIP_SUB_SLOT && action <= __MENU_EQUIP_SUB_SLOT + __MENU_SIZE)
	{
		return Menus_Equip_Sub_Upgrade(player, _go, sender, action);
	}
	else if (action >= __MENU_EQUIP_SUB_MODDISP && action <= __MENU_EQUIP_SUB_MODDISP + __MENU_SIZE)
	{
		return Menus_Equip_Sub_DispMod(player, _go, sender, action);
	}
	else if (action >= __MENU_EQUIP_RESTORE_MAIN && action <= __MENU_EQUIP_RESTORE_MAIN + __MENU_SIZE)
	{
		return Menus_Equip_Sub_Restore(player, _go, sender, action);
	}

	return false;
}


void AddSC_qzqstar_rune_creatures()
{
	Script* newscript;
	
	newscript = new Script;
	newscript->Name = "qzqstar_rune_npc";
	newscript->pGossipHello = [](Player *p, Creature *c) -> bool { return Rune_Menus(p, c, 0, __MENU_TISH_MAIN); };
	newscript->pGossipSelect = &Rune_Menus;
	newscript->RegisterSelf(false);


	//split the rune code to seprate scripte
	//equip system
	newscript = new Script;
	newscript->Name = "qzqstar_equip_system";
	newscript->pGOGossipHello = [](Player *p, GameObject *g) -> bool { return Equip_Menus(p, g, 0, __MENU_EQUIP_MAIN); };
	newscript->pGOGossipSelect = &Equip_Menus;
	newscript->RegisterSelf(false);
}


