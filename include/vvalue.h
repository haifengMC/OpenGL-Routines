#pragma once

template<typename T>
class Copier//拷贝适配器
{
	T* pt;
	const T* cpt;
public:
	size_t size() const
	{
		if (cpt) return  sizeof(*cpt);
		else return 0;
	}

	unsigned int read(void* buf, const unsigned int& len)
	{
		if (!pt || !buf || len < size())
			return false;

		memcpy(pt, buf, sizeof(*pt));
		return size();
	}

	unsigned int write(void* buf, const unsigned int& len) const
	{
		if (!cpt || !buf || len < size())
			return false;

		memcpy(buf, cpt, sizeof(*cpt));
		return size();
	}

	Copier(T& t) : pt(&t), cpt(&t) {}
	Copier(const T& t) : pt(NULL), cpt(&t) {}
};

template<> class Copier<std::string>//拷贝适配器
{
	std::string* pt;
	const std::string* cpt;
public:
	size_t size() const
	{
		if (cpt) return cpt->size();
		else return 0;
	}

	unsigned int read(void* buf, const unsigned int& len) { return 0; }

	unsigned int write(void* buf, const unsigned int& len) const
	{
		if (!cpt || !buf || len < size())
			return false;

		memcpy(buf, cpt->c_str(), cpt->size());
		return size();
	}

	Copier(std::string& t) : pt(&t), cpt(&t) {}
	Copier(const std::string& t) : pt(NULL), cpt(&t) {}
};

// 变长模板的声明
template<typename... A> class vValue {};

// 边界条件
template<> class vValue<>
{
protected:
	unsigned short tNum = 0;
public:
	const unsigned short& num() const { return tNum; }
	size_t size() const { return 0; }
	unsigned int read(void* buf, const unsigned int& len) { return 0; }
	unsigned int write(void* buf, const unsigned int& len) const { return 0; }

	bool operator==(const vValue<>& bmw) const { return true; }
	bool operator<(const vValue<>& bmw) const { return false; }
	vValue<>& operator()() { return *this; }
	void* operator[](const unsigned int& index) const { return NULL; }
};

template<typename Head, typename... Tail>  // 递归的偏特化定义
class vValue<Head, Tail...> : public vValue<Tail...>
{//当实例化对象时，则会引起基类的递归构造
	Head head;

public:
	vValue() : vValue<Tail...>(), head(Head())
	{
		vValue<>::tNum = sizeof...(Tail) + 1;
	}
	vValue(const Head& h, const Tail&... t) : vValue<Tail...>(t...)
	{
		head = h;
		vValue<>::tNum = sizeof...(Tail) + 1;
	}
	vValue(Head&& h, Tail&&... t) : vValue<Tail...>(t...)
	{
		head = std::move(h);
		vValue<>::tNum = sizeof...(Tail) + 1;
	}

	size_t size() const { return Copier<Head>(head).size() + this->vValue<Tail...>::size(); }
	unsigned int read(void* buf, const unsigned int& len)
	{
		if (!buf || len < size())
			return 0;
		Copier<Head> cHead(head);

		unsigned int n = cHead.read(buf, len);
		if (n)
			return n + this->vValue<Tail...>::read((char*)buf + cHead.size(), len - cHead.size());
		else
			return n;
	}

	unsigned int write(void* buf, const unsigned int& len) const
	{
		if (!buf || len < size())
			return 0;
		Copier<Head> cHead(head);

		unsigned int n = cHead.write(buf, len);
		if (n)
			return n + this->vValue<Tail...>::write((char*)buf + cHead.size(), len - cHead.size());
		else
			return n;
	}

	//==, !=
	bool operator==(const vValue<Head, Tail...>& bmw) const
	{
		if (head == bmw.head)
			return (vValue<Tail...>) * this == (vValue<Tail...>)bmw;
		else
			return false;
	}
	bool operator!=(const vValue<Head, Tail...>& bmw) const { return !(*this == bmw); }

	//<, >, <=, >=
	bool operator<(const vValue<Head, Tail...>& bmw) const
	{
		if (head < bmw.head)
			return true;

		if (head == bmw.head)
			return (vValue<Tail...>) * this < (vValue<Tail...>)bmw;

		return false;
	}
	bool operator>(const vValue<Head, Tail...>& bmw) const { return bmw < *this; }
	bool operator<=(const vValue<Head, Tail...>& bmw) const { return !(bmw < *this); }
	bool operator>=(const vValue<Head, Tail...>& bmw) const { return !(*this < bmw); }

	vValue<Head, Tail...>& operator()(const Head& h, const Tail&... t)
	{
		head = h;
		this->vValue<Tail...>::operator()(t...);
		return *this;
	}
	//Head& operator[](const unsigned int& index)
	void* operator[](const unsigned int& index)
	{
		if (!index) return &head;
		else return this->vValue<Tail...>::operator[](index - 1);
	}

	const void* operator[](const unsigned int& index) const 
	{
		if (!index) return &head;
		else return this->vValue<Tail...>::operator[](index - 1);
	}
};


template<typename Head, typename... Tail>
std::ostream& operator<<(std::ostream& os, const vValue<Head, Tail...>& bmw)
{
	if (sizeof...(Tail) + 1 == bmw.num()) os << "{";
	else os << ",";

	return os << bmw.head << (vValue<Tail...>)bmw;
}
std::ostream& operator<<(std::ostream& os, const vValue<>& bmw) { return os << "}"; }



