#include "WPXProperty.h"

WPXPropertyList::~WPXPropertyList()
{ 
	for (map<string, WPXProperty *>::iterator iter = m_map.begin();
	     iter != m_map.end();
	     iter++) { delete iter->second; } 
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
