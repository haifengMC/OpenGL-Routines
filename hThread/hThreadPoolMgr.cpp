#include "global.h"
#include "hThread.h"
#include "hThread/hThreadPoolMgr.h"

namespace hThread
{
	ThreadPoolMgr::ThreadPoolMgr() : _valid(true),
		cfgData("hThread.yml")
	{
		if (!cfgData.loadCfg())
		{//Err
			_valid = false;
			return;
		}
	}
}