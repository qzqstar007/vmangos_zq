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

#ifndef _QZQSTAR_HELPER_H
#define _QZQSTAR_HELPER_H


class Top10Ranking {
private:
	std::multimap<int, std::string, std::greater<int>> ranking; // ��������������
	static const size_t MAX_SIZE = 10; // ����ǰ10��


public:
	Top10Ranking() {
		for (int i = 1; i <= MAX_SIZE; ++i) {
			ranking.emplace(0, "---");
		}
	}
	// ���ӻ����Ԫ��
	void addOrUpdate(int score, const std::string& name);
	// ɾ��ָ�����Ƶ�Ԫ��
	bool remove(const std::string& name);
	// ������������ȡ��N��������
	std::pair<int, std::string> getEntryByRank(int rank) const;
	// ��ȡ��ǰ���а��С
	size_t size() const;
	// ����Ƿ����ĳ������
	bool contains(const std::string& name) const;
};




#define COLOR_BLUE "|cff0829C9"
extern std::string Helper_MakeString(std::string color, const char* fmt, ...);


extern uint32_t Helper_GetDateInt(void);

extern void Helper_Chat(Player *player, const char *fmt, ...);

#endif
