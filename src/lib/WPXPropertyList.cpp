/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2004 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2005 Net Integration Technologies (http://www.net-itech.com)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include <libwpd/libwpd.h>
#include <map>
#include <string>
#include <utility>

class WPXMapImpl
{
public:
	WPXMapImpl() : m_map() {}
	~WPXMapImpl();
	void insert(const char *name, WPXProperty *property);
	const WPXProperty *operator[](const char *name) const;
	void remove(const char *name);
	void clear();

private:
	// disable copy construction and assignment
	WPXMapImpl(const WPXMapImpl &other);
	WPXMapImpl &operator=(const WPXMapImpl &other);

private:
	mutable std::map<std::string, WPXProperty *> m_map;

	friend class WPXMapIterImpl;
};

WPXMapImpl::~WPXMapImpl()
{
	for (std::map<std::string, WPXProperty *>::iterator iter = m_map.begin();
	        iter != m_map.end();
	        ++iter)
	{
		delete iter->second;
	}
}

const WPXProperty *WPXMapImpl::operator[](const char *name) const
{
	std::map<std::string, WPXProperty *>::iterator i = m_map.find(name);
	if (i != m_map.end())
	{
		return i->second;
	}

	return 0;
}

void WPXMapImpl::insert(const char *name, WPXProperty *prop)
{
	std::map<std::string, WPXProperty *>::iterator i = m_map.lower_bound(name);
	if (i != m_map.end() && !(m_map.key_comp()(name, i->first)))
	{
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
	if (i != m_map.end())
	{
		if (i->second) delete (i->second);
		m_map.erase(i);
	}
}

void WPXMapImpl::clear()
{
	for (std::map<std::string, WPXProperty *>::iterator iter = m_map.begin();
	        iter != m_map.end();
	        ++iter)
	{
		delete iter->second;
	}

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

void WPXPropertyList::insert(const char *name, WPXProperty *prop)
{
	m_mapImpl->insert(name, prop);
}

void WPXPropertyList::insert(const char *name, const int val)
{
	m_mapImpl->insert(name, WPXPropertyFactory::newIntProp(val));
}

void WPXPropertyList::insert(const char *name, const bool val)
{
	m_mapImpl->insert(name, WPXPropertyFactory::newBoolProp(val));
}

void WPXPropertyList::insert(const char *name, const char *val)
{
	m_mapImpl->insert(name, WPXPropertyFactory::newStringProp(val));
}

void WPXPropertyList::insert(const char *name, const WPXString &val)
{
	m_mapImpl->insert(name, WPXPropertyFactory::newStringProp(val));
}

void WPXPropertyList::insert(const char *name, const double val, const WPXUnit units)
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

void WPXPropertyList::remove(const char *name)
{
	m_mapImpl->remove(name);
}

const WPXPropertyList &WPXPropertyList::operator=(const WPXPropertyList &propList)
{
	WPXPropertyList tmp(propList);
	std::swap(m_mapImpl, tmp.m_mapImpl);
	return *this;
}

const WPXProperty *WPXPropertyList::operator[](const char *name) const
{
	return (*m_mapImpl)[name];
}

void WPXPropertyList::clear()
{
	m_mapImpl->clear();
}

#if 0
void WPXPropertyList::swap(WPXPropertyList &other)
{
	std::swap(m_mapImpl, other.m_mapImpl);
}
#endif

class WPXMapIterImpl
{
public:
	WPXMapIterImpl(const WPXMapImpl *impl);
	void rewind();
	bool next();
	bool last();
	const WPXProperty *operator()() const;
	const char *key();

private:
	// not implemented
	WPXMapIterImpl(const WPXMapIterImpl &other);
	WPXMapIterImpl &operator=(const WPXMapIterImpl &other);

private:
	bool m_imaginaryFirst;
	std::map<std::string, WPXProperty *>::iterator m_iter;
	std::map<std::string, WPXProperty *> *m_map;
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
		++m_iter;
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

const WPXProperty *WPXMapIterImpl::operator()() const
{
	return m_iter->second;
}

const char *WPXMapIterImpl::key()
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

const WPXProperty *WPXPropertyList::Iter::operator()() const
{
	return (*m_iterImpl)();
}

const char *WPXPropertyList::Iter::key()
{
	return m_iterImpl->key();
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
