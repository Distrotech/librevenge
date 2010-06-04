/* libwpd
 * Copyright (C) 2004 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2005 Net Integration Technologies (http://www.net-itech.com)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WPXPropertyList.h"
#include <map>
#include <string>

class WPXMapImpl
{
public:
	WPXMapImpl() : m_map() {}
	~WPXMapImpl();
	void insert(const char *name, WPXProperty *property);
	const WPXProperty * operator[](const char *name) const;
	void remove(const char *name);
	void clear();

private:
	mutable std::map<std::string, WPXProperty *> m_map;

	friend class WPXMapIterImpl;
};

WPXMapImpl::~WPXMapImpl()
{
	for (std::map<std::string, WPXProperty *>::iterator iter = m_map.begin();
	     iter != m_map.end();
	     iter++) { delete iter->second; } 
}

const WPXProperty * WPXMapImpl::operator[](const char *name) const
{
	std::map<std::string, WPXProperty *>::iterator i = m_map.find(name);
	if (i != m_map.end()) {
		return i->second;
	}

	return 0;
}

void WPXMapImpl::insert(const char *name, WPXProperty *prop)
{
	std::map<std::string, WPXProperty *>::iterator i = m_map.lower_bound(name);
	if (i != m_map.end() && !(m_map.key_comp()(name, i->first))) {
		WPXProperty *tmpProp = i->second;
		i->second = prop;
		delete tmpProp;
		return;
	}
	m_map.insert(i, std::map<std::string, WPXProperty *>::value_type(name, prop));
}

void WPXMapImpl::remove(const char *name)
{
	std::map<std::string, WPXProperty *>::iterator i = m_map.find(name);
	if (i != m_map.end()) {
		if (i->second) delete (i->second);
		m_map.erase(i);
	}
}

void WPXMapImpl::clear()
{
	for (std::map<std::string, WPXProperty *>::iterator iter = m_map.begin();
	     iter != m_map.end();
	     iter++) { delete iter->second; } 

	m_map.clear();
}

WPXPropertyList::WPXPropertyList() :
	m_mapImpl(new WPXMapImpl())
{
}

WPXPropertyList::WPXPropertyList(const WPXPropertyList &propList) :
	m_mapImpl(new WPXMapImpl())
{
        WPXPropertyList::Iter i(propList);
        for (i.rewind(); i.next(); )
	{
		insert(i.key(), i()->clone());
	}
}

WPXPropertyList::~WPXPropertyList()
{ 
	delete m_mapImpl;
}

void WPXPropertyList::insert(const char * name, WPXProperty *prop)
{ 
	m_mapImpl->insert(name, prop); 
}

void WPXPropertyList::insert(const char * name, const int val)
{ 
	m_mapImpl->insert(name, WPXPropertyFactory::newIntProp(val));
}

void WPXPropertyList::insert(const char * name, const bool val)
{ 
	m_mapImpl->insert(name, WPXPropertyFactory::newBoolProp(val));
}

void WPXPropertyList::insert(const char * name, const char *val)
{ 
	m_mapImpl->insert(name, WPXPropertyFactory::newStringProp(val));
}

void WPXPropertyList::insert(const char * name, const WPXString &val)
{ 
	m_mapImpl->insert(name, WPXPropertyFactory::newStringProp(val));
}

void WPXPropertyList::insert(const char * name, const double val, const WPXUnit units)
{ 
	if (units == WPX_INCH)
		m_mapImpl->insert(name, WPXPropertyFactory::newInchProp(val));
	else if (units == WPX_PERCENT)
		m_mapImpl->insert(name, WPXPropertyFactory::newPercentProp(val));
	else if (units == WPX_POINT)		
		m_mapImpl->insert(name, WPXPropertyFactory::newPointProp(val));
	else if (units == WPX_TWIP)
		m_mapImpl->insert(name, WPXPropertyFactory::newTwipProp(val));
	else if (units == WPX_GENERIC)
		m_mapImpl->insert(name, WPXPropertyFactory::newDoubleProp(val));
}

void WPXPropertyList::remove(const char * name)
{
	m_mapImpl->remove(name);
}

const WPXPropertyList& WPXPropertyList::operator=(const WPXPropertyList& propList)
{
        WPXPropertyList::Iter i(propList);
        for (i.rewind(); i.next(); )
        {
            insert(i.key(), i()->clone());
        }
        return *this;
}

const WPXProperty * WPXPropertyList::operator[](const char *name) const
{
	return (*m_mapImpl)[name];
}

void WPXPropertyList::clear()
{
	m_mapImpl->clear();
}

class WPXMapIterImpl
{
public:
	WPXMapIterImpl(const WPXMapImpl *impl);
	void rewind();
	bool next();
	bool last();
	const WPXProperty * operator()() const;
	const char * key();

private:
	bool m_imaginaryFirst;
	std::map<std::string, WPXProperty *>::iterator m_iter;
	std::map<std::string, WPXProperty *> * m_map;
};


WPXMapIterImpl::WPXMapIterImpl(const WPXMapImpl *impl) :
	m_imaginaryFirst(false),
	m_iter(impl->m_map.begin()),
	m_map(&impl->m_map)
{
}

void WPXMapIterImpl::rewind()
{
    // rewind to an imaginary element that preceeds the first one
    m_imaginaryFirst = true;
    m_iter = m_map->begin(); 
}

bool WPXMapIterImpl::next()
{
    if (!m_imaginaryFirst) 
        m_iter++; 
    if (m_iter==m_map->end()) 
        return false; 
    m_imaginaryFirst = false;

    return true; 
}

bool WPXMapIterImpl::last()
{
	if (m_iter == m_map->end()) 
		return true;
	
	return false;
}

const WPXProperty * WPXMapIterImpl::operator()() const
{
	return m_iter->second;
}

const char * WPXMapIterImpl::key()
{
	return m_iter->first.c_str();
}

WPXPropertyList::Iter::Iter(const WPXPropertyList &propList) :
	m_iterImpl(new WPXMapIterImpl(static_cast<WPXMapImpl *>(propList.m_mapImpl)))
{
}

WPXPropertyList::Iter::~Iter() 
{
	delete m_iterImpl;
}

void WPXPropertyList::Iter::rewind()
{ 
	// rewind to an imaginary element that preceeds the first one
	m_iterImpl->rewind();
}

bool WPXPropertyList::Iter::next()
{ 
	return m_iterImpl->next();
}

bool WPXPropertyList::Iter::last()
{
	return m_iterImpl->last();
}

const WPXProperty * WPXPropertyList::Iter::operator()() const
{
	return (*m_iterImpl)();
}

const char * WPXPropertyList::Iter::key()
{
	return m_iterImpl->key();
}
