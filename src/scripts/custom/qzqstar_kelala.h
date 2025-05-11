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

#ifndef _QZQSTAR_CPP_KELALA_H
#define _QZQSTAR_CPP_KELALA_H

typedef struct _shop_list {
	uint32			price;
	uint32			item_id;
	std::string		text;
}Shop_List_t;

#define	__XSTR(x)		((std::string)(x)).c_str()

Shop_List_t shop_list_boxes[] = {	
	{ 10, 100001, __XSTR("抽奖一次：必出提升1碎片、1符文石  ") },
	{ 100, 100001, __XSTR("抽奖十次：必出提升10碎片、10符文石  ") },
};

Shop_List_t shop_list_tools[] = {	
	{ 200, 100001, __XSTR("一键永久BUFF  ") },
	{ 50, 100000, __XSTR("道具：一键拾取  ") },
	{ 50, 100001, __XSTR("道具：移动银行  ") },	
	{ 50, 100001, __XSTR("道具：维修机器人  ") },
	{ 50, 100001, __XSTR("背包：猎人专用袋  ") },
	{ 100, 100001, __XSTR("背包：24格VIP背包  ") },
};

Shop_List_t shop_list_mounts[] = {	
	{ 100, 100001, __XSTR("第二赛季坐骑：  ") },
	{ 100, 100001, __XSTR("第三赛季坐骑：  ") },
	{ 100, 100001, __XSTR("第四赛季坐骑：  ") },
	{ 100, 100001, __XSTR("第五赛季坐骑：  ") },
	{ 100, 100001, __XSTR("第六赛季坐骑：  ") },
	{ 100, 100001, __XSTR("第七赛季坐骑：  ") },
};



#endif	//_QZQSTAR_CPP_KELALA_H

