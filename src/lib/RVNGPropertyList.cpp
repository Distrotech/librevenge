/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
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
 */

#include <librevenge/librevenge.h>
#include <map>
#include <string>
#include <utility>

class RVNGMapImpl
{
public:
	RVNGMapImpl() : m_map() {}
	~RVNGMapImpl();
	void insert(const char *name, RVNGProperty *property);
	const RVNGProperty *operator[](const char *name) const;
	void remove(const char *name);
	void clear();

private:
	// disable copy construction and assignment
	RVNGMapImpl(const RVNGMapImpl &other);
	RVNGMapImpl &operator=(const RVNGMapImpl &other);

private:
	mutable std::map<std::string, RVNGProperty *> m_map;

	friend class RVNGMapIterImpl;
};

RVNGMapImpl::~RVNGMapImpl()
{
	for (std::map<std::string, RVNGProperty *>::iterator iter = m_map.begin();
	        iter != m_map.end();
	        ++iter)
	{
		delete iter->second;
	}
}

const RVNGProperty *RVNGMapImpl::operator[](const char *name) const
{
	std::map<std::string, RVNGProperty *>::iterator i = m_map.find(name);
	if (i != m_map.end())
	{
		return i->second;
	}

	return 0;
}

void RVNGMapImpl::insert(const char *name, RVNGProperty *prop)
{
	std::map<std::string, RVNGProperty *>::iterator i = m_map.lower_bound(name);
	if (i != m_map.end() && !(m_map.key_comp()(name, i->first)))
	{
		RVNGProperty *tmpProp = i->second;
		i->second = prop;
		delete tmpProp;
		return;
	}
	m_map.insert(i, std::map<std::string, RVNGProperty *>::value_type(name, prop));
}

void RVNGMapImpl::remove(const char *name)
{
	std::map<std::string, RVNGProperty *>::iterator i = m_map.find(name);
	if (i != m_map.end())
	{
		if (i->second) delete (i->second);
		m_map.erase(i);
	}
}

void RVNGMapImpl::clear()
{
	for (std::map<std::string, RVNGProperty *>::iterator iter = m_map.begin();
	        iter != m_map.end();
	        ++iter)
	{
		delete iter->second;
	}

	m_map.clear();
}

RVNGPropertyList::RVNGPropertyList() :
	m_mapImpl(new RVNGMapImpl())
{
}

RVNGPropertyList::RVNGPropertyList(const RVNGPropertyList &propList) :
	m_mapImpl(new RVNGMapImpl())
{
	RVNGPropertyList::Iter i(propList);
	for (i.rewind(); i.next(); )
	{
		insert(i.key(), i()->clone());
	}
}

RVNGPropertyList::~RVNGPropertyList()
{
	delete m_mapImpl;
}

void RVNGPropertyList::insert(const char *name, RVNGProperty *prop)
{
	m_mapImpl->insert(name, prop);
}

void RVNGPropertyList::insert(const char *name, const int val)
{
	m_mapImpl->insert(name, RVNGPropertyFactory::newIntProp(val));
}

void RVNGPropertyList::insert(const char *name, const bool val)
{
	m_mapImpl->insert(name, RVNGPropertyFactory::newBoolProp(val));
}

void RVNGPropertyList::insert(const char *name, const char *val)
{
	m_mapImpl->insert(name, RVNGPropertyFactory::newStringProp(val));
}

void RVNGPropertyList::insert(const char *name, const RVNGString &val)
{
	m_mapImpl->insert(name, RVNGPropertyFactory::newStringProp(val));
}

void RVNGPropertyList::insert(const char *name, const double val, const RVNGUnit units)
{
	if (units == RVNG_INCH)
		m_mapImpl->insert(name, RVNGPropertyFactory::newInchProp(val));
	else if (units == RVNG_PERCENT)
		m_mapImpl->insert(name, RVNGPropertyFactory::newPercentProp(val));
	else if (units == RVNG_POINT)
		m_mapImpl->insert(name, RVNGPropertyFactory::newPointProp(val));
	else if (units == RVNG_TWIP)
		m_mapImpl->insert(name, RVNGPropertyFactory::newTwipProp(val));
	else if (units == RVNG_GENERIC)
		m_mapImpl->insert(name, RVNGPropertyFactory::newDoubleProp(val));
}

void RVNGPropertyList::remove(const char *name)
{
	m_mapImpl->remove(name);
}

const RVNGPropertyList &RVNGPropertyList::operator=(const RVNGPropertyList &propList)
{
	RVNGPropertyList tmp(propList);
	std::swap(m_mapImpl, tmp.m_mapImpl);
	return *this;
}

const RVNGProperty *RVNGPropertyList::operator[](const char *name) const
{
	return (*m_mapImpl)[name];
}

void RVNGPropertyList::clear()
{
	m_mapImpl->clear();
}

#if 0
void RVNGPropertyList::swap(RVNGPropertyList &other)
{
	std::swap(m_mapImpl, other.m_mapImpl);
}
#endif

class RVNGMapIterImpl
{
public:
	RVNGMapIterImpl(const RVNGMapImpl *impl);
	void rewind();
	bool next();
	bool last();
	const RVNGProperty *operator()() const;
	const char *key();

private:
	// not implemented
	RVNGMapIterImpl(const RVNGMapIterImpl &other);
	RVNGMapIterImpl &operator=(const RVNGMapIterImpl &other);

private:
	bool m_imaginaryFirst;
	std::map<std::string, RVNGProperty *>::iterator m_iter;
	std::map<std::string, RVNGProperty *> *m_map;
};


RVNGMapIterImpl::RVNGMapIterImpl(const RVNGMapImpl *impl) :
	m_imaginaryFirst(false),
	m_iter(impl->m_map.begin()),
	m_map(&impl->m_map)
{
}

void RVNGMapIterImpl::rewind()
{
	// rewind to an imaginary element that preceeds the first one
	m_imaginaryFirst = true;
	m_iter = m_map->begin();
}

bool RVNGMapIterImpl::next()
{
	if (!m_imaginaryFirst)
		++m_iter;
	if (m_iter==m_map->end())
		return false;
	m_imaginaryFirst = false;

	return true;
}

bool RVNGMapIterImpl::last()
{
	if (m_iter == m_map->end())
		return true;

	return false;
}

const RVNGProperty *RVNGMapIterImpl::operator()() const
{
	return m_iter->second;
}

const char *RVNGMapIterImpl::key()
{
	return m_iter->first.c_str();
}

RVNGPropertyList::Iter::Iter(const RVNGPropertyList &propList) :
	m_iterImpl(new RVNGMapIterImpl(static_cast<RVNGMapImpl *>(propList.m_mapImpl)))
{
}

RVNGPropertyList::Iter::~Iter()
{
	delete m_iterImpl;
}

void RVNGPropertyList::Iter::rewind()
{
	// rewind to an imaginary element that preceeds the first one
	m_iterImpl->rewind();
}

bool RVNGPropertyList::Iter::next()
{
	return m_iterImpl->next();
}

bool RVNGPropertyList::Iter::last()
{
	return m_iterImpl->last();
}

const RVNGProperty *RVNGPropertyList::Iter::operator()() const
{
	return (*m_iterImpl)();
}

const char *RVNGPropertyList::Iter::key()
{
	return m_iterImpl->key();
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
