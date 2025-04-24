#ifndef _AUTOBROADCAST_MGR_H
#define _AUTOBROADCAST_MGR_H

#include "Policies/Singleton.h"
#include "Platform/Define.h"

#include <vector>

struct AutoBroadCastEntry
{
    int32 stringId;
};

class AutoBroadCastMgr
{
    public :
        AutoBroadCastMgr();
        ~AutoBroadCastMgr();

        void Load();
        void Update(uint32 diff);

    protected:
        std::vector<AutoBroadCastEntry> entries;
        time_t _constInterval;
        time_t _current;
        time_t _currentMinute;
        time_t _lastBroadCast1Min; // last broadcast time, used to prevent the same message from being broadcasted too frequently, 10 minutes interval is recommended.
        time_t _lastBroadCast10Min;
        time_t _lastBroadCast60Min;

        
        void UpdatePetStatus();
};

#define sAutoBroadCastMgr MaNGOS::Singleton<AutoBroadCastMgr>::Instance()
#endif
