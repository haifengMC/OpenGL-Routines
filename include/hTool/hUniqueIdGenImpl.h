#pragma once

namespace hTool
{
	template<typename Key, typename Val>
	hUniqueMapVal<Key, Val>::hUniqueMapVal(Key& id, Val* pVal) : idRef(id), pVal(pVal)
	{
		if (!dynamic_cast<hUniqueMapVal*>(pVal))
			error = true;
	}

	template<typename Key, typename Val>
	void hUniqueMapVal<Key, Val>::refreshMe(Key k, typename std::map<Key, Val>::iterator it, std::map<Key, Val>& m) const
	{
		if (!init)
			init = true;

		if (idRef != k)
			idRef = k;

		itSelf = it;
		if (it == m.end())
			return;

		if (it->second.idRef != k)
			it->second.idRef = k;

		it->second.itSelf = it;
	}

	template<typename Key, typename Val>
	std::ostream& operator<<(std::ostream& os, const hUniqueIdGen<Key, Val>& gen)
	{
		Container c(os);
		os << "min=" << gen.minN << ", max=" << gen.maxN << ", cur=" << gen.curN << ", set=";
		c(gen.keySet) << ", map=";
		c(gen.mapRef);
		return os;
	}

	template<typename Key, typename Val>
	hUniqueIdGen<Key, Val>::hUniqueIdGen(std::map<Key, Val>& m,
		size_t range) : mapRef(m)
	{
		genRange = range;

		minN = 0;
		maxN = (Key)-1;
		curN = minN;
	}

	template<typename Key, typename Val>
	hUniqueIdGen<Key, Val>::hUniqueIdGen(std::map<Key, Val>& m,
		size_t range, const Key& min, const Key& max) : mapRef(m)
	{
		genRange = range;

		minN = min < max ? min : max;
		maxN = min < max ? max : min;
		curN = minN;
	}
	
	template<typename Key, typename Val>
	void hUniqueIdGen<Key, Val>::reset()
	{
		keySet.clear();
		mapRef.clear();
		curN = minN;
	}

	template<typename Key, typename Val>
	bool hUniqueIdGen<Key, Val>::resize(const Key& min, const Key& max)
	{
		if (min == max)
			return false;

		minN = min < max ? min : max;
		maxN = min < max ? max : min;

		while (keySet.begin() != keySet.end() && *keySet.begin() < minN)
			keySet.erase(keySet.begin());
		while (keySet.rbegin() != keySet.rend() && *keySet.rbegin() >= maxN)
			keySet.erase(*keySet.rbegin());

		itBack = mapRef.end();
		while (mapRef.begin() != mapRef.end() && mapRef.begin()->first < minN)
			mapRef.erase(mapRef.begin());
		while (mapRef.rbegin() != mapRef.rend() && mapRef.rbegin()->first >= maxN)
			mapRef.erase(mapRef.rbegin()->first);

		refreshCurNum();

		return true;
	}

	template<typename Key, typename Val>
	bool hUniqueIdGen<Key, Val>::invaild(const Key& k)
	{
		return k < minN || k >= maxN;
	}
	
	template<typename Key, typename Val>
	bool hUniqueIdGen<Key, Val>::checkKey(const Key& k)
	{
		if (invaild(k))
			return false;

		return mapRef.find(k) != mapRef.end();
	}

	template<typename Key, typename Val>
	Key hUniqueIdGen<Key, Val>::getKey()
	{
		auto it = keySet.begin();
		if (it != keySet.end())
		{
			auto keyTmp = *it;
			keySet.erase(it);
			return keyTmp;
		}

		if (curN < maxN)
			return curN++;

		if (minN + mapRef.size() < maxN)
		{
			genCheck();
			return getKey();
		}

		return maxN;
	}

	template<typename Key, typename Val>
	bool hUniqueIdGen<Key, Val>::putKey(const Key& k)
	{
		if (invaild(k))
			return false;

		auto it = mapRef.find(k);
		if (it == mapRef.end())
			return false;

		if (it == itBack)
			itBack = mapRef.end();

		if (keySet.size() < 3 * genRange)
			keySet.insert(k);

		return mapRef.erase(k);
	}

	template<typename Key, typename Val>
	std::pair<typename std::map<Key, Val>::iterator, bool> 
		hUniqueIdGen<Key, Val>::insert(const Val& v)
	{
		if (v.error)
			return std::make_pair(mapRef.end(), false);

		if (!v.init)//未初始化初始化
			v.refreshMe(v, mapRef.end(), mapRef);
		else if (v.itSelf != mapRef.end())//元素已插入
			return std::make_pair(v.itSelf, false);

		if (!invaild(v))
		{
			keySet.erase(v);
			auto ret = mapRef.insert(std::make_pair(v, v));
			if (ret.second)
			{
				itBack = ret.first;
				v.refreshMe(v, ret.first, mapRef);
				refreshCurNum();
			}

			return ret;
		}

		Key keyTmp = getKey();
		if (invaild(keyTmp))
			return std::make_pair(mapRef.end(), false);

		auto ret = mapRef.insert(std::make_pair(keyTmp, v));
		if (ret.second)
		{
			itBack = ret.first;
			v.refreshMe(keyTmp, ret.first, mapRef);
		}

		return ret;
	}

	template<typename Key, typename Val>
	bool hUniqueIdGen<Key, Val>::erase(const Val& v)
	{
		if (v.error)
			return false;

		if (!v.init)
			return putKey(v.idRef);

		if (v.itSelf == mapRef.end())
			return false;

		if (v.itSelf == itBack)
			itBack = mapRef.end();

		if (keySet.size() < 3 * genRange)
			keySet.insert(v.idRef);

		mapRef.erase(v.itSelf);
		v.itSelf = mapRef.end();

		return true;
	}

	template<typename Key, typename Val>
	void hUniqueIdGen<Key, Val>::genCheck()
	{
		if (keySet.size() >= genRange)
			return;//生成数足够

		if (mapRef.empty())
			return;//容器为空

		if (mapRef.size() + minN >= curN)
			return;//紧密排列

		unsigned int idx = 0;
		unsigned int rangeGenCnt = genRange - keySet.size();
		unsigned int canGenCnt = curN - minN - mapRef.size();
		unsigned int genCnt = rangeGenCnt < canGenCnt ? rangeGenCnt : canGenCnt;
		Key tmpKey = minN;
		for (const auto& p : mapRef)
		{
			if (idx >= genCnt)
				break;

			if (tmpKey > p.first)
				continue;

			if (tmpKey == p.first)
			{
				tmpKey = p.first + 1;
				continue;
			}

			for (Key i = tmpKey; i < p.first; ++i)
			{
				if (keySet.find(i) != keySet.end())
					continue;

				keySet.insert(i);
				if (++idx > genCnt)
					break;
			}
			tmpKey = p.first + 1;
		}
		refreshCurNum();
	}

	template<typename Key, typename Val>
	hUniqueIdGen<Key, Val>& hUniqueIdGen<Key, Val>::operator<<(const Val& v)
	{
		insert(v);
		return *this;
	}

	template<typename Key, typename Val>
	void hUniqueIdGen<Key, Val>::refreshCurNum()
	{
		auto it = mapRef.rbegin();
		if (it == mapRef.rend())
		{
			curN = minN;
			return;
		}

		if (curN < it->first && keySet.size() < 3 * genRange)
		{
			Key canInsertCnt = it->first - curN;
			Key rangeInserCnt = 3 * genRange - keySet.size();
			Key insertCnt = canInsertCnt < rangeInserCnt ? canInsertCnt : rangeInserCnt;

			for (Key n = 0; n < insertCnt; ++n)
				keySet.insert(curN + n);
		}

		curN = it->first + 1;
	}

}