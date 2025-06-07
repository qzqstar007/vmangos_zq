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
#include "qzqstar_teleport.h"
#include "qzqstar_db.h"
#include "qzqstar_helper.h"
#include "Chat.h"

#define	__MENU_NONE						0
#define	__MENU_SIZE						999
#define __MENU_SUB_SIZE					99

#define __MENU_TELEPORT_MAIN			 1000
#define __MENU_TELEPORT_EASTKINGDOM		 3000
#define __MENU_TELEPORT_KALIMDOR		 3001
#define __MENU_TELEPORT_TRAINERS		 3002
#define __MENU_TELEPORT_DUNGEONS		 5000
#define __MENU_TELEPORT_RAIDS			 6000



#define	__STR(x)		((std::string)(x)).c_str()
#define	__NSTR(x)		(std::to_string(x))

#define	__BLUE(x)		"|cff002fa7"##x##"|r"
#define	__GREEN(x)		"|cff00b72f"##x##"|r"
#define	__ORANGE(x)		"|cffe85827"##x##"|r"
#define __RED(x)		"|cfff00019"##x##"|r"
#define __YELLOW(x)		"|cfff9dc24"##x##"|r"



const Teleport_Point_t TP_MainCities_ALLIANCE[] = {
	{0, 0, "Elysium", 0, 1015.0f, 115.0f, 2.0f, 0.0f, {0}},	//Elysium	
};

const Teleport_Point_t TP_MainCities_HORDE[] = {
	{0, 0, "Elysium", 0, 1015.0f, 115.0f, 2.0f, 0.0f, {0}},	//Elysium
};

const Teleport_Point_t TP_Trainers_ALLIANCE[] = {
	// Makeup from upper comment, reverse order
	{1, 0,    __XSTR("战士、骑士　 "),   		0, -9468.0f,115.5f,58.0f,5.3f, {0}},
	{2, 1,    __XSTR("猎人、德鲁伊　 "), 		1, 10131.0f,2541.0f,1322.0f,5.8f, {0}},
	{3, 0,    __XSTR("急救烹饪、法牧术贼　 "), 	0, -9471.0f,23.5f,64.5f,0.3f, {0}},
	{4, 0,    __XSTR("采矿、锻造、工程　 "), 	0, -8375.0f,631.0f,95.0f,3.2f, {0}},
	{5, 0,    __XSTR("剥皮、制皮、钓鱼　 "), 	0, -9393.0f,-89.0f,64.5f,6.2f, {0}},
	{6, 0,    __XSTR("炼金、采药、裁缝、附魔　 "), 0, -8915.0f,786.0f,87.5f,3.6f, {0}}
};

const Teleport_Point_t TP_Trainers_HORDE[] = {
/*80122	0	0	6	0	0	0	0	0	0	0	1	0	0	0	0	2257	242	33.5	0.2	0	法师、牧师、术士、盗贼
80121	0	0	6	1	0	0	0	0	0	0	1	0	0	0	0	-2313	-351	-9.4	6.2	0	烹饪急救、战德猎萨
80120	0	0	6	1	0	0	0	0	0	0	1	0	0	0	0	2021	-4725	24.7	6	0	采矿、锻造、工程、钓鱼
80119	0	0	6	1	0	0	0	0	0	0	1	0	0	0	0	-1122	29	143	2.8	0	剥皮制皮、裁缝附魔、采药炼金
*/
	// Makeup from upper comment, reverse order
	{1, 0,    __XSTR("法师、牧师、术士、盗贼　 "),   		0, 2257.0f,242.0f,33.5f,0.2f, {0}},
	{2, 1,    __XSTR("烹饪急救、战德猎萨　 "), 		1, -2313.0f,-351.0f,-9.4f,6.2f, {0}},
	{3, 1,    __XSTR("采矿、锻造、工程、钓鱼　 "), 	1, 2021.0f,-4725.0f,24.7f,6.0f, {0}},
	{4, 1,    __XSTR("剥皮制皮、裁缝附魔、采药炼金　 "), 1, -1122.0f,29.0f,143.0f,2.8f, {0}}
};


