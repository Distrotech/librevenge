#ifndef WPXPROPERTY_H
#define WPXPROPERTY_H
#include <map>
#include "WPXString.h"

enum WPXUnit { INCH, PERCENT, POINT, TWIP };

class WPXProperty
{
public:
	virtual ~WPXProperty();
	virtual int getInt() const = 0;
	virtual float getFloat() const = 0;
	virtual WPXString getStr() const = 0;
	virtual WPXProperty * clone() const = 0;
};

class WPXPropertyList
{
public:
	WPXPropertyList();
	WPXPropertyList(const WPXPropertyList &);
	virtual ~WPXPropertyList();
	void insert(std::string name, WPXProperty *prop);
	void insert(std::string name, const char *val);
	void insert(std::string name, const int val);
	void insert(std::string name, const bool val);
	void insert(std::string name, const WPXString &val);
	void insert(std::string name, float val, const WPXUnit units = INCH); 

	void remove(std::string name);
	const WPXProperty * operator[](const std::string s) const;
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
		std::string key() { return i->first; }
	private:
		bool m_imaginaryFirst;
		std::map<std::string, WPXProperty *>::iterator i;
		std::map<std::string, WPXProperty *> * m_map;
	};
	friend class WPXPropertyList::Iter;

private:
	mutable std::map<std::string, WPXProperty *> m_map;
};
#endif /* WPXPROPERTY_H */
