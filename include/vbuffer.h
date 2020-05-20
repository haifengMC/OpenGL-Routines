#pragma once

template<typename T, typename TC = unsigned char, typename C = unsigned char>
class BufferCountData
{
	C index = 0;
	std::string bufName;
	T size = 0;
	TC count = 0;
	T total = 0;
public:
	BufferCountData(const std::string& name, const T& size, const T& count) : index(0), bufName(name), count(count), size(size), total(count* size) {}
	const std::string& getName() const { return bufName; }
	bool isVaild() const { return total ? true : false; }
	void setIndex(const T& index) { this->index = index; }
	const T& getIndex() const { return index; }
	const T& getSize() const { return size; }
	const TC& getCount() const { return count; }
	const T& getTotal() const { return total; }
	T getOffset(const TC& cntId) const 
	{
		if (isVaild() && cntId < count)
			return cntId * size;

		return 0;
	}
	T operator[](const TC& cntId) const { return getOffset(cntId); }
};

template<typename T, typename TC = unsigned char, typename C = unsigned char>
class BufferCountManager
{
	C index = 0;
	std::list<BufferCountData<T, TC, C>> bufCntData;
	std::map<C, BufferCountData<T, TC, C>*> indexMap;
	std::map<std::string, BufferCountData<T, TC, C>*> nameMap;
public:
	bool addData(const std::string& name, const T& size, const T& count)
	{
		BufferCountData bc(name, size, count);
		if (!bc.isVaild())
			return false;
		
		if (nameMap.find(bc.getName()) != nameMap.end())
			return false;

		bc.setIndex(index++);

		std::list<BufferCountData<T, TC>>::iterator it = bufCntData.insert(bufCntData.end(), std::move(bc));
		indexMap.insert(bc.getIndex(), &*it);
		nameMap.insert(bc.getName(), &*it);

		return true;
	}

	const BufferCountData<T, TC, C>* getDataByName(const std::string& name) const
	{
		std::map<std::string, BufferCountData<T, TC, C>*>::const_iterator it = nameMap.find(name);
		if (it == nameMap.end())
			return NULL;

		return it->second;
	}

	const BufferCountData<T, TC, C>* getDataById(const C& index) const
	{
		std::map<C, BufferCountData<T, TC, C>*>::const_iterator it = indexMap.find(index);
		if (it == indexMap.end())
			return NULL;

		return it->second;
	}

	const BufferCountData<T, TC, C>* operator[](const std::string& name) const
	{
		std::map<std::string, BufferCountData<T, TC, C>*>::const_iterator it = nameMap.find(name);
		if (it == nameMap.end())
			return NULL;

		return it->second;
	}

	const BufferCountData<T, TC, C>* operator[](const C& index) const
	{
		std::map<C, BufferCountData<T, TC, C>*>::const_iterator it = indexMap.find(index);
		if (it == indexMap.end())
			return NULL;

		return it->second;
	}
};