#pragma once

namespace hTool
{
	template<typename Key, typename Val>
	class hUniqueIdGen;

	//�뱣֤Val��hUniqueMapVal������
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
		mutable typename std::map<Key, Val>::iterator selfIt;//ָ���Լ��ĵ�����
	private:
		mutable bool init = false;
		bool error = false;

		void refreshMe(Key k, typename std::map<Key, Val>::iterator selfIt, std::map<Key, Val>& m) const;
	};

	//Ψһid������
	template<typename Key, typename Val>
	class hUniqueIdGen
	{
		DefLog_Init();
		template<typename _Key, typename _Val>
		friend class hUniqueMapVal;
		template<typename _Key, typename _Val>
		friend std::ostream& operator<<(std::ostream& os, const hUniqueIdGen<_Key, _Val>&);

		std::set<Key> keySet;
		std::map<Key, Val>& mapRef;
		Key minN, maxN, curN;
		size_t genRange = 10;//����id�����ķ�Χ����
		//ָ�����һ�������Ԫ�أ�δ�������ɾ����ָ��end
		typename std::map<Key, Val>::iterator backIt = mapRef.end();
		
	public:
		hUniqueIdGen(std::map<Key, Val>& m, size_t range);
		hUniqueIdGen(std::map<Key, Val>& m, size_t range, Key min, Key max);
		
		void reset();
		typename std::map<Key, Val>::iterator& back() { return backIt; }
		bool resize(Key min, Key max);
		bool invaild(Key k);
		const Key& getInvaild() const { return maxN; }
		
		bool checkKey(Key k);
		Key getKey();
		bool putKey(Key k);

		std::pair<typename std::map<Key, Val>::iterator, bool> 
			insert(Val&& v);
		std::pair<typename std::map<Key, Val>::iterator, bool> 
			insert(Val& v);
		std::pair<typename std::map<Key, Val>::iterator, bool>
			insert(const Val& v);

		bool erase(const Val& v);

		void genCheck();
		
		hUniqueIdGen& operator<<(Val&& v);
		hUniqueIdGen& operator<<(const Val& v);
	private:
		void refreshCurNum();
	};
}
#define Types_Key_Val Key, Val
template<typename Key, typename Val>
DefLog_Template(hTool::hUniqueMapVal<Types_Key_Val>, idRef, valPtr);
template<typename Key, typename Val>
DefLog_Template(hTool::hUniqueIdGen<Types_Key_Val>, minN, maxN, curN, genRange, keySet, mapRef, backIt);
#undef Types_Key_Val