#pragma once

namespace hTool
{
	template <typename T>
	hRWeight<T>::hRWeight(size_t weight)
	{
		_weight = weight;
	}

	template <typename T>
	hRWeight<T>::hRWeight(size_t weight, std::initializer_list<T> il)
	{
		_weight = weight;
		_tVec = il;
		_total = _weight * il.size();
	}

	template <typename T>
	void hRWeight<T>::pushBack(size_t weight, const T& t)
	{
		if (!_weight)
			_weight = weight;

		if (_weight != weight)
			return;

		_total += _weight;
		_tVec.push_back(t);
	}

	template<typename T>
	bool hRWeight<T>::getRandVal(T* pT, size_t& idx, size_t randWeight)
	{
		if (randWeight >= _total)
			return false;

		size_t randId = randWeight / _weight;
		pT[idx++] = _tVec[randId];
		std::swap(_tVec[randId], _tVec.back());
		_tVec.pop_back();
		_total = _total < _weight ? 0 : _total - _weight;

		return true;
	}

	template<typename T>
	bool hRWeight<T>::getRandVal(std::vector<T>& buf, size_t randWeight)
	{
		if (randWeight >= _total)
			return false;

		size_t randId = randWeight / _weight;
		buf.push_back(_tVec[randId]);
		std::swap(_tVec[randId], _tVec.back());
		_tVec.pop_back();
		_total = _total < _weight ? 0 : _total - _weight;

		return true;
	}

	template<typename T>
	hRWeight<T>& hRWeight<T>::operator=(std::initializer_list<T> il)
	{
		_tVec = il;
		_total = _weight * il.size();

		return *this;
	}

	template<typename T>
	hRWeight<T>& hRWeight<T>::operator+=(std::initializer_list<T> il)
	{
		for (auto& t : il)
			_tVec.push_back(t);

		_total += _weight * il.size();

		return *this;
	}

	template<typename T>
	hRWeight<T>& hRWeight<T>::operator+=(const hRWeight<T>& w)
	{
		if (!_weight)
			_weight = w.getWeight();

		if (_weight != w.getWeight())
			return *this;

		_total += w.getTotal();

		for (auto& t : w.getVal())
			_tVec.push_back(t);

		return *this;
	}

	template <typename T>
	std::ostream& operator<<(std::ostream& os, const hRWeight<T>& w)
	{
		os << "[" << w._weight << "]";
		bool first = true;
		for (auto& t : w._tVec)
		{
			if (first) first = false;
			else os << " ";
			os << t;
		}
		return os;
	}

	template<typename T>
	bool hRWeightMap<T>::getRandVal(std::vector<T>& buf, size_t num)
	{
		if (!_total || !num)
			return false;

		for (size_t n = 0 ; n < num; ++n)
		{
			if (_weights.empty())
				break;

			size_t tmpWeight = 0;
			size_t randWeight = 0;
			RANDOM(RandomType::UniformInt, &randWeight, 1, 1, _total);
			for (typename std::map<size_t, hRWeight<T>>::iterator itWeight = _weights.begin(); 
				itWeight != _weights.end();)
			{
				hRWeight<T>& weight = itWeight->second;
				if (randWeight >= tmpWeight + weight.getTotal())
				{
					tmpWeight += weight.getTotal();
					++itWeight;
					continue;
				}

				bool ret = weight.getRandVal(buf, randWeight - tmpWeight);
				if (!ret)
				{
					++itWeight;
					continue;
				}

				_total -= weight.getWeight();
				if (weight.empty())
					itWeight = _weights.erase(itWeight);
				break;
			}
		}

		return true;
	}

	template <typename T>
	void hRWeightMap<T>::pushBack(size_t weight, const T& t)
	{
		auto it = _weights.find(weight);
		if (it != _weights.end())
			it->second.pushBack(weight, t);
		else
			_weights[weight].pushBack(weight, t);

		_total += weight;
	}

	template <typename T>
	hRWeightMap<T>& hRWeightMap<T>::operator=(std::initializer_list<hRWeight<T>> il)
	{
		_total = 0;
		_weights.clear();

		return *this += il;
	}

	template <typename T>
	hRWeightMap<T>& hRWeightMap<T>::operator+=(std::initializer_list<hRWeight<T>> il)
	{
		for (auto& w : il)
		{
			_weights[w.getWeight()] += w;
			_total += w.getTotal();
		}

		return *this;
	}

	template <typename T1, typename T2, typename T3>
	size_t hRandom::operator()(RandomType type, T1* buf, size_t n, T2 min, T3 max)
	{
		switch (type)
		{
		case RandomType::UniformReal:
			{
				typename std::uniform_real<>::param_type param((double)min, (double)max);
				uniformReal.param(param);
				for (size_t i = 0; i < n; ++i)
					buf[i] = (T1)uniformReal(gen);
			}
			break;
		case RandomType::UniformInt:
			{
				typename std::uniform_int<>::param_type param((int)min, (int)max);
				uniformInt.param(param);
				for (size_t i = 0; i < n; ++i)
					buf[i] = (T1)uniformInt(gen);
			}
			break;
		case RandomType::Normal:
		{
			typename std::normal_distribution<>::param_type param((double)min, (double)max);
			normal.param(param);
			for (size_t i = 0; i < n; ++i)
				buf[i] = (T1)normal(gen);
		}
		break;
		default:
			break;
		}

		return n;
	}

	template <typename T1, size_t N, typename T2, typename T3>
	size_t hRandom::operator()(RandomType type, T1(&buf)[N], T2 min, T3 max)
	{
		return this->operator()(type, buf, N, min, max);
	}

	template <typename T>
	size_t hRandom::operator()(RandomType type, T* buf, size_t bufN, hRWeightMap<T>& weightM)
	{
		switch (type)
		{
		case RandomType::UniformDeInt:
		{
			if (!buf || !bufN || !weightM.getTotal())
				return 0;

			size_t ret = 0;
			for (size_t i = 0; i < bufN; ++i)
			{
				size_t total = weightM.getTotal() ? weightM.getTotal() - 1 : 0;
				typename std::uniform_int<>::param_type param(0, total);
				uniformInt.param(param);

				size_t randWeight = uniformInt(gen);
				if (!weightM.getRandVal(buf, ret, randWeight))
					return ret;
			}
			return ret;
		}
		break;
		default:
			break;
		}

		return 0;
	}

	template<typename T, size_t N>
	size_t hRandom::operator()(RandomType type, T(&buf)[N], hRWeightMap<T>& weightM)
	{
		return this->operator()(type, buf, N, weightM);
	}


	template<typename T>
	size_t hRandom::operator()(RandomType type, std::vector<T>& buf, size_t bufN, hRWeightMap<T>& weightM)
	{
		switch (type)
		{
		case RandomType::UniformDeInt:
		{
			if (!bufN || !weightM.getTotal())
				return 0;

			size_t ret = 0;
			for (size_t i = 0; i < bufN; ++i)
			{
				size_t total = weightM.getTotal() ? weightM.getTotal() - 1 : 0;
				typename std::uniform_int<>::param_type param(0, total);
				uniformInt.param(param);

				size_t randWeight = uniformInt(gen);
				if (!weightM.getRandVal(buf, randWeight))
					return ret;

				++ret;
			}
			return ret;
		}
		break;
		default:
			break;
		}

		return 0;
	}
}