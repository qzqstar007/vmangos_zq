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

#ifndef _QZQSTAR_RUNE_STR_H
#define _QZQSTAR_RUNE_STR_H

typedef struct __rep_list {
	uint32			rep_level;
	uint32			rep_id;
	std::string		rep_name;
}Rep_List_t;

typedef struct __rune_spell_menu {
	uint32			flags;
	uint32			spell_id;
	std::string		text; 
	std::string		desc;
}Rune_Spell_Menu_t;

#ifndef __XSTR
#define	__XSTR(x)		(x)
#endif 
//Rep lists
const Rep_List_t _Rep_List[] =
{
	{1, 1001, __XSTR("主城声望　 ")},
	{1, 369, __XSTR("加基森　 ")},
	{15, 909, __XSTR("暗月马戏团　 ")},
	{25, 59, __XSTR("瑟银兄弟会　 ")},
	{25, 349, __XSTR("拉文霍德　 ")},
	{45, 576, __XSTR("木喉要塞　 ")},
	{60, 270, __XSTR("赞达拉部族　 ")},
	{60, 910, __XSTR("诺兹多姆的子嗣　 ")},
	{15, 1002, __XSTR("战歌峡谷　 ")},
	{25, 1003, __XSTR("阿拉希盆地　 ")},
	{45, 1004, __XSTR("奥特兰克山谷　 ")}
};





#define	RUNE_SPELLS_NUM				(9)

//Should not modify by users.
const Rune_Spell_Menu_t _Spells_Menu_Warrior[RUNE_SPELLS_NUM] = 	//Warrior
{
	/* 0 */{ 0, 31021, __XSTR("无畏冲锋"), __XSTR("释放冲锋、拦截后，攻击速度提升30%，持续15秒。 ") },
	/* 1 */{ 0, 31023, __XSTR("破坏能手"), __XSTR("近战爆击后，自身伤害提升3%并忽略目标200点护甲，持续15秒，可叠加5层。 ") },
	/* 2 */{ 0, 31025, __XSTR("狂暴怒火"), __XSTR("狂暴姿态下的物理伤害和移动速度提升10%。 ") },
	/* 3 */{ 0, 31027, __XSTR("陷阵之志"), __XSTR("受伤后伤害和免伤增加，血量越低加成越高。 ") },
	/* 4 */{ 0, 31029, __XSTR("狂暴回复"), __XSTR("将怒气转换为生命值（受力量加成），持续10秒，1分钟CD。 ") },
	/* 5 */{ 0, 31031, __XSTR("致命猛击"), __XSTR("攻击几率触发下一次猛击为瞬发，伤害提升50%。 ") },
	/* 6 */{ 0, 31033, __XSTR("坚不可摧"), __XSTR("装备盾牌时，所有伤害提升10%，免伤5%。 ") },
	/* 7 */{ 0, 31035, __XSTR("致命重伤"), __XSTR("致命一击导致的重伤效果提升200%。 ") },
	/* 8 */{ 0, 31037, __XSTR("血之气息|"), __XSTR("释放撕裂可激活压制技能，并提升其爆伤100%，持续6秒。 ") }
};

const Rune_Spell_Menu_t _Spells_Menu_Mage[RUNE_SPELLS_NUM] = 	//Mage
{
	/* 0 */{ 0, 31039, __XSTR("寒冰回响"), __XSTR("寒冰系技能几率刷新霜冻新星、寒冰护盾CD。 ") },
	/* 1 */{ 0, 31041, __XSTR("奥术迅捷"), __XSTR("闪现闪现后移动速度增加50%，受伤降低50%，持续6秒。 ") },
	/* 2 */{ 0, 31043, __XSTR("烈焰之痛"), __XSTR("强化灼烧每次提升3层火焰易伤，灼烧暴伤增加50%。 ") },
	/* 3 */{ 0, 31045, __XSTR("奥术唤醒"), __XSTR("唤醒技能同时恢复生命值，并进入隐形状态，持续20秒。 ") },
	/* 4 */{ 0, 31047, __XSTR("寒冰破碎"), __XSTR("当你的寒冰护盾被打破后，自动释放冰霜新星冰冻周围敌人。 ") },
	/* 5 */{ 0, 31049, __XSTR("火焰连击"), __XSTR("火焰冲击暴击伤害提升50%，并使下次炎爆术成为瞬发。 ") },
	/* 6 */{ 0, 31051, __XSTR("玻璃大炮"), __XSTR("造成所有伤害提升，受魔法/生命比例影响。 ") },
	/* 7 */{ 0, 31053, __XSTR("冰冷血脉|"), __XSTR("使所有法术变为瞬发，持续15 秒，CD 2分钟。 ") },
	/* 8 */{ 0, 31055, __XSTR("陨石坠落"), __XSTR("消耗一半法力召唤陨石对区域内敌人造成总计2倍法力伤害并昏迷，3秒施法，30秒CD。 ") }
};

