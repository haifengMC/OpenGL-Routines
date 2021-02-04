#pragma once

namespace hTool
{
	template<typename Key, typename Val>
	class hUniqueIdGen;

	//须保证Val是hUniqueMapVal的子类
	template<typename Key, typename Val>
	class hUniqueMapVal
	{
		DefLog_Init();
		template<typename _Key, typename _Val>
		friend class hUniqueIdGen;
	public:
		const Key& getIndex() const { return idRef; }
		void setIndex(const Key& k) { idRef = k; }

		operator Key() const { return idRef; }
		void operator()(const Key& k) { setIndex(k); }

		bool isInit() const { return init; }
		bool isErr() const { return error; }
	protected:
		hUniqueMapVal(Key& id, Val* valPtr);
		hUniqueMapVal(const hUniqueMapVal&) = delete;
		hUniqueMapVal(hUniqueMapVal&&) = delete;

		Key& idRef;
		Val* valPtr;
		mutable typename std::map<Key, hTool::hAutoPtr<Val>>::iterator _selfIt;//指向自己的迭代器
	private:
		mutable bool init = false;
		bool error = false;

		void refreshMe(
			Key k, 
			typename std::map<Key, hTool::hAutoPtr<Val>>::iterator it,
			std::map<Key, hTool::hAutoPtr<Val>>& m) const;
	};

	//唯一id生成器
	template<typename Key, typename Val>
	class hUniqueIdGen
	{
		DefLog_Init();
		template<typename _Key, typename _Val>
		friend class hUniqueMapVal;
		typedef std::map<Key, hTool::hAutoPtr<Val>> MapData;

		std::set<Key> _keySet;
		std::map<Key, hTool::hAutoPtr<Val>> _mapData;
		Key _minN, _maxN, _curN;
		size_t _genRange = 10;//生成id个数的范围基数
		
	public:
		hUniqueIdGen(size_t range);
		hUniqueIdGen(size_t range, Key min, Key max);
		
		void reset();
		bool resize(Key min, Key max);
		bool invaild(Key k);
		const Key& getInvaild() const { return _maxN; }
		
		bool checkKey(Key k);
		Key getKey();
		bool putKey(Key k);

		auto insert(hTool::hAutoPtr<Val> v) ->
			std::pair<typename MapData::iterator, bool>;
		bool erase(hTool::hAutoPtr<Val> pV);
		hTool::hAutoPtr<Val> get(Key k);

		void genCheck();
		
		hUniqueIdGen& operator<<(hTool::hAutoPtr<Val> pV);
	private:
		void refreshCurNum();
	};
}
#define Types_Key_Val Key, Val
template<typename Key, typename Val>
DefLog_Template(hTool::hUniqueMapVal<Types_Key_Val>, idRef, valPtr);
template<typename Key, typename Val>
DefLog_Template(hTool::hUniqueIdGen<Types_Key_Val>, _minN, _maxN, _curN, _genRange, _keySet, _mapData);
#undef Types_Key_Val