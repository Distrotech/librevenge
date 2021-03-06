/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

namespace librevenge
{

class RVNGBinaryData;
class RVNGInputStream;
class RVNGProperty;
class RVNGPropertyFactory;
class RVNGPropertyList;
class RVNGPropertyListVector;
class RVNGString;
class RVNGStringVector;

enum RVNGUnit
{
	RVNG_UNIT_DUMMY
};

}

// librevenge
namespace librevenge
{

class RVNGString
{
public:
	RVNGString()
	{
	}

	RVNGString(const RVNGString &other)
	{
		__coverity_tainted_data_transitive__(m_p, other.m_p);
	}

	RVNGString(const char *str)
	{
		if (str)
			__coverity_tainted_data_transitive__(m_p, const_cast<char *>(str));
	}

	static RVNGString escapeXML(const RVNGString &s)
	{
		return escapeXML(s.m_p);
	}

	static RVNGString escapeXML(const char *s)
	{
		return RVNGString(s);
	}

	const char *cstr() const
	{
		return m_p;
	}

	int len() const
	{
		int r;
		__coverity_tainted_data_transitive__(&r, m_p);
		return r;
	}

	bool empty() const
	{
		bool e;
		return e;
	}

	void sprintf(const char *format, ...)
	{
		__coverity_format_string_sink__(const_cast<char *>(format));
	}

	void append(const RVNGString &s)
	{
		__coverity_tainted_data_transitive__(m_p, s.m_p);
	}

	void append(const char *s)
	{
		__coverity_tainted_data_transitive__(m_p, const_cast<char *>(s));
	}

	void append(const char c)
	{
		char cc = c;
		__coverity_tainted_data_transitive__(m_p, &cc);
	}

	void appendEscapedXML(const RVNGString &s)
	{
		__coverity_tainted_data_transitive__(m_p, s.m_p);
	}

	void appendEscapedXML(const char *s)
	{
		__coverity_tainted_data_transitive__(m_p, const_cast<char *>(s));
	}

	void clear()
	{
		__coverity_tainted_data_sanitize__(m_p);
	}

	RVNGString &operator=(const RVNGString &str)
	{
		__coverity_tainted_data_transitive__(m_p, str.m_p);
	}

	RVNGString &operator=(const char *s)
	{
		__coverity_tainted_data_transitive__(m_p, const_cast<char *>(s));
	}

private:
	char *m_p;
};

class RVNGBinaryData
{
public:
	RVNGBinaryData()
	{
		m_empty = true;
	}

	RVNGBinaryData(const RVNGBinaryData &other)
	{
		m_empty = other.m_empty;
		__coverity_tainted_data_transitive__(m_p, other.m_p);
	}

	RVNGBinaryData(const unsigned char *buffer, const unsigned long bufferSize)
	{
		m_empty = 0 < bufferSize;
		__coverity_tainted_data_transitive__(m_p, const_cast<unsigned char *>(buffer));
	}

	RVNGBinaryData(const RVNGString &base64)
	{
		m_empty = base64.empty();
		__coverity_tainted_data_transitive__(m_p, const_cast<char *>(base64.cstr()));
	}

	RVNGBinaryData(const char *base64)
	{
		m_empty = (0 == base64) || ('\0' == base64[0]);
		__coverity_tainted_data_transitive__(m_p, const_cast<char *>(base64));
	}

	unsigned long size() const
	{
		if (m_empty)
			return 0;
		unsigned long s;
		return s;
	}

	bool empty() const
	{
		return m_empty;
	}

	void append(const RVNGBinaryData &data)
	{
		m_empty |= data.m_empty;
		__coverity_tainted_data_transitive__(m_p, data.m_p);
	}

	void append(const unsigned char *buffer, const unsigned long bufferSize)
	{
		m_empty |= 0 < bufferSize;
		__coverity_tainted_data_transitive__(m_p, const_cast<unsigned char *>(buffer));
	}

	void append(const unsigned char c)
	{
		m_empty = false;
		char cc = c;
		__coverity_tainted_data_transitive__(m_p, &cc);
	}

	void appendBase64Data(const RVNGString &base64)
	{
		m_empty |= base64.empty();
		__coverity_tainted_data_transitive__(m_p, const_cast<char *>(base64.cstr()));
	}

	void appendBase64Data(const char *base64)
	{
		m_empty |= (0 == base64) || ('\0' == base64[0]);
		__coverity_tainted_data_transitive__(m_p, const_cast<char *>(base64));
	}

	void clear()
	{
		m_empty = true;
		__coverity_tainted_data_sanitize__(m_p);
	}

	const unsigned char *getDataBuffer() const
	{
		if (m_empty)
			return 0;
		return m_p;
	}

	const RVNGString getBase64Data() const
	{
		if (m_empty)
			return RVNGString();
		char *r;
		__coverity_tainted_data_transitive__(r, m_p);
		return RVNGString(r);
	}

	const RVNGInputStream *getDataStream() const
	{
		if (m_empty)
			return 0;
		RVNGInputStream *r = reinterpret_cast<RVNGInputStream *>(__coverity_new__(sizeof(RVNGInputStream)));
		__coverity_escape__(r);
		return r;
	}

	RVNGBinaryData &operator=(const RVNGBinaryData &other)
	{
		m_empty = other.m_empty;
		__coverity_tainted_data_transitive__(m_p, other.m_p);
	}

private:
	unsigned char *m_p;
	bool m_empty;
};

class RVNGProperty
{
public:
	virtual ~RVNGProperty()
	{
	}

	virtual int getInt() const
	{
		int r;
		__coverity_tainted_data_transitive__(&r, m_p);
		return r;
	}

