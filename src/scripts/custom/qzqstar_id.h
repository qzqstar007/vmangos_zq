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

/******************************************************************** 
 *  Spell IDs, largest first (33386).
 ********************************************************************/
#define ZQ_SPELL_HEARTSTONE			33386	//Heart Stone, 炉石修改（Item ID：6948）

//summon creature spells, such as critter, pet, mount, etc.
#define ZQ_SPELL_SUMMON_ROBOT		33371	//Summon Robot, 召唤机器人
#define ZQ_SPELL_SUMMON_BANK		33372	//Summon Bank, 召唤银行
#define ZQ_SPELL_SUMMON_AH			33373	//Summon AH, 召唤拍卖行
#define ZQ_SPELL_SUMMON_STABLE		33374	//Summon Stable, 召唤兽栏


/******************************************************************** 
 *  Creature IDs, least first (33386).
 ********************************************************************/
#define ZQ_NPC_KELALA				30000	//Kelala, 克尔拉


#define ZQ_NPC_ROBOT				30020	//Robot, 机器人
#define ZQ_NPC_BANK 				30021	//Bank, 银行

#endif	//_QZQSTAR_CPP_IDS_H