const Teleport_Point_t TP_MainLand_EastKingdom[] = {
	// Makeup from upper comment, reverse order
	{1, MAP_EASTERN_KINGDOMS,    __XSTR("西部荒野　 "), MAP_EASTERN_KINGDOMS, -10919.0f,1426.0f,43.0f,3.2f, {0}},
	{2, MAP_EASTERN_KINGDOMS,    __XSTR("赤脊山　 "), MAP_EASTERN_KINGDOMS, -9555.0f,-2201.0f,93.3f,5.5f, {0}},
	{3, MAP_EASTERN_KINGDOMS,    __XSTR("暮色森林/荆棘谷　 "), MAP_EASTERN_KINGDOMS, -11351.0f,-387.0f,65.6f,2.2f, {0}},
	{4, MAP_EASTERN_KINGDOMS,    __XSTR("湿地/阿拉希　 "), MAP_EASTERN_KINGDOMS, -2397.0f,-2505.0f,87.0f,6.3f, {0}},
	{5, MAP_EASTERN_KINGDOMS,    __XSTR("洛克莫丹/荒芜之地　 "), MAP_EASTERN_KINGDOMS, -5957.0f,-3291.0f,275.3f,3.4f, {0}},
	{6, MAP_EASTERN_KINGDOMS,    __XSTR("奥特兰克山脉　 "), MAP_EASTERN_KINGDOMS, 213.4f,-643.2f,115.0f,0.4f, {0}},
	{7, MAP_EASTERN_KINGDOMS,    __XSTR("银松森林　 "), MAP_EASTERN_KINGDOMS, -360.0f,1200.0f,64.82f,0.1f, {0}},
	{8, MAP_EASTERN_KINGDOMS,    __XSTR("悲伤沼泽/诅咒之地　 "), MAP_EASTERN_KINGDOMS, -10682.0f,-2981.0f,36.6f,3.2f, {0}},
	{9, MAP_EASTERN_KINGDOMS,    __XSTR("辛特兰　 "), MAP_EASTERN_KINGDOMS, 2.4f,-2926.0f,118.0f,1.2f, {0}},
	{10, MAP_EASTERN_KINGDOMS,   __XSTR("西瘟疫之地　 "), MAP_EASTERN_KINGDOMS, 1716.5f,-795.0f,57.1f,1.6f, {0}},
	{11, MAP_EASTERN_KINGDOMS,   __XSTR("东瘟疫之地　 "), MAP_EASTERN_KINGDOMS, 1782.0f,-3095.0f,80.0f,1.2f, {0}},
	{12, MAP_EASTERN_KINGDOMS,   __XSTR("黑石山　 "), MAP_EASTERN_KINGDOMS, -7319.7f,-1086.1f,277.0f,3.5f, {0}}
};

