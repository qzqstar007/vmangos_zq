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

#ifndef _QZQSTAR_CPP_TELEPORT_H
#define _QZQSTAR_CPP_TELEPORT_H

#ifndef __XSTR
#define	__XSTR(x)		((std::string)(x)).c_str()
#endif

//point of intreset
typedef struct _teleport_point_t {
	uint32 id;		//point id, unique in map, also used as the index of the array, so it should be continuous, starting from 0.
	uint32 mapId;	//map id, the map id of the point of intreset.
	std::string name;	//name of the point of intreset.
    uint32 tele_mapid;  //map id of the teleport destination.
	float tele_x;		//x coordinate of the point of intreset.
	float tele_y;		//y coordinate of the point of intreset.
	float tele_z;		//z coordinate of the point of intreset.
	float tele_o;		//orientation of the point of intreset.
    uint32 npc_list[10];
}Teleport_Point_t;

extern const Teleport_Point_t TP_Dungeons[18];
extern const Teleport_Point_t TP_Raids[7];

extern uint32 QZQSTAR_GET_AC_MAPID(uint32 mapid);

#endif	//_QZQSTAR_CPP_TELEPORT_H