const Rune_Spell_Menu_t _Spells_Menu_Rogue[RUNE_SPELLS_NUM] = 	//Rogue
{
	/* 0 */{ 0, 31057, __XSTR("夜行迅捷"), __XSTR("潜行不再降低移动速度，并提高潜行等级。 ") },
	/* 1 */{ 0, 31059, __XSTR("致命反击"), __XSTR("躲闪后也可以触发反击，反击和鬼魅攻击爆伤提高50%。 ") },
	/* 2 */{ 0, 31061, __XSTR("隐秘刺客"), __XSTR("背刺、伏击不需要目标背后，并且消耗能量减少10点。 ") },
	/* 3 */{ 0, 31063, __XSTR("敏锐大师"), __XSTR("隐身解除后，所有伤害、攻速提升10%，持续6秒。 ") },
	/* 4 */{ 0, 31065, __XSTR("刀扇利刃|"), __XSTR("对周围的所有目标造成150%武器伤害，消耗50能量。 ") },
	/* 5 */{ 0, 31067, __XSTR("暗影斗篷"), __XSTR("立即移除所有负面效果，并免疫魔法6 秒，1分钟CD。 ") },
	/* 6 */{ 0, 31069, __XSTR("持久战意"), __XSTR("普攻暴击可以使得暴击伤害提升10%，持续6秒，最多叠加5层。 ") },
	/* 7 */{ 0, 31071, __XSTR("嗜血成性|"), __XSTR("击中一定几率对目标造成100%武器伤害，并自身恢复5%血量，6秒CD。 ") },
	/* 8 */{ 0, 31073, __XSTR("精力充沛"), __XSTR("满血状态时能量恢复及伤害提升，受伤后能量及伤害降低。 ") }
};

const Rune_Spell_Menu_t _Spells_Menu_Paladin[RUNE_SPELLS_NUM] = 	//Paladin
{
	/* 0 */{ 0, 31075, __XSTR("战争艺术"), __XSTR("攻击几率刷新神圣震击、驱邪术冷却时间。 ") },
	/* 1 */{ 0, 31077, __XSTR("光之优雅"), __XSTR("释放法术几率使下一个圣光术变为瞬发，提升此次治疗效果50%。 ") },
	/* 2 */{ 0, 31079, __XSTR("神圣之地"), __XSTR("释放奉献时可以为周围小队成员恢复生命值。 ") },
	/* 3 */{ 0, 31081, __XSTR("正义盾击"), __XSTR("用盾牌造成100%武器伤害，并嘲讽目标，10秒CD。 ") },
	/* 4 */{ 0, 31083, __XSTR("圣光宠爱"), __XSTR("圣盾术、圣佑术的持续时间及自律时间均延长50%，冷却时间降低一半。 ") },
	/* 5 */{ 0, 28789, __XSTR("神圣之力"), __XSTR("治疗几率使目标获得效果：战骑德增血量，贼猎攻强，其他法伤，持续5分钟。 ") },
	/* 6 */{ 0, 31087, __XSTR("圣光背弃"), __XSTR("造成所有伤害提升15%，治疗效果降低50%，持续30分钟，15分钟CD。 ") },
	/* 7 */{ 0, 31089, __XSTR("殉难打击"), __XSTR("对目标造成200%武器伤害，自身受到一半伤害，6秒CD。 ") },
	/* 8 */{ 0, 31091, __XSTR("神圣恳求"), __XSTR("神每秒恢复魔法总量3%，治疗效果提升100%，持续15秒，1分钟CD。 ") }
};