const Teleport_Point_t TP_MainLand_Kalimdor[] = {
	// Makeup from upper comment, reverse order
	{1, MAP_KALIMDOR,    	__XSTR("十字路口　 "), MAP_KALIMDOR, -849.5f,-2562.0f,91.6f,2.6f, {0}},
	{2, MAP_KALIMDOR,    	__XSTR("黑海岸　 "), MAP_KALIMDOR, 4829.0f,218.0f,49.6f,6.1f, {0}},
	{3, MAP_KALIMDOR,    	__XSTR("灰谷　 "), MAP_KALIMDOR, 2250.0f,-1743.0f,110.7f,6.0f, {0}},
	{4, MAP_KALIMDOR,    	__XSTR("石爪山　 "), MAP_KALIMDOR, 1389.0f,765.0f,147.6f,1.0f, {0}},
	{5, MAP_KALIMDOR,   	__XSTR("千针石林　 "), MAP_KALIMDOR, -5338.6f,-2772.1f,-41.8f,4.2f, {0}},
	{6, MAP_KALIMDOR,   	__XSTR("凄凉之地　 "), MAP_KALIMDOR, -1064.0f,1979.0f,63.0f,0.0f, {0}},
	{7, MAP_KALIMDOR,    	__XSTR("菲拉斯　 "), MAP_KALIMDOR, -4844.0f,1317.0f,81.0f,0.1f, {0}},
	{8, MAP_KALIMDOR,   	__XSTR("尘泥沼泽　 "), MAP_KALIMDOR, -4081.0f,-3715.0f,45.7f,1.1f, {0}},
	{9, MAP_KALIMDOR,    	__XSTR("艾萨拉　 "), MAP_KALIMDOR, 8004.5f,-6073.4f,113.3f,0.5f, {0}},
	{10, MAP_KALIMDOR,    	__XSTR("费伍德森林　 "), MAP_KALIMDOR, 5242.0f,-718.0f,343.0f,0.1f, {0}},
	{11, MAP_KALIMDOR,    	__XSTR("安戈洛环形山　 "), MAP_KALIMDOR, -6113.0f,-1142.0f,-187.7f,3.5f, {0}},
	{12, MAP_KALIMDOR,   	__XSTR("希利苏斯　"), MAP_KALIMDOR, -6811.0f,836.0f,50.0f,1.0f, {0}}
};

const Teleport_Point_t TP_Dungeons[] = {
	{ 1, MAP_RAGEFIRE_CHASM,    __XSTR("怒焰裂谷　"), 1, 1815,-4419,-18.7,5.2, {11518,11520,11517,4,5,6,7,8,1728,1}},
	{ 2, MAP_WAILING_CAVERNS,   __XSTR("哀嚎洞穴　"), 1, -731.607f,-2218.39f,17.0281f,2.78486f, {3653,3654,3671,3674,3673,3670,7,8,1728,1}},
	{ 3, MAP_DEADMINES,         __XSTR("死亡矿井　"), 0, -11208.7f,1673.52f,24.6361f,1.51067f, {644,1763,646,639,645,6,7,8,1728,1}},
	{ 4, MAP_SHADOWFANG_KEEP,   __XSTR("影牙城堡　"), 0, -234.675,1561.63,76.8921,1.24031, {1,2,3,4,5,6,7,8,9,1}},
	{ 5, MAP_BLACKFATHOM_DEEPS, __XSTR("黑暗深渊　"), 1, 4249.99,740.102,-25.671,1.34062, {1,2,3,4,5,6,7,8,9,1}},
	{ 6, MAP_GNOMEREGAN,        __XSTR("诺莫瑞根　"), 0, -5163.54,925.423,257.181,1.57423, {0}},
	{ 7, MAP_MONASTERY,         __XSTR("血色修道院　"), 1, 2872.6,-764.398,160.332,5.05735, {0}},
	{ 8, MAP_RAZORFEN_KRAUL,    __XSTR("剃刀沼泽　"), 1, -4470.28,-1677.77,81.3925,1.16302, {0}},
	{ 9, MAP_RAZORFEN_DOWNS,    __XSTR("剃刀高地　"), 1, -4657.3,-2519.35,81.0529,4.54808, {0}},
	{10, MAP_ULDAMAN,           __XSTR("奥达曼　"), 0, -6071.37,-2955.16,209.782,0.015708, {0}},
	{11, MAP_MARAUDON,          __XSTR("玛拉顿　"), 1, -1188.37,2879.61,85.7888,5.07366, {0}},
	{12, MAP_ZUL_FARRAK,        __XSTR("祖尔法拉克　"), 1, -6801.19,-2893.02,9.00388,0.158639, {0}},
	{13, MAP_SUNKEN_TEMLE,      __XSTR("沉没的神庙　"), 0, -10177.9,-3994.9,-111.239,6.01885, {0}},
	{14, MAP_BLACKROCK_DEPTHS,  __XSTR("黑石深渊　"), 0, -7179.34,-921.212,165.821,5.09599, {0}},
	{15, MAP_DIRE_MAUL,         __XSTR("厄运之锤　"), 1, -3521.29,1085.2,161.097,4.7281, {0}},
	{16, MAP_SCHOLOMANCE,       __XSTR("通灵学院　"), 0, 1269.64,-2556.21,93.6088,0.620623, {0}},
	{17, MAP_STRATHOLME,        __XSTR("斯坦索姆　"), 0, 3352.92,-3379.03,144.782,6.25978, {0}},
	{18, MAP_BLACKROCK_SPIRE,   __XSTR("黑石塔　"), 0, -7527.05,-1226.77,285.732,5.29626, {0}},
};


