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

class WPXPropertyList
{
public:
	WPXPropertyList();
	WPXPropertyList(const WPXPropertyList &);
	virtual ~WPXPropertyList();
	void insert(string name, WPXProperty *prop);
	void insert(string name, const char *val);
	void insert(string name, const int val);
	void insert(string name, const bool val);
	void insert(string name, const UTF8String &val);
	void insert(string name, float val, const WPXUnit units = INCH); 

	void remove(string name);
	const WPXProperty * operator[](const string s) const;
	void clear();

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
