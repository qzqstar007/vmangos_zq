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

// ���ӻ����Ԫ��
void Top10Ranking::addOrUpdate(int score, const std::string& name) {
	// �ȳ���ɾ��ͬ����Ŀ��������ڣ�
	remove(name);

	// ��������Ŀ
	ranking.emplace(score, name);

	// ���������С���ƣ�ɾ����ͷֵ�
	if (ranking.size() > MAX_SIZE) {
		auto last = ranking.end();
		--last; // ָ�����һ��Ԫ�أ���ͷ֣�
		ranking.erase(last);
	}
}

// ɾ��ָ�����Ƶ�Ԫ��
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

// ��ȡ��ǰ���а��С
size_t Top10Ranking::size() const {
	return ranking.size();
}

// ����������ͬʱ��ȡ��N���ķ���������
std::pair<int, std::string> Top10Ranking::getEntryByRank(int rank) const {
	if (rank < 1 || rank > MAX_SIZE) {
		return {0, "---"};
	}

	auto it = ranking.begin();
	std::advance(it, rank - 1);
	return{ std::get<0>(*it), std::get<1>(*it) };
}

// ����Ƿ����ĳ������
bool Top10Ranking::contains(const std::string& name) const {
	for (const auto& entry : ranking) {
		if (entry.second == name) {
			return true;
		}
	}
	return false;
}



extern std::string Helper_MakeString(std::string color, const char* fmt, ...) {
    std::string result;
    
    // 添加颜色代码前缀
    if(color.length() > 2)result.append(color);
    
    // 处理可变参数
    va_list args;
    va_start(args, fmt);
    
    // 计算格式化后的字符串长度
    int length = vsnprintf(nullptr, 0, fmt, args);
    va_end(args);
    
    if (length > 0) {
        // 分配缓冲区
        char* buffer = new char[length + 1];
        
        // 重新获取参数并格式化
        va_start(args, fmt);
        vsnprintf(buffer, length + 1, fmt, args);
        va_end(args);
        
        // 添加格式化后的字符串
        result.append(buffer);
        
        // 释放缓冲区
        delete[] buffer;
    }
    
    // 添加颜色重置代码
	if (color.length() > 2) result.append("|r");
    
    return result;
}


//like 25092114
uint32_t Helper_GetDateInt() {
    // 获取当前时间戳
    std::time_t now = std::time(nullptr);
    
    // 转换为本地时间（考虑时区）
    std::tm* local_time = std::localtime(&now);
    
    // 提取年月日并格式化为整数
    int year = 1900 + local_time->tm_year;  // tm_year 是从1900开始的年数
    int month = 1 + local_time->tm_mon;     // tm_mon 从0开始，0=1月
    
    // 组合成 YYMMDD 格式的整数
    return (year % 100) * 1000000 + month * 10000 + local_time->tm_mday*100 + local_time->tm_hour;
}

void Helper_Chat(Player *player, const char *fmt, ...) {
    std::string result;
    
    // 处理可变参数
    va_list args;
    va_start(args, fmt);
    
    // 计算格式化后的字符串长度
    int length = vsnprintf(nullptr, 0, fmt, args);
    va_end(args);
    
    if (length > 0) {
        // 分配缓冲区
        char* buffer = new char[length + 1];
        
        // 重新获取参数并格式化
        va_start(args, fmt);
        vsnprintf(buffer, length + 1, fmt, args);
        va_end(args);
        
        // 添加格式化后的字符串
        result.append(buffer);
        
        // 释放缓冲区
        delete[] buffer;
    }
    
    // 发送聊天消息
	ChatHandler(player).PSendSysMessage(result.c_str());	
}

