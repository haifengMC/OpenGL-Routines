#pragma once

namespace hTool
{
	template<typename Key, typename Val>
	hUniqueMapVal<Key, Val>::hUniqueMapVal(Key& id, Val* valPtr) : idRef(id), valPtr(valPtr)
	{
		if (!dynamic_cast<hUniqueMapVal*>(valPtr))
			error = true;
	}

	template<typename Key, typename Val>
	void hUniqueMapVal<Key, Val>::refreshMe(
		Key k,
		typename std::map<Key, hTool::hAutoPtr<Val>>::iterator it,
		std::map<Key, hTool::hAutoPtr<Val>>& m) const
	{
		if (!init)
			init = true;

		if (idRef != k)
			idRef = k;

		_selfIt = it;
		if (it == m.end())
			return;

		if (it->second->idRef != k)
			it->second->idRef = k;

		it->second->_selfIt = it;
	}

	template<typename Key, typename Val>
	hUniqueIdGen<Key, Val>::hUniqueIdGen(size_t range)
	{
		_genRange = range;

		_minN = 0;
		_maxN = (Key)-1;
		_curN = _minN;
	}

	template<typename Key, typename Val>
	hUniqueIdGen<Key, Val>::hUniqueIdGen(size_t range, Key min, Key max)
	{
		_genRange = range;

		_minN = std::min(min, max);
		_maxN = std::max(min, max);
		_curN = _minN;
	}
	
	template<typename Key, typename Val>
	void hUniqueIdGen<Key, Val>::reset()
	{
		_keySet.clear();
		_mapData.clear();
		_curN = _minN;
	}

	template<typename Key, typename Val>
	bool hUniqueIdGen<Key, Val>::resize(Key min, Key max)
	{
		if (min == max)
			return false;

		_minN = min < max ? min : max;
		_maxN = min < max ? max : min;

		while (_keySet.begin() != _keySet.end() && *_keySet.begin() < _minN)
			_keySet.erase(_keySet.begin());
		while (_keySet.rbegin() != _keySet.rend() && *_keySet.rbegin() >= _maxN)
			_keySet.erase(*_keySet.rbegin());

		while (_mapData.begin() != _mapData.end() && _mapData.begin()->first < _minN)
			_mapData.erase(_mapData.begin());
		while (_mapData.rbegin() != _mapData.rend() && _mapData.rbegin()->first >= _maxN)
			_mapData.erase(_mapData.rbegin()->first);

		refreshCurNum();

		return true;
	}

	template<typename Key, typename Val>
	bool hUniqueIdGen<Key, Val>::invaild(Key k)
	{
		return k < _minN || k >= _maxN;
	}
	
	template<typename Key, typename Val>
	bool hUniqueIdGen<Key, Val>::checkKey(Key k)
	{
		if (invaild(k))
			return false;

		return _mapData.find(k) != _mapData.end();
	}

	template<typename Key, typename Val>
	Key hUniqueIdGen<Key, Val>::getKey()
	{
		auto it = _keySet.begin();
		if (it != _keySet.end())
		{
			auto keyTmp = *it;
			_keySet.erase(it);
			return keyTmp;
		}

		if (_curN < _maxN)
			return _curN++;

		if (_minN + _mapData.size() < _maxN)
		{
			genCheck();
			return getKey();
		}

		return _maxN;
	}

	template<typename Key, typename Val>
	bool hUniqueIdGen<Key, Val>::putKey(Key k)
	{
		if (invaild(k))
			return false;

		auto it = _mapData.find(k);
		if (it == _mapData.end())
			return false;

		if (_keySet.size() < 3 * _genRange)
			_keySet.insert(k);

		return _mapData.erase(k);
	}

	template<typename Key, typename Val>
	auto hUniqueIdGen<Key, Val>::insert(hTool::hAutoPtr<Val> pV) ->
		std::pair<typename MapData::iterator, bool>
	{
		if (!pV || pV->isErr())
			return std::make_pair(_mapData.end(), false);
		if (!pV->isInit())
			pV->refreshMe(pV->getIndex(), _mapData.end(), _mapData);
		else if (pV->_selfIt != _mapData.end())
			return std::make_pair(pV->_selfIt, false);

		if (!invaild(pV->getIndex()))
		{
			_keySet.erase(pV->getIndex());
			auto ret = _mapData.insert(std::make_pair(pV->getIndex(), pV));
			if (ret.second)
			{
				pV->refreshMe(pV->getIndex(), ret.first, _mapData);
				refreshCurNum();
			}
			return ret;
		}

		Key keyTmp = getKey();
		if (invaild(keyTmp))
			return std::make_pair(_mapData.end(), false);
		auto ret = _mapData.insert(std::make_pair(keyTmp, pV));
		if (ret.second)
			pV->refreshMe(keyTmp, ret.first, _mapData);
			
		return ret;
	}

	template<typename Key, typename Val>
	bool hUniqueIdGen<Key, Val>::erase(hTool::hAutoPtr<Val> pV)
	{
		if (!pV || pV->isErr())
			return false;

		if (!pV->isInit())
			return putKey(pV->getIndex());

		if (pV->_selfIt == _mapData.end())
			return false;

		if (_keySet.size() < 3 * _genRange)
			_keySet.insert(pV->getIndex());

		_mapData.erase(pV->_selfIt);
		pV->_selfIt = _mapData.end();

		return true;
	}

	template<typename Key, typename Val>
	hTool::hAutoPtr<Val> hUniqueIdGen<Key, Val>::get(Key k)
	{
		auto it = _mapData.find(k);
		if (it == _mapData.end())
			return hTool::hAutoPtr<Val>();

		return it->second;
	}

	template<typename Key, typename Val>
	void hUniqueIdGen<Key, Val>::genCheck()
	{
		if (_keySet.size() >= _genRange)
			return;//Éú³ÉÊý×ã¹»

		if (_mapData.empty())
			return;//ÈÝÆ÷Îª¿Õ

		if (_mapData.size() + _minN >= _curN)
			return;//½ôÃÜÅÅÁÐ

		Key idx = 0;
		Key rangeGenCnt = _genRange - _keySet.size();
		Key canGenCnt = _curN - _minN - _mapData.size();
		Key genCnt = std::min(rangeGenCnt, canGenCnt);
		Key tmpKey = _minN;
		for (const auto& p : _mapData)
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
				if (_keySet.find(i) != _keySet.end())
					continue;

				_keySet.insert(i);
				if (++idx > genCnt)
					break;
			}
			tmpKey = p.first + 1;
		}
		refreshCurNum();
	}

	template<typename Key, typename Val>
	hUniqueIdGen<Key, Val>& hUniqueIdGen<Key, Val>::operator<<(hTool::hAutoPtr<Val> v)
	{
		insert(v);
		return *this;
	}

	template<typename Key, typename Val>
	void hUniqueIdGen<Key, Val>::refreshCurNum()
	{
		auto it = _mapData.rbegin();
		if (it == _mapData.rend())
		{
			_curN = _minN;
			return;
		}

		if (_curN < it->first && _keySet.size() < 3 * _genRange)
		{
			Key canInsertCnt = it->first - _curN;
			Key rangeInserCnt = 3 * _genRange - _keySet.size();
			Key insertCnt = std::min(canInsertCnt, rangeInserCnt);

			for (Key n = 0; n < insertCnt; ++n)
				_keySet.insert(_curN + n);
		}

		_curN = it->first + 1;
	}

}