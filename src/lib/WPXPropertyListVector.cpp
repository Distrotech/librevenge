/* libwpd
 * Copyright (C) 2005 William Lachance (william.lachance@sympatico.ca)
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WPXVector.h"
#include "WPXPropertyList.h"
#include <vector>

using namespace std;

WPXPropertyListVectorImpl::~WPXPropertyListVectorImpl()
{
}

class WPXStdPropertyListVectorImpl : public WPXPropertyListVectorImpl
{
friend class WPXPropertyListVector;
public:
	WPXStdPropertyListVectorImpl(const vector<WPXPropertyList> &_vector) : m_vector(_vector) {}
	WPXStdPropertyListVectorImpl() {}
	virtual void append(const WPXPropertyList &elem) { m_vector.push_back(elem); }
	virtual size_t count() const { return m_vector.size(); }
	vector<WPXPropertyList> m_vector;
};

class WPXStdPropertyListVectorIterImpl : public WPXPropertyListVectorIterImpl
{
public:
        WPXStdPropertyListVectorIterImpl(vector<WPXPropertyList> * vector) { 
                m_vector = vector; 
                m_iter = m_vector->begin(); 
                m_imaginaryFirst = false; 
	}
	virtual ~WPXStdPropertyListVectorIterImpl() {}
	virtual void rewind() { 
                m_iter = m_vector->begin(); 
                m_imaginaryFirst = true; 
        }
	virtual bool next() { 
                if (!m_imaginaryFirst && m_iter != m_vector->end()) 
                        m_iter++; 
                m_imaginaryFirst = false; 
                if (m_iter != m_vector->end()) 
			return true; 
                return false; 
        }
	virtual bool last() {
                if (m_iter == m_vector->end()) 
                        return true; 
                return false;
        }
	virtual const WPXPropertyList & operator()() const { return (*m_iter); }

private:
        vector<WPXPropertyList> * m_vector;
        vector<WPXPropertyList>::iterator m_iter;
        bool m_imaginaryFirst;
};

WPXPropertyListVector::WPXPropertyListVector(const WPXPropertyListVector &vect)
{
        WPXStdPropertyListVectorImpl *vectorImpl = static_cast<WPXStdPropertyListVectorImpl* >(vect.m_impl);
        m_impl = new WPXStdPropertyListVectorImpl(vectorImpl->m_vector);
}

WPXPropertyListVector::WPXPropertyListVector()
{
        m_impl = new WPXStdPropertyListVectorImpl;
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

WPXPropertyListVector::Iter::Iter(const WPXPropertyListVector &vector) 
{
        WPXStdPropertyListVectorImpl *vectorImpl = static_cast<WPXStdPropertyListVectorImpl* >(vector.m_impl);
        m_iterImpl = new WPXStdPropertyListVectorIterImpl(&(vectorImpl->m_vector));
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

WPXPropertyListVectorIterImpl::~WPXPropertyListVectorIterImpl()
{
}
