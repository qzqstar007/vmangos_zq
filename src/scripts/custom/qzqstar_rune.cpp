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

#include "chat.h"


// RUNE NPC
#define	__MENU_NONE					0
#define __MENU_ZQ_MAIN				100
#define __MENU_RUNE_MAIN			1000
#define __MENU_FRAG_MAIN			2000
#define __MENU_PEAK_MAIN			3000
#define __MENU_SLOT_MAIN			4000
#define __MENU_MODE_MAIN			5000
#define __MENU_SOCIAL_MAIN			6000
#define __MENU_CREATE_MAIN			7000
#define __MENU_OTHER_MAIN			9000
#define __MENU_END					10000

#define	__STR(x)		((std::string)(x)).c_str()
#define	__NSTR(x)		(std::to_string(x))
	
#define	__BLUE(x)		"|cff002fa7"##x##"|r"
#define	__GREEN(x)		"|cff00b72f"##x##"|r"
#define	__ORANGE(x)		"|cffe85827"##x##"|r"
#define __RED(x)		"|cfff00019"##x##"|r"
#define __YELLOW(x)		"|cfff9dc24"##x##"|r"
#define __PINK(x)		"|cff800080"##x##"|r"


bool GossipHello_Rune(Player *player, Creature *_Creature)
{
	//check levels
	//if (player->GetLevel() == 1)  player->ADD_GOSSIP_ITEM(5, __STR(__RED("挑战::|一命自强模式|")), GOSSIP_SENDER_MAIN, __MENU_ZQ_MAIN);
	if (player->GetLevel() >= 5)  player->ADD_GOSSIP_ITEM(5, __STR(__BLUE("=== |　社　区　贡　献　| ===")), GOSSIP_SENDER_MAIN,	__MENU_SOCIAL_MAIN);
	if (player->GetLevel() >= 5)  player->ADD_GOSSIP_ITEM(5, __STR(__BLUE("=== |　挑　战　模　式　| ===")), GOSSIP_SENDER_MAIN,	__MENU_MODE_MAIN);
	if (player->GetLevel() >= 5)  player->ADD_GOSSIP_ITEM(5, __STR(__BLUE("=== |　装　备　创　造　| ===")), GOSSIP_SENDER_MAIN,   __MENU_CREATE_MAIN);
	if (player->GetLevel() >= 5)  player->ADD_GOSSIP_ITEM(5, __STR(__BLUE("=== |　符　文　系　统　| ===")), GOSSIP_SENDER_MAIN,	__MENU_RUNE_MAIN);
	if (player->GetLevel() >= 15) player->ADD_GOSSIP_ITEM(5, __STR(__BLUE("=== |　字　条　提　取　| ===")), GOSSIP_SENDER_MAIN,	__MENU_SLOT_MAIN);
	if (player->GetLevel() >= 20) player->ADD_GOSSIP_ITEM(5, __STR(__BLUE("=== |　碎　片　兑　换　| ===")), GOSSIP_SENDER_MAIN,	__MENU_FRAG_MAIN);
	if (player->GetLevel() >= 60) player->ADD_GOSSIP_ITEM(5, __STR(__BLUE("=== |　巅　峰　等　级　| ===")), GOSSIP_SENDER_MAIN,	__MENU_PEAK_MAIN);
	//if (player->GetLevel() >= 60) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("==下赛季继承点券查询 | 不断完善中 | (赞助另算)=="), GOSSIP_SENDER_MAIN, __MENU_OTHER_MAIN);
	
	//if(player->GetLevel() >= 60 && player->HasSpell(32990) &&(!player->HasSpell(32988)) )	player->ADD_GOSSIP_ITEM(5, __STR(__BLUE("=== |继续 一命自强模式| ====")), GOSSIP_SENDER_MAIN, __MENU_ZQ_MAIN+100);

	player->SEND_GOSSIP_MENU(16001, _Creature->GetGUID());
	return true;
}


#pragma region MENU ZQ Challenge

#define	__MENU_ZQ_SUB_MENU		(__MENU_ZQ_MAIN + 1)