	virtual double getDouble() const
	{
		int r;
		__coverity_tainted_data_transitive__(&r, m_p);
		return r;
	}

	virtual RVNGString getStr() const
	{
		char *s;
		__coverity_tainted_data_transitive__(s, m_p);
		return RVNGString(s);
	}

	virtual RVNGProperty *clone() const
	{
		RVNGProperty *r = reinterpret_cast<RVNGProperty *>(__coverity_new__(sizeof(RVNGProperty)));
		__coverity_tainted_data_transitive__(r->m_p, m_p);
		return r;
	}

private:
	void *m_p;
};

class RVNGPropertyFactory
{
public:
	static RVNGProperty *newStringProp(const RVNGString &str)
	{
		return newStringProp(str.cstr());
	}

	static RVNGProperty *newStringProp(const char *str)
	{
		RVNGProperty *r = reinterpret_cast<RVNGProperty *>(__coverity_new__(sizeof(RVNGProperty)));
		__coverity_tainted_data_transitive__(r->m_p, const_cast<char *>(str));
		return r;
	}

	static RVNGProperty *newBinaryDataProp(const RVNGBinaryData &data)
	{
		return newBinaryDataProp(data.getDataBuffer(), data.size());
	}

	static RVNGProperty *newBinaryDataProp(const unsigned char *buffer,
	                                       const unsigned long bufferSize)
	{
		RVNGProperty *r = reinterpret_cast<RVNGProperty *>(__coverity_new__(sizeof(RVNGProperty)));
		__coverity_tainted_data_transitive__(r->m_p, const_cast<unsigned char *>(buffer));
		return r;
	}

	static RVNGProperty *newIntProp(int val)
	{
		RVNGProperty *r = reinterpret_cast<RVNGProperty *>(__coverity_new__(sizeof(RVNGProperty)));
		__coverity_tainted_data_transitive__(r->m_p, &val);
		return r;
	}

	static RVNGProperty *newBoolProp(bool val)
	{
		RVNGProperty *r = reinterpret_cast<RVNGProperty *>(__coverity_new__(sizeof(RVNGProperty)));
		__coverity_tainted_data_transitive__(r->m_p, &val);
		return r;
	}

	static RVNGProperty *newDoubleProp(double val)
	{
		RVNGProperty *r = reinterpret_cast<RVNGProperty *>(__coverity_new__(sizeof(RVNGProperty)));
		__coverity_tainted_data_transitive__(r->m_p, &val);
		return r;
	}

	static RVNGProperty *newInchProp(const double val)
	{
		return newDoubleProp(val);
	}

	static RVNGProperty *newPercentProp(const double val)
	{
		return newDoubleProp(val);
	}

	static RVNGProperty *newPointProp(const double val)
	{
		return newDoubleProp(val);
	}

	static RVNGProperty *newTwipProp(const double val)
	{
		return newDoubleProp(val);
	}

};

class RVNGPropertyList
{
public:
	void insert(const char *name, RVNGProperty *prop)
	{
		__coverity_tainted_data_sink__(const_cast<char *>(name));
		__coverity_escape__(prop);
	}

	void insert(const char *name, const char *val)
	{
		return insert(name, RVNGPropertyFactory::newStringProp(val));
	}

	void insert(const char *name, const int val)
	{
		return insert(name, RVNGPropertyFactory::newIntProp(val));
	}

	void insert(const char *name, const bool val)
	{
		return insert(name, RVNGPropertyFactory::newBoolProp(val));
	}

	void insert(const char *name, const RVNGString &val)
	{
		return insert(name, RVNGPropertyFactory::newStringProp(val));
	}

	void insert(const char *name, const double val, const RVNGUnit units)
	{
		return insert(name, RVNGPropertyFactory::newDoubleProp(val));
	}

	void insert(const char *name, const unsigned char *buffer, const unsigned long bufferSize)
	{
		return insert(name, RVNGPropertyFactory::newBinaryDataProp(buffer, bufferSize));
	}

	void insert(const char *name, const RVNGBinaryData &data)
	{
		return insert(name, RVNGPropertyFactory::newBinaryDataProp(data));
	}

	void insert(const char *name, const RVNGPropertyListVector &vec)
	{
		__coverity_tainted_data_sink__(const_cast<char *>(name));
	}
};

}

// librevenge-stream
namespace librevenge
{

class RVNGInputStream
{
	const unsigned char *read(unsigned long numBytes, unsigned long &numBytesRead)
	{
		__coverity_writeall__(&numBytesRead);
		return reinterpret_cast<unsigned char *>(__coverity_tainted_string_return_content__());
	}

	const char *subStreamName(unsigned id)
	{
		return reinterpret_cast<char *>(__coverity_tainted_string_return_content__());
	}

	RVNGInputStream *getSubStreamById(unsigned id)
	{
		RVNGInputStream *r = reinterpret_cast<RVNGInputStream *>(__coverity_new__(sizeof(RVNGInputStream)));
		return r;
	}

	RVNGInputStream *getSubStreamByName(const char *name)
	{
		RVNGInputStream *r = reinterpret_cast<RVNGInputStream *>(__coverity_new__(sizeof(RVNGInputStream)));
		return r;
	}
};

class RVNGDirectoryStream : public RVNGInputStream
{
public:
	static RVNGDirectoryStream *createForParent(const char *path)
	{
		RVNGDirectoryStream *r = reinterpret_cast<RVNGDirectoryStream *>(__coverity_new__(sizeof(RVNGDirectoryStream)));
		return r;
	}
};

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
