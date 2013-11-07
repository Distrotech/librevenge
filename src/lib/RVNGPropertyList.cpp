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
#include <boost/property_tree/ptree.hpp>
#include <string>
#include <utility>

namespace librevenge
{

typedef boost::property_tree::basic_ptree<std::string, RVNGProperty *> RVNGPtree;

class RVNGPropertyListImpl
{
public:
	RVNGPropertyListImpl() : m_ptree() {}
	~RVNGPropertyListImpl();
	void insert(const char *name, RVNGProperty *property);
	const RVNGProperty *operator[](const char *name) const;
	void remove(const char *name);
	void clear();

private:
	// disable copy construction and assignment
	RVNGPropertyListImpl(const RVNGPropertyListImpl &other);
	RVNGPropertyListImpl &operator=(const RVNGPropertyListImpl &other);

private:
	mutable RVNGPtree m_ptree;

	friend class RVNGPropertyListIterImpl;
};

RVNGPropertyListImpl::~RVNGPropertyListImpl()
{
	for (RVNGPtree::iterator iter = m_ptree.begin();
	        iter != m_ptree.end();
	        ++iter)
	{
		if (iter->second.data())
			delete iter->second.data();
	}
}

const RVNGProperty *RVNGPropertyListImpl::operator[](const char *name) const
{
	return m_ptree.get<RVNGProperty *>(name, 0);
}

void RVNGPropertyListImpl::insert(const char *name, RVNGProperty *prop)
{
	RVNGProperty *i = m_ptree.get<RVNGProperty *>(name, 0);
	m_ptree.put(name, prop);
	if (i)
		delete i;
}

void RVNGPropertyListImpl::remove(const char *name)
{
	RVNGProperty *i = m_ptree.get<RVNGProperty *>(name, 0);
	if (i)
		delete i;
	m_ptree.erase(name);
}

void RVNGPropertyListImpl::clear()
{
	for (RVNGPtree::iterator iter = m_ptree.begin();
	        iter != m_ptree.end();
	        ++iter)
	{
		if (iter->second.data())
			delete iter->second.data();
	}
	m_ptree.clear();
}

RVNGPropertyList::RVNGPropertyList() :
	m_impl(new RVNGPropertyListImpl())
{
}

RVNGPropertyList::RVNGPropertyList(const RVNGPropertyList &propList) :
	m_impl(new RVNGPropertyListImpl())
{
	RVNGPropertyList::Iter i(propList);
	for (i.rewind(); i.next(); )
	{
		insert(i.key(), i()->clone());
	}
}

RVNGPropertyList::~RVNGPropertyList()
{
	delete m_impl;
}

void RVNGPropertyList::insert(const char *name, RVNGProperty *prop)
{
	m_impl->insert(name, prop);
}

void RVNGPropertyList::insert(const char *name, const int val)
{
	m_impl->insert(name, RVNGPropertyFactory::newIntProp(val));
}

void RVNGPropertyList::insert(const char *name, const bool val)
{
	m_impl->insert(name, RVNGPropertyFactory::newBoolProp(val));
}

void RVNGPropertyList::insert(const char *name, const char *val)
{
	m_impl->insert(name, RVNGPropertyFactory::newStringProp(val));
}

void RVNGPropertyList::insert(const char *name, const RVNGString &val)
{
	m_impl->insert(name, RVNGPropertyFactory::newStringProp(val));
}

void RVNGPropertyList::insert(const char *name, const double val, const RVNGUnit units)
{
	if (units == RVNG_INCH)
		m_impl->insert(name, RVNGPropertyFactory::newInchProp(val));
	else if (units == RVNG_PERCENT)
		m_impl->insert(name, RVNGPropertyFactory::newPercentProp(val));
	else if (units == RVNG_POINT)
		m_impl->insert(name, RVNGPropertyFactory::newPointProp(val));
	else if (units == RVNG_TWIP)
		m_impl->insert(name, RVNGPropertyFactory::newTwipProp(val));
	else if (units == RVNG_GENERIC)
		m_impl->insert(name, RVNGPropertyFactory::newDoubleProp(val));
}

void RVNGPropertyList::remove(const char *name)
{
	m_impl->remove(name);
}

const RVNGPropertyList &RVNGPropertyList::operator=(const RVNGPropertyList &propList)
{
	RVNGPropertyList tmp(propList);
	std::swap(m_impl, tmp.m_impl);
	return *this;
}

const RVNGProperty *RVNGPropertyList::operator[](const char *name) const
{
	return (*m_impl)[name];
}

void RVNGPropertyList::clear()
{
	m_impl->clear();
}

#if 0
void RVNGPropertyList::swap(RVNGPropertyList &other)
{
	std::swap(m_impl, other.m_impl);
}
#endif

class RVNGPropertyListIterImpl
{
public:
	RVNGPropertyListIterImpl(const RVNGPropertyListImpl *impl);
	void rewind();
	bool next();
	bool last();
	const RVNGProperty *operator()() const;
	const char *key();

private:
	// not implemented
	RVNGPropertyListIterImpl(const RVNGPropertyListIterImpl &other);
	RVNGPropertyListIterImpl &operator=(const RVNGPropertyListIterImpl &other);

private:
	bool m_imaginaryFirst;
	RVNGPtree::const_assoc_iterator m_iter;
	RVNGPtree *m_ptree;
};


RVNGPropertyListIterImpl::RVNGPropertyListIterImpl(const RVNGPropertyListImpl *impl) :
	m_imaginaryFirst(false),
	m_iter(impl->m_ptree.ordered_begin()),
	m_ptree(&impl->m_ptree)
{
}

void RVNGPropertyListIterImpl::rewind()
{
	// rewind to an imaginary element that preceeds the first one
	m_imaginaryFirst = true;
	m_iter = m_ptree->ordered_begin();
}

bool RVNGPropertyListIterImpl::next()
{
	if (!m_imaginaryFirst)
		++m_iter;
	if (m_iter==m_ptree->not_found())
		return false;
	m_imaginaryFirst = false;

	return true;
}

bool RVNGPropertyListIterImpl::last()
{
	if (m_iter == m_ptree->not_found())
		return true;

	return false;
}

const RVNGProperty *RVNGPropertyListIterImpl::operator()() const
{
	return m_iter->second.data();
}

const char *RVNGPropertyListIterImpl::key()
{
	return m_iter->first.c_str();
}

RVNGPropertyList::Iter::Iter(const RVNGPropertyList &propList) :
	m_iterImpl(new RVNGPropertyListIterImpl(static_cast<RVNGPropertyListImpl *>(propList.m_impl)))
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

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
