/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
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

#include "WPXPageSpan.h"
#include "libwpd_internal.h"
#include <math.h>

const float WP6_DEFAULT_PAGE_MARGIN_TOP = 1.0f;
const float WP6_DEFAULT_PAGE_MARGIN_BOTTOM = 1.0f;
const float PAGE_MAX_DELTA = 0.05f;
const uint8_t DUMMY_INTERNAL_HEADER_FOOTER = 16;

// precondition: 0 <= headerFooterType <= 3 (i.e.: we don't handle watermarks here)
const WPXHeaderFooterType _convertHeaderFooterType(const uint8_t headerFooterType)
{       
	WPXHeaderFooterType wpxType;

	(headerFooterType <= WP6_HEADER_FOOTER_GROUP_HEADER_B) ? wpxType = HEADER : wpxType = FOOTER;

	return wpxType;
}

const WPXHeaderFooterOccurence _convertHeaderFooterOccurence(const uint8_t occurenceBits)
{
	if (occurenceBits & WP6_HEADER_FOOTER_GROUP_EVEN_BIT && occurenceBits & WP6_HEADER_FOOTER_GROUP_ODD_BIT)
		return ALL;
	else if (occurenceBits & WP6_HEADER_FOOTER_GROUP_EVEN_BIT)
		return EVEN;
	else
		return ODD;

}

WPXHeaderFooter::WPXHeaderFooter(const WPXHeaderFooterType headerFooterType, const WPXHeaderFooterOccurence occurence, 
				 const uint8_t internalType, const uint16_t textPID, WPXTableList *tableList) :
	m_type(headerFooterType),
	m_occurence(occurence),
	m_internalType(internalType),
	m_textPID(textPID),
	m_tableList(tableList)
{
}

WPXHeaderFooter::WPXHeaderFooter(const WPXHeaderFooter &headerFooter) :
	m_type(headerFooter.getType()),
	m_occurence(headerFooter.getOccurence()),
	m_internalType(headerFooter.getInternalType()),
	m_textPID(headerFooter.getTextPID())
{
	WPXTableList *tableList = headerFooter.getTableList();
	if (tableList)
	{
		m_tableList = tableList;
		m_tableList->addRef();
	}
	else
		m_tableList = NULL;
}

WPXHeaderFooter::~WPXHeaderFooter()
{
	if (m_tableList)
		m_tableList->unRef();
}

WPXPageSpan::WPXPageSpan() :
	m_formLength(11.0f),
	m_formWidth(8.5f),
	m_formOrientation(PORTRAIT),
	m_marginLeft(1.0f),
	m_marginRight(1.0f),
	m_marginTop(WP6_DEFAULT_PAGE_MARGIN_TOP),
	m_marginBottom(WP6_DEFAULT_PAGE_MARGIN_BOTTOM),
	m_pageSpan(1)
{
	for (int i=0; i<WP6_NUM_HEADER_FOOTER_TYPES; i++)
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
	m_pageSpan(page.getPageSpan()),
	m_headerFooterList(page.getHeaderFooterList())
{
	for (int i=0; i<WP6_NUM_HEADER_FOOTER_TYPES; i++)
		m_isHeaderFooterSuppressed[i] = false;
}


void WPXPageSpan::setHeaderFooter(const uint8_t headerFooterType, const uint8_t occurenceBits, 
				  const uint16_t textPID, WPXTableList *tableList)
{
        WPXHeaderFooterType wpxType = _convertHeaderFooterType(headerFooterType);
	WPXHeaderFooterOccurence wpxOccurence = _convertHeaderFooterOccurence(occurenceBits);
	WPXHeaderFooter headerFooter(wpxType, wpxOccurence, headerFooterType, textPID, tableList);
	switch (wpxOccurence) 
	{
	case ALL:
		_removeHeaderFooter(wpxType, ODD); _removeHeaderFooter(wpxType, EVEN); _removeHeaderFooter(wpxType, ALL);
		break;
	case ODD:
		_removeHeaderFooter(wpxType, ODD); _removeHeaderFooter(wpxType, ALL);
		break;
	case EVEN:
		_removeHeaderFooter(wpxType, EVEN); _removeHeaderFooter(wpxType, ALL);
		break;		
	}
	m_headerFooterList.push_back(headerFooter);

	bool containsHFLeft = _containsHeaderFooter(wpxType, ODD);
	bool containsHFRight = _containsHeaderFooter(wpxType, EVEN);

	WPD_DEBUG_MSG(("Contains HFL: %i HFR: %i\n", containsHFLeft, containsHFRight));
	if (containsHFLeft && !containsHFRight)
	{
		WPD_DEBUG_MSG(("Inserting dummy header right\n"));
		WPXHeaderFooter dummyHeader(wpxType, EVEN, DUMMY_INTERNAL_HEADER_FOOTER, 0, NULL);
		m_headerFooterList.push_back(dummyHeader);
	}
	else if (!containsHFLeft && containsHFRight)
	{
		WPD_DEBUG_MSG(("Inserting dummy header left\n"));
		WPXHeaderFooter dummyHeader(wpxType, ODD, DUMMY_INTERNAL_HEADER_FOOTER, 0, NULL);
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
	for (vector<WPXHeaderFooter>::iterator iter = m_headerFooterList.begin(); iter != m_headerFooterList.end(); iter++) 
	{
		if ((*iter).getType() == type && (*iter).getOccurence() == occurence) {
			WPD_DEBUG_MSG(("WordPerfect: Removing header/footer element of type: %i since it is identical to %i\n",(*iter).getType(), type));
			m_headerFooterList.erase(iter);
			WPD_DEBUG_MSG(("WordPerfect: Now our list looks like this:\n"));
			for (vector<WPXHeaderFooter>::const_iterator iter1 = m_headerFooterList.begin(); 
			     iter1 != m_headerFooterList.end(); 
			     iter1++) 
			{
			WPD_DEBUG_MSG(("WordPerfect: El.: %i %i\n", (*iter1).getType(), (*iter1).getTextPID()));
			}
			return;
		}
	}
}

bool WPXPageSpan::_containsHeaderFooter(WPXHeaderFooterType type, WPXHeaderFooterOccurence occurence)
{
	for (vector<WPXHeaderFooter>::iterator iter = m_headerFooterList.begin(); iter != m_headerFooterList.end(); iter++) 
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


	for (int i=0; i<WP6_NUM_HEADER_FOOTER_TYPES; i++) {
		if (page1.getHeaderFooterSuppression(i) != page2.getHeaderFooterSuppression(i))
			return false;
	}

	// NOTE: yes this is O(n^2): so what? n=4 at most
	const vector<WPXHeaderFooter> headerFooterList1 = page1.getHeaderFooterList();	
	for (vector<WPXHeaderFooter>::const_iterator iter1 = headerFooterList1.begin(); 
	     iter1 != headerFooterList1.end(); 
	     iter1++) 
	{
		const vector<WPXHeaderFooter> headerFooterList2 = page2.getHeaderFooterList();	
		vector<WPXHeaderFooter>::const_iterator iter2;
		for (iter2 = headerFooterList2.begin(); iter2 != headerFooterList2.end(); iter2++) 
		{
			WPD_DEBUG_MSG(("WordPerfect: WPXPageSpan ==  header/footer comparison (%i, %i))\n", 
					       (*iter1).getTextPID(), (*iter2).getTextPID()));
			if ((*iter1).getType() == (*iter2).getType() && (*iter1).getTextPID() == (*iter2).getTextPID())
			{
				WPD_DEBUG_MSG(("WordPerfect: WPXPageSpan == found same header/footer (%i, %i), breaking)\n", 
					       (*iter1).getTextPID(), (*iter2).getTextPID()));
				break;
			}
		}		
		if (iter2 == headerFooterList2.end())
			return false;
	}
	WPD_DEBUG_MSG(("WordPerfect: WPXPageSpan == comparison finished, found no differences\n"));

	return true;
}
