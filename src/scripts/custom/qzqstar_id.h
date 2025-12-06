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
// ID plans
// 30000 - 30009 basic items or must have during several seasons

// 30100 - Season 7

#define ZQ_ITEM_VOUCHER				30000	//Voucher, 点券
#define ZQ_ITEM_BUFF           		30001	//Buff, 一键BUFF
#define ZQ_ITEM_FRAGMENTS   		30002	//Fragments, 提升碎片
#define ZQ_ITEM_FOOD  			    30003	//Food, 食物蜜汁肉松
#define ZQ_ITEM_NEWBAG			    30004	//New Bag, 新背包
#define ZQ_ITEM_RUNE_STONE			30005	//Rune Stone, 符文石

//Functional Items
#define ZQ_ITEM_PET_EGG				30010	//Pet Egg, 宠物蛋，所有玩家出生时获得，跟炉石一样
#define ZQ_ITEM_PET_FOOD			30011	//Pet Food, 宠物食物

#define ZQ_ITEM_BAG_HUNTER			30012	//Bag Hunter, 猎人背包

#define ZQ_ITEM_GOLD_COIN           30016   //Gold Bar, 金条
#define ZQ_ITEM_GOLD_BAR            30018   //Gold Bar, 金砖

//Faction upgrade items
#define ZQ_ITEM_FACTION_UPGRADE		30020	//Faction Upgrade, 阵营升级

//Newbie Boxes
#define ZQ_ITEM_NEWBIE_INTRO        30030	//Newbie Intro, 新手介绍
#define ZQ_ITEM_NEWBIE_BOX			30031	//Newbie Box, 新手宝箱 ~ 30036

//Newbie Items
#define ZQ_ITEM_NEWBIE_ITEM			30041	//Newbie Item, 新手物品 ~ 30046

//VIP Items
#define ZQ_ITEM_VIP_UPGRADE_ITEM	30052	//VIP Item, 会员物品 ~ 30056

//Mounts Items
#define ZQ_ITEM_MOUNTS_ITEM			30061	//Mounts Item, 坐骑物品 ~ 30066


#define ZQ_ITEM_TURTLE              30066   //Turtle, 乌龟
//Bonus Box
#define ZQ_ITEM_NEWBIE_BOX          30078
#define ZQ_ITEM_BONUS_BOX			30088	//Bonus Box, 奖励宝箱 ~ 30061

//raid Reset 
#define ZQ_ITEM_RAID_ZG             30091   //Raid ZG, 副本重置
#define ZQ_ITEM_RAID_MC             30092   //Raid HL, 副本重置黑龙MM
#define ZQ_ITEM_RAID_HL             30093   //Raid MC, 副本重置MC
#define ZQ_ITEM_RAID_FX             30094   //Raid BWL, 副本重置BWL
#define ZQ_ITEM_RAID_BWL            30095   //Raid FX, 副本废墟
#define ZQ_ITEM_RAID_TAQ            30096   //Raid TAQ, 副本TZQ
#define ZQ_ITEM_RAID_NAXX           30097   //Raid NAXX, 副本NAXX

// 30200 - season 8
#define ZQ_ITEM_ACHIVE_ITEM         30201   //Achievement item, 成就物品
#define ZQ_ITEM_CHENYI		        30251	//Chenyi Zhanpao, 陈怡之杖
#define ZQ_ITEM_ZHANPAO		        30261	//Chenyi Zhanpao, 陈怡之杖

/******************************************************************** 
 *  Spell IDs, largest first (33386).
 ********************************************************************/
#define ZQ_SPELL_HEARTSTONE			33386	//Heart Stone, 炉石修改（Item ID：6948）
#define ZQ_SPELL_HS_VIP     		33385	//Dungeon Fly, 副本爆本，传送
#define ZQ_SPELL_AUTOPICK   		33384	//Auto Pick, 自动拾取
#define ZQ_SPELL_BONDING   		    33383	//Bonding, 羁绊系统

#define ZQ_SPELL_VIP_HASTE			33380	//VIP Haste, 会员急速
#define ZQ_SPELL_VIP_HASTE_ONOFF    33381	//VIP APSP, 会员ONOFF
#define ZQ_SPELL_VIP_DMG			33379	//VIP APSP, 会员Damage

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

#define ZQ_SPELL_SPELL_DIFFICULTY0  33350	//Spell Difficulty 0, Wolrd Difficutly 0
#define ZQ_SPELL_SPELL_DIFFICULTY1	33351	//Spell Difficulty 1, 副本难度1
#define ZQ_SPELL_SPELL_DIFFICULTY2	33352	//Spell Difficulty 2, 副本难度2
#define ZQ_SPELL_SPELL_DIFFICULTY3	33353	//Spell Difficulty 3, 副本难度3

#define ZQ_SPELL_BUFF_ALL           33355   //Spell of Buff all
#define ZQ_SPELL_BUFF_DRAGON_SLAYER 33356   //Call of Dragon slayer
#define ZQ_SPELL_BUFF_WARCHIEF      33357   //Call of Warchief
#define ZQ_SPELL_BUFF_ZANDALA       33358   //Call of ZANDALA

#define ZQ_SPELL_RERANDOM			33348	//Rerandom, 随机附魔重置


#define ZQ_SPELL_CHALLENGE_BONUS_RICH	    33330	//Spell Rich Bonus, 富豪奖励   //33368
#define ZQ_SPELL_CHALLENGE_BONUS_ASCE       33331   //Challenge Mode ASCE, 修行模式
#define ZQ_SPELL_CHALLENGE_BONUS_LEADER     33332   //Challenge Mode LEADER, 领袖模式奖励

#define ZQ_SPELL_PET_AURA           33311   //Pet Aura, 宠物光环 33311-33317
#define ZQ_SPELL_PET_TRIGGERED      33321   //Pet Triggered Spell, 宠物触发技能 33321-33327, never forget the spell_Proc

#define SPELL_ENCHANT_WEAPON_BEGIN      32511
#define SPELL_ENCHANT_ARMOR_BEGIN       32521
#define SPELL_ENCHANT_JEWELRY_BEGIN     32526

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


#define ZQ_SPELL_RACE_BONUS_END             32940
#define ZQ_SPELL_RACE_BONUS_START           32850

#define ZQ_SPELL_BONUS_AP         32830   //Bonus, 奖励，例如坐骑之类，被动奖励 攻强
#define ZQ_SPELL_BONUS_SP         32831   //Bonus, 奖励，例如坐骑之类，被动奖励 法强

//Promotion Group spells lower id
#define ZQ_SPELL_PROMOTION_STAT     32800	//Promotion Stat, 晋升 
#define ZQ_SPELL_PROMOTION_DAMAGE   32805	//Promotion Skill, 晋升 
#define ZQ_SPELL_PROMOTION_CRITDM   32810	//Promotion Critial Damage, 晋升 
#define ZQ_SPELL_PROMOTION_VOIDAM   32815	//Promotion Haste, 晋升 
#define ZQ_SPELL_PROMOTION_HASTE    32820	//Promotion Haste, 晋升 
//Other Promitions from Achievements database
#define ZQ_SPELL_MOUNTS_BONUS_TIGER         32825   //Mounts, Bonus
#define ZQ_SPELL_MOUNTS_BONUS_GRIYP         32826   //Mounts, Bonus
#define ZQ_SPELL_MOUNTS_BONUS_DKM           32827   //Mounts, Bonus
#define ZQ_SPELL_MOUNTS_BONUS_ZGH           32828   //Mounts, Bonus
#define ZQ_SPELL_MOUNTS_BONUS_ZGL           32829   //Mounts, Bonus
#define ZQ_ITEM_MOUNTS_DKM                  13335
#define ZQ_ITEM_MOUNTS_ZGH                  19902
#define ZQ_ITEM_MOUNTS_ZGL                  19872


#define ZQ_SPELL_RAND_ENCHANT_END	    32733//随机附魔
#define ZQ_SPELL_RAND_ENCHANT_START	    32690//随机附魔

#define ZQ_SPELL_SLOTS_END              32684	//Slots, 技能栏 - end
#define ZQ_SPELL_SLOTS_START            32531	//Slots 1, 技能栏1 - Start

#define ZQ_SPELL_EQ_BONUS_END               32499   //Equipment spell bonus for special slots
#define ZQ_SPELL_EQ_BONUS_START             32401


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
#define ZQ_MANGOS_STRING_PVP_KILLERS            		9022	//Pvp Killers, 玩家击杀

/******************************************************************** 
 *  Random properties.
 ********************************************************************/
#define ZQ_RANDOM_DIFFICULTY_NORMAL		3301	//Normal, 普通
#define ZQ_RANDOM_DIFFICULTY_TRIAL 		3306	//Trial, 试炼
#define ZQ_RANDOM_DIFFICULTY_HELL		3311	//Hell, 地狱
#define ZQ_RANDOM_DIFFICULTY_NIGHTMARE	3316	//Nightmare, 噩梦
#define ZQ_RANDOM_DIFFICULTY_END        3320

#define ZQ_ENCHANT_KILLERS		        3350   //Begin need +1 


#define ZQ_ENCHANT_LEECH                3355    //Leech, 吸血
#define ZQ_ENCHANT_SPELL_POWER_BONUS    3357    //Spell Power Bonus, 法术能量奖励
#define ZQ_ENCHANT_HEROIC               3371    //Heroic, 英雄 to 3373, 3374 to 3376 and 3377 is best
#define ZQ_ENCHANT_HEROIC_LEECH2        3378    //Heroic Leech 2, 英雄吸血2,78/79/80, and best 81

#define ZQ_ENCHANT_TALENT_START         5001    //Talent Start, 天赋开始
#define ZQ_ENCHANT_TALENT_END           5999    //Talent End, 天赋结束
#define ZQ_ENCHANT_ROBE                 5001
#define ZQ_ENCHANT_ROBE_NUM              192
#define ZQ_ENCHANT_LEG                  5201
#define ZQ_ENCHANT_LEG_NUM               187
#define ZQ_ENCHANT_FEET                 5401
#define ZQ_ENCHANT_FEET_NUM              189
#define ZQ_ENCHANT_GLOVE                5601
#define ZQ_ENCHANT_GLOVE_NUM             134
#define ZQ_ENCHANT_CLOAK                5801
#define ZQ_ENCHANT_CLOAK_NUM             149
#define ZQ_ENCHANT_BELT                 6001
#define ZQ_ENCHANT_BELT_NUM              150
#define ZQ_ENCHANT_SHOUWAN              6201
#define ZQ_ENCHANT_SHOUWAN_NUM            92
#define ZQ_ENCHANT_SHOULDER             6301
#define ZQ_ENCHANT_SHOULDER_NUM           99
#define ZQ_ENCHANT_HEAD                 6401
#define ZQ_ENCHANT_HEAD_NUM               82



#endif	//_QZQSTAR_CPP_IDS_H

//Ways to generate mapfile
//mangosd->linker->debug etc....

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
7102 腐化传染
血之诅咒 ID: 8282
玛格拉姆灵魂诅咒 ID: 18159


//update the creatures in raid maps .. ruins of anqiraj 


update the items sell price to 1 silver //盛典
update item_template set sell_price=100,bonding=0,max_count=0 where entry in (11754,18335,12219, 12735, 22526,22527, 22528, 11078, 22525, 22529);
update item_template set sell_price=1000,bonding=0,max_count=0 where entry in (11732, 11733, 11734, 11736, 11737, 18332, 18333, 18334);


update item_template set bonding=2
where (quality > 2 and quality < 5)  and (class = 4 or class = 2);


//update spell_template recovery time
update spell_template set recoveryTime = 7200000 where recoveryTime > 7200000;
update spell_template set recoveryTime = 7200000 where recoveryTime > 7200000;
update spell_template set categoryRecoveryTime = 7200000 where categoryRecoveryTime > 7200000;

//所有掉落图纸不限制专业分支。
update item_template set required_spell = 0 where required_spell > 0;


//制造类的法术释放时间降低
update spell_template set castingTimeIndex=5 where effect1=24 and castingTimeIndex>5;
update spell_template set startRecoveryTime=1100 where startRecoveryTime=1500;

//掉落G
update creature_template set gold_min = gold_min/20, gold_max=gold_max/20 where gold_max > 50000;

//ZG BOSS
update creature_template set health_multiplier=1500, damage_multiplier=50 where entry in (14510,14509,14507, 14517, 14515, 11382, 14834, 11380);
update creature_template set health_multiplier=500, damage_multiplier=30 where entry in (14510,14509,14507, 14517, 14515, 11382, 14834, 11380);

//MC BOSS, and ONYKIA
11988 12057 12259 11502 12264 11982 12098 12056 12118
update creature_template set health_multiplier=2000, damage_multiplier=60 where entry in (11988, 12057, 12259, 11502, 12264, 11982, 12098, 12056, 12118, 10184);
-- alians 12099 11673 11661 11663 11662 12119 11664 11672 11659 11658, 12129
update creature_template set health_multiplier=500, damage_multiplier=30 where entry in (12099, 11673, 11661, 11663, 11662, 12119, 11664, 11672, 11659, 11658, 12129);

//S8
update creature_template set health_multiplier=800, damage_multiplier=30 where entry in (11988, 12057, 12259, 11502, 12264, 11982, 12098, 12056, 12118, 10184);
update creature_template set health_multiplier=100, damage_multiplier=15 where entry in (12099, 11673, 11661, 11663, 11662, 12119, 11664, 11672, 11659, 11658, 12129);

-- ONY
update creature_template set health_multiplier=2000, damage_multiplier=100 where entry in (10184, 12129);
update creature_template set health_multiplier=800, damage_multiplier=30 where entry in (10184, 12129);
update creature_template set health_multiplier=500, damage_multiplier=30 where entry in (14510,14509,14507, 14517, 14515, 11382, 14834, 11380);
update creature_template set health_multiplier=800, damage_multiplier=30 where entry in (11988, 12057, 12259, 11502, 12264, 11982, 12098, 12056, 12118, 10184);
update creature_template set health_multiplier=100, damage_multiplier=15 where entry in (12099, 11673, 11661, 11663, 11662, 12119, 11664, 11672, 11659, 11658, 12129);


-- BWL BOSS
-- 14020, 14601 13020 11583 11981 12435 11983 12017
update creature_template set health_multiplier=2500, damage_multiplier=70 where entry in (14020, 14601, 13020, 11583, 11981, 12435, 11983, 12017);
-- some spell 19873, destroy eggs
update spell_template set castingTimeIndex=1, recoveryTime=1000 where entry = 19873;

-- AQ boss
-- 15339, 15370, 15369, 15348, 15341, 15340
update creature_template set health_multiplier=3000, damage_multiplier=80 where entry in (15339, 15370, 15369, 15348, 15341, 15340);
-- 15510, 15543,15727,15511,15509,15517,15516,15275,15276,15544,15299,15263
update creature_template set health_multiplier=3500, damage_multiplier=90 where entry in (15510, 15543,15727,15511,15509,15517,15516,15275,15276,15544,15299,15263);

-- NAXX BOSS
-- 15936 16060  15954 15930 15990 15932 15928 15953 16062 16064 16065 16028 15929 16063 15952 16011 15989 15931 16061 15956
update creature_template set health_multiplier=4000, damage_multiplier=110 where entry in (15936, 16060, 15954, 15930, 15990, 15932, 15928, 15953, 16062, 16064, 16065, 16028, 15929, 16063, 15952, 16011, 15989, 15931, 16061, 15956);



-- T2.5
start from 21329 to  21418



ChatHandler(m_caster->ToPlayer()).HandleInstanceUnbindHelper(m_caster->ToPlayer(), false, 0);


//Mounts descriptions
#define ZQ_ITEM_MOUNTS_ZGH                  19902
#define ZQ_ITEM_MOUNTS_ZGL                  19872
update locales_item set description_loc4="此物品放在背包或者银行里生效。" where entry in (13335,19902, 19872 );
17481, 24252, 24242
update spell_template set castingTimeIndex=1 where entry in (17481, 24252, 24242);









//BAGS Modify
//slot 4-6
//1537	11845	805	828	2082	4238	4496	4957	5081	5571	5572	5762	6756	184	2115	4930
//spell id 9139,9394
update item_template set container_slots=12,spellid_1=9139,spelltrigger_1=1, spellid_2=9394,spelltrigger_2=1 where entry in (1537,11845,805,828,2082,4238,4496,4957,5081,5571,5572,5762,6756,184,2115,4930);

//Slot 8 -> 14
//856	2657	3233	3343	4240	4241	4498	5573	5574	5763	806	855	965	3298	3568	5603	6754
//spelldi 9141,9396
update item_template set container_slots=14,spellid_1=9141,spelltrigger_1=1, spellid_2=9396,spelltrigger_2=1 where entry in (856,2657,3233,3343,4240,4241,4498,5573,5574,5763,806,855,965,3298,3568,5603,6754);

//Slot 10 -> 16
//804	857	918	932	933	1470	1729	3352	4245	4497	5575	5576	5764	5765	6446	930	931	1014	1724
//spellid 9329,9398
update item_template set container_slots=16,spellid_1=9329,spelltrigger_1=1, spellid_2=9398,spelltrigger_2=1 where entry in (804,857,918,932,933,1470,1729,3352,4245,4497,5575,5576,5764,5765,6446,930,931,1014,1724);

//Slot 12 -> 18
//1652	1725	4499	10050	10051	16057	1623	3762	4981
//spellid 9331,9416
update item_template set container_slots=18,spellid_1=9331,spelltrigger_1=1, spellid_2=9416,spelltrigger_2=1 where entry in (1652,1725,4499,10050,10051,16057,1623,3762,4981);

//Slot 14 -> 20
//1685	3914	9587	11324	14046	19291
//spellid 14027,9417
update item_template set container_slots=20,spellid_1=14027,spelltrigger_1=1, spellid_2=9417,spelltrigger_2=1 where entry in (1685,3914,9587,11324,14046,19291);

//Slot 16 -> 22
//4500	10959	11742	14155	17966	20400	4501	10683	22233	4500	10959	11742	14155
//spellid 9335,9343
update item_template set container_slots=22,spellid_1=9335,spelltrigger_1=1, spellid_2=9343,spelltrigger_2=1 where entry in (4500,10959,11742,14155,17966,20400,4501,10683,22233,4500,10959,11742,14155);

//Slot 18 - > 24
//14156	19914	22679	17966	13330	1977	
//spellid 15807,9345
update item_template set container_slots=24,spellid_1=15807,spelltrigger_1=1, spellid_2=9345,spelltrigger_2=1 where entry in (14156,19914,22679,17966,13330,1977);

//// PERM Enchanted ID 
-- SQL
SELECT DISTINCT ST.entry, ST.effectMiscValue1, DBC.EffectArg_1, DBC.Name_deDE, STX.description, STX.effectBasePoints1
from spell_template as ST 
join db_spellitemenchantment_5875 as DBC on DBC.ID=ST.effectMiscValue1
join spell_template as STX on STX.entry=DBC.EffectArg_1
where ST.effect1=53
order by entry asc;
7473,14673,13838,14587,19984,19985,19986,13825,19990,19991,19992,21930,19987,13826,13827,13923,13823,13368,13828,13928,20007,20005,
26158,22748,13889,13839,14439,13735,14412,13198,20006,20004,19988,25066,25065,25064,25067,23794,25063,13697,23795,23796,13690,25068,
25069,13840,25070,25071,13897,13615,13927,27836,28162,28164,28166,29468,29474,29481,29482,7784,13363,7851,13650,13651,13907,23793,23797,
23798,6297,7516,24148,24151,24153,24154,24155,24156,24157,24158,24159,24303,9318,9346,9336,15345,13862,13918,13370,7852,14585,14546,14668,
14626,14707,18676,22780,22841,22843,22847,15568,9784,7217,7219,7419,7445,7427,7442,7452,7455,7747,7456,7773,7778,7780,7850,9782,7215,13369,
13523,13371,7446,13606,13611,13614,13624,13364,13639,13930,13652,13372,13664,13625,13796,13365,13366,13922,13824,6347,15388,15390,15393,
15396,15399,15401,15403,15405,15408,15428,15437,15440,15442,15445,15447,15456,15462,15489,16624

update spell_template set effectBasePoints1 = effectBasePoints1 * 2 + 1 where entry in (
7473,14673,13838,14587,19984,19985,19986,13825,19990,19991,19992,21930,19987,13826,13827,13923,13823,13368,13828,13928,20007,20005,
26158,22748,13889,13839,14439,13735,14412,13198,20006,20004,19988,25066,25065,25064,25067,23794,25063,13697,23795,23796,13690,25068,
25069,13840,25070,25071,13897,13615,13927,27836,28162,28164,28166,29468,29474,29481,29482,7784,13363,7851,13650,13651,13907,23793,23797,
23798,6297,7516,24148,24151,24153,24154,24155,24156,24157,24158,24159,24303,9318,9346,9336,15345,13862,13918,13370,7852,14585,14546,14668,
14626,14707,18676,22780,22841,22843,22847,15568,9784,7217,7219,7419,7445,7427,7442,7452,7455,7747,7456,7773,7778,7780,7850,9782,7215,13369,
13523,13371,7446,13606,13611,13614,13624,13364,13639,13930,13652,13372,13664,13625,13796,13365,13366,13922,13824,6347,15388,15390,15393,
15396,15399,15401,15403,15405,15408,15428,15437,15440,15442,15445,15447,15456,15462,15489,16624);




-- Scroll and other potions
select DISTINCT  ST.entry as "spellID"
from item_template as ITX
join spell_template as ST on ST.entry = ITX.spellid_1
join locales_item as LIT on LIT.entry = ITX.entry
where ITX.spellid_1 > 0 and ITX.spellcharges_1 = -1 and (ST.effect1 = 10 or ST.effect1 = 6) and ST.effectBasePoints1 > 0 and LIT.name_loc4 != "" ;

433,439,430,434,435,5005,3591,440,441,8118,8096,5006,673,431,8099,8112,432,746,7396,8116,8094,1127,1135,1138,1133,2024,8100,8113,
8070,8119,8097,2367,2374,2378,2379,1159,5004,2639,8115,8091,3219,3166,2380,3222,3220,3160,3164,3267,3268,5007,3680,3593,3223,6512,
1129,4042,8098,8095,8101,8114,8117,8120,4941,5020,5021,2052,6114,6410,5720,5723,6263,6262,7242,7233,7239,7245,7254,7737,7840,7844,
7926,7927,8202,8898,8899,8212,1131,1137,22734,10667,10668,10669,10692,10693,6727,10838,10839,11328,11349,11350,11364,11371,11390,
11392,11396,11334,11405,11406,11474,11732,12175,12177,12178,12176,12174,12179,12608,15229,15231,15233,15279,26030,18234,10256,16322,
16323,16326,16325,16327,16321,16329,17038,11348,17534,17535,17538,17537,17539,17540,17544,17543,17546,17548,17549,17626,17627,17628,
17629,18071,18124,18140,18229,18230,18231,18233,18232,18608,18610,18832,15700,833,20875,21149,21393,21394,21920,21955,22731,22789,
22790,23179,23400,23468,23469,23470,23471,23472,23473,23474,23475,23476,23477,23540,23541,23542,23567,23568,23569,23692,23696,23698,
24005,24355,24361,24363,24384,24411,24409,24410,24413,24414,24412,24382,24417,24383,24707,24800,24869,24833,25037,25660,25722,25804,
25990,26263,26276,27653,27652,27720,27723,27721,27722,7211,29432,29073,29008,29007,30089,30090,29334,29333,29332,29335,11319,17545,
806,6614,8900,29006,29055,29041,30018,30020


update spell_template set effectBasePoints1 = effectBasePoints1 * 2 + 1 where entry in (
433,439,430,434,435,5005,3591,440,441,8118,8096,5006,673,431,8099,8112,432,746,7396,8116,8094,1127,1135,1138,1133,2024,8100,8113,
8070,8119,8097,2367,2374,2378,2379,1159,5004,2639,8115,8091,3219,3166,2380,3222,3220,3160,3164,3267,3268,5007,3680,3593,3223,6512,
1129,4042,8098,8095,8101,8114,8117,8120,4941,5020,5021,2052,6114,6410,5720,5723,6263,6262,7242,7233,7239,7245,7254,7737,7840,7844,
7926,7927,8202,8898,8899,8212,1131,1137,22734,10667,10668,10669,10692,10693,6727,10838,10839,11328,11349,11350,11364,11371,11390,
11392,11396,11334,11405,11406,11474,11732,12175,12177,12178,12176,12174,12179,12608,15229,15231,15233,15279,26030,18234,10256,16322,
16323,16326,16325,16327,16321,16329,17038,11348,17534,17535,17538,17537,17539,17540,17544,17543,17546,17548,17549,17626,17627,17628,
17629,18071,18124,18140,18229,18230,18231,18233,18232,18608,18610,18832,15700,833,20875,21149,21393,21394,21920,21955,22731,22789,
22790,23179,23400,23468,23469,23470,23471,23472,23473,23474,23475,23476,23477,23540,23541,23542,23567,23568,23569,23692,23696,23698,
24005,24355,24361,24363,24384,24411,24409,24410,24413,24414,24412,24382,24417,24383,24707,24800,24869,24833,25037,25660,25722,25804,
25990,26263,26276,27653,27652,27720,27723,27721,27722,7211,29432,29073,29008,29007,30089,30090,29334,29333,29332,29335,11319,17545,
806,6614,8900,29006,29055,29041,30018,30020);



--- pet collections
4401,8485,8486,8487,8488,8489,8490,8491,8491,8492,8494,8495,8496,8497,8498,8499,8500,8501,10360,10361,10392,10393,10394,10398,10822,
10822,11023,11026,11027,11110,11474,11825,11826,11903,12185,12264,12529,13582,13583,15996,18597,18598,19054,19055,19450,20371,20651,
20769,21168,21277,22235,22780,22781,23002,23007,23015,23083,23712

select * from item_template where entry in (4401,8485,8486,8487,8488,8489,8490,8491,8491,8492,8494,8495,8496,8497,8498,8499,8500,8501,10360,10361,10392,10393,10394,10398,10822, 10822,11023,11026,11027,11110,11474,11825,11826,11903,12185,12264,12529,13582,13583,15996,18597,18598,19054,19055,19450,20371,20651,
20769,21168,21277,22235,22780,22781,23002,23007,23015,23083,23712);

dropped good nice pets
8490,8491,8492,8494,8498,8499,10398,10822,11474,12185,12264,12529,13582,13583,18597,18598,
19054,19055,19450,20371,20651,20769,21277,22235,22780,22781,23002,23007,23015,23083


*/

/* Spell Enchanced.
7744
20555
20575 20576 21563
20591
20580
20549
20595
20550
20572
20582
20589
20577
20598
20594
26296 26297 20554
20600
*/


