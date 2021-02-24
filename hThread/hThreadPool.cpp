#include "global.h"
#include "hThread.h"
#include "hThread/hThreadPoolMgr.h"
#include "hThread/hThreadPool.h"

namespace hThread
{
//#define RD_LOCK sThreadPool.readLock()
//#define RD_UNLOCK sThreadPool.readUnlock()
//#define WT_LOCK sThreadPool.writeLock()
//#define WT_UNLOCK sThreadPool.writeUnlock()

	void ThreadMemData::init(size_t num)
	{
		ThreadMemStatType stat = ThreadMemStatType::Init;
		for (size_t i = 0; i < num; ++i)
		{
			size_t id = _memArr.size();
			auto it = _thrdId[stat].insert(_thrdId[stat].end(), id);
			ThreadMem* pMem = createThrdMem(_type, id);
			pMem->setStat(stat, it);
			_memArr.push_back(pMem);
		}
	}

	void ThreadMemData::run()
	{
		execEvery(ThreadMemStatType::Init,
			[&](PThrdMem pMem) { pMem->run(); return true; });

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	void ThreadMemData::stop()
	{
		for (auto& mem : _memArr)
			mem->stop();
	}

	void ThreadMemData::join()
	{
		for (auto& mem : _memArr)
			mem->join();
	}

	void ThreadMemData::execEvery(ThreadMemStatType statTy,
		std::function<bool(PThrdMem)> func)
	{
		auto& thrdIds = _thrdId[statTy];
		for (auto it = thrdIds.begin(); it != thrdIds.end();)
		{
			auto pMem = _memArr[*it++];
			if (!pMem)
				continue;

			if (!func(pMem))
				break;
		}
	}

	ThreadPool::ThreadPool() :
		_valid(sThrdPoolMgr), 
		_base(sThrdPoolMgr.getBaseCfg())
	{ 
		init(); 
	}

	ThreadPool::~ThreadPool() 
	{
		final();
	}

	void ThreadPool::init()
	{
		if (!_valid)//Err
			return;

		for (auto& item : sThrdPoolMgr.getTaskMgrCfg())
			_taskMgr.emplace_back(new TaskMgr(item.second));
		for (auto& pMgr : _taskMgr)
			pMgr->init();
		COUT_LK("初始化任务管理器完毕...");
		_memData[ThreadMemType::Work]._type = ThreadMemType::Work;
		_memData[ThreadMemType::Mgr]._type = ThreadMemType::Mgr;
		createThrd(_base.data.initMgrThd, ThreadMemType::Mgr);
		createThrd(_base.data.initThd, ThreadMemType::Work);
		COUT_LK("初始化线程成员完毕...");
	}

	void ThreadPool::final()
	{
#if 0
		for (auto& pMem : memMgr)
			DEL(pMem);
#endif
		COUT_LK("线程池释放...");

	}

	void ThreadPool::run()
	{
		for (auto& data : _memData)
			data.run();
	}

	void ThreadPool::stop()
	{
		COUT_LK("线程池停止开始...");
		for (auto& data : _memData)
			data.stop();
		for (auto& data : _memData)
			data.join();
		COUT_LK("线程池停止完毕...");
	}

	size_t ThreadPool::commitTasks(PTask& task, TaskMgrPriority priority)
	{
		if (TaskMgrPriority::Max <= priority)
			return 0;

		return _taskMgr[priority]->commitTasks(task);
	}

	PTask ThreadPool::readyTasks()
	{
		size_t busyThdNum = getThrdMemNum(ThreadMemType::Work, ThreadMemStatType::Run);
		for (auto pMgr : _taskMgr)
		{
			if (!pMgr)
				continue;

			PTask pTask = pMgr->readyTasks(busyThdNum);
			if (pTask)
				return pTask;
		}

		return PTask();
	}

	bool ThreadPool::initTasks(PTask pTask, size_t thrdNum)
	{
		if (!pTask || !thrdNum)
			return false;

		size_t needNum = pTask->calcNeedThrdNum(thrdNum);
		size_t initNum = 0;
		ThreadMemData& memData = sThrdPool.getThrdMemData(ThreadMemType::Work);
		memData.execEvery(ThreadMemStatType::Wait, 
			[&](PThrdMem pMem)
			{
				PWThrdMemWork pDyMem = pMem.dynamic<ThreadMemWork>();
				if (!pDyMem)
				{
					COUT_LK("[" <<
						pMem->getStat().getName() <<
						pMem->getId() << "]不是工作线程...");
					return true;
				}

				pTask->addThrdMem(pDyMem);
				if (++initNum < needNum)
					return true;
		
				return false;
			});

		return true;
	}

	void ThreadPool::runTasks()
	{
		ThreadMemData& memData = sThrdPool.getThrdMemData(ThreadMemType::Work);
		memData.execEvery(ThreadMemStatType::Ready,
			[&](PThrdMem pMem)
			{
				PWThrdMemWork pDyMem = pMem.dynamic<ThreadMemWork>();
				if (!pDyMem)
				{
					COUT_LK("[" <<
						pMem->getStat().getName() <<
						pMem->getId() << "]不是工作线程...");
					return true;
				}

				pDyMem->runTask();
				return true;
			});
	}

	void ThreadPool::createThrd(size_t num, ThreadMemType t)
	{
		//rwLock.writeLock();
		if (ThreadMemType::Max <= t)
			return;

		ThreadMemData& data = _memData[t];
		data.init(num);

		//rwLock.writeUnlock();
	}

	size_t ThreadPool::getThrdMemNum(ThreadMemType memTy, ThreadMemStatType statTy)
	{
		ThreadMemData& data = _memData[memTy];
		return data._thrdId[statTy].size();
	}

	void ThreadPool::notifyMgrThrd()
	{
		for (auto pMem : _memData[ThreadMemType::Mgr]._memArr)
			pMem->notify();
	}
#if 0
	void ThreadPool::closeThrd(const size_t& id)
	{
		rwLock.writeLock();
		memMgr[id]->close();
		rwLock.writeUnlock();
	}

	void ThreadPool::runThrd(const size_t& num)
	{
		rwLock.readLock();
		size_t needNum = readyThd.size() < num ? readyThd.size() : num;
		rwLock.readUnlock();
		//预期运行任务数为0或没有可用线程不处理
		if (!needNum)
			return;

		//Task** pTasks = new Task*[needNum];
		//size_t begNum = 0;

		for (auto& mgr : taskMgr)
		{
			rwLock.writeLock();
			size_t canNum = mgr.readyTasks(needNum, busyThd.size());
			rwLock.writeUnlock();

			if (!canNum)
				continue;

			rwLock.readLock();
			//任务可以获取线程的倍率
			size_t canRate = 1.5 * readyThd.size() / canNum;
			rwLock.readUnlock();
			//已运行的任务数
			size_t runNum = mgr.runTasks(canNum, canRate);

			if (needNum <= runNum)//需求任务都运行则不再运行新的
				break;

			needNum -= runNum;
		}
	}

	void ThreadPool::removeThrd(const size_t& id)
	{
		rwLock.writeLock();

		DEL(memMgr[id]);
		memMgr[id] = *memMgr.rbegin();
		memMgr[id]->setId(id);
		memMgr.pop_back();
		readyThd.erase(id);

		rwLock.writeUnlock();
	}
#endif

//#undef RD_LOCK
//#undef RD_UNLOCK
//#undef WT_LOCK
//#undef WT_UNLOCK
}