const Rune_Spell_Menu_t _Spells_Menu_Druid[RUNE_SPELLS_NUM] = 	//Druid
{
	/* 0 */{ 0, 31093, __XSTR("熊的力量"), __XSTR("熊状态减伤10%，力量转AP额外提升50%。 ") },
	/* 1 */{ 0, 31095, __XSTR("豹的速度"), __XSTR("猫状态下移除法术室内限制，敏捷转AP提升100%，提升潜行速度。 ") },
	/* 2 */{ 0, 31097, __XSTR("鹰的智慧"), __XSTR("枭兽形态下法术暴击率提高5%，法术爆伤提升10%。 ") },
	/* 3 */{ 0, 31099, __XSTR("丛林之王"), __XSTR("猛虎之怒时间/增伤提升100%，激活期间攻击额外一个目标；撕碎不需要目标背后。 ") },
	/* 4 */{ 0, 31101, __XSTR("日月之蚀"), __XSTR("月火术暴击后，下一次星火术变为瞬发。 ") },
	/* 5 */{ 0, 31103, __XSTR("生命之树"), __XSTR("化身为生命之树，法术治疗效果提升50%，法术伤害效果降低50%。 ") },
	/* 6 */{ 0, 31105, __XSTR("变形金刚"), __XSTR("熊状态减伤10%，猫状态伤害提升20%，枭兽状态法术爆伤提升25%。 ") },
	/* 7 */{ 0, 31107, __XSTR("刀锋护甲|"), __XSTR("所有状态下，每1000点护甲提升1%伤害。 ") },
	/* 8 */{ 0, 31109, __XSTR("自然武器"), __XSTR("持双手武器时，熊状态调整为此武器攻速，猫状态为一半攻速，可以触发武器绿字效果。 ") }
};

const Rune_Spell_Menu_t _Spells_Menu_Hunter[RUNE_SPELLS_NUM] = 	//Hunter
{
	/* 0 */{ 0, 31111, __XSTR("惺惺相惜"), __XSTR("宠物可以继承猎人20%的五维属性，宠物快乐时伤害额外加成25%。 ") },
	/* 1 */{ 0, 31113, __XSTR("奥术爆炸"), __XSTR("奥术射击带有爆炸效果，造成范围伤害。 ") },
	/* 2 */{ 0, 31115, __XSTR("静息训练"), __XSTR("静止状态每2秒提升2%暴击和暴伤，可叠加5层，移动后状态消失。 ") },
	/* 3 */{ 0, 31117, __XSTR("爆头狙击"), __XSTR("远程普通攻击几率造成自身50%生命值或者2倍AP（较大者）的伤害。 ") },
	/* 4 */{ 0, 31119, __XSTR("疾速杀戮|"), __XSTR("减少急速射击50%冷却时间，开启急速射击时可以提升近战攻速50%。 ") },
	/* 5 */{ 0, 31121, __XSTR("逃脱专家"), __XSTR("往后跳往后跳跃至20码的距离，并增加移速50%，持续6秒。 ") },
	/* 6 */{ 0, 31123, __XSTR("独来独往"), __XSTR("没有宠物时，所有伤害提升30%。 ") },
	/* 7 */{ 0, 31125, __XSTR("野兽鼓舞"), __XSTR("普通攻击暴击可以使宠物回血5%，CD 6秒。 ") },
	/* 8 */{ 0, 31127, __XSTR("百步穿杨"), __XSTR("射出一只闪电之箭并使目标昏迷，伤害随距离增加而提升，CD 20秒。 ") }
};

const Rune_Spell_Menu_t _Spells_Menu_Priest[RUNE_SPELLS_NUM] = 	//Priest
{
	/* 0 */{ 0, 31129, __XSTR("白衣天使"), __XSTR("治疗效果提升25%，治疗类法术耗蓝降低10%，暗影形态下自动失效。 ") },
	/* 1 */{ 0, 31131, __XSTR("神圣庇护"), __XSTR("自身受到伤害后减伤50%，持续6秒，12秒CD。 ") },
	/* 2 */{ 0, 31133, __XSTR("暗影爆炸"), __XSTR("以目标为中心制造一次暗影爆炸，对周围敌人造成暗影伤害。瞬发，3秒CD。 ") },
	/* 3 */{ 0, 31135, __XSTR("神火连击"), __XSTR("神火使惩击爆伤提升25%，惩击暴击时触发下一次神圣之火瞬发。 ") },
	/* 4 */{ 0, 31137, __XSTR("起死复生"), __XSTR("为目标施加一个生命标记，当其濒临死亡时，避免死亡并立刻恢复30%血量。 ") },
	/* 5 */{ 0, 31139, __XSTR("精神引导"), __XSTR("精神分流持续时间延长一倍，伤害性法术几率触发精神分流。 ") },
	/* 6 */{ 0, 31141, __XSTR("坚定信仰"), __XSTR("真言术盾可以为目标增加一个反恐结界，抵抗下一次恐惧。 ") },
	/* 7 */{ 0, 31143, __XSTR("暗影绝望"), __XSTR("使得暗言术痛、精神鞭笞可以暴击。 ") },
	/* 8 */{ 0, 31145, __XSTR("圣光链接"), __XSTR("对目标治疗时，自己可以受到30%治疗。 ") }
};


const Rune_Spell_Menu_t _Spells_Menu_Warlock[RUNE_SPELLS_NUM] = 	//Warlock
{
	/* 0 */{ 0, 31147, __XSTR("暗影迅捷"), __XSTR("造成法伤后，可提升移速10%，减伤2%，持续12秒，可叠加5次。 ") },
	/* 1 */{ 0, 31149, __XSTR("生命冲击"), __XSTR("消耗最多50%生命总值对目标造成2倍伤害，2秒施法，20秒CD。 ") },
	/* 2 */{ 0, 31151, __XSTR("无尽痛苦"), __XSTR("使你的腐蚀术、痛苦诅咒、献祭可以暴击。 ") },
	/* 3 */{ 0, 31153, __XSTR("恶魔知识"), __XSTR("召唤的恶魔仆从继承术士20%的五维属性，恶魔伤害提升100%。 ") },
	/* 4 */{ 0, 31155, __XSTR("生命掌控"), __XSTR("生命吸取、生命虹吸的伤害提升30%，治疗效果翻倍。 ") },
	/* 5 */{ 0, 31157, __XSTR("献祭灵魂"), __XSTR("提高献祭/灼热爆伤30%，暴击触发灵魂之火为瞬发。 ") },
	/* 6 */{ 0, 31159, __XSTR("怜悯之心"), __XSTR("恶魔牺牲不再牺牲仆从，并额外激活一个BUFF。（小鬼+魅魔，虚空+地狱犬） ") },
	/* 7 */{ 0, 31161, __XSTR("恶魔形态|"), __XSTR("化身为恶魔形态，耐力提高10%，耐力50%转化为法伤，智力精神不再转化。 ") },
	/* 8 */{ 0, 31163, __XSTR("暗影风暴"), __XSTR("制造暗影风暴攻击周围敌人并恢复自身生命，瞬发，15秒CD。 ") }
};

const Rune_Spell_Menu_t _Spells_Menu_Shaman[RUNE_SPELLS_NUM] = 	//Shaman
{
	/* 0 */{ 0, 31165, __XSTR("能量涌动"), __XSTR("地震、震击爆伤提升30%，暴击触发治疗链、闪电箭、闪电链为瞬发。 ") },
	/* 1 */{ 0, 31167, __XSTR("元素传导"), __XSTR("治疗链、闪电链传递效果不衰减，并且额外跳跃2个目标。 ") },
	/* 2 */{ 0, 31169, __XSTR("先祖庇护"), __XSTR("格挡/招架几率+5%，每次格挡或者招架时，恢复最大法力5%。 ") },
	/* 3 */{ 0, 31171, __XSTR("双持武器"), __XSTR("可以双持武器，并且副手武器伤害提升25%。 ") },
	/* 4 */{ 0, 31173, __XSTR("闪电过载"), __XSTR("闪电法术有一定几率对目标触发相同法术，不消耗法力，但造成一半伤害。 ") },
	/* 5 */{ 0, 31175, __XSTR("图腾艺术"), __XSTR("力量图腾、风之图腾、法力、治疗图腾效果提升100%。 ") },
	/* 6 */{ 0, 31177, __XSTR("漩涡武器"), __XSTR("近战伤害几率使下一个法术变为瞬发。 ") },
	/* 7 */{ 0, 31179, __XSTR("电荷感应"), __XSTR("闪电法术使得下次闪电伤害提升2%，最多叠加5次。 ") },
	/* 8 */{ 0, 31181, __XSTR("先祖指引"), __XSTR("激活10秒内，每次攻击均可以为队友恢复生命值，受攻击伤害加成。 ") }
};

//here, should be something exactly for string storage

typedef struct __spell_dbc_desc {
	uint32			spell_id;
	uint32			spell_learn_id;	//to be learnt by player
	uint32			dbc_id;
	std::string		text;
}Spell_DBC_t;


extern const Spell_DBC_t __Spells_DBC_Array[630];





#endif	//_QZQSTAR_RUNE_STR_H

