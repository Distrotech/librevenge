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
		Iter(const WPXPropertyList &propList);
		void rewind();
		bool next();
		bool last();
		const WPXProperty * operator()() const;
		std::string key();
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