const Teleport_Point_t TP_Raids[] = {
	{0,  0,"Elysium", 0, 1015.0f, 115.0f, 2.0f, 0.0f, {0}},	//Elysium
};


#pragma region Teleport Dungeons
#define __MENU_TELEPORT_DUNGEONS_MAIN		(__MENU_TELEPORT_DUNGEONS)
#define __MENU_TELEPORT_DUNGEONS_ACT1		(100)
#define __MENU_TELEPORT_DUNGEONS_ACT2		(200)

#define __DUNGEONS_NUM_PER_PAGE				(8) //display slots per page

const std::string __DUNGEONS_DIFFICULTY_MINIMUM[4] = { __STR(__BLUE("普通　")), __STR(__BLUE("试炼　")), __STR(__BLUE("地狱　")), __STR(__BLUE("梦魇　")) };
const std::string __DUNGEONS_DIFFICULTY[4] = {__STR(__BLUE("＝＝＞普通＜＝＝＝ ")), __STR(__BLUE("＝＝＞试炼＜＝＝＝ ")), __STR(__BLUE("＝＝＞地狱＜＝＝＝ ")), __STR(__BLUE("＝＝＞梦魇＜＝＝＝ "))};
const std::string __DUNGEONS_DIFFICULTY_DISABLED[4] = {__STR(__BLUE("＝＝＞普通＜＝＝＝ ")), __STR(__RED("＝＝＝试炼（未开启）＝＝＝ ")), __STR(__RED("＝＝＝地狱（未开启）＝＝＝ ")), __STR(__RED("＝＝＝梦魇（未开启）＝＝＝ "))};

