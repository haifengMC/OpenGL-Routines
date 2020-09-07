#pragma once

namespace hTool
{
	template<typename Key, typename Val>
	class hUniqueIdGen;

	//�뱣֤Val��hUniqueMapVal������
	template<typename Key, typename Val>
	struct hUniqueMapVal
	{
		template<typename _Key, typename _Val>
		friend class hUniqueIdGen;

		const Key& getIndex() const { return idRef; }
		void setIndex(const Key& k) { idRef = k; }

		bool isInit() const { return init; }
		bool isErr() const { return error; }

		operator Key() const { return idRef; }
		void operator()(const Key& k) { setIndex(k); }
	protected:
		hUniqueMapVal(Key& id, Val* pVal);
		hUniqueMapVal(const hUniqueMapVal&) = delete;

		Key& idRef;
		Val* pVal;
		mutable typename std::map<Key, Val>::iterator itSelf;//ָ���Լ��ĵ�����
	private:
		mutable bool init = false;
		bool error = false;

		void refreshMe(Key k, typename std::map<Key, Val>::iterator itSelf, std::map<Key, Val>& m) const;
	};

	//Ψһid������
	template<typename Key, typename Val>
	class hUniqueIdGen
	{
		template<typename _Key, typename _Val>
		friend class hUniqueMapVal;
		template<typename _Key, typename _Val>
		friend std::ostream& operator<<(std::ostream& os, const hUniqueIdGen<_Key, _Val>&);

		std::set<Key> keySet;
		std::map<Key, Val>& mapRef;
		Key minN, maxN, curN;
		size_t genRange = 10;//����id�����ķ�Χ����
		//ָ�����һ�������Ԫ�أ�δ�������ɾ����ָ��end
		typename std::map<Key, Val>::iterator itBack = mapRef.end();
		
	public:
		hUniqueIdGen(std::map<Key, Val>& m, size_t range);
		hUniqueIdGen(std::map<Key, Val>& m, size_t range, const Key& min, const Key& max);
		
		void reset();
		typename std::map<Key, Val>::iterator& back() { return itBack; }
		bool resize(const Key& min, const Key& max);
		bool invaild(const Key& k);
		const Key& getInvaild() const { return maxN; }
		
		bool checkKey(const Key& k);
		Key getKey();
		bool putKey(const Key& k);

		std::pair<typename std::map<Key, Val>::iterator, bool> 
			insert(const Val& v);
		bool erase(const Val& v);

		void genCheck();

		hUniqueIdGen& operator<<(const Val& v);
	private:
		void refreshCurNum();
	};
}