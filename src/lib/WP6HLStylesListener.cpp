#include "WP6HLStylesListener.h"
#include "WPXTable.h"
#include "WP6FileStructure.h"
#include "WPXFileStructure.h"
#include "libwpd_internal.h"

// WP6HLStylesListener: creates intermediate table and page span representations, given a
// sequence of messages passed to it by the parser.

WP6HLStylesListener::WP6HLStylesListener(vector<WPXPageSpan *> *pageList, vector<WPXTable *> *tableList) : 
	m_pageList(pageList),
	m_currentPage(new WPXPageSpan()),
	m_tableList(tableList), 
	m_currentPageHasContent(false)
{
}

void WP6HLStylesListener::endDocument()
{	
	insertBreak(WPX_SOFT_PAGE_BREAK); // pretend we just had a soft page break (for the last page)
	delete(m_currentPage); // and delete the non-existent page that was allocated as a result (scandalous waste!)
}

void WP6HLStylesListener::insertBreak(const guint8 breakType)
{
	if (!isUndoOn())
	{	
		switch (breakType) 
		{
		case WPX_PAGE_BREAK:
		case WPX_SOFT_PAGE_BREAK:
			if (m_pageList->size() > 0 && (*m_currentPage)==(*(m_pageList->back())))
			{
				int oldPageSpan = m_pageList->back()->getPageSpan();
				m_pageList->back()->setPageSpan(oldPageSpan + 1);
				delete(m_currentPage);
			}
			else
			{
				m_pageList->push_back(m_currentPage);
			}
			m_currentPage = new WPXPageSpan(*(m_pageList->back()));
			break;
		}
	}
}

void WP6HLStylesListener::pageMarginChange(const guint8 side, const guint16 margin)
{
	if (!isUndoOn()) 
	{
		float marginInch = (float)(((double)margin + (double)WP6_NUM_EXTRA_WPU) / (double)WPX_NUM_WPUS_PER_INCH);
		switch(side)
		{
		case WP6_PAGE_GROUP_TOP_MARGIN_SET:
			m_currentPage->setMarginTop(marginInch);
			break;
		case WP6_PAGE_GROUP_BOTTOM_MARGIN_SET:
			m_currentPage->setMarginBottom(marginInch);
			break;
		}
	}
}

void WP6HLStylesListener::marginChange(const guint8 side, const guint16 margin)
{
	if (!isUndoOn()) 
	{
		/*
		float marginInch = (float)(((double)margin + (double)WP6_NUM_EXTRA_WPU) / (double)WPX_NUM_WPUS_PER_INCH);
		switch(side)
		{
		case WP6_COLUMN_GROUP_LEFT_MARGIN_SET:
			if (!m_currentPageHasContent)
				m_currentPage->setMarginLeft(marginInch);
			else
				m_tempMarginLeft = marginInch;
			break;
		case WP6_COLUMN_GROUP_RIGHT_MARGIN_SET:
			if (!m_currentPageHasContent)
				m_currentPage->setMarginRight(marginInch);
			else
				m_tempMarginRight = marginInch;
			break;
		}
		*/
	}

}

void WP6HLStylesListener::headerFooterGroup(const guint8 headerFooterType, const guint8 occurenceBits, const guint16 textPID)
{
	if (!isUndoOn()) 
	{			
		WPD_DEBUG_MSG(("WordPerfect: headerFooterGroup (headerFooterType: %i, occurenceBits: %i, textPID: %i)\n", 
			       headerFooterType, occurenceBits, textPID));
		if (headerFooterType <= WP6_HEADER_FOOTER_GROUP_FOOTER_B) // ignore watermarks for now
			m_currentPage->setHeaderFooter(headerFooterType, occurenceBits, textPID);
	}
}

void WP6HLStylesListener::suppressPageCharacteristics(const guint8 suppressCode)
{
	if (!isUndoOn()) 
	{			
		WPD_DEBUG_MSG(("WordPerfect: suppressPageCharacteristics (suppressCode: %u)\n", suppressCode));
		if (suppressCode & WP6_PAGE_GROUP_SUPPRESS_HEADER_A)
			m_currentPage->setHeadFooterSuppression(WP6_HEADER_FOOTER_GROUP_HEADER_A, true);
		if (suppressCode & WP6_PAGE_GROUP_SUPPRESS_HEADER_B)
			m_currentPage->setHeadFooterSuppression(WP6_HEADER_FOOTER_GROUP_HEADER_B, true);
		if (suppressCode & WP6_PAGE_GROUP_SUPPRESS_FOOTER_A)
			m_currentPage->setHeadFooterSuppression(WP6_HEADER_FOOTER_GROUP_FOOTER_A, true);
		if (suppressCode & WP6_PAGE_GROUP_SUPPRESS_FOOTER_B)
			m_currentPage->setHeadFooterSuppression(WP6_HEADER_FOOTER_GROUP_FOOTER_B, true);			
	}
}

void WP6HLStylesListener::startTable()
{
	if (!isUndoOn()) 
	{			
		m_currentTable = new WPXTable();
		m_tableList->push_back(m_currentTable);
	}
}

void WP6HLStylesListener::insertRow()
{
	if (!isUndoOn() && m_currentTable != NULL)
		m_currentTable->insertRow();
}

void WP6HLStylesListener::insertCell(const guint8 colSpan, const guint8 rowSpan, const bool boundFromLeft, const bool boundFromAbove, 
				  const guint8 borderBits, 
				  const RGBSColor * cellFgColor, const RGBSColor * cellBgColor)
{
	if (!isUndoOn() && m_currentTable != NULL)
		m_currentTable->insertCell(colSpan, rowSpan, boundFromLeft, boundFromAbove, borderBits);
}

