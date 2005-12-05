/* libwpd
 * Copyright (C) 2004 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2005 Net Integration Technologies (http://www.net-itech.com)
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

class WPXStdMapImpl : public WPXMapImpl
{
public:
	WPXStdMapImpl() {}
	virtual ~WPXStdMapImpl();
	virtual void insert(const char *name, WPXProperty *property);
	virtual const WPXProperty * operator[](const char *name) const;
	virtual void remove(const char *name);
	virtual void clear();

private:
	mutable std::map<std::string, WPXProperty *> m_map;

	friend class WPXStdMapIterImpl;
};

WPXStdMapImpl::~WPXStdMapImpl()
{
	for (std::map<std::string, WPXProperty *>::iterator iter = m_map.begin();
	     iter != m_map.end();
	     iter++) { delete iter->second; } 
}

const WPXProperty * WPXStdMapImpl::operator[](const char *name) const
{
	std::map<std::string, WPXProperty *>::iterator i;
	const std::string s(name);
	i = m_map.find(s);
	if (i != m_map.end()) {
		return i->second;
	}

	return NULL;
}

void WPXStdMapImpl::insert(const char *name, WPXProperty *prop)
{
	m_map[name] = prop;
}

void WPXStdMapImpl::remove(const char *name)
{
	// FIXME: delete element?
	m_map.erase(name);
}

void WPXStdMapImpl::clear()
{
	for (std::map<std::string, WPXProperty *>::iterator iter = m_map.begin();
	     iter != m_map.end();
	     iter++) { delete iter->second; } 

	m_map.clear();
}

WPXPropertyList::WPXPropertyList() :
	m_mapImpl(new WPXStdMapImpl())
{
}

WPXPropertyList::WPXPropertyList(const WPXPropertyList &propList) :
	m_mapImpl(new WPXStdMapImpl())
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

void WPXPropertyList::insert(const char * name, const float val, const WPXUnit units)
{ 
	if (units == INCH)
		m_mapImpl->insert(name, WPXPropertyFactory::newInchProp(val));
	else if (units == PERCENT)
		m_mapImpl->insert(name, WPXPropertyFactory::newPercentProp(val));
	else if (units == POINT)		
		m_mapImpl->insert(name, WPXPropertyFactory::newPointProp(val));
	else
		m_mapImpl->insert(name, WPXPropertyFactory::newTwipProp(val));
}

void WPXPropertyList::remove(const char * name)
{
	m_mapImpl->remove(name);
}

const WPXProperty * WPXPropertyList::operator[](const char *name) const
{
	return (*m_mapImpl)[name];
}

void WPXPropertyList::clear()
{
	m_mapImpl->clear();
}

class WPXStdMapIterImpl : public WPXMapIterImpl
{
public:
	WPXStdMapIterImpl(const WPXStdMapImpl *impl);
	virtual void rewind();
	virtual bool next();
	virtual bool last();
	virtual const WPXProperty * operator()() const;
	virtual const char * key();

private:
	bool m_imaginaryFirst;
	std::map<std::string, WPXProperty *>::iterator m_iter;
	std::map<std::string, WPXProperty *> * m_map;
};


WPXStdMapIterImpl::WPXStdMapIterImpl(const WPXStdMapImpl *impl)
{
	m_map = &(impl->m_map);
	m_iter = impl->m_map.begin();
	m_imaginaryFirst = false;
}

void WPXStdMapIterImpl::rewind()
{
    // rewind to an imaginary element that preceeds the first one
    m_imaginaryFirst = true;
    m_iter = m_map->begin(); 
}

bool WPXStdMapIterImpl::next()
{
    if (!m_imaginaryFirst) 
        m_iter++; 
    if (m_iter==m_map->end()) 
        return false; 
    m_imaginaryFirst = false;

    return true; 
}

bool WPXStdMapIterImpl::last()
{
	if (m_iter == m_map->end()) 
		return true;
	
	return false;
}

const WPXProperty * WPXStdMapIterImpl::operator()() const
{
	return m_iter->second;
}

const char * WPXStdMapIterImpl::key()
{
	return m_iter->first.c_str();
}

WPXPropertyList::Iter::Iter(const WPXPropertyList &propList) 
{
	WPXMapImpl *impl = propList.m_mapImpl;
	m_iterImpl = new WPXStdMapIterImpl(static_cast<WPXStdMapImpl *>(impl));
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