void SendDefaultMenu_ZQ(Player *player, Creature *_Creature, uint32 action)
{
	std::string text = "";
	switch (action)
	{
	case __MENU_ZQ_MAIN: // Main Page
		player->ADD_GOSSIP_ITEM(5, __STR("=====请确认是否开启挑战??======"), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(5, __STR(__RED("!!!=====|开启挑战|======!!!")), GOSSIP_SENDER_MAIN, __MENU_ZQ_SUB_MENU);
		player->ADD_GOSSIP_ITEM(5, __STR(__GREEN("<<<=====|放弃挑战|======>>>")), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->SEND_GOSSIP_MENU(16010, _Creature->GetGUID());
		break;
	case __MENU_ZQ_SUB_MENU: // Cities [ALLIANCE]		
		if ( (player->GetLevel() == 1) && (!(player->GetGroup())) && (!(player->HasSpell(32990))))
		{
			player->LearnSpell(32990, false); //learn the spell
			player->SetPersonalXpRate(1.0f);
			player->ADD_GOSSIP_ITEM(5, __STR(__RED("!!!=====|开启挑战, 一路安全|======!!!")), GOSSIP_SENDER_MAIN, __MENU_NONE);
			ChatHandler(player).PSendSysMessage(9035);
		}
		else player->ADD_GOSSIP_ITEM(5, __STR(__RED("你尚未达到挑战的要求, 例如已经开启挑战或者等级不符合!")), GOSSIP_SENDER_MAIN, __MENU_NONE);

		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
		break;

	case __MENU_ZQ_MAIN + 100:
	{
		//有2个选择， 1. 退出自强模式，获得称号：钢铁荣耀 2.继续自强，获得称号：自强不息
		player->ADD_GOSSIP_ITEM(5, __STR(__GREEN("===== | 退出自强模式 | 正常游戏 | ======!!!")), GOSSIP_SENDER_MAIN, __MENU_ZQ_MAIN + 101);
		player->ADD_GOSSIP_ITEM(5, __STR(__RED("===== | 继续自强模式 | 获得称号 --> ||自强不息||======>>>")), GOSSIP_SENDER_MAIN, __MENU_ZQ_MAIN + 102);
		player->SEND_GOSSIP_MENU(16012, _Creature->GetGUID());
		break;
	}
	case __MENU_ZQ_MAIN + 101:
	{
		//有2个选择， 1. 退出自强模式，获得称号：钢铁荣耀 
		player->RemoveSpell(32990, false, false);
		//player->AddItem(30106);
		//player->AddItem(30523, 100);
		/*
		std::string __name = player->GetName();
		__name.append("·钢铁荣耀");
		sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "Username %s to %s.", player->GetName(), __name);
		player->SetName(__name);*/
		player->ADD_GOSSIP_ITEM(5, __STR(__GREEN("=====|已完成|..|小退后生效|======")), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->SEND_GOSSIP_MENU(16012, _Creature->GetGUID());
		break;
	}
	case __MENU_ZQ_MAIN + 102:
	{
		player->LearnSpell(32988, false); //continue the zq
		//player->AddItem(30106);
		//player->AddItem(30523, 100);
		//std::string __name = player->GetName();
		//__name.append("·自强不息");
		//sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "Username %s to %s.", player->GetName(), __name);
		//player->SetName(__name);
		player->ADD_GOSSIP_ITEM(5, __STR(__GREEN("=====|已完成|..|小退后生效|======")), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->SEND_GOSSIP_MENU(16012, _Creature->GetGUID());
		break;
	}
	}
}

#pragma endregion

#pragma region MENU RUNE

#define	__MENU_RUNE_SLOT_1			(__MENU_RUNE_MAIN)

#define	__RUNE_UPGRADE_ITEM_ALL			(30136)	//ALL
#define	__RUNE_UPGRADE_ITEM_ZLS			(30137)	//战猎萨
#define	__RUNE_UPGRADE_ITEM_QMS			(30138)	//骑牧术
#define	__RUNE_UPGRADE_ITEM_ZFD			(30139)	//贼法德
#define	__MENU_RUNE_TEST_MENU		(__MENU_RUNE_MAIN + 999)

#define __MODE_KILLER       (30849)
uint32 __rune_slot_numbers(Player *player)
{
	//will have one slot every level/15
	uint32 _nums = 1;

	_nums += player->GetLevel() / 15;

	//check if has the __MODE_KILLER
	if (player->HasSpell(__MODE_KILLER)) _nums += 1;

	//extra slots

	return _nums;
}

uint32 __rune_need_num(Player *player, uint32 curslots)
{
	auto needNum = 0;
	auto freeNum = 1;
	if (player->HasSpell(__MODE_KILLER)) freeNum += 1;

	if (curslots < freeNum) needNum = 0;
	else
	{
		needNum = 10 * (curslots - freeNum + 1);
	}
	return needNum;
}

void SendDefaultMenu_Rune(Player *player, Creature *_Creature, uint32 action)
{
	if (!player) return;

	uint32 _MAX_RUNE_SLOT = 0;
	uint32 __RUNE_UPGRADE_ITEM = 0;
	uint32 __RUNE_UPGRADE_ITEM_BASIC = 10;
	uint32 _CLASS_DESC = DEFAULT_GOSSIP_MESSAGE;

	//All player enter this menu, and then got the class menu belongs to him
	Rune_Spell_Menu_t _LocalMenus[RUNE_SPELLS_NUM];

	if (player->GetClass() == CLASS_WARRIOR) { _copy_rune(_Spells_Menu_Warrior, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16021; }
	else if (player->GetClass() == CLASS_MAGE) { _copy_rune(_Spells_Menu_Mage, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16022; }
	else if (player->GetClass() == CLASS_ROGUE) { _copy_rune(_Spells_Menu_Rogue, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16023; }
	else if (player->GetClass() == CLASS_PALADIN) { _copy_rune(_Spells_Menu_Paladin, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16024; }
	else if (player->GetClass() == CLASS_DRUID) { _copy_rune(_Spells_Menu_Druid, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16025; }
	else if (player->GetClass() == CLASS_HUNTER) { _copy_rune(_Spells_Menu_Hunter, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16026; }
	else if (player->GetClass() == CLASS_PRIEST) { _copy_rune(_Spells_Menu_Priest, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16027; }
	else if (player->GetClass() == CLASS_WARLOCK) { _copy_rune(_Spells_Menu_Warlock, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16028; }
	else if (player->GetClass() == CLASS_SHAMAN) { _copy_rune(_Spells_Menu_Shaman, _LocalMenus, RUNE_SPELLS_NUM);  __RUNE_UPGRADE_ITEM = __RUNE_UPGRADE_ITEM_ALL; _CLASS_DESC = 16029; }
	else
	{
		player->ADD_GOSSIP_ITEM(5, __STR("========|该职业暂时不支持...|========="), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->SEND_GOSSIP_MENU(_CLASS_DESC, _Creature->GetGUID());
		return;
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
	
	if (action == __MENU_RUNE_MAIN)
	{
		player->ADD_GOSSIP_ITEM(5, __STR("========|已有符文状态|========="), GOSSIP_SENDER_MAIN, __MENU_NONE);
		text = __STR(__BLUE("=== 最多开启符文槽数量： "));
		text.append(" |cffdd0000 " + __NSTR(_MAX_RUNE_SLOT) + "  |r ");
		player->ADD_GOSSIP_ITEM(5, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);


		//add to the gossip item
		for (size_t i = 0; i < RUNE_SPELLS_NUM; i++)
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
				text.append(__RED("[ 已启用 - 点击遗忘 ]"));
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN + 130 + i);
			}
			else
			{
				text.append(__GREEN("[ 未启用 - 点击学习 ]"));
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN + 120 + i);
			}
		}
	}

#define RUNE_CONFIRM_OFFSET		100
	//1120-1129, learn spell
	else if ( (action >= __MENU_RUNE_MAIN + 120) && ( action <= __MENU_RUNE_MAIN + 129))
	{
		uint32 spell_to_learn_slot = action - (__MENU_RUNE_MAIN + 120 );
		if (!_can_learn_more)
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__RED("符文已满, 没有额外位置, 请返回并删除部分符文再来哦.")), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN);
		}
		else
		{
			text = "学习技能:";
			text.append(_LocalMenus[spell_to_learn_slot].text);
			text.append(__RED(", 需要花费符文石数量Ｘ "));
			text.append(__NSTR((__rune_need_num(player, _player_learned_num))));

			player->ADD_GOSSIP_ITEM(5, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(5,   __STR(__RED("=====| 确定 | 返回 |=====")), GOSSIP_SENDER_MAIN, action + RUNE_CONFIRM_OFFSET);
			player->ADD_GOSSIP_ITEM(5, __STR(__GREEN("=====| 取消 | 返回 |=====")), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN);
		}
	}

	//confirm the spell
	else if ((action >= __MENU_RUNE_MAIN + 120 + RUNE_CONFIRM_OFFSET) && (action <= __MENU_RUNE_MAIN + 129 + RUNE_CONFIRM_OFFSET))
	{
		uint32 spell_to_learn_slot = action - (__MENU_RUNE_MAIN + 120 + RUNE_CONFIRM_OFFSET);

		//check the materials
		if (player->HasItemCount(__RUNE_UPGRADE_ITEM , __rune_need_num(player, _player_learned_num)))
		{
			//delete the item count
			player->DestroyItemCount(__RUNE_UPGRADE_ITEM, __rune_need_num(player, _player_learned_num), true);
			player->LearnSpell(_LocalMenus[spell_to_learn_slot].spell_id, false);
			player->ADD_GOSSIP_ITEM(5, __STR(__RED("====| 学习成功! |=====")), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN);
		}
		else
		{
			player->ADD_GOSSIP_ITEM(5, __STR(__GREEN("====| 材料不够 | 返回 |=====")), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN);
		}

	}

	//1130-1139, forget spell
	else if ((action >= __MENU_RUNE_MAIN + 130) && (action <= __MENU_RUNE_MAIN + 139))
	{
		uint32 spell_to_delete_slot = action - (__MENU_RUNE_MAIN + 130);

		text = __RED("确定要遗忘此技能?-->");
		text.append(_LocalMenus[spell_to_delete_slot].text);
		text.append(__RED("<-- 返还符文石数量Ｘ "));
		text.append(__NSTR(__rune_need_num(player, _player_learned_num - 1) / 2));

		player->ADD_GOSSIP_ITEM(5, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(5, __STR(__RED("====| 确定 | 返回 | =====")), GOSSIP_SENDER_MAIN, action + RUNE_CONFIRM_OFFSET);
		player->ADD_GOSSIP_ITEM(5, __STR(__GREEN("====| 取消 | 返回 |=====")), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN);
	}
	//confirm the spell
	else if ((action >= __MENU_RUNE_MAIN + 130 + RUNE_CONFIRM_OFFSET) && (action <= __MENU_RUNE_MAIN + 139 + RUNE_CONFIRM_OFFSET))
	{
		uint32 spell_to_delete_slot = action - (__MENU_RUNE_MAIN + 130 + RUNE_CONFIRM_OFFSET);

		if (player && player->HasSpell(_LocalMenus[spell_to_delete_slot].spell_id))
		{
			player->RemoveSpell(_LocalMenus[spell_to_delete_slot].spell_id);

			//add runes back to players
			if((_player_learned_num > 0) && (__rune_need_num(player, _player_learned_num-1) > 0))
				player->AddItem(__RUNE_UPGRADE_ITEM,  (__rune_need_num(player, _player_learned_num-1)) / 2 );

			player->ADD_GOSSIP_ITEM(5, __STR(__GREEN("====已经遗忘, |返回|=====")), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN);
		}
		else
			player->ADD_GOSSIP_ITEM(5, __STR(__RED("====出错, |返回|=====")), GOSSIP_SENDER_MAIN, __MENU_RUNE_MAIN);
	}

	player->SEND_GOSSIP_MENU(_CLASS_DESC, _Creature->GetGUID());
}


#pragma endregion

#pragma region  MENU FRAG



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


void SendDefaultMenu_Frag(Player *player, Creature *_Creature, uint32 action)
{
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
void SendDefaultMenu_Peak(Player *player, Creature *_Creature, uint32 action)
{
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
}

#pragma endregion


#pragma region Menu ESS Equipment Slot System

#define	__GOSSIP_SLOT_DESC				(16101)

#define __MENU_SLOT_ACT_1					(10)
#define __MENU_SLOT_ACT_2					(20)
#define __MENU_SLOT_ACT_3					(30)

void SendDefaultMenu_Slot(Player *player, Creature *_Creature, uint32 action)
{
	//Now need to find the first bag
	auto pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START);


	/* not implemented yet
	std::string text = __BLUE("[|功能后续添加|]");
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_SLOT_MAIN);
	player->SEND_GOSSIP_MENU(__GOSSIP_SLOT_DESC, _Creature->GetGUID());

	return; */


	if (!pItem || ( pItem->GetProto()->Class != ITEM_CLASS_WEAPON && pItem->GetProto()->Class != ITEM_CLASS_ARMOR ) || pItem->GetProto()->Spells[0].SpellId == 0
		|| pItem->GetProto()->Quality > 4
		)
	{
		std::string text = __BLUE("[|请将可提取绿字字条的装备放在角色行囊里第一个格子|]");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_SLOT_MAIN);
		player->SEND_GOSSIP_MENU(__GOSSIP_SLOT_DESC, _Creature->GetGUID());
		
		return;
	}

	//get allowed slots for the equipment
	uint8 slots[4];
	pItem->GetProto()->GetAllowedEquipSlots(slots, player->GetClass(), false);
	auto pEquippedItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slots[0]);
	if (!pEquippedItem || pEquippedItem->GetProto()->InventoryType != pItem->GetProto()->InventoryType || pEquippedItem->GetEnchantmentId(PROP_ENCHANTMENT_SLOT_2)<3000
		|| pEquippedItem->GetProto()->SubClass != pItem->GetProto()->SubClass
		|| pEquippedItem->GetProto()->Class != pItem->GetProto()->Class		
		)
	{
		std::string text = __BLUE("[|目标装备需要在角色身上，并有字条槽，保证装备类型一致|]");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_SLOT_MAIN);
		player->SEND_GOSSIP_MENU(__GOSSIP_SLOT_DESC, _Creature->GetGUID());
		
		return;
	}

	//now get the proto spell of old item (to be destroyed)
	auto localIdx = player->GetSession()->GetSessionDbLocaleIndex();

	auto item_destroy_local = sObjectMgr.GetItemLocale(pItem->GetProto()->ItemId);
	auto item_destroy_text = (item_destroy_local == nullptr ? pItem->GetProto()->Name1: item_destroy_local->Name[localIdx]);
	
	auto item_equipped_local = sObjectMgr.GetItemLocale(pEquippedItem->GetProto()->ItemId);
	auto item_equipped_text = (item_equipped_local == nullptr ? pEquippedItem->GetProto()->Name1 : item_equipped_local->Name[localIdx]);

	auto _needGold = pEquippedItem->GetProto()->ItemLevel * pEquippedItem->GetProto()->ItemLevel * pEquippedItem->GetProto()->ItemLevel / 1500 + 1;

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

		if(spell_id[i] != 0)
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
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR(text), GOSSIP_SENDER_MAIN, __MENU_SLOT_MAIN + 10 + i * 10);
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

				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
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
				if (((player->GetMoney()) >(uint32)_needGold * 10000) && (_spellId > 0 && _dbcId > 0) )
				{
					//set the target 
					pEquippedItem->SetEnchantment(PROP_ENCHANTMENT_SLOT_2, _dbcId, 0, 0);

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

			player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());

			break;
		}


		default:
			break;
	}


}
#pragma endregion

#pragma region MENU Mode Exit Menus - ONE Life, ZQ, Killer ... etc

#define	__GOSSIP_MODE_MSG						(16201)

#define __MENU_MODE_SUB_1						(__MENU_MODE_MAIN + 10)
#define	__MENU_MODE_SUB_1_NAME					"[退出一命模式，领取奖励]"
#define __MENU_MODE_SUB_1_ACT_1					(__MENU_MODE_SUB_1 + 1)
#define __MENU_MODE_SUB_1_SPELL					(30841)

#define __MENU_MODE_SUB_2						(__MENU_MODE_SUB_1 + 10)
#define	__MENU_MODE_SUB_2_NAME					"[退出自强模式，领取奖励]"
#define __MENU_MODE_SUB_2_ACT_1					(__MENU_MODE_SUB_2 + 1)
#define __MENU_MODE_SUB_2_SPELL					(30843)

#define __MENU_MODE_SUB_3						(__MENU_MODE_SUB_2 + 10)
#define	__MENU_MODE_SUB_3_NAME					"[退出收藏模式，领取奖励]"
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

//Mode function
#define __MENU_MODE_SUB_31						(__MENU_MODE_MAIN + 100)
#define	__MENU_MODE_SUB_31_NAME					"[收藏模式：查看装等，突破等级]"
#define __MENU_MODE_SUB_31_ACT_1				(__MENU_MODE_SUB_31 + 1)

#define __MENU_MODE_SUB_51						(__MENU_MODE_MAIN + 110)
#define	__MENU_MODE_SUB_51_NAME					"[杀手模式：隐姓埋名，更改名字]"
#define __MENU_MODE_SUB_51_ACT_1				(__MENU_MODE_SUB_51 + 1)


void SendDefaultMenu_Mode(Player *player, Creature *_Creature, uint32 action)
{
	std::string text = "";
	auto pLevel = player->GetLevel();

	switch (action)
	{
		case __MENU_MODE_MAIN:
		{
			player->ADD_GOSSIP_ITEM(5, __STR("=======|满级可以退出挑战，领取奖励|========"), GOSSIP_SENDER_MAIN, __MENU_NONE);
			//Five Modes Exit
			if (player->GetLevel() == 60 && player->HasSpell(__MENU_MODE_SUB_1_SPELL)) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__BLUE(__MENU_MODE_SUB_1_NAME)), GOSSIP_SENDER_MAIN, __MENU_MODE_SUB_1);
			if (player->GetLevel() == 60 && player->HasSpell(__MENU_MODE_SUB_2_SPELL)) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__BLUE(__MENU_MODE_SUB_2_NAME)), GOSSIP_SENDER_MAIN, __MENU_MODE_SUB_2);
			if (player->GetLevel() == 60 && player->HasSpell(__MENU_MODE_SUB_3_SPELL)) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__BLUE(__MENU_MODE_SUB_3_NAME)), GOSSIP_SENDER_MAIN, __MENU_MODE_SUB_3);
			if (player->GetLevel() == 60 && player->HasSpell(__MENU_MODE_SUB_4_SPELL)) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__BLUE(__MENU_MODE_SUB_4_NAME)), GOSSIP_SENDER_MAIN, __MENU_MODE_SUB_4);
			if (player->GetLevel() == 60 && player->HasSpell(__MENU_MODE_SUB_5_SPELL)) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__BLUE(__MENU_MODE_SUB_5_NAME)), GOSSIP_SENDER_MAIN, __MENU_MODE_SUB_5);


			player->ADD_GOSSIP_ITEM(5, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(5, __STR("=======模式功能========"), GOSSIP_SENDER_MAIN, __MENU_NONE);
			
			// change the player's level to 25/35/45/55 etc..
			if (player->HasSpell(__MENU_MODE_SUB_3_SPELL) && pLevel>20 && pLevel<60 && ( (pLevel+5) % 10 == 0)) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__BLUE(__MENU_MODE_SUB_31_NAME)), GOSSIP_SENDER_MAIN, __MENU_MODE_SUB_31);
			//if (player->HasSpell(__MENU_MODE_SUB_5_SPELL) ) player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__BLUE(__MENU_MODE_SUB_51_NAME)), GOSSIP_SENDER_MAIN, __MENU_MODE_SUB_51);


			player->ADD_GOSSIP_ITEM(5, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(5, __STR("<===返回===="), GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN);

			player->SEND_GOSSIP_MENU(__GOSSIP_MODE_MSG, _Creature->GetGUID());
			break;
		}

		case __MENU_MODE_SUB_1: 
		{	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__BLUE(__MENU_MODE_SUB_1_NAME)), GOSSIP_SENDER_MAIN, __MENU_NONE);
			player->ADD_GOSSIP_ITEM(5, __STR(__RED("==|确定退出|===")), GOSSIP_SENDER_MAIN, __MENU_MODE_SUB_1+1);
			/*FIX*/player->ADD_GOSSIP_ITEM(5, "<==取消===", GOSSIP_SENDER_MAIN, __MENU_NONE); player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID()); break;
		}
		case __MENU_MODE_SUB_1 + 1:
		{	if(player->HasSpell(__MENU_MODE_SUB_1_SPELL)) player->RemoveSpell(__MENU_MODE_SUB_1_SPELL, false, false);
			/*FIX*/player->ADD_GOSSIP_ITEM(5, "<==成功退出，返回首页===", GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN); player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID()); break;
		}

		case __MENU_MODE_SUB_2:
		{	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__BLUE(__MENU_MODE_SUB_2_NAME)), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(5, __STR(__RED("==|确定退出|===")), GOSSIP_SENDER_MAIN, __MENU_MODE_SUB_2 + 1);
		/*FIX*/player->ADD_GOSSIP_ITEM(5, "<==取消===", GOSSIP_SENDER_MAIN, __MENU_NONE); player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID()); break;
		}
		case __MENU_MODE_SUB_2 + 1:
		{	if (player->HasSpell(__MENU_MODE_SUB_2_SPELL)) player->RemoveSpell(__MENU_MODE_SUB_2_SPELL, false, false);
		/*FIX*/player->ADD_GOSSIP_ITEM(5, "<==成功退出，返回首页===", GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN); player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID()); break;
		}

		case __MENU_MODE_SUB_3:
		{	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__BLUE(__MENU_MODE_SUB_3_NAME)), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(5, __STR(__RED("==|确定退出|===")), GOSSIP_SENDER_MAIN, __MENU_MODE_SUB_3 + 1);
		/*FIX*/player->ADD_GOSSIP_ITEM(5, "<==取消===", GOSSIP_SENDER_MAIN, __MENU_NONE); player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID()); break;
		}
		case __MENU_MODE_SUB_3 + 1:
		{	if (player->HasSpell(__MENU_MODE_SUB_3_SPELL)) player->RemoveSpell(__MENU_MODE_SUB_3_SPELL, false, false);
		/*FIX*/player->ADD_GOSSIP_ITEM(5, "<==成功退出，返回首页===", GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN); player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID()); break;
		}

		case __MENU_MODE_SUB_4:
		{	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__BLUE(__MENU_MODE_SUB_4_NAME)), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(5, __STR(__RED("==|确定退出|===")), GOSSIP_SENDER_MAIN, __MENU_MODE_SUB_4 + 1);
		/*FIX*/player->ADD_GOSSIP_ITEM(5, "<==取消===", GOSSIP_SENDER_MAIN, __MENU_NONE); player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID()); break;
		}
		case __MENU_MODE_SUB_4 + 1:
		{	if (player->HasSpell(__MENU_MODE_SUB_4_SPELL)) player->RemoveSpell(__MENU_MODE_SUB_4_SPELL, false, false);
		/*FIX*/player->ADD_GOSSIP_ITEM(5, "<==成功退出，返回首页===", GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN); player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID()); break;
		}
		
		case __MENU_MODE_SUB_5:
		{	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__BLUE(__MENU_MODE_SUB_5_NAME)), GOSSIP_SENDER_MAIN, __MENU_NONE);
		player->ADD_GOSSIP_ITEM(5, __STR(__RED("==|确定退出，需要花费１０个符文石|===")), GOSSIP_SENDER_MAIN, __MENU_MODE_SUB_5 + 1);
		/*FIX*/player->ADD_GOSSIP_ITEM(5, "<==取消===", GOSSIP_SENDER_MAIN, __MENU_NONE); player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID()); break;
		}
		case __MENU_MODE_SUB_5 + 1:
		{	if( (player->HasSpell(__MENU_MODE_SUB_5_SPELL))&&(player->HasItemCount(__RUNE_UPGRADE_ITEM_ALL, 10)) )
			{
					player->DestroyItemCount(__RUNE_UPGRADE_ITEM_ALL, 10, true);
					player->RemoveSpell(__MENU_MODE_SUB_5_SPELL, false, false);
			/*FIX*/player->ADD_GOSSIP_ITEM(5, "<==成功退出，返回首页===", GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN); player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID()); break;
			}
			else
			{
				player->ADD_GOSSIP_ITEM(5, "<==符文石数量不够，不能退出===", GOSSIP_SENDER_MAIN, __MENU_MODE_MAIN); player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID()); break;

			}
		}

		//Advanced the level
		case __MENU_MODE_SUB_31:
		{	
			//get the all levels, and expected level
			//directly assign the eqlevel
			//const int _eqLevelEach[] = {25, 40, 55, 70}; - First stage
			const int _eqLevelEach[] = { 20, 40, 55, 70 };	//Second Stage


			auto __pick = pLevel < 26 ? 0 : pLevel < 36 ? 1 : pLevel < 46 ? 2 : 3;
			int32 _needEQLevel = 20 * _eqLevelEach[__pick];

			if ((player->GetLevel()) < 46 && (player->GetClass() == CLASS_DRUID || player->GetClass() == CLASS_PALADIN || player->GetClass() == CLASS_SHAMAN))
			{
				_needEQLevel = _needEQLevel * 19 / 20;
			}

			int32 _curEQLevel = 0;
			for (int i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i)
				if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
				{
					_curEQLevel += pItem->GetProto()->ItemLevel;
					if(pItem->GetProto()->InventoryType == INVTYPE_2HWEAPON) _curEQLevel += pItem->GetProto()->ItemLevel;
				}
			text = "|需要装等: ";
			text.append(__NSTR(_needEQLevel));
			text.append(" , 当前装等:");
			text.append(__NSTR(_curEQLevel));
			text.append(" .");

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE);
			if (_curEQLevel >= _needEQLevel && pLevel<60 )
			{
				player->GiveLevel(pLevel + 1);
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__GREEN("===|满足装等要求,, 已突破等级|===")), GOSSIP_SENDER_MAIN, __MENU_NONE);

				//Announce the player
				auto const& sessions = sWorld.GetAllSessions();
				for (const auto& itr : sessions)
				{
					if (WorldSession* session = itr.second)
					{
						Player* _onlineplayer = session->GetPlayer();
						if (_onlineplayer && _onlineplayer->IsInWorld() && player->IsAlive())
						{
							ChatHandler(_onlineplayer).PSendSysMessage(9051, player->GetName(), _curEQLevel, pLevel, pLevel+1);
						}
					}
				}

			}
			else player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(__RED("==装等不够,,等会再来===")), GOSSIP_SENDER_MAIN, __MENU_NONE);

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
}
#pragma endregion

