#include "WPXProperty.h"

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

UTF8String WPXFloatProperty::getStr() const 
{ 
	UTF8String str; 
	str.sprintf("%finch", m_val); 
	return str; 
}

WPXProperty * WPXFloatProperty::clone() const
{
	return new WPXFloatProperty(m_val);
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

