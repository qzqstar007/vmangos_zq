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
#include "qzqstar_helper.h"
#include "Chat.h"

// 添加或更新元素
void Top10Ranking::addOrUpdate(int score, const std::string& name) {
	// 先尝试删除同名条目（如果存在）
	remove(name);

	// 添加新条目
	ranking.emplace(score, name);

	// 如果超过大小限制，删除最低分的
	if (ranking.size() > MAX_SIZE) {
		auto last = ranking.end();
		--last; // 指向最后一个元素（最低分）
		ranking.erase(last);
	}
}

// 删除指定名称的元素
bool Top10Ranking::remove(const std::string& name) {
	bool found = false;
	for (auto it = ranking.begin(); it != ranking.end(); ) {
		if (it->second == name) {
			it = ranking.erase(it);
			found = true;
		}
		else {
			++it;
		}
	}
	return found;
}

// 获取当前排行榜大小
size_t Top10Ranking::size() const {
	return ranking.size();
}

// 新增函数：同时获取第N名的分数和名字
std::pair<int, std::string> Top10Ranking::getEntryByRank(int rank) const {
	if (rank < 1 || rank > MAX_SIZE) {
		return {0, "---"};
	}

	auto it = ranking.begin();
	std::advance(it, rank - 1);
	return{ std::get<0>(*it), std::get<1>(*it) };
}

// 检查是否包含某个名称
bool Top10Ranking::contains(const std::string& name) const {
	for (const auto& entry : ranking) {
		if (entry.second == name) {
			return true;
		}
	}
	return false;
}