#pragma region SOCIAL POINTS

//#define __MENU_SOCIAL_MAIN			6000
#define	__MENU_SOCIAL_SELL					(__MENU_SOCIAL_MAIN + 100)
#define	__MENU_SOCIAL_BUY					(__MENU_SOCIAL_MAIN + 200)

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

void SendDefaultMenu_Social(Player *player, Creature *_Creature, uint32 action)
{
	int _socialPoints = 0;
	std::string text;
	auto localIdx = player->GetSession()->GetSessionDbLocaleIndex();
	_socialPoints = player->GetReputationMgr().GetReputation(967);
	int ITEM_COUNTS = sizeof(__item_list) / sizeof(__item_list[0]);

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
		return;
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
					if (FactionEntry const* factionEntry = sObjectMgr.GetFactionEntry(967))
						player->GetReputationMgr().ModifyReputation(factionEntry, _addPoints);

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
		}

	}

	else if (action >= __MENU_SOCIAL_BUY)
	{
		//the listed items should be upon player's level, socialpoints etc..
		//1. first check if chances runs out
#define	_CHANCE_QUEST_END 10254 /*10251-3*/
		auto chances = 3;

		while ((chances > 0) && (action == __MENU_SOCIAL_BUY))
		{
			if (player->CanTakeQuest(sObjectMgr.GetQuestTemplate(_CHANCE_QUEST_END - chances), false))
			{
				player->CompleteQuest(_CHANCE_QUEST_END - chances);
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
			else if (_socialPoints < 15000 || player->GetLevel() < 58) __canSeeRange = _ITEM_BUY_RANGE_3;
			else if (_socialPoints < 20000) __canSeeRange = _ITEM_BUY_RANGE_4;
			else if (_socialPoints < 30000)  __canSeeRange = _ITEM_BUY_RANGE_5;
			else __canSeeRange = _ITEM_BUY_RANGE_6;

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
					if (FactionEntry const* factionEntry = sObjectMgr.GetFactionEntry(967))
					{
						player->GetReputationMgr().ModifyReputation(factionEntry, 0 - __getSocialPointsBySeq(_buy_item_seq));

						player->AddItem(__item_list[_buy_item_seq]);

						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __BLUE(" ===> 已经购买成功，返回 <==== "), GOSSIP_SENDER_MAIN, __MENU_SOCIAL_BUY);
					}
				}
				else
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __RED(" ===> 物品选择有误，返回 <==== "), GOSSIP_SENDER_MAIN, __MENU_SOCIAL_BUY);
				}
			}

		}
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
	}
}

