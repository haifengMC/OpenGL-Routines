#pragma once

namespace hTool
{
	template<typename Key = unsigned int>
	struct UniqueMapVal
	{
		const Key& getIndex() const { return idRef; }
		void setIndex(const Key& k) { idRef = k; }
	protected:
		UniqueMapVal(Key& id) : idRef(id) {}
		UniqueMapVal(const UniqueMapVal&) = delete;
		Key& idRef;
	};

	//Î¨Ò»idÉú³ÉÆ÷
	template<typename Key, typename Val, unsigned int GenRange = 10>
	class UniqueIdGen
	{
		std::set<Key> keySet;
		std::map<Key, Val>& mapRef;
		Key minN, maxN, curN;
		template<typename CKey, typename CVal, unsigned int CGenRange>
		friend std::ostream& operator<<(std::ostream& os, const UniqueIdGen<CKey, CVal, CGenRange>&);
	public:
		UniqueIdGen(std::map<Key, Val>& m);
		UniqueIdGen(std::map<Key, Val>& m, const Key& min, const Key& max);

		void reset();
		bool resize(const Key& min, const Key& max);
		bool invaild(const Key& k);
		const Key& getInvaild() const { return maxN; }

		bool checkKey(const Key& k);
		Key getKey();
		bool putKey(const Key& k);

		bool insert(const Val& v);
		bool erase(const Val& v);

		void genCheck();
	private:
		void refreshCurNum();
	};
}