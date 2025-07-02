/*
* Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
* Copyright (C) 2009-2011 MaNGOSZero <https://github.com/mangos/zero>
* Copyright (C) 2011-2016 Nostalrius <https://nostalrius.org>
* Copyright (C) 2016-2017 Elysium Project <https://github.com/elysium-project>
*
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

#ifndef _QZQSTAR_CPP_IDS_H
#define _QZQSTAR_CPP_IDS_H

//this file contains all the IDs of the creatures, items, spells, etc. that are used in the server.

/******************************************************************** 
 *  Item IDs, least first (30000).
 ********************************************************************/
#define ZQ_ITEM_VOUCHER				30000	//Voucher, 点券
#define ZQ_ITEM_BUFF           		30001	//Buff, 一键BUFF
#define ZQ_ITEM_FRAGMENTS   		30002	//Fragments, 提升碎片
#define ZQ_ITEM_FOOD  			    30003	//Food, 食物蜜汁肉松
#define ZQ_ITEM_NEWBAG			    30004	//New Bag, 新背包
#define ZQ_ITEM_RUNE_STONE			30005	//Rune Stone, 符文石

//Functional Items
#define ZQ_ITEM_PET_EGG				30010	//Pet Egg, 宠物蛋，所有玩家出生时获得，跟炉石一样
#define ZQ_ITEM_PET_FOOD			30011	//Pet Food, 宠物食物

//Faction upgrade items
#define ZQ_ITEM_FACTION_UPGRADE		30020	//Faction Upgrade, 阵营升级

//Newbie Boxes
#define ZQ_ITEM_NEWBIE_INTRO        30030	//Newbie Intro, 新手介绍
#define ZQ_ITEM_NEWBIE_BOX			30031	//Newbie Box, 新手宝箱 ~ 30036

//Newbie Items
#define ZQ_ITEM_NEWBIE_ITEM			30041	//Newbie Item, 新手物品 ~ 30046

//VIP Items
#define ZQ_ITEM_VIP_UPGRADE_ITEM	30051	//VIP Item, 会员物品 ~ 30056

//Mounts Items
#define ZQ_ITEM_MOUNTS_ITEM			30061	//Mounts Item, 坐骑物品 ~ 30066

//CHenyi ZHanpao
#define ZQ_ITEM_CHENYI		        30401	//Chenyi Zhanpao, 陈怡之杖
#define ZQ_ITEM_ZHANPAO		        30501	//Chenyi Zhanpao, 陈怡之杖

/******************************************************************** 
 *  Spell IDs, largest first (33386).
 ********************************************************************/
#define ZQ_SPELL_HEARTSTONE			33386	//Heart Stone, 炉石修改（Item ID：6948）
#define ZQ_SPELL_DUNGEON_FLY		33385	//Dungeon Fly, 副本飞行
#define ZQ_SPELL_AUTOPICK   		33384	//Auto Pick, 自动拾取
#define ZQ_SPELL_BONDING   		    33383	//Bonding, 羁绊系统

//summon creature spells, such as critter, pet, mount, etc.
#define ZQ_SPELL_SUMMON_PET			33370	//Summon Pet, 召唤宠物
#define ZQ_SPELL_SUMMON_ROBOT		33371	//Summon Robot, 召唤机器人
#define ZQ_SPELL_SUMMON_BANK		33372	//Summon Bank, 召唤银行
#define ZQ_SPELL_SUMMON_AH			33373	//Summon AH, 召唤拍卖行
#define ZQ_SPELL_SUMMON_STABLE		33374	//Summon Stable, 召唤兽栏

#define ZQ_SPELL_LEECH_PHY		    33361	//Physical Leech, 物理吸血
#define ZQ_SPELL_LEECH_SPELL	    33362	//Spell Leech, 法术吸血

#define ZQ_SPELL_ACHIEVE_STOLE		33363	//Achieve Stolen, 偷取
#define ZQ_SPELL_ACHIEVE_CAST		33364	//Achieve Cast, 施法

#define ZQ_SPELL_SPELL_CHUAN_PROPERTY	33365	//Spell Chuan Property, 传家宝属性
#define ZQ_SPELL_SPELL_CHUAN_APSP   	33366	//Spell Chuan APSP, 传家宝法强攻强
#define ZQ_SPELL_SPELL_CHUAN_HASTE	    33367	//Spell Chuan Haste, 传家宝急速
#define ZQ_SPELL_SPELL_RICH_BONUS	    33368	//Spell Rich Bonus, 富豪奖励

#define ZQ_SPELL_SPELL_DIFFICULTY1	33351	//Spell Difficulty 1, 副本难度1
#define ZQ_SPELL_SPELL_DIFFICULTY2	33352	//Spell Difficulty 2, 副本难度2
#define ZQ_SPELL_SPELL_DIFFICULTY3	33353	//Spell Difficulty 3, 副本难度3

#define ZQ_SPELL_BUFF_ALL           33355   //Spell of Buff all
#define ZQ_SPELL_BUFF_DRAGON_SLAYER 33356   //Call of Dragon slayer
#define ZQ_SPELL_BUFF_WARCHIEF      33357   //Call of Warchief
#define ZQ_SPELL_BUFF_ZANDALA       33358   //Call of ZANDALA




#define ZQ_SPELL_PET_AURA           33311   //Pet Aura, 宠物光环 33311-33317
#define ZQ_SPELL_PET_TRIGGERED      33321   //Pet Triggered Spell, 宠物触发技能 33321-33327, never forget the spell_Proc


#define ZQ_SPELL_MOUNTS_REINDEER            32980   //Mounts, 麋鹿新手坐骑
#define ZQ_SPELL_MOUNTS_REINDEER_LEARN      32981   //Mounts, 麋鹿新手坐骑
#define ZQ_SPELL_MOUNTS_LAND                32982   //Mounts, 坐骑100%
#define ZQ_SPELL_MOUNTS_LAND_LEARN          32983   //Mounts, 坐骑100%
#define ZQ_SPELL_MOUNTS_FLY                 32984   //Mounts, 飞行坐骑
#define ZQ_SPELL_MOUNTS_FLY_LEARN           32985   //Mounts, 飞行坐骑
#define ZQ_SPELL_MOUNTS_TIGER               32986   //Mounts, 幽灵虎
#define ZQ_SPELL_MOUNTS_TIGER_LEARN         32987   //Mounts, 幽灵虎
#define ZQ_SPELL_MOUNTS_GRIYP               32988   //Mounts, 幽灵狮鹫
#define ZQ_SPELL_MOUNTS_GRIYP_LEARN         32989   //Mounts, 幽灵狮鹫
#define ZQ_SPELL_MOUNTS_TURTLE              32990   //Mounts, 海龟
#define ZQ_SPELL_MOUNTS_TURTLE_LEARN        32991   //Mounts, 海龟


#define ZQ_SPELL_BONUS_APSP         32830   //Bonus, 奖励，例如坐骑之类，被动奖励 50攻强，20法伤

//Promotion Group spells lower id
#define ZQ_SPELL_PROMOTION_STAT     32800	//Promotion Stat, 晋升 
#define ZQ_SPELL_PROMOTION_DAMAGE   32805	//Promotion Skill, 晋升 
#define ZQ_SPELL_PROMOTION_CRITDM   32810	//Promotion Critial Damage, 晋升 
#define ZQ_SPELL_PROMOTION_VOIDAM   32815	//Promotion Haste, 晋升 
#define ZQ_SPELL_PROMOTION_HASTE    32820	//Promotion Haste, 晋升 
//Other Promitions from Achievements database

#define ZQ_SPELL_RAND_ENCHANT_END	    32733//随机附魔
#define ZQ_SPELL_RAND_ENCHANT_START	    32690//随机附魔

#define ZQ_SPELL_SLOTS_END              32684	//Slots, 技能栏 - end
#define ZQ_SPELL_SLOTS_START            32531	//Slots 1, 技能栏1 - Start





#define ZQ_SPELL_RUNE_TRIG              31246   //Rune Trigger, 符文触发
#define ZQ_SPELL_RUNE_TRIGGED           31245   //Rune Trigger, 符文触发
#define ZQ_SPELL_JIBAN                  31244   //羁绊系统

#define ZQ_SPELL_RUNE_END               31192	//Rune, 符文 - End
#define ZQ_SPELL_RUNE_START             31021	//Rune, 符文 - Start

/******************************************************************** 
 *  Creature IDs, least first (33386).
 ********************************************************************/
//NPCs from 30000 to 30019
#define ZQ_NPC_KELALA				30000	//Kelala, 克尔拉 +4XXXXX is creature instance id

#define ZQ_NPC_BATTLE_ALLIANCE		30010	//Battle Alliance, 联盟战场指挥官
#define ZQ_NPC_BATTLE_HORDE			30011	//Battle Horde, 部落战场指挥官

//Personal pet from 30020 to 30029
#define ZQ_NPC_ROBOT				30020	//Robot, 机器人
#define ZQ_NPC_BANK 				30021	//Bank, 银行

//Critter combat pet from 30030 to 30039
#define ZQ_NPC_PET_FROST			30031	//Ice Pet, 冰宠
#define ZQ_NPC_PET_FIRE				30032	//Fire Pet, 火宠
#define ZQ_NPC_PET_SHADOW			30033	//Shadow Pet, 影宠
#define ZQ_NPC_PET_NATURE			30034	//Nature Pet, 自然宠
#define ZQ_NPC_PET_ARCANE			30035	//Arcane Pet, 奥术宠
#define ZQ_NPC_PET_HOLY				30036	//Divine Pet, 神圣宠
#define ZQ_NPC_PET_PHY				30037	//Physical Pet, 物理宠

//Dummy training from 30040 to 30049
#define ZQ_NPC_DUMMY_TRAINING		30041	//Dummy Training, 训练假人

//Player summonned creature from 30050 to 30059
#define ZQ_NPC_SPEC_TIGER			30050   //Mounts, 幽灵虎


/******************************************************************** 
 *  Quest IDs, least first (9701).
 ********************************************************************/
#define ZQ_QUEST_NEWBIE				(9699)	//Newbie, 新手
#define ZQ_QUEST_LOGON_IND	        (9700)	//Logon Points Complete Indicator, 登录积分完成指示
#define ZQ_QUEST_SOCIAL_IND	        (9701+3)	//9701-9703Social Points Complete Indicator, 社交积分完成指示


/******************************************************************** 
 *  Gossip IDs, least first (15001).  // two tables,
 ********************************************************************/
#define ZQ_GOSSIP_KELALA_VIP			15001	//VIP Upgrade, 会员升级
#define ZQ_GOSSIP_KELALA_EQ_COLLECT		15002	//Kelala Equipment Collection, 克尔拉装备采集
#define ZQ_GOSSIP_KELALA_MODE_FUNC		15003	//Kelala Mode Function, 克尔拉模式功能
#define ZQ_GOSSIP_SPELL_STOLE   		15010	//Spell Steal, 偷取技能


/******************************************************************** 
 *  MANgos String IDs, least first (9001).
 ********************************************************************/
#define ZQ_MANGOS_STRING_DUNGEON_PLAYER_TELEPORT		9010	//Dungeon Player Teleport, 副本玩家传送
#define ZQ_MANGOS_STRING_DUNGEON_PLAYER_CPLT_NPCS		9011	//Dungeon Player Finish Level, 副本玩家完成等级
#define ZQ_MANGOS_STRING_CHALLENGE_BREAKTHROUGH     	9021	//Dungeon Player Finish Teleport, 副本玩家完成传送

/******************************************************************** 
 *  Random properties.
 ********************************************************************/
#define ZQ_RANDOM_DIFFICULTY_NORMAL		3301	//Normal, 普通
#define ZQ_RANDOM_DIFFICULTY_TRIAL 		3306	//Trial, 试炼
#define ZQ_RANDOM_DIFFICULTY_HELL		3311	//Hell, 地狱
#define ZQ_RANDOM_DIFFICULTY_NIGHTMARE	3316	//Nightmare, 噩梦
#define ZQ_RANDOM_DIFFICULTY_END        3320

#endif	//_QZQSTAR_CPP_IDS_H



//creature id to be removed
/* //movement delete the guards
delete from creature where guid in (23596, 23562,23588, 23560, 23591, 23564, 23563, 23575, 23594, 23586, 23574, 23570);
delete from creature_movement where id in (23575,23594, 23574, 23586, 23570);
//update gazagan npc locations, 



//and some gameobjects should be updated.
//battleground flags template(310001,300002?)
//desks and moonwells 500001-510000
//Black Forge, sieze is 0.3, GO_ID 174045
//Black Anvil, Siege is 0.3, GO_ID 172911

//Position
Start points: every player is in Gazadgan;
1, -7137    -3821   8.5 2.84


//all spell that with unique target should not be push back


//npc_trainer price lower 10 times
update npc_trainer
set spellcost = spellcost/10;
update npc_trainer_template
set spellcost = spellcost/10;


祭坛激活人数的设置参数在
数据库 mangos
表单 gameobject_template
数据列 data0


entry=130511，奥达曼守护者祭坛
entry=133234，奥达曼阿扎达斯祭坛
entry=175706，黑石祭坛
update gameobject_template set data0 = 1 where entry in (130511, 133234, 175706);


update item_template set bonding = 0 where entry in(6339,11130,11145,16207,9149);


//spell id that is tooooo dangerous
creature id   spell id
1200    8909
8550    22917   16592

fireshield name
poison clounde

8137 异种蝎群瘟疫


*/



