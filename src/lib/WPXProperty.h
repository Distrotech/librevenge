#ifndef WPXPROPERTY_H
#define WPXPROPERTY_H
#include <map>
#include "libwpd_support.h"

using namespace std;

class WPXProperty
{
public:
	virtual ~WPXProperty();
	virtual int getInt() const = 0;
	virtual float getFloat() const = 0;
	virtual UTF8String getStr() const = 0;
	virtual WPXProperty * clone() const = 0;
};

class WPXStringProperty : public WPXProperty
{
public:
	WPXStringProperty(const UTF8String &str);
	WPXStringProperty(const char * str);
	virtual int getInt() const;
	virtual float getFloat() const;
	virtual UTF8String getStr() const;
	virtual WPXProperty * clone() const;

private:
	UTF8String m_str;
};

class WPXIntProperty : public WPXProperty
{
public:
	WPXIntProperty(const int val);
	virtual int getInt() const;
	virtual float getFloat() const;
	virtual UTF8String getStr() const;
	virtual WPXProperty * clone() const;

private:
	int m_val;
};

class WPXFloatProperty : public WPXProperty
{
public:
	WPXFloatProperty(const float val);
	virtual int getInt() const;
	virtual float getFloat() const;
	virtual UTF8String getStr() const; 
	virtual WPXProperty * clone() const;

private:
	float m_val;
};

#if 0 
class WPXTabProperty : public WPXProperty
{
public:
	WPXTabProperty(const vector<WPXTabStop> &tabStops);
	virtual int getInt() const;
	virtual float getFloat() const;
	virtual UTF8String getStr() const;
	virtual vector<WPXTabStop> getTabStops() const;
}
#endif

class WPXPropertyFactory
{
public:
	static WPXProperty * newStringProp(const UTF8String &str) { return static_cast<WPXProperty *>(new WPXStringProperty(str)); }
	static WPXProperty * newStringProp(const char *str) { return static_cast<WPXProperty *>(new WPXStringProperty(str)); }
	static WPXProperty * newIntProp(const int val) { return static_cast<WPXProperty *>(new WPXIntProperty(val)); }
	static WPXProperty * newFloatProp(const float val) { return static_cast<WPXProperty *>(new WPXFloatProperty(val)); }
};

class WPXPropertyList
{
public:
	WPXPropertyList();
	WPXPropertyList(const WPXPropertyList &);
	virtual ~WPXPropertyList();
	void insert(string name, WPXProperty *prop) { m_map[name] = prop; }
	void remove(string name);
	const WPXProperty * operator[](const string s) const;

	class Iter
	{
	public:
		Iter(const WPXPropertyList &propList) { m_map = &(propList.m_map); i = m_map->begin(); m_imaginaryFirst = false; }
		void rewind() { 
			// rewind to an imaginary element that preceeds the first one
			m_imaginaryFirst = true;
			i = m_map->begin(); 
		}
		bool next() { 
			if (!m_imaginaryFirst) 
				i++; 
			if (i==m_map->end()) 
				return false; 
			m_imaginaryFirst = false;
			return true; 
		}
		bool last() {
			if (i==m_map->end()) 
				return true;
			return false;
		}
		const WPXProperty * operator()() const { return i->second; }
		string key() { return i->first; }
	private:
		bool m_imaginaryFirst;
		map<string, WPXProperty *>::iterator i;
		map<string, WPXProperty *> * m_map;
	};
	friend class WPXPropertyList::Iter;

private:
	mutable map<string, WPXProperty *> m_map;
};
#endif /* WPXPROPERTY_H */
