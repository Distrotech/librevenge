/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
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

#include <math.h>
#include "WPXPageSpan.h"
#include "libwpd_internal.h"

const float WPX_DEFAULT_PAGE_MARGIN_TOP = 1.0f;
const float WPX_DEFAULT_PAGE_MARGIN_BOTTOM = 1.0f;
const float PAGE_MAX_DELTA = 0.05f;
const uint8_t DUMMY_INTERNAL_HEADER_FOOTER = 16;

// precondition: 0 <= headerFooterType <= 3 (i.e.: we don't handle watermarks here)
WPXHeaderFooter::WPXHeaderFooter(const WPXHeaderFooterType headerFooterType, const WPXHeaderFooterOccurence occurence, 
				 const uint8_t internalType, const WPXSubDocument * subDocument, WPXTableList tableList) :
	m_type(headerFooterType),
	m_occurence(occurence),
	m_internalType(internalType),
	m_subDocument(subDocument),
	m_tableList(tableList)
{
}

WPXHeaderFooter::WPXHeaderFooter(const WPXHeaderFooterType headerFooterType, const WPXHeaderFooterOccurence occurence, 
				 const uint8_t internalType, const WPXSubDocument * subDocument) :
	m_type(headerFooterType),
	m_occurence(occurence),
	m_internalType(internalType),
	m_subDocument(subDocument)
{
}

WPXHeaderFooter::WPXHeaderFooter(const WPXHeaderFooter &headerFooter) :
	m_type(headerFooter.getType()),
	m_occurence(headerFooter.getOccurence()),
	m_internalType(headerFooter.getInternalType()),
	m_subDocument(headerFooter.getSubDocument()),
	m_tableList(headerFooter.getTableList())
{
}

WPXHeaderFooter::~WPXHeaderFooter()
{
//	delete m_subDocument;
}

WPXPageSpan::WPXPageSpan() :
	m_formLength(11.0f),
	m_formWidth(8.5f),
	m_formOrientation(PORTRAIT),
	m_marginLeft(1.0f),
	m_marginRight(1.0f),
	m_marginTop(WPX_DEFAULT_PAGE_MARGIN_TOP),
	m_marginBottom(WPX_DEFAULT_PAGE_MARGIN_BOTTOM),
	m_pageSpan(1)
{
	for (int i=0; i<WPX_NUM_HEADER_FOOTER_TYPES; i++)
		m_isHeaderFooterSuppressed[i]=false;
}

// NB: this is not a literal "clone" function: it is contingent on the side margins that are passed,
// and suppression variables are not copied
WPXPageSpan::WPXPageSpan(WPXPageSpan &page, float paragraphMarginLeft, float paragraphMarginRight) :
	m_formLength(page.getFormLength()),
	m_formWidth(page.getFormWidth()),
	m_formOrientation(page.getFormOrientation()),
	m_marginLeft(page.getMarginLeft()+paragraphMarginLeft),
	m_marginRight(page.getMarginRight()+paragraphMarginRight),
	m_marginTop(page.getMarginTop()),
	m_marginBottom(page.getMarginBottom()),
	m_headerFooterList(page.getHeaderFooterList()),
	m_pageSpan(page.getPageSpan())
{
	for (int i=0; i<WPX_NUM_HEADER_FOOTER_TYPES; i++)
		m_isHeaderFooterSuppressed[i] = false;	
}

WPXPageSpan::~WPXPageSpan()
{
}


void WPXPageSpan::setHeaderFooter(const WPXHeaderFooterType type, const uint8_t headerFooterType, const WPXHeaderFooterOccurence occurence, 
				  const  WPXSubDocument * subDocument, WPXTableList tableList)
{
	WPXHeaderFooter headerFooter(type, occurence, headerFooterType, subDocument, tableList);
	switch (occurence) 
	{
	case ALL:
	case NEVER:
		_removeHeaderFooter(type, ODD); _removeHeaderFooter(type, EVEN); _removeHeaderFooter(type, ALL);
		break;
	case ODD:
		_removeHeaderFooter(type, ODD); _removeHeaderFooter(type, ALL);
		break;
	case EVEN:
		_removeHeaderFooter(type, EVEN); _removeHeaderFooter(type, ALL);
		break;		
	}

	if ((occurence != NEVER) && (subDocument))
		m_headerFooterList.push_back(headerFooter);

	bool containsHFLeft = _containsHeaderFooter(type, ODD);
	bool containsHFRight = _containsHeaderFooter(type, EVEN);

	WPD_DEBUG_MSG(("Contains HFL: %i HFR: %i\n", containsHFLeft, containsHFRight));
	if (containsHFLeft && !containsHFRight)
	{
		WPD_DEBUG_MSG(("Inserting dummy header right\n"));
		WPXHeaderFooter dummyHeader(type, EVEN, DUMMY_INTERNAL_HEADER_FOOTER, 0);
		m_headerFooterList.push_back(dummyHeader);
	}
	else if (!containsHFLeft && containsHFRight)
	{
		WPD_DEBUG_MSG(("Inserting dummy header left\n"));
		WPXHeaderFooter dummyHeader(type, ODD, DUMMY_INTERNAL_HEADER_FOOTER, 0);
		m_headerFooterList.push_back(dummyHeader);
	}
}

// makeConsistent: post-process page spans (i.e.: save this until all page spans are fully parsed)
// since this is a span, not an individuated page, we have to swap header/footer odd/even paramaters
// if we're not starting on an odd page
// ALSO: add a left/right footer to the page, if we have one but not the other (post-processing step)
void WPXPageSpan::makeConsistent(int startingPageNumber)
{
	if (!(startingPageNumber % 2))
	{
	}
}

void WPXPageSpan::_removeHeaderFooter(WPXHeaderFooterType type, WPXHeaderFooterOccurence occurence)
{
	for (std::vector<WPXHeaderFooter>::iterator iter = m_headerFooterList.begin(); iter != m_headerFooterList.end(); iter++) 
	{
		if ((*iter).getType() == type && (*iter).getOccurence() == occurence) {
			WPD_DEBUG_MSG(("WordPerfect: Removing header/footer element of type: %i since it is identical to %i\n",(*iter).getType(), type));
			m_headerFooterList.erase(iter);
			return;
#if 0
			WPD_DEBUG_MSG(("WordPerfect: Now our list looks like this:\n"));
			for (std::vector<WPXHeaderFooter>::const_iterator iter1 = m_headerFooterList.begin(); 
			     iter1 != m_headerFooterList.end(); 
			     iter1++) 
			{
			WPD_DEBUG_MSG(("WordPerfect: El.: %i %i\n", (*iter1).getType(), (*iter1).getTextPID()));
			}
			return;
#endif
		}
	}
}

bool WPXPageSpan::_containsHeaderFooter(WPXHeaderFooterType type, WPXHeaderFooterOccurence occurence)
{
	for (std::vector<WPXHeaderFooter>::iterator iter = m_headerFooterList.begin(); iter != m_headerFooterList.end(); iter++) 
	{
		if ((*iter).getType()==type && (*iter).getOccurence()==occurence)
			return true;
	}

	return false;
}

bool operator==(const WPXPageSpan &page1, const WPXPageSpan &page2)
{
	if (fabs(page1.getMarginLeft() - page2.getMarginLeft()) > PAGE_MAX_DELTA ||
	    fabs(page1.getMarginRight() - page2.getMarginRight())  > PAGE_MAX_DELTA ||
	    fabs(page1.getMarginTop() - page2.getMarginTop()) > PAGE_MAX_DELTA ||
	    fabs(page1.getMarginBottom() - page2.getMarginBottom()) > PAGE_MAX_DELTA)
		return false;


	for (int i=0; i<WPX_NUM_HEADER_FOOTER_TYPES; i++) {
		if (page1.getHeaderFooterSuppression(i) != page2.getHeaderFooterSuppression(i))
			return false;
	}

	// NOTE: yes this is O(n^2): so what? n=4 at most
	const std::vector<WPXHeaderFooter> headerFooterList1 = page1.getHeaderFooterList();	
	for (std::vector<WPXHeaderFooter>::const_iterator iter1 = headerFooterList1.begin(); 
	     iter1 != headerFooterList1.end(); 
	     iter1++) 
	{
		const std::vector<WPXHeaderFooter> headerFooterList2 = page2.getHeaderFooterList();	
		std::vector<WPXHeaderFooter>::const_iterator iter2;
		for (iter2 = headerFooterList2.begin(); iter2 != headerFooterList2.end(); iter2++) 
		{
			WPD_DEBUG_MSG(("WordPerfect: WPXPageSpan ==  header/footer comparison)\n"));
			if ((*iter1).getType() == (*iter2).getType() && (*iter1).getSubDocument() == (*iter2).getSubDocument())
			{
				WPD_DEBUG_MSG(("WordPerfect: WPXPageSpan == found same header/footer, breaking)\n"));
				break;
			}
		}		
		if (iter2 == headerFooterList2.end())
			return false;
	}
	WPD_DEBUG_MSG(("WordPerfect: WPXPageSpan == comparison finished, found no differences\n"));

	return true;
}
