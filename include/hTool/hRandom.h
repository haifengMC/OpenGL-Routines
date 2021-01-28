#pragma once

#include "hSingleton.h"

namespace hTool
{
	enum class RandomType : size_t
	{
		UniformReal,		//平均分布(实数)
		UniformInt,			//平均分布(整数)
		UniformDeInt,		//平均分布(整数去重)
		Normal,				//正态分布
	};

	template <typename T>
	class hRWeight
	{
		DefLog_Init();
		size_t _weight = 0;
		std::vector<T> _tVec;

		size_t _total = 0;
	public:
		hRWeight() {}
		hRWeight(size_t weight);
		hRWeight(size_t weight, std::initializer_list<T> il);

		void pushBack(size_t weight, const T& t);
		bool empty() const { return _tVec.empty(); }

		size_t getWeight() const { return _weight; }
		size_t getTotal() const { return _total; }
		const std::vector<T>& getVal() const { return _tVec; }
		bool getRandVal(T* pT, size_t& idx, size_t randWeight);
		bool getRandVal(std::vector<T>& buf, size_t randWeight);

		hRWeight& operator=(std::initializer_list<T> il);
		hRWeight& operator+=(std::initializer_list<T> il);
		hRWeight& operator+=(const hRWeight<T>& w);

		template <typename C>
		friend std::ostream& operator<<(std::ostream& os, const hRWeight<C>& w);
	};

	template <typename T>
	class hRWeightMap
	{
		DefLog_Init();
		size_t _total = 0;
		std::map<size_t, hRWeight<T>> _weights;
	public:
		size_t getTotal() const { return _total; }
		bool getRandVal(std::vector<T>& buf, size_t num);

		hRWeightMap() {}
		void pushBack(size_t weight, const T& t);

		hRWeightMap(std::initializer_list<hRWeight<T>> il) { *this = il; }
		hRWeightMap& operator=(std::initializer_list<hRWeight<T>> il);
		hRWeightMap& operator+=(std::initializer_list<hRWeight<T>> il);

		template <typename C>
		friend std::ostream& operator<<(std::ostream& os, const hRWeightMap<C>& w);
	};

	class hRandom : public Singleton<hRandom>
	{
		std::random_device rd; // 将用于获得随机数引擎的种子
		std::mt19937 gen; // 以 rd() 播种的标准 mersenne_twister_engine

		std::uniform_int_distribution<> uniformInt;
		std::uniform_real_distribution<> uniformReal;
		std::normal_distribution<> normal;
	public:
		hRandom();

		template <typename T1, typename T2, typename T3>
		size_t operator()(RandomType type, T1* buf, size_t n, T2 min, T3 max);
		template <typename T1, size_t N, typename T2, typename T3>
		size_t operator()(RandomType type, T1 (&buf)[N], T2 min, T3 max);
		template <typename T>
		size_t operator()(RandomType type, T* buf, size_t bufN, hRWeightMap<T>& weightM);
		template <typename T, size_t N>
		size_t operator()(RandomType type, T(&buf)[N], hRWeightMap<T>& weightM);
		template <typename T>
		size_t operator()(RandomType type, std::vector<T>& buf, size_t bufN, hRWeightMap<T>& weightM);
	};
#define RANDOM hTool::hRandom::getMe()
}
DefLog_Template(typename T, hTool::hRWeight<T>, _weight, _total, _tVec);
DefLog_Template(typename T, hTool::hRWeightMap<T>, _total, _weights);