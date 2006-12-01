/* libwpd
 * Copyright (C) 2005 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2005 Net Integration Technologies, Inc. (http://www.net-itech.com)
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

#include "WPXPropertyListVector.h"
#include "WPXPropertyList.h"
#include <vector>

class WPXPropertyListVectorImpl
{
friend class WPXPropertyListVector;
public:
	WPXPropertyListVectorImpl(const std::vector<WPXPropertyList> &_vector) : m_vector(_vector) {}
	WPXPropertyListVectorImpl() {}
	void append(const WPXPropertyList &elem) { m_vector.push_back(elem); }
	size_t count() const { return m_vector.size(); }
	std::vector<WPXPropertyList> m_vector;
};

class WPXPropertyListVectorIterImpl
{
public:
        WPXPropertyListVectorIterImpl(std::vector<WPXPropertyList> * vect) { 
                m_vector = vect; 
                m_iter = m_vector->begin(); 
                m_imaginaryFirst = false; 
	}
	~WPXPropertyListVectorIterImpl() {}
	void rewind() { 
                m_iter = m_vector->begin(); 
                m_imaginaryFirst = true; 
        }
	bool next() { 
                if (!m_imaginaryFirst && m_iter != m_vector->end()) 
                        m_iter++; 
                m_imaginaryFirst = false; 
                if (m_iter != m_vector->end()) 
			return true; 
                return false; 
        }
	bool last() {
                if (m_iter == m_vector->end()) 
                        return true; 
                return false;
        }
	const WPXPropertyList & operator()() const { return (*m_iter); }

private:
        std::vector<WPXPropertyList> * m_vector;
        std::vector<WPXPropertyList>::iterator m_iter;
        bool m_imaginaryFirst;
};

WPXPropertyListVector::WPXPropertyListVector(const WPXPropertyListVector &vect)
{
        WPXPropertyListVectorImpl *vectorImpl = static_cast<WPXPropertyListVectorImpl* >(vect.m_impl);
        m_impl = new WPXPropertyListVectorImpl(vectorImpl->m_vector);
}

WPXPropertyListVector::WPXPropertyListVector()
{
        m_impl = new WPXPropertyListVectorImpl;
}

WPXPropertyListVector::~WPXPropertyListVector()
{
	delete m_impl;
}

void WPXPropertyListVector::append(const WPXPropertyList &elem)
{
        m_impl->append(elem);
}

size_t WPXPropertyListVector::count() const
{
        return m_impl->count();
}

WPXPropertyListVector::Iter::Iter(const WPXPropertyListVector &vect) 
{
        WPXPropertyListVectorImpl *vectorImpl = static_cast<WPXPropertyListVectorImpl* >(vect.m_impl);
        m_iterImpl = new WPXPropertyListVectorIterImpl(&(vectorImpl->m_vector));
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

const WPXPropertyList & WPXPropertyListVector::Iter::operator()() const
{
        return (*m_iterImpl)();
}
