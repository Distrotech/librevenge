/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2007 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

#include <boost/algorithm/string.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/remove_whitespace.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include <vector>
#include <string>
#include <stdarg.h>
#include <stdio.h>

#include <librevenge/librevenge.h>
#include "RVNGMemoryStream.h"


namespace librevenge
{

namespace
{

struct DataImpl
{
	DataImpl() : m_buf(), m_stream() {}

	std::vector<unsigned char> m_buf;
	boost::scoped_ptr<RVNGMemoryInputStream> m_stream;
};

void convertFromBase64(std::vector<unsigned char> &result, const std::string &source)
{
	std::string::const_iterator paddingIter = std::find(source.begin(), source.end(), '=');
	typedef boost::archive::iterators::transform_width<
	boost::archive::iterators::binary_from_base64<
	boost::archive::iterators::remove_whitespace< std::string::const_iterator > >, 8, 6 > base64_decoder;

	std::copy(base64_decoder(source.begin()), base64_decoder(paddingIter), std::back_inserter(result));
}

void convertToBase64(std::string &result, const std::vector<unsigned char> &source)
{
	unsigned numPadding = unsigned((3- (source.size()%3)) %3);

	typedef boost::archive::iterators::base64_from_binary<
	boost::archive::iterators::transform_width<std::vector<unsigned char>::const_iterator, 6, 8 > > base64_encoder;

	// Encode the buffer and create a string
	std::copy(
	    base64_encoder(source.begin()),
	    base64_encoder(source.end()), std::back_inserter(result));

	result.append(numPadding, '=');  // add '=' for each padded character
}

} // anonymous namespace

struct RVNGBinaryDataImpl
{
	RVNGBinaryDataImpl();

	void makeUnique();

	boost::shared_ptr<DataImpl> m_ptr;
};

RVNGBinaryDataImpl::RVNGBinaryDataImpl()
	: m_ptr(new DataImpl())
{
}

void RVNGBinaryDataImpl::makeUnique()
{
	if (!m_ptr.unique())
	{
		boost::shared_ptr<DataImpl> ptr(new DataImpl());
		ptr->m_buf = m_ptr->m_buf;
		m_ptr = ptr;
	}
}

RVNGBinaryData::~RVNGBinaryData()
{
	delete m_binaryDataImpl;
}

RVNGBinaryData::RVNGBinaryData() :
	m_binaryDataImpl(new RVNGBinaryDataImpl)
{
}

RVNGBinaryData::RVNGBinaryData(const RVNGBinaryData &data) :
	m_binaryDataImpl(new RVNGBinaryDataImpl)
{
	m_binaryDataImpl->m_ptr = data.m_binaryDataImpl->m_ptr;
}

RVNGBinaryData::RVNGBinaryData(const unsigned char *buffer, const unsigned long bufferSize) :
	m_binaryDataImpl(new RVNGBinaryDataImpl)
{
	if (buffer)
	{
		m_binaryDataImpl->m_ptr->m_buf = std::vector<unsigned char> (bufferSize);
		for (unsigned long i = 0; i < bufferSize; i++)
			m_binaryDataImpl->m_ptr->m_buf[i] = buffer[i];
	}
}

RVNGBinaryData::RVNGBinaryData(const RVNGString &base64) :
	m_binaryDataImpl(new RVNGBinaryDataImpl)
{
	std::string base64String(base64.cstr(), base64.size());
	boost::trim(base64String);
	convertFromBase64(m_binaryDataImpl->m_ptr->m_buf, base64String);
}

RVNGBinaryData::RVNGBinaryData(const char *base64) :
	m_binaryDataImpl(new RVNGBinaryDataImpl)
{
	if (base64)
	{
		std::string base64String(base64);
		boost::trim(base64String);
		convertFromBase64(m_binaryDataImpl->m_ptr->m_buf, base64String);
	}
}

void RVNGBinaryData::append(const RVNGBinaryData &data)
{
	m_binaryDataImpl->makeUnique();

	unsigned long previousSize = m_binaryDataImpl->m_ptr->m_buf.size();
	m_binaryDataImpl->m_ptr->m_buf.reserve(previousSize + data.m_binaryDataImpl->m_ptr->m_buf.size());
	for (unsigned long i = 0; i < data.m_binaryDataImpl->m_ptr->m_buf.size(); i++)
		m_binaryDataImpl->m_ptr->m_buf.push_back(data.m_binaryDataImpl->m_ptr->m_buf[i]);
}

void RVNGBinaryData::appendBase64Data(const RVNGString &base64)
{
	std::string base64String(base64.cstr(), base64.size());
	boost::trim(base64String);
	std::vector<unsigned char> buffer;
	convertFromBase64(buffer, base64String);
	if (!buffer.empty())
		append(&buffer[0], buffer.size());
}

void RVNGBinaryData::appendBase64Data(const char *base64)
{
	if (base64)
	{
		std::string base64String(base64);
		boost::trim(base64String);
		std::vector<unsigned char> buffer;
		convertFromBase64(buffer, base64String);
		if (!buffer.empty())
			append(&buffer[0], buffer.size());
	}
}

void RVNGBinaryData::append(const unsigned char *buffer, const unsigned long bufferSize)
{
	if (buffer)
	{
		m_binaryDataImpl->makeUnique();

		unsigned long previousSize = m_binaryDataImpl->m_ptr->m_buf.size();
		m_binaryDataImpl->m_ptr->m_buf.reserve(previousSize + bufferSize);
		for (unsigned long i = 0; i < bufferSize; i++)
			m_binaryDataImpl->m_ptr->m_buf.push_back(buffer[i]);
	}
}

void RVNGBinaryData::append(const unsigned char c)
{
	m_binaryDataImpl->makeUnique();

	m_binaryDataImpl->m_ptr->m_buf.push_back(c);
}

void RVNGBinaryData::clear()
{
	m_binaryDataImpl->makeUnique();

	m_binaryDataImpl->m_ptr->m_buf.clear();
}

unsigned long RVNGBinaryData::size() const
{
	return (unsigned long)m_binaryDataImpl->m_ptr->m_buf.size();
}
bool RVNGBinaryData::empty() const
{
	return (unsigned long)m_binaryDataImpl->m_ptr->m_buf.empty();
}

RVNGBinaryData &RVNGBinaryData::operator=(const RVNGBinaryData &dataBuf)
{
	m_binaryDataImpl->m_ptr = dataBuf.m_binaryDataImpl->m_ptr;
	return *this;
}

const unsigned char *RVNGBinaryData::getDataBuffer() const
{
	if (m_binaryDataImpl->m_ptr->m_buf.empty())
		return 0;
	return &(m_binaryDataImpl->m_ptr->m_buf[0]);
}

const RVNGString RVNGBinaryData::getBase64Data() const
{
	std::string base64;
	convertToBase64(base64, m_binaryDataImpl->m_ptr->m_buf);
	return RVNGString(base64.c_str());
}

RVNGInputStream *RVNGBinaryData::getDataStream() const
{
	if (m_binaryDataImpl->m_ptr->m_stream)
	{
		m_binaryDataImpl->m_ptr->m_stream.reset();
	}
	if (m_binaryDataImpl->m_ptr->m_buf.empty())
		return 0;
	m_binaryDataImpl->m_ptr->m_stream.reset(new RVNGMemoryInputStream(&(m_binaryDataImpl->m_ptr->m_buf[0]), m_binaryDataImpl->m_ptr->m_buf.size()));
	return m_binaryDataImpl->m_ptr->m_stream.get();
}

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
