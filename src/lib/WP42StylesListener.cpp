/* libwpd
 * Copyright (C) 2003 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2004 Marc Maurer (j.m.maurer@student.utwente.nl)
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
 
#include "WP42StylesListener.h"
#include "WPXTable.h"
#include "WP42FileStructure.h"
#include "WPXFileStructure.h"
#include "libwpd_internal.h"

WP42StylesListener::WP42StylesListener(std::list<WPXPageSpan> &pageList, WPXTableList tableList) : 
	WP42Listener(pageList, NULL),
	m_currentPage(WPXPageSpan()),
	m_tableList(tableList), 
	m_tempMarginLeft(1.0f),
	m_tempMarginRight(1.0f),
	m_currentPageHasContent(false)
{
}

void WP42StylesListener::endDocument()
{	
	insertBreak(WPX_SOFT_PAGE_BREAK); // pretend we just had a soft page break (for the last page)
}

void WP42StylesListener::insertBreak(const uint8_t breakType)
{
	//if (!isUndoOn())
	//{	
		switch (breakType) 
		{
		case WPX_PAGE_BREAK:
		case WPX_SOFT_PAGE_BREAK:
			if ((WPXListener::m_pageList.size()) > 0 && (m_currentPage==m_pageList.back()))
				m_pageList.back().setPageSpan(m_pageList.back().getPageSpan() + 1);
			else
				m_pageList.push_back(WPXPageSpan(m_currentPage));
			m_currentPage.setMarginLeft(m_tempMarginLeft);
			m_currentPage.setMarginRight(m_tempMarginRight);
			m_currentPageHasContent = false;
			break;
		}
	//}
}
/*
void WP42StylesListener::pageMarginChange(const uint8_t side, const uint16_t margin)
{
	if (!isUndoOn()) 
	{
		float marginInch = (float)(((double)margin + (double)WP6_NUM_EXTRA_WPU) / (double)WPX_NUM_WPUS_PER_INCH);
		switch(side)
		{
			case WP6_PAGE_GROUP_TOP_MARGIN_SET:
				m_currentPage.setMarginTop(marginInch);
				break;
			case WP6_PAGE_GROUP_BOTTOM_MARGIN_SET:
				m_currentPage.setMarginBottom(marginInch);
				break;
		}
	}
}

void WP42StylesListener::marginChange(const uint8_t side, const uint16_t margin)
{
	if (!isUndoOn()) 
	{		
		float marginInch = (float)(((double)margin + (double)WP6_NUM_EXTRA_WPU) / (double)WPX_NUM_WPUS_PER_INCH);
		switch(side)
		{
			case WP6_COLUMN_GROUP_LEFT_MARGIN_SET:
				if (!m_currentPageHasContent)
					m_currentPage.setMarginLeft(marginInch);
				m_tempMarginLeft = marginInch;
				break;
			case WP6_COLUMN_GROUP_RIGHT_MARGIN_SET:
				if (!m_currentPageHasContent)
					m_currentPage.setMarginRight(marginInch);
				m_tempMarginRight = marginInch;
				break;
		}
		
	}

}

void WP42StylesListener::headerFooterGroup(const uint8_t headerFooterType, const uint8_t occurenceBits, const uint16_t textPID)
{
	if (!isUndoOn()) 
	{			
		WPD_DEBUG_MSG(("WordPerfect: headerFooterGroup (headerFooterType: %i, occurenceBits: %i, textPID: %i)\n", 
			       headerFooterType, occurenceBits, textPID));
		if (headerFooterType <= WP6_HEADER_FOOTER_GROUP_FOOTER_B) // ignore watermarks for now
			m_currentPage.setHeaderFooter(headerFooterType, occurenceBits, textPID);
	}
}

void WP42StylesListener::suppressPageCharacteristics(const uint8_t suppressCode)
{
	if (!isUndoOn()) 
	{			
		WPD_DEBUG_MSG(("WordPerfect: suppressPageCharacteristics (suppressCode: %u)\n", suppressCode));
		if (suppressCode & WP6_PAGE_GROUP_SUPPRESS_HEADER_A)
			m_currentPage.setHeadFooterSuppression(WP6_HEADER_FOOTER_GROUP_HEADER_A, true);
		if (suppressCode & WP6_PAGE_GROUP_SUPPRESS_HEADER_B)
			m_currentPage.setHeadFooterSuppression(WP6_HEADER_FOOTER_GROUP_HEADER_B, true);
		if (suppressCode & WP6_PAGE_GROUP_SUPPRESS_FOOTER_A)
			m_currentPage.setHeadFooterSuppression(WP6_HEADER_FOOTER_GROUP_FOOTER_A, true);
		if (suppressCode & WP6_PAGE_GROUP_SUPPRESS_FOOTER_B)
			m_currentPage.setHeadFooterSuppression(WP6_HEADER_FOOTER_GROUP_FOOTER_B, true);			
	}
}
*/
void WP42StylesListener::startTable()
{
	//if (!isUndoOn()) 
	//{			
		m_currentPageHasContent = true;
		m_currentTable = new WPXTable();
		m_tableList.add(m_currentTable);
	//}
}

void WP42StylesListener::insertRow(const uint16_t rowHeight, const bool isMinimumHeight, const bool isHeaderRow)
{
	if (/*!isUndoOn() && */m_currentTable != NULL) 
	{
		m_currentPageHasContent = true;
		m_currentTable->insertRow();
	}
}

void WP42StylesListener::insertCell(const uint8_t colSpan, const uint8_t rowSpan, const uint8_t borderBits, 
				  const RGBSColor * cellFgColor, const RGBSColor * cellBgColor,
				  const RGBSColor * cellBorderColor, const WPXVerticalAlignment cellVerticalAlignment, 
				  const bool useCellAttributes, const uint32_t cellAttributes)
{
	if (/*!isUndoOn() &&*/ m_currentTable != NULL)
	{
		m_currentPageHasContent = true;
		m_currentTable->insertCell(colSpan, rowSpan, borderBits);
	}
}
