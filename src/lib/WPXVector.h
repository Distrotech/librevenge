/* libwpd
 * Copyright (C) 2005 William Lachance (william.lachance@sympatico.ca)
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

#ifndef WPXVECTOR_H
#define WPXVECTOR_H
#include <sys/types.h>

// NOTE: this class is meant to be used in libwpd's headers (to work around symbol problems 
// when different versions of the STL are in use), and should not be used inside your application
// (it will only slow it down with a pointless layer of abstraction)

template<class T>
class WPXVectorImpl
{
public:
	virtual void append(const T &elem) = 0;
	virtual size_t count() const = 0;
};

template<class T>
class WPXVectorIterImpl
{
public:
	virtual void rewind() = 0;
	virtual bool next() = 0;
	virtual bool last() = 0;
	virtual const T & operator()() const = 0;
};

template<class T>
class WPXVector
{
public:
	WPXVector(const WPXVector &);
        WPXVector();
	virtual void append(const T &elem);
	virtual size_t count() const;

        class Iter
        {
        public:
                Iter(const WPXVector<T> &vector);
                virtual ~Iter() { delete m_iterImpl; }
                void rewind();
		bool next();
		bool last();
                const T & operator()() const;

        private:
                WPXVectorIterImpl<T> *m_iterImpl;
        };
        friend class WPXVector<T>::Iter;

private:
	WPXVectorImpl<T> *m_impl;
};

#endif /* WPXVECTOR_H */
