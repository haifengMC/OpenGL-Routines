#include "global.h"
#include "hThread.h"
#include "hThread/hPoolMgr.h"

namespace hThread
{
	hPoolMgr::hPoolMgr() : _valid(true),
		cfgData("hThread.yml")
	{
		if (!cfgData.loadCfg())
		{//Err
			_valid = false;
			return;
		}
	}
}