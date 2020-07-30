#pragma once

namespace hTool
{
	template<typename Key, typename Val, unsigned int GenRange>
	UniqueIdGen<Key, Val, GenRange>::UniqueIdGen(std::map<Key, Val>& m) : mapRef(m)
	{
		minN = 0;
		maxN = (Key)-1;
		curN = minN;
	}

	template<typename Key, typename Val, unsigned int GenRange>
	UniqueIdGen<Key, Val, GenRange>::UniqueIdGen(std::map<Key, Val>& m,
		const Key& min, const Key& max) : mapRef(m)
	{
		minN = min < max ? min : max;
		maxN = min < max ? max : min;
		curN = minN;
	}

	template<typename Key, typename Val, unsigned int GenRange>
	void UniqueIdGen<Key, Val, GenRange>::reset()
	{
		keySet.clear();
		mapRef.clear();
		curN = minN;
	}

	template<typename Key, typename Val, unsigned int GenRange>
	bool UniqueIdGen<Key, Val, GenRange>::resize(const Key& min, const Key& max)
	{
		if (min == max)
			return false;

		minN = min < max ? min : max;
		maxN = min < max ? max : min;

		while (keySet.begin() != keySet.end() && *keySet.begin() < minN)
			keySet.erase(keySet.begin());
		while (keySet.rbegin() != keySet.rend() && *keySet.rbegin() >= maxN)
			keySet.erase(*keySet.rbegin());

		while (mapRef.begin() != mapRef.end() && mapRef.begin()->first < minN)
			mapRef.erase(mapRef.begin());
		while (mapRef.rbegin() != mapRef.rend() && mapRef.rbegin()->first >= maxN)
			mapRef.erase(mapRef.rbegin()->first);

		refreshCurNum();

		return true;
	}

	template<typename Key, typename Val, unsigned int GenRange>
	bool UniqueIdGen<Key, Val, GenRange>::invaild(const Key& k)
	{
		return k < minN || k >= maxN;
	}

	template<typename Key, typename Val, unsigned int GenRange>
	bool UniqueIdGen<Key, Val, GenRange>::checkKey(const Key& k)
	{
		if (invaild(k))
			return false;

		return mapRef.find(k) != mapRef.end();
	}

	template<typename Key, typename Val, unsigned int GenRange>
	Key UniqueIdGen<Key, Val, GenRange>::getKey()
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

	template<typename Key, typename Val, unsigned int GenRange>
	bool UniqueIdGen<Key, Val, GenRange>::putKey(const Key& k)
	{
		if (!checkKey(k))
			return false;

		if (keySet.size() < 3 * GenRange)
			keySet.insert(k);
		mapRef.erase(k);
	}

	template<typename Key, typename Val, unsigned int GenRange>
	bool UniqueIdGen<Key, Val, GenRange>::insert(const Val& v)
	{
		if (checkKey(v.getIndex()))
			return false;

		if (!invaild(v.getIndex()))
		{
			keySet.erase(v.getIndex());
			mapRef.insert(std::make_pair(v.getIndex(), v));
			refreshCurNum();
			return true;
		}

		Key keyTmp = getKey();
		if (invaild(keyTmp))
			return false;

		auto ret = mapRef.insert(std::make_pair(keyTmp, v));
		if (ret.second)
			ret.first->second.setIndex(keyTmp);

		return true;
	}

	template<typename Key, typename Val, unsigned int GenRange>
	bool UniqueIdGen<Key, Val, GenRange>::erase(const Val& v)
	{
		if (!checkKey(v.getIndex()))
			return false;

		putKey(v.getIndex());
		return true;
	}

	template<typename Key, typename Val, unsigned int GenRange>
	void UniqueIdGen<Key, Val, GenRange>::genCheck()
	{
		if (keySet.size() >= GenRange)
			return;//…˙≥… ˝◊„πª

		if (mapRef.empty())
			return;//»›∆˜Œ™ø’

		if (mapRef.size() + minN >= curN)
			return;//ΩÙ√‹≈≈¡–

		unsigned int idx = 0;
		unsigned int rangeGenCnt = GenRange - keySet.size();
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

	template<typename Key, typename Val, unsigned int GenRange>
	void UniqueIdGen<Key, Val, GenRange>::refreshCurNum()
	{
		auto it = mapRef.rbegin();
		if (it == mapRef.rend())
		{
			curN = minN;
			return;
		}

		if (curN < it->first && keySet.size() < 3 * GenRange)
		{
			Key canInsertCnt = it->first - curN;
			Key rangeInserCnt = 3 * GenRange - keySet.size();
			Key insertCnt = canInsertCnt < rangeInserCnt ? canInsertCnt : rangeInserCnt;

			for (Key n = 0; n < insertCnt; ++n)
				keySet.insert(curN + n);
		}

		curN = it->first + 1;
	}

	template<typename Key, typename Val, unsigned int GenRange>
	std::ostream& operator<<(std::ostream& os, const UniqueIdGen<Key, Val, GenRange>& gen)
	{
		Container c(os);
		os << gen.minN << ", " << gen.maxN << ", " << gen.curN << ", ";
		c(gen.keySet) << ", ";
		c(gen.mapRef);
		return os;
	}

}