#ifndef WPXPROPERTY_H
#define WPXPROPERTY_H
#include <map>
#include "libwpd_support.h"

using namespace std;

class WPXProperty
{
public:
	virtual ~WPXProperty() {}
	virtual int getInt() const = 0;
	virtual float getFloat() const = 0;
	virtual UTF8String getStr() const = 0;
};

class WPXStringProperty : public WPXProperty
{
public:
	WPXStringProperty(const UTF8String &str) :
		m_str(str) {}
	WPXStringProperty(const UCSString &str) :
		m_str(str) {}
	WPXStringProperty(const char * str) :
		m_str(str) {}
	virtual ~WPXStringProperty() {}
	virtual int getInt() const { return 0; }
	virtual float getFloat() const { return 0.0f; }
	virtual UTF8String getStr() const { return m_str; }

private:
	UTF8String m_str;
};

class WPXIntProperty : public WPXProperty
{
public:
	WPXIntProperty(const int val) :
		m_val(val) {}
	virtual ~WPXIntProperty() {}
	virtual int getInt() const { return m_val; }
	virtual float getFloat() const { return (float)m_val; }
	virtual UTF8String getStr() const { UTF8String str; str.sprintf("%d", m_val); return str; }

private:
	int m_val;
};

class WPXFloatProperty : public WPXProperty
{
public:
	WPXFloatProperty(const float val) :
		m_val(val) {}
	virtual ~WPXFloatProperty() {}
	virtual int getInt() const { return (int)m_val; }
	virtual float getFloat() const { return m_val; }
	virtual UTF8String getStr() const { UTF8String str; str.sprintf("%f", m_val); return str; }

private:
	float m_val;
};

class WPXPropertyFactory
{
public:
	static WPXProperty * newStringProp(const UTF8String &str) { return static_cast<WPXProperty *>(new WPXStringProperty(str)); }
	static WPXProperty * newStringProp(const UCSString &str) { return static_cast<WPXProperty *>(new WPXStringProperty(str)); }
	static WPXProperty * newStringProp(const char *str) { return static_cast<WPXProperty *>(new WPXStringProperty(str)); }
	static WPXProperty * newIntProp(const int val) { return static_cast<WPXProperty *>(new WPXIntProperty(val)); }
	static WPXProperty * newFloatProp(const float val) { return static_cast<WPXProperty *>(new WPXFloatProperty(val)); }
};

class WPXPropertyList
{
public:
	virtual ~WPXPropertyList();
	void insert(string name, WPXProperty *prop) { m_map[name] = prop; }
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
