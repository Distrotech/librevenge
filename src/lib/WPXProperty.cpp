#include "WPXProperty.h"

using namespace std;

class WPXStringProperty : public WPXProperty
{
public:
	WPXStringProperty(const WPXString &str);
	WPXStringProperty(const char * str);
	virtual int getInt() const;
	virtual float getFloat() const;
	virtual WPXString getStr() const;
	virtual WPXProperty * clone() const;

private:
	WPXString m_str;
};

class WPXIntProperty : public WPXProperty
{
public:
	WPXIntProperty(const int val);
	virtual int getInt() const;
	virtual float getFloat() const;
	virtual WPXString getStr() const;
	virtual WPXProperty * clone() const;

private:
	int m_val;
};

class WPXBoolProperty : public WPXIntProperty
{
public:
	WPXBoolProperty(const bool val);
	virtual WPXString getStr() const;
	virtual WPXProperty * clone() const;
};

class WPXFloatProperty : public WPXProperty
{
public:
	WPXFloatProperty(const float val);
	virtual int getInt() const;
	virtual float getFloat() const;

private:
	float m_val;
};

class WPXInchProperty : public WPXFloatProperty
{
public:
	WPXInchProperty(const float val);
	virtual WPXString getStr() const; 
	virtual WPXProperty * clone() const;
};

class WPXPercentProperty : public WPXFloatProperty
{
public:
	WPXPercentProperty(const float val);
	virtual WPXString getStr() const; 
	virtual WPXProperty * clone() const;
};

class WPXPointProperty : public WPXFloatProperty
{
public:
	WPXPointProperty(const float val);
	virtual WPXString getStr() const; 
	virtual WPXProperty * clone() const;
};

class WPXTwipProperty : public WPXFloatProperty
{
public:
	WPXTwipProperty(const float val);
	virtual WPXString getStr() const; 
	virtual WPXProperty * clone() const;
};

class WPXPropertyFactory
{
public:
	static WPXProperty * newStringProp(const WPXString &str) { return static_cast<WPXProperty *>(new WPXStringProperty(str)); }
	static WPXProperty * newStringProp(const char *str) { return static_cast<WPXProperty *>(new WPXStringProperty(str)); }
	static WPXProperty * newIntProp(const int val) { return static_cast<WPXProperty *>(new WPXIntProperty(val)); }
	static WPXProperty * newBoolProp(const bool val) { return static_cast<WPXProperty *>(new WPXBoolProperty(val)); }
	static WPXProperty * newInchProp(const float val) { return static_cast<WPXProperty *>(new WPXInchProperty(val)); }
	static WPXProperty * newPercentProp(const float val) { return static_cast<WPXProperty *>(new WPXPercentProperty(val)); }
	static WPXProperty * newPointProp(const float val) { return static_cast<WPXProperty *>(new WPXPointProperty(val)); }
	static WPXProperty * newTwipProp(const float val) { return static_cast<WPXProperty *>(new WPXTwipProperty(val)); }
};

WPXProperty::~WPXProperty()
{
}

WPXStringProperty::WPXStringProperty(const WPXString &str) :
	m_str(str) 
{
}
WPXStringProperty::WPXStringProperty(const char * str) :
	m_str(str) 
{
}
int WPXStringProperty::getInt() const 
{	
	return 0; 
}

float WPXStringProperty::getFloat() const 
{ 
	return 0.0f; 
}

WPXString WPXStringProperty::getStr() const 
{ 
	return m_str; 
}

WPXProperty * WPXStringProperty::clone() const
{
	return new WPXStringProperty(m_str);
}

WPXIntProperty::WPXIntProperty(const int val)  :
	m_val(val) 
{
}

int WPXIntProperty::getInt() const 
{ 
	return m_val; 
}

float WPXIntProperty::getFloat() const 
{ 
	return (float)m_val; 
}

WPXString WPXIntProperty::getStr() const 
{ 
	WPXString str; str.sprintf("%d", m_val); 
	return str; 
}

WPXProperty * WPXIntProperty::clone() const
{
	return new WPXIntProperty(m_val);
}

WPXBoolProperty::WPXBoolProperty(const bool val)  :
	WPXIntProperty(val) 
{
}

WPXString WPXBoolProperty::getStr() const 
{ 
	WPXString str; 

	if (getInt())
		str.sprintf("true"); 
	else
		str.sprintf("false"); 

	return str; 
}

WPXProperty * WPXBoolProperty::clone() const
{
	return new WPXBoolProperty((const bool)getInt());
}

WPXFloatProperty::WPXFloatProperty(const float val) :
	m_val(val) 
{
}

int WPXFloatProperty::getInt() const 
{ 
	return (int)m_val; 
}

float WPXFloatProperty::getFloat() const 
{ 
	return m_val; 
}

WPXInchProperty::WPXInchProperty(const float val) :
	WPXFloatProperty(val) 
{
}

WPXString WPXInchProperty::getStr() const 
{ 
	WPXString str; 
	str.sprintf("%.04finch", getFloat()); 
	return str; 
}

WPXProperty * WPXInchProperty::clone() const
{
	return new WPXInchProperty(getFloat());
}

WPXPercentProperty::WPXPercentProperty(const float val) :
	WPXFloatProperty(val) 
{
}

WPXString WPXPercentProperty::getStr() const
{
	WPXString str; 
	str.sprintf("%.04f%%", getFloat()*100.0f); 
	return str; 
}

WPXProperty * WPXPercentProperty::clone() const
{
	return new WPXPercentProperty(getFloat());
}

WPXPointProperty::WPXPointProperty(const float val) :
	WPXFloatProperty(val) 
{
}

WPXString WPXPointProperty::getStr() const
{
	WPXString str; 
	str.sprintf("%ipt", getInt()); 
	return str; 
}

WPXProperty * WPXPointProperty::clone() const
{
	return new WPXPointProperty(getFloat());
}

WPXTwipProperty::WPXTwipProperty(const float val) :
	WPXFloatProperty(val) 
{
}

WPXString WPXTwipProperty::getStr() const
{
	WPXString str; 
	str.sprintf("%i*", getInt()); 
	return str; 
}

WPXProperty * WPXTwipProperty::clone() const
{
	return new WPXTwipProperty(getFloat());
}

WPXPropertyList::WPXPropertyList()
{
}

WPXPropertyList::WPXPropertyList(const WPXPropertyList &propList)
{
        WPXPropertyList::Iter i(propList);
        for (i.rewind(); i.next(); )
	{
		insert(i.key(), i()->clone());
	}
}

WPXPropertyList::~WPXPropertyList()
{ 
	for (map<string, WPXProperty *>::iterator iter = m_map.begin();
	     iter != m_map.end();
	     iter++) { delete iter->second; } 
}

void WPXPropertyList::insert(string name, WPXProperty *prop)
{ 
	m_map[name] = prop; 
}

void WPXPropertyList::insert(string name, const int val)
{ 
	m_map[name] = WPXPropertyFactory::newIntProp(val);
}

void WPXPropertyList::insert(string name, const bool val)
{ 
	m_map[name] = WPXPropertyFactory::newBoolProp(val);
}

void WPXPropertyList::insert(string name, const char *val)
{ 
	m_map[name] = WPXPropertyFactory::newStringProp(val);
}

void WPXPropertyList::insert(string name, const WPXString &val)
{ 
	m_map[name] = WPXPropertyFactory::newStringProp(val);
}

void WPXPropertyList::insert(string name, const float val, const WPXUnit units)
{ 
	if (units == INCH)
		m_map[name] = WPXPropertyFactory::newInchProp(val);
	else if (units == PERCENT)
		m_map[name] = WPXPropertyFactory::newPercentProp(val);
	else if (units == POINT)		
		m_map[name] = WPXPropertyFactory::newPointProp(val);
	else
		m_map[name] = WPXPropertyFactory::newTwipProp(val);
}

void WPXPropertyList::remove(string name)
{
	m_map.erase(name);
}

const WPXProperty * WPXPropertyList::operator[](const string s) const
{
	map<string, WPXProperty *>::iterator i;
	i = m_map.find(s);
	if (i != m_map.end()) {
		return i->second;
	}
	
	return NULL;
}

void WPXPropertyList::clear()
{
	for (map<string, WPXProperty *>::iterator iter = m_map.begin();
	     iter != m_map.end();
	     iter++) { delete iter->second; } 

	m_map.clear();
}

WPXPropertyList::Iter::Iter(const WPXPropertyList &propList)
{
    m_map = &(propList.m_map);
    i = m_map->begin();
    m_imaginaryFirst = false;
}

void WPXPropertyList::Iter::rewind()
{ 
    // rewind to an imaginary element that preceeds the first one
    m_imaginaryFirst = true;
    i = m_map->begin(); 
}

bool WPXPropertyList::Iter::next()
{ 
    if (!m_imaginaryFirst) 
        i++; 
    if (i==m_map->end()) 
        return false; 
    m_imaginaryFirst = false;
    return true; 
}

bool WPXPropertyList::Iter::last()
{
    if (i==m_map->end()) 
        return true;
    return false;
}

const WPXProperty * WPXPropertyList::Iter::operator()() const
{
    return i->second;
}

std::string WPXPropertyList::Iter::key()
{
    return i->first;
}