bool Menus_teleport_Dungeons(Player *player, Creature *_cr, uint32 sender, uint32 action)
{
	//check if player is null and creature is null
	if (!player ||!_cr) return false;

	std::string text = "";

	//display the dungeons list, split to two pages, each page has 8 slots, each slot has 2 lines of text, the first line is the dungeon name, the second line is the difficulty.
	if(action >= __MENU_TELEPORT_DUNGEONS_MAIN && action < __MENU_TELEPORT_DUNGEONS_MAIN + 2)
	{
		//add to the gossip item, split to two pages.
		auto _start_id = 0;
		auto _end_id = __DUNGEONS_NUM_PER_PAGE;
		if(action == __MENU_TELEPORT_DUNGEONS_MAIN) 
		{
			_start_id = 0;
			_end_id = __DUNGEONS_NUM_PER_PAGE; //not include the RUNE_SLOT_PAGE_ONE
		}
		else if (action == __MENU_TELEPORT_DUNGEONS_MAIN + 1)
		{
			_start_id = __DUNGEONS_NUM_PER_PAGE;
			_end_id = __DUNGEONS_NUM_PER_PAGE;
		}

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝请选择要传送的地图＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);	

		for (auto i = _start_id; i < _end_id; i++)
		{
			text = "";
			//get the dungeon name from the array, if the name is empty, then skip it.
			text.append(__STR(TP_Dungeons[i].name));
			
			//get the dungeon information
			uint32 _playerDungeonInfo = sQZAchievements.GetDungeonsInfo(player, TP_Dungeons[i].id);

			//lower 2bit is current difficulty, higher 2bit is achieved difficulty.
			uint32 _currentDifficulty = _playerDungeonInfo & 0x03;
			uint32 _achievedDifficulty = (_playerDungeonInfo >> 2) & 0x03;

			text.append(__STR(" => "));
			text.append(__DUNGEONS_DIFFICULTY_MINIMUM[_achievedDifficulty]);
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR(text), GOSSIP_SENDER_MAIN, __MENU_TELEPORT_DUNGEONS_MAIN + __MENU_TELEPORT_DUNGEONS_ACT1 + i); //add the difficulty to the menu
		}

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE);	
		if (action == __MENU_TELEPORT_DUNGEONS_MAIN)
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("＝＝＝＝＝＝丨下一页｜＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TELEPORT_DUNGEONS_MAIN + 1);	
		}else
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, __STR(__BLUE("＝＝＝＝＝＝丨上一页｜＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TELEPORT_DUNGEONS_MAIN);	
		}
	}

	else if (action >= __MENU_TELEPORT_DUNGEONS_MAIN + __MENU_TELEPORT_DUNGEONS_ACT1 && action < __MENU_TELEPORT_DUNGEONS_MAIN + __MENU_TELEPORT_DUNGEONS_ACT1 + __MENU_SUB_SIZE)
	{	
		//get the real actions = acID
		uint32 _acID = action - __MENU_TELEPORT_DUNGEONS_MAIN - __MENU_TELEPORT_DUNGEONS_ACT1;
		//get the dungeon information
		uint32 _playerDungeonInfo = sQZAchievements.GetDungeonsInfo(player, _acID);

		//lower 2bit is current difficulty, higher 2bit is achieved difficulty.
		uint32 _currentDifficulty = _playerDungeonInfo & 0x03;
		uint32 _achievedDifficulty = (_playerDungeonInfo >> 2) & 0x03;

		//display the info
		text.append(__STR("当前地图：　"));
		text.append(__STR(TP_Dungeons[_acID].name));
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE); 

		text = "";
		text.append(__STR("可挑战难度：　"));
		text.append(__DUNGEONS_DIFFICULTY_MINIMUM[_achievedDifficulty]);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(text), GOSSIP_SENDER_MAIN, __MENU_NONE); 

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(" "), GOSSIP_SENDER_MAIN, __MENU_NONE); 
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR("＝＝＝选择难度传送＝＝＝ "), GOSSIP_SENDER_MAIN,  __MENU_NONE);

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR(__DUNGEONS_DIFFICULTY[0]), GOSSIP_SENDER_MAIN, __MENU_TELEPORT_DUNGEONS_MAIN + __MENU_TELEPORT_DUNGEONS_ACT2 + _acID + 0); 

		for (auto i = 1; i < 4; i++)
		{
			//if the difficulty is not achieved, then display it in red.
			if (i <= _achievedDifficulty)	
			{
				// i * 16 means the difficulty is achieved, so we can teleport to the dungeon.
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR(__DUNGEONS_DIFFICULTY[i]), GOSSIP_SENDER_MAIN, __MENU_TELEPORT_DUNGEONS_MAIN + __MENU_TELEPORT_DUNGEONS_ACT2 + _acID + i * 16); 
			}else
			{
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR(__DUNGEONS_DIFFICULTY_DISABLED[i]), GOSSIP_SENDER_MAIN, __MENU_NONE); 
			}
		}
	}

	else if (action >= __MENU_TELEPORT_DUNGEONS_MAIN + __MENU_TELEPORT_DUNGEONS_ACT2 && action < __MENU_TELEPORT_DUNGEONS_MAIN + __MENU_TELEPORT_DUNGEONS_ACT2 + __MENU_SUB_SIZE)
	{
		//get the real actions = acID
		uint32 _localBytes = action - __MENU_TELEPORT_DUNGEONS_MAIN - __MENU_TELEPORT_DUNGEONS_ACT2;

		//map id is lower 4 bit
		uint32 _acID = _localBytes & 0x0F;

		//difficulty is higher 4 bit, 0 means normal, 1 means trial, 2 means hell, 3 means nightmare.
		uint32 _difficulty = (_localBytes >> 4) & 0x03;

		//get the dungeon information
		uint32 _playerDungeonInfo = sQZAchievements.GetDungeonsInfo(player, _acID);	
		//save the current difficulty
		_playerDungeonInfo &= 0x0C; //clear the lower 4 bit, keep the higher 4 bit.
		_playerDungeonInfo |= _difficulty; //set the lower 4 bit to the difficulty.
		sQZAchievements.SetDungeonsInfo(player, _acID, _playerDungeonInfo); //save the dungeon information to the player's achievements vector.

		player->CLOSE_GOSSIP_MENU();
		//chathandler ...
		ChatHandler(player).PSendSysMessage(9039, TP_Dungeons[_acID].name, _difficulty==0?__STR("普通　 "):_difficulty==1?__STR("试炼　 "):_difficulty==2?__STR("地狱　 "):__STR("梦魇　 "));
		//teleport to the dungeon.
		player->TeleportTo(TP_Dungeons[_acID].tele_mapid, TP_Dungeons[_acID].tele_x, TP_Dungeons[_acID].tele_y, TP_Dungeons[_acID].tele_z, TP_Dungeons[_acID].tele_o);

		return true;
	}

	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _cr->GetGUID());
	return true;
}
#pragma endregion

#pragma region Teleport EastKingdom and Kalimdor

bool Menus_teleport_Common(Player *player, Creature *_cr, uint32 sender, uint32 action)
{
	//check if player is null and creature is null
	if (!player ||!_cr) return false;

	std::string text = "";
	uint32      counts = 0;
	uint32      offset = 0;
	const Teleport_Point_t *pPoints = nullptr;

	

	//display the EastKingdom list, split to two pages, each page has 8 slots, each slot has 2 lines of text, the first line is the dungeon name, the second line is the difficulty.
	if(action >= __MENU_TELEPORT_EASTKINGDOM && action < __MENU_TELEPORT_EASTKINGDOM + __MENU_SUB_SIZE)
	{

		if(action == __MENU_TELEPORT_EASTKINGDOM) 
		{
			counts = sizeof(TP_MainLand_EastKingdom)/sizeof(Teleport_Point_t);
			pPoints = TP_MainLand_EastKingdom;
			offset = 100;
		}
		else if (action == __MENU_TELEPORT_KALIMDOR) 
		{
			counts = sizeof(TP_MainLand_Kalimdor)/sizeof(Teleport_Point_t);
			pPoints = TP_MainLand_Kalimdor;
			offset = 200;
		}
		else if (action == __MENU_TELEPORT_TRAINERS)
		{
			if(player->GetTeam() == ALLIANCE)
			{
				counts = sizeof(TP_Trainers_ALLIANCE)/sizeof(Teleport_Point_t);
				pPoints = TP_Trainers_ALLIANCE;
				offset = 300;
			}else
			{
				counts = sizeof(TP_Trainers_HORDE)/sizeof(Teleport_Point_t);
				pPoints = TP_Trainers_HORDE;
				offset = 400;	
			}
		}

		else return false;

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝请选择要传送的地图＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);

		for (auto i = 0; i < counts ; i++)
		{
			text = "";
			//get the dungeon name from the array, if the name is empty, then skip it.
			text.append(__STR(pPoints[i].name));

			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, __STR(text), GOSSIP_SENDER_MAIN, __MENU_TELEPORT_EASTKINGDOM + offset + i); 
		}

		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _cr->GetGUID());
	}

	else if (action >= __MENU_TELEPORT_EASTKINGDOM + 100 )
	{
		
		auto _acID = action - __MENU_TELEPORT_EASTKINGDOM - 100;
		if(_acID < __MENU_SUB_SIZE) 
		{
			pPoints = TP_MainLand_EastKingdom;
		}
		else if (_acID < __MENU_SUB_SIZE + 100){
			_acID -= 100;
			pPoints = TP_MainLand_Kalimdor;
		}
		else if (_acID < __MENU_SUB_SIZE + 200) {
			_acID -= 200;
			pPoints = TP_Trainers_ALLIANCE;
		}
		else if (_acID < __MENU_SUB_SIZE + 300) {
			_acID -= 300;
			pPoints = TP_Trainers_HORDE;	
		}
		else return false;

		player->CLOSE_GOSSIP_MENU();
		sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "Teleport to id %d", action);

		if(pPoints) player->TeleportTo(pPoints[_acID].tele_mapid, pPoints[_acID].tele_x, pPoints[_acID].tele_y, pPoints[_acID].tele_z, pPoints[_acID].tele_o);
	}

	return true;
}

#pragma endregion


#pragma region Teleport Main
bool Menus_teleport_Main(Player *player, Creature *_cr, uint32 sender, uint32 action)
{
	//check if player is null and go is null
	if (!player || !_cr) return false;

	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
	//player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　主城传送　＜＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
	//player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　专业职业　＜＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TELEPORT_TRAINERS);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　东部王国　＜＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TELEPORT_EASTKINGDOM);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　卡利姆多　＜＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TELEPORT_KALIMDOR);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　副本传送　＜＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_TELEPORT_DUNGEONS);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＞　团本传送　＜＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, " ", GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, __STR(__BLUE("＝＝＝＝＝＝＝＝＝＝＝＝＝＝　")), GOSSIP_SENDER_MAIN, __MENU_NONE);
	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, _cr->GetGUID());
	return true;
}
#pragma endregion

//define a wrapper function for the equip system menus
bool Teleport_Menus(Player *player, Creature *_cr, uint32 sender, uint32 action)
{
	//check if player is null and go is null
	if (!player || !_cr) return false;

	sLog.Out(LOG_BASIC, LOG_LVL_BASIC, "[teleport Menus] PLAYER:[%u][%s] === action: %d", player->GetGUID(), player->GetName(), action);

	// Main menu
	if (action >= __MENU_TELEPORT_MAIN && action <= __MENU_TELEPORT_MAIN + __MENU_SIZE)
	{
		return Menus_teleport_Main(player, _cr, sender, action);
	}

	// East Kingdom
	else if (action >= __MENU_TELEPORT_EASTKINGDOM && action <= __MENU_TELEPORT_EASTKINGDOM + __MENU_SIZE)
	{
		return Menus_teleport_Common(player, _cr, sender, action);
	}

	// Teleport to dungeons menu, return true if the menu is displayed, false otherwise.
	else if (action >= __MENU_TELEPORT_DUNGEONS && action <= __MENU_TELEPORT_DUNGEONS + __MENU_SIZE)
	{
		return Menus_teleport_Dungeons(player, _cr, sender, action);
	}

	return false;
}



void AddSC_qzqstar_cpp_teleport()
{
	Script* newscript;

	newscript = new Script;
	newscript->Name = "qzqstar_teleport";
	newscript->pGossipHello = [](Player *p, Creature *c) -> bool { return Teleport_Menus(p, c, 0, __MENU_TELEPORT_MAIN); };
	newscript->pGossipSelect = &Teleport_Menus;
	newscript->RegisterSelf(false);
}


uint32 QZQSTAR_GET_AC_MAPID(uint32 mapid)
{
	for (uint32 i = 0; i < sizeof(TP_Dungeons) / sizeof(Teleport_Point_t); i++) {
		if (TP_Dungeons[i].mapId == mapid) {
			return i;
		}   
	}
    return 0;
}