#pragma endregion


#pragma region MENU Equipment Creation

#define	__GOSSIP_EQCREATE_DESC				(16501)

std::string __get_Item_Name(uint32 Class, uint32 SubClass, uint32 Inv)
{
	switch (Class)
	{
	case ITEM_CLASS_WEAPON:
		switch (SubClass)
		{
		case ITEM_SUBCLASS_WEAPON_AXE: 			return  __STR("创造的单手斧·升华　");
		case ITEM_SUBCLASS_WEAPON_AXE2:			return  __STR("创造的双手斧·升华　");
		case ITEM_SUBCLASS_WEAPON_BOW:			return  __STR("创造的弓·升华　");
		case ITEM_SUBCLASS_WEAPON_GUN:			return  __STR("创造的枪·升华　");
		case ITEM_SUBCLASS_WEAPON_MACE:			return  __STR("创造的单手锤·升华　");
		case ITEM_SUBCLASS_WEAPON_MACE2:		return  __STR("创造的双手锤·升华　");
		case ITEM_SUBCLASS_WEAPON_POLEARM:		return  __STR("创造的长柄·升华　");
		case ITEM_SUBCLASS_WEAPON_SWORD:		return  __STR("创造的单手剑·升华　");
		case ITEM_SUBCLASS_WEAPON_SWORD2:		return  __STR("创造的双手剑·升华　");
		case ITEM_SUBCLASS_WEAPON_STAFF:		return  __STR("创造的法杖·升华　");
		case ITEM_SUBCLASS_WEAPON_DAGGER:		return  __STR("创造的匕首·升华　");
		case ITEM_SUBCLASS_WEAPON_THROWN:		return  __STR("创造的飞刀·升华　");
		case ITEM_SUBCLASS_WEAPON_SPEAR:		return  __STR("创造的长矛·升华　");
		case ITEM_SUBCLASS_WEAPON_CROSSBOW:		return  __STR("创造的弩·升华　");
		case ITEM_SUBCLASS_WEAPON_WAND:			return  __STR("创造的魔杖·升华　");
		}
		return  __STR("创造的未知武器·升华　");
	case ITEM_CLASS_ARMOR:
		switch (Inv)
		{
		case INVTYPE_HEAD:				return  __STR("创造的头饰·升华　");
		case INVTYPE_NECK:				return  __STR("创造的项链·升华　");
		case INVTYPE_SHOULDERS:			return  __STR("创造的护肩·升华　");
		case INVTYPE_BODY:				return  __STR("创造的衬衣·升华　");
		case INVTYPE_ROBE:				return  __STR("创造的长袍·升华　");
		case INVTYPE_CHEST:				return  __STR("创造的胸甲·升华　");
		case INVTYPE_WAIST:				return  __STR("创造的腰带·升华　");
		case INVTYPE_LEGS:				return  __STR("创造的护腿·升华　");
		case INVTYPE_FEET:				return  __STR("创造的靴子·升华　");
		case INVTYPE_WRISTS:			return  __STR("创造的护腕·升华　");
		case INVTYPE_HANDS:				return  __STR("创造的护手·升华　");
		case INVTYPE_FINGER:			return  __STR("创造的戒指·升华　");
		case INVTYPE_TRINKET:			return  __STR("创造的饰品·升华　");
		case INVTYPE_SHIELD:			return  __STR("创造的盾牌·升华　");
		case INVTYPE_CLOAK:				return  __STR("创造的披风·升华　");
		case INVTYPE_HOLDABLE:			return  __STR("创造的副手·升华　");
		case INVTYPE_RELIC:				return  __STR("创造的圣物·升华　");
		}
	}
	return  __STR("创造的未知装备·升华　");
}

void SendDefaultMenu_EQCreate(Player *player, Creature *_Creature, uint32 action)
{
	//Now need to find the first bag
	auto pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START);
	if (!pItem || (pItem->GetProto()->Class != ITEM_CLASS_WEAPON && pItem->GetProto()->Class != ITEM_CLASS_ARMOR) 
		|| pItem->GetProto()->DisenchantID == 0
		|| pItem->GetProto()->Quality > 4 || pItem->GetProto()->Quality < 2
		|| pItem->GetProto()->ItemId > 38000
		)
	{
		std::string text = __BLUE("[|请将不一样的装备放在角色行囊里前两个格子，（仅限绿、蓝、紫等可分解装备）|]");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_SLOT_MAIN);
		player->SEND_GOSSIP_MENU(__GOSSIP_EQCREATE_DESC, _Creature->GetGUID());

		return;
	}

	auto pItem2 = player->GetItemByPos(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START + 1);
	if (!pItem2 || (pItem2->GetProto()->Class != ITEM_CLASS_WEAPON && pItem2->GetProto()->Class != ITEM_CLASS_ARMOR) 
		|| pItem2->GetProto()->DisenchantID == 0
		|| pItem2->GetProto()->Quality > 4 || pItem2->GetProto()->Quality < 2
		|| pItem2->GetProto()->ItemId > 38000
		|| pItem->GetProto()->ItemId == pItem2->GetProto()->ItemId
		)
	{
		std::string text = __BLUE("[|请将不一样的装备放在角色行囊里前两个格子，（仅限绿、蓝、紫等可分解装备）|]");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_SLOT_MAIN);
		player->SEND_GOSSIP_MENU(__GOSSIP_EQCREATE_DESC, _Creature->GetGUID());

		return;
	}

	if (   pItem2->GetProto()->InventoryType != pItem->GetProto()->InventoryType
		//|| pItem2->GetProto()->SubClass != pItem->GetProto()->SubClass
		|| pItem2->GetProto()->Class != pItem->GetProto()->Class
		)
	{
		std::string text = __BLUE("[|待合成装备类型必须一致，可以跨甲！|]");
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_SLOT_MAIN);
		player->SEND_GOSSIP_MENU(__GOSSIP_EQCREATE_DESC, _Creature->GetGUID());

		return;
	}

	//now get the proto spell of old item (to be destroyed)
	auto localIdx = player->GetSession()->GetSessionDbLocaleIndex();

	auto item_1_local = sObjectMgr.GetItemLocale(pItem->GetProto()->ItemId);
	auto item_1_text = (item_1_local == nullptr ? pItem->GetProto()->Name1 : item_1_local->Name[localIdx]);

	auto item_2_local = sObjectMgr.GetItemLocale(pItem2->GetProto()->ItemId);
	auto item_2_text = (item_2_local == nullptr ? pItem2->GetProto()->Name1 : item_2_local->Name[localIdx]);

	auto _needGold = pItem->GetProto()->ItemLevel * pItem2->GetProto()->ItemLevel / 300;
	if (_needGold < 1)  _needGold = 1;

	std::string item_new_text = "　";
	item_new_text.append(__STR(player->GetName()));
	//item_new_text.append(__STR("创造的"));
	item_new_text.append(__get_Item_Name(pItem->GetProto()->Class, pItem->GetProto()->SubClass, pItem->GetProto()->InventoryType));

	std::string item_desc = "原合成物品：①　";
	item_desc.append(__STR(item_1_text));
	item_desc.append(__STR("、　②　"));
	item_desc.append(__STR(item_2_text));
	item_desc.append(__STR("。　"));
	item_desc.append(__STR(__GREEN("装备等级：　")));
	//item_desc.append(__STR(player->GetName()));

	//now get the proto of 
	switch (action)
	{
	case __MENU_CREATE_MAIN:
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
		text.append(item_new_text.substr(0, item_new_text.length() - 11));
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_CREATE_MAIN);

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, __STR(" ======= [确定] ======= "), GOSSIP_SENDER_MAIN, __MENU_CREATE_MAIN + __MENU_SLOT_ACT_1);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, __STR(" ======= [返回] ======= "), GOSSIP_SENDER_MAIN, __MENU_CREATE_MAIN);


		player->SEND_GOSSIP_MENU(__GOSSIP_EQCREATE_DESC, _Creature->GetGUID());

		break;
	}

	case __MENU_CREATE_MAIN + __MENU_SLOT_ACT_1:
	case __MENU_CREATE_MAIN + __MENU_SLOT_ACT_2:
	case __MENU_CREATE_MAIN + __MENU_SLOT_ACT_3:
	{

		if ( (player->GetMoney()) < (uint32)_needGold * 10000 )
		{
			player->ADD_GOSSIP_ITEM(5, __RED("<== |金币不够或者武器错误，返回首页| ==="), GOSSIP_SENDER_MAIN, __MENU_CREATE_MAIN);
		}
		else
		{
			//
			auto newItem = sObjectMgr.DynamicGenerateItem(pItem, pItem2, item_new_text, item_desc);

			//remove the item
			player->DestroyItem(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START, true);
			player->DestroyItem(INVENTORY_SLOT_BAG_0, INVENTORY_SLOT_ITEM_START + 1, true);

			//remove money
			player->ModifyMoney(0 - (_needGold * 10000));

			//Set can be used by creator
			Item *ppItem = player->AddItem(newItem->ItemId);
			ppItem->SetGuidValue(ITEM_FIELD_CREATOR, player->GetObjectGuid());

			//Annouce to players online
			//【幸运者】
			if (newItem->Quality > pItem->GetProto()->Quality)
			{
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

			//add new item
			player->ADD_GOSSIP_ITEM(5, __BLUE("<== |合成成功，请检查背包| ==="), GOSSIP_SENDER_MAIN, __MENU_CREATE_MAIN);
		}

		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());

		break;
	}


	default:
		break;
	}


}

#pragma endregion


#pragma region MENU VOUCHER SCORE

#define GET_ITEM_SCORE(p, item, score)		((p)->GetItemCount(item) * score)
#define GET_SPEL_SCORE(p, spell, score)		((p)->HasSpell(spell)? score : 0)
//Other actions
void SendDefaultMenu_Other(Player *player, Creature *_Creature, uint32 action)
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

	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, __STR(text), GOSSIP_SENDER_MAIN, __MENU_OTHER_MAIN);
	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _Creature->GetGUID());
}


#pragma endregion

bool GossipSelect_Rune(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
	// Main menu
	if (action < __MENU_RUNE_MAIN)
		SendDefaultMenu_ZQ(player, _Creature, action);
	if (action < __MENU_FRAG_MAIN)
		SendDefaultMenu_Rune(player, _Creature, action);
	else if (action < __MENU_PEAK_MAIN)
		SendDefaultMenu_Frag(player, _Creature, action);
	else if (action < __MENU_SLOT_MAIN)
		SendDefaultMenu_Peak(player, _Creature, action);
	else if (action < __MENU_MODE_MAIN)
		SendDefaultMenu_Slot(player, _Creature, action);
	else if (action < __MENU_SOCIAL_MAIN)
		SendDefaultMenu_Mode(player, _Creature, action);
	else if (action < __MENU_CREATE_MAIN)
		SendDefaultMenu_Social(player, _Creature, action);
	else if (action < __MENU_OTHER_MAIN)
		SendDefaultMenu_EQCreate(player, _Creature, action);
	else if (action < __MENU_END)
		SendDefaultMenu_Other(player, _Creature, action);

	return true;
}

void AddSC_qzqstar_rune_creatures()
{
	Script* newscript;
	
	newscript = new Script;
	newscript->Name = "qzqstar_rune_npc";
	newscript->pGossipHello = &GossipHello_Rune;
	newscript->pGossipSelect = &GossipSelect_Rune;
	newscript->RegisterSelf(false);
}


