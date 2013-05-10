/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2005 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2005 Net Integration Technologies, Inc. (http://www.net-itech.com)
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
#include <vector>

class WPXPropertyListVectorImpl
{
	friend class WPXPropertyListVector;
public:
	WPXPropertyListVectorImpl(const std::vector<WPXPropertyList> &_vector) : m_vector(_vector) {}
	WPXPropertyListVectorImpl() : m_vector() {}
	void append(const WPXPropertyList &elem)
	{
		m_vector.push_back(elem);
	}
	unsigned long count() const
	{
		return m_vector.size();
	}
	std::vector<WPXPropertyList> m_vector;
	const WPXPropertyList &operator[](unsigned long index) const
	{
		return m_vector[index];
	}
};

class WPXPropertyListVectorIterImpl
{
public:
	WPXPropertyListVectorIterImpl(std::vector<WPXPropertyList> *vect) :
		m_vector(vect),
		m_iter(m_vector->begin()),
		m_imaginaryFirst(false) {}
	~WPXPropertyListVectorIterImpl() {}
	void rewind()
	{
		m_iter = m_vector->begin();
		m_imaginaryFirst = true;
	}
	bool next()
	{
		if (!m_imaginaryFirst && m_iter != m_vector->end())
			++m_iter;
		m_imaginaryFirst = false;
		return (m_iter != m_vector->end());
	}
	bool last()
	{
		return (m_iter == m_vector->end());
	}
	const WPXPropertyList &operator()() const
	{
		return (*m_iter);
	}

private:
	WPXPropertyListVectorIterImpl(const WPXPropertyListVectorIterImpl &);
	WPXPropertyListVectorIterImpl &operator=(const WPXPropertyListVectorIterImpl &);
	std::vector<WPXPropertyList> *m_vector;
	std::vector<WPXPropertyList>::iterator m_iter;
	bool m_imaginaryFirst;
};

WPXPropertyListVector::WPXPropertyListVector(const WPXPropertyListVector &vect) :
	m_impl(new WPXPropertyListVectorImpl(static_cast<WPXPropertyListVectorImpl *>(vect.m_impl)->m_vector))
{
}

WPXPropertyListVector::WPXPropertyListVector() :
	m_impl(new WPXPropertyListVectorImpl)
{
}

WPXPropertyListVector::~WPXPropertyListVector()
{
	if (m_impl)
		delete m_impl;
}

void WPXPropertyListVector::append(const WPXPropertyList &elem)
{
	m_impl->append(elem);
}

void WPXPropertyListVector::append(const WPXPropertyListVector &vec)
{
	WPXPropertyListVector::Iter i(vec);
	for (i.rewind(); i.next(); )
		m_impl->append(i());
}

unsigned long WPXPropertyListVector::count() const
{
	return m_impl->count();
}

const WPXPropertyList &WPXPropertyListVector::operator[](unsigned long index) const
{
	return m_impl->operator[](index);
}

WPXPropertyListVector &WPXPropertyListVector::operator=(const WPXPropertyListVector &vect)
{
	m_impl->m_vector = vect.m_impl->m_vector;
	return *this;
}

WPXPropertyListVector::Iter::Iter(const WPXPropertyListVector &vect) :
	m_iterImpl(new WPXPropertyListVectorIterImpl(&(static_cast<WPXPropertyListVectorImpl * >(vect.m_impl)->m_vector)))
{
}

WPXPropertyListVector::Iter::~Iter()
{
	delete m_iterImpl;
}

void WPXPropertyListVector::Iter::rewind()
{
	m_iterImpl->rewind();
}

bool WPXPropertyListVector::Iter::next()
{
	return m_iterImpl->next();
}

bool WPXPropertyListVector::Iter::last()
{
	return m_iterImpl->last();
}

const WPXPropertyList &WPXPropertyListVector::Iter::operator()() const
{
	return (*m_iterImpl)();
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
