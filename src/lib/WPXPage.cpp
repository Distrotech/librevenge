/* libwpd2
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

#include "WPXPage.h"

const float WP6_DEFAULT_PAGE_MARGIN_TOP = 1.0f;
const float WP6_DEFAULT_PAGE_MARGIN_BOTTOM = 1.0f;

WPXPage::WPXPage() :
	m_headerAPID(0), 
	m_headerBPID(0),
	m_footerAPID(0), 
	m_footerBPID(0),
	m_marginLeft(0.0f), 
	m_marginRight(0.0f),
	m_marginTop(WP6_DEFAULT_PAGE_MARGIN_TOP), 
	m_marginBottom(WP6_DEFAULT_PAGE_MARGIN_BOTTOM),
	m_pageSpan(1)

{
}

WPXPage::WPXPage(WPXPage &page, float paragraphMarginLeft, float paragraphMarginRight) :
	m_headerAPID(page.getHeaderAPID()), 
	m_headerBPID(page.getHeaderBPID()),
	m_footerAPID(page.getFooterAPID()), 
	m_footerBPID(page.getFooterBPID()),
	m_marginLeft(page.getMarginLeft()+paragraphMarginLeft), 
	m_marginRight(page.getMarginRight()+paragraphMarginRight),
	m_marginTop(page.getMarginTop()), 
	m_marginBottom(page.getMarginBottom()),
	m_pageSpan(page.getPageSpan())

{
}

bool operator==(const WPXPage &page1, const WPXPage &page2)
{
	if (page1.getHeaderAPID() == page2.getHeaderAPID() &&
	    page1.getHeaderBPID() == page2.getHeaderBPID() &&
	    page1.getFooterAPID() == page2.getFooterAPID() &&
	    page1.getFooterBPID() == page2.getFooterBPID() &&
	    page1.getMarginLeft() == page2.getMarginLeft() &&
	    page1.getMarginRight() == page2.getMarginRight() &&
	    page1.getMarginTop() == page2.getMarginTop() &&
	    page1.getMarginBottom() == page2.getMarginBottom())
		return true;

	return false;
}
