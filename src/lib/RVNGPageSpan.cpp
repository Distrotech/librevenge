/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006-2007 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://librevenge.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include <math.h>
#include <algorithm>
#include "RVNGPageSpan.h"
#include "librevenge_internal.h"

const double RVNG_DEFAULT_PAGE_MARGIN_TOP = 1.0;
const double RVNG_DEFAULT_PAGE_MARGIN_BOTTOM = 1.0;

const uint8_t DUMMY_INTERNAL_HEADER_FOOTER = 16;

// precondition: 0 <= headerFooterType <= 3 (i.e.: we don't handle watermarks here)
RVNGHeaderFooter::RVNGHeaderFooter(const RVNGHeaderFooterType headerFooterType, const RVNGHeaderFooterOccurence occurence,
                                 const uint8_t internalType, const RVNGSubDocument *subDocument, RVNGTableList tableList) :
	m_type(headerFooterType),
	m_occurence(occurence),
	m_internalType(internalType),
	m_subDocument(subDocument),
	m_tableList(tableList)
{
}

RVNGHeaderFooter::RVNGHeaderFooter(const RVNGHeaderFooterType headerFooterType, const RVNGHeaderFooterOccurence occurence,
                                 const uint8_t internalType, const RVNGSubDocument *subDocument) :
	m_type(headerFooterType),
	m_occurence(occurence),
	m_internalType(internalType),
	m_subDocument(subDocument),
	m_tableList()
{
}

RVNGHeaderFooter::RVNGHeaderFooter(const RVNGHeaderFooter &headerFooter) :
	m_type(headerFooter.getType()),
	m_occurence(headerFooter.getOccurence()),
	m_internalType(headerFooter.getInternalType()),
	m_subDocument(headerFooter.getSubDocument()),
	m_tableList(headerFooter.getTableList())
{
}

RVNGHeaderFooter &RVNGHeaderFooter::operator=(const RVNGHeaderFooter &headerFooter)
{
	if (this != &headerFooter)
	{
		m_type = headerFooter.getType();
		m_occurence = headerFooter.getOccurence();
		m_internalType = headerFooter.getInternalType();
		m_subDocument = headerFooter.getSubDocument();
		m_tableList = headerFooter.getTableList();
	}
	return *this;
}

RVNGHeaderFooter::~RVNGHeaderFooter()
{
//	delete m_subDocument;
}

RVNGPageSpan::RVNGPageSpan() :
	m_isPageNumberSuppressed(false),
	m_formLength(11.0),
	m_formWidth(8.5f),
	m_formOrientation(PORTRAIT),
	m_marginLeft(1.0),
	m_marginRight(1.0),
	m_marginTop(RVNG_DEFAULT_PAGE_MARGIN_TOP),
	m_marginBottom(RVNG_DEFAULT_PAGE_MARGIN_BOTTOM),
	m_pageNumberPosition(PAGENUMBER_POSITION_NONE),
	m_isPageNumberOverridden(false),
	m_pageNumberOverride(0),
	m_pageNumberingType(ARABIC),
	m_pageNumberingFontName(/*WP6_DEFAULT_FONT_NAME*/"Times New Roman"), // EN PAS DEFAULT FONT AAN VOOR WP5/6/etc
	m_pageNumberingFontSize(12.0/*WP6_DEFAULT_FONT_SIZE*/), // FIXME ME!!!!!!!!!!!!!!!!!!! HELP WP6_DEFAULT_FONT_SIZE
	m_headerFooterList(),
	m_pageSpan(1)
{
	for (int i=0; i<RVNG_NUM_HEADER_FOOTER_TYPES; i++)
		m_isHeaderFooterSuppressed[i]=false;
}

// NB: this is not a literal "clone" function: it is contingent on the side margins that are passed,
// and suppression and override variables are not copied
RVNGPageSpan::RVNGPageSpan(const RVNGPageSpan &page, double paragraphMarginLeft, double paragraphMarginRight) :
	m_isPageNumberSuppressed(false),
	m_formLength(page.getFormLength()),
	m_formWidth(page.getFormWidth()),
	m_formOrientation(page.getFormOrientation()),
	m_marginLeft(page.getMarginLeft()+paragraphMarginLeft),
	m_marginRight(page.getMarginRight()+paragraphMarginRight),
	m_marginTop(page.getMarginTop()),
	m_marginBottom(page.getMarginBottom()),
	m_pageNumberPosition(page.getPageNumberPosition()),
	m_isPageNumberOverridden(false),
	m_pageNumberOverride(0),
	m_pageNumberingType(page.getPageNumberingType()),
	m_pageNumberingFontName(page.getPageNumberingFontName()),
	m_pageNumberingFontSize(page.getPageNumberingFontSize()),
	m_headerFooterList(page.getHeaderFooterList()),
	m_pageSpan(page.getPageSpan())
{
	for (int i=0; i<RVNG_NUM_HEADER_FOOTER_TYPES; i++)
		m_isHeaderFooterSuppressed[i] = false;
}

RVNGPageSpan::~RVNGPageSpan()
{
}


void RVNGPageSpan::setHeaderFooter(const RVNGHeaderFooterType type, const uint8_t headerFooterType, const RVNGHeaderFooterOccurence occurence,
                                  const  RVNGSubDocument *subDocument, RVNGTableList tableList)
{
	RVNGHeaderFooter headerFooter(type, occurence, headerFooterType, subDocument, tableList);
	switch (occurence)
	{
	case ALL:
	case NEVER:
		_removeHeaderFooter(type, ODD);
		_removeHeaderFooter(type, EVEN);
		_removeHeaderFooter(type, ALL);
		break;
	case ODD:
		_removeHeaderFooter(type, ODD);
		_removeHeaderFooter(type, ALL);
		break;
	case EVEN:
		_removeHeaderFooter(type, EVEN);
		_removeHeaderFooter(type, ALL);
		break;
	default:
		break;
	}

	if ((occurence != NEVER) && (subDocument))
		m_headerFooterList.push_back(headerFooter);

	bool containsHFLeft = _containsHeaderFooter(type, ODD);
	bool containsHFRight = _containsHeaderFooter(type, EVEN);

	RVNG_DEBUG_MSG(("Contains HFL: %i HFR: %i\n", containsHFLeft, containsHFRight));
	if (containsHFLeft && !containsHFRight)
	{
		RVNG_DEBUG_MSG(("Inserting dummy header right\n"));
		RVNGHeaderFooter dummyHeader(type, EVEN, DUMMY_INTERNAL_HEADER_FOOTER, 0);
		m_headerFooterList.push_back(dummyHeader);
	}
	else if (!containsHFLeft && containsHFRight)
	{
		RVNG_DEBUG_MSG(("Inserting dummy header left\n"));
		RVNGHeaderFooter dummyHeader(type, ODD, DUMMY_INTERNAL_HEADER_FOOTER, 0);
		m_headerFooterList.push_back(dummyHeader);
	}
}

void RVNGPageSpan::_removeHeaderFooter(RVNGHeaderFooterType type, RVNGHeaderFooterOccurence occurence)
{
	for (std::vector<RVNGHeaderFooter>::iterator iter = m_headerFooterList.begin(); iter != m_headerFooterList.end(); ++iter)
	{
		if ((*iter).getType() == type && (*iter).getOccurence() == occurence)
		{
			RVNG_DEBUG_MSG(("WordPerfect: Removing header/footer element of type: %i since it is identical to %i\n",(*iter).getType(), type));
			m_headerFooterList.erase(iter);
			return;
		}
	}
}

bool RVNGPageSpan::_containsHeaderFooter(RVNGHeaderFooterType type, RVNGHeaderFooterOccurence occurence)
{
	for (std::vector<RVNGHeaderFooter>::iterator iter = m_headerFooterList.begin(); iter != m_headerFooterList.end(); ++iter)
	{
		if ((*iter).getType()==type && (*iter).getOccurence()==occurence)
			return true;
	}

	return false;
}

inline bool operator==(const RVNGHeaderFooter &headerFooter1, const RVNGHeaderFooter &headerFooter2)
{
	return ((headerFooter1.getType() == headerFooter2.getType()) &&
	        (headerFooter1.getSubDocument() == headerFooter2.getSubDocument()) &&
	        (headerFooter1.getOccurence() == headerFooter2.getOccurence()) &&
	        (headerFooter1.getInternalType() == headerFooter2.getInternalType()) );
}

bool operator==(const RVNGPageSpan &page1, const RVNGPageSpan &page2)
{
	if ((page1.getMarginLeft() != page2.getMarginLeft()) || (page1.getMarginRight() != page2.getMarginRight()) ||
	        (page1.getMarginTop() != page2.getMarginTop())|| (page1.getMarginBottom() != page2.getMarginBottom()))
		return false;

	if (page1.getPageNumberPosition() != page2.getPageNumberPosition())
		return false;

	if (page1.getPageNumberSuppression() != page2.getPageNumberSuppression())
		return false;

	if (page1.getPageNumberOverriden() != page2.getPageNumberOverriden() ||
	        page1.getPageNumberOverride() != page2.getPageNumberOverride())
		return false;

	if (page1.getPageNumberingType() != page2.getPageNumberingType())
		return false;

	if (page1.getPageNumberingFontName() != page2.getPageNumberingFontName() ||
	        page1.getPageNumberingFontSize() != page2.getPageNumberingFontSize())
		return false;

	for (uint8_t i=0; i<RVNG_NUM_HEADER_FOOTER_TYPES; i++)
	{
		if (page1.getHeaderFooterSuppression(i) != page2.getHeaderFooterSuppression(i))
			return false;
	}

	// NOTE: yes this is O(n^2): so what? n=4 at most
	const std::vector<RVNGHeaderFooter> headerFooterList1 = page1.getHeaderFooterList();
	const std::vector<RVNGHeaderFooter> headerFooterList2 = page2.getHeaderFooterList();
	std::vector<RVNGHeaderFooter>::const_iterator iter1;
	std::vector<RVNGHeaderFooter>::const_iterator iter2;

	for (iter1 = headerFooterList1.begin(); iter1 != headerFooterList1.end(); ++iter1)
	{
		if (std::find(headerFooterList2.begin(), headerFooterList2.end(), (*iter1)) == headerFooterList2.end())
			return false;
	}

	// If we came here, we know that every header/footer that is found in the first page span is in the second too.
	// But this is not enought for us to know whether the page spans are equal. Now we have to check in addition
	// whether every header/footer that is in the second one is in the first too. If someone wants to optimize this,
	// (s)he is most welcome :-)

	for (iter2 = headerFooterList2.begin(); iter2 != headerFooterList2.end(); ++iter2)
	{
		if (std::find(headerFooterList1.begin(), headerFooterList1.end(), (*iter2)) == headerFooterList1.end())
			return false;
	}


	RVNG_DEBUG_MSG(("WordPerfect: RVNGPageSpan == comparison finished, found no differences\n"));

	return true;
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
