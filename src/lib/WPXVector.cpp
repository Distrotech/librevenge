#include "WPXVector.h"
#include "WPXPropertyList.h"
#include <vector>

using namespace std;

// declare the template for the only class we will use it with: WPXPropertyList
template WPXVector<WPXPropertyList>;

template<class T>
WPXVectorImpl<T>::~WPXVectorImpl()
{
}

template<class T>
class WPXStdVectorImpl : public WPXVectorImpl<T>
{
friend class WPXVector<T>;
public:
	WPXStdVectorImpl(const vector<T> &vector) : m_vector(vector) {}
	WPXStdVectorImpl() {}
	virtual void append(const T &elem) { m_vector.push_back(elem); }
	virtual size_t count() const { return m_vector.size(); }
	vector<T> m_vector;
};

template<class T>
class WPXStdVectorIterImpl : public WPXVectorIterImpl<T>
{
public:
        WPXStdVectorIterImpl(vector<T> * vector) { 
                m_vector = vector; 
                m_iter = m_vector->begin(); 
                m_imaginaryFirst = false; 
	}
	virtual ~WPXStdVectorIterImpl() {}
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
	virtual const T & operator()() const { return (*m_iter); }
private:
        vector<T> * m_vector;
        typename vector<T>::iterator m_iter;
        bool m_imaginaryFirst;
};

template<class T>
WPXVector<T>::WPXVector(const WPXVector<T> &vector)
{
        WPXStdVectorImpl<T> *vectorImpl = static_cast<WPXStdVectorImpl<T>* >(vector.m_impl);
        m_impl = new WPXStdVectorImpl<T>(vectorImpl->m_vector);
}

template<class T>
WPXVector<T>::WPXVector()
{
        m_impl = new WPXStdVectorImpl<T>;
}

template<class T>
WPXVector<T>::~WPXVector()
{
	delete m_impl;
}

template<class T>
void WPXVector<T>::append(const T &elem)
{
        m_impl->append(elem);
}

template<class T>
size_t WPXVector<T>::count() const
{
        return m_impl->count();
}

template<class T>
WPXVector<T>::Iter::Iter(const WPXVector<T> &vector) 
{
        WPXStdVectorImpl<T> *vectorImpl = static_cast<WPXStdVectorImpl<T>* >(vector.m_impl);
        m_iterImpl = new WPXStdVectorIterImpl<T>(&(vectorImpl->m_vector));
}

template<class T>
WPXVector<T>::Iter::~Iter() 
{ 
	 delete m_iterImpl; 
}

template<class T>
void WPXVector<T>::Iter::rewind() 
{
        m_iterImpl->rewind();
}

template<class T>
bool WPXVector<T>::Iter::next() 
{
        return m_iterImpl->next();
}

template<class T>
bool WPXVector<T>::Iter::last() 
{
        return m_iterImpl->last();
}

template<class T>
const T & WPXVector<T>::Iter::operator()() const
{
        return (*m_iterImpl)();
}

template<class T>
WPXVectorIterImpl<T>::~WPXVectorIterImpl()
{
}
