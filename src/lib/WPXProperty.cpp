#include "WPXProperty.h"


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

private:
	float m_val;
};

class WPXInchProperty : public WPXFloatProperty
{
public:
	WPXInchProperty(const float val);
	virtual UTF8String getStr() const; 
	virtual WPXProperty * clone() const;
};

class WPXPercentProperty : public WPXFloatProperty
{
public:
	WPXPercentProperty(const float val);
	virtual UTF8String getStr() const; 
	virtual WPXProperty * clone() const;
};

class WPXPointProperty : public WPXFloatProperty
{
public:
	WPXPointProperty(const float val);
	virtual UTF8String getStr() const; 
	virtual WPXProperty * clone() const;
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
	static WPXProperty * newInchProp(const float val) { return static_cast<WPXProperty *>(new WPXInchProperty(val)); }
	static WPXProperty * newPercentProp(const float val) { return static_cast<WPXProperty *>(new WPXPercentProperty(val)); }
	static WPXProperty * newPointProp(const float val) { return static_cast<WPXProperty *>(new WPXPointProperty(val)); }
};

WPXProperty::~WPXProperty()
{
}

WPXStringProperty::WPXStringProperty(const UTF8String &str) :
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

UTF8String WPXStringProperty::getStr() const 
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

UTF8String WPXIntProperty::getStr() const 
{ 
	UTF8String str; str.sprintf("%d", m_val); 
	return str; 
}

WPXProperty * WPXIntProperty::clone() const
{
	return new WPXIntProperty(m_val);
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

UTF8String WPXInchProperty::getStr() const 
{ 
	UTF8String str; 
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

UTF8String WPXPercentProperty::getStr() const
{
	UTF8String str; 
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

UTF8String WPXPointProperty::getStr() const
{
	UTF8String str; 
	str.sprintf("%ipt", getInt()); 
	return str; 
}

WPXProperty * WPXPointProperty::clone() const
{
	return new WPXPointProperty(getFloat());
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

void WPXPropertyList::insert(string name, const char *val)
{ 
	m_map[name] = WPXPropertyFactory::newStringProp(val);
}

void WPXPropertyList::insert(string name, const UTF8String &val)
{ 
	m_map[name] = WPXPropertyFactory::newStringProp(val);
}

void WPXPropertyList::insert(string name, const float val, const WPXUnit units)
{ 
	if (units == INCH)
		m_map[name] = WPXPropertyFactory::newInchProp(val);
	else if (units == PERCENT)
		m_map[name] = WPXPropertyFactory::newPercentProp(val);
	else
		m_map[name] = WPXPropertyFactory::newPointProp(val);
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
