/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002-2003 Marc Maurer (j.m.maurer@student.utwente.nl)
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WPXHLListener.h"
#include "WPXPageSpan.h"
#include "libwpd_internal.h"

_WPXParsingState::_WPXParsingState(bool sectionAttributesChanged) :
	m_textAttributeBits(0),
	m_textAttributesChanged(false),
	m_fontSize(12.0f/*WP6_DEFAULT_FONT_SIZE*/), // FIXME ME!!!!!!!!!!!!!!!!!!! HELP WP6_DEFAULT_FONT_SIZE
	m_fontName(g_string_new(/*WP6_DEFAULT_FONT_NAME*/"Times New Roman")), // EN PAS DEFAULT FONT AAN VOOR WP5/6/etc
	m_fontColor(new RGBSColor(0x00,0x00,0x00,0x64)), //Set default to black. Maybe once it will change, but for the while...
	m_highlightColor(NULL),

	m_isParagraphColumnBreak(false),
	m_isParagraphPageBreak(false),
/*	m_paragraphLineSpacing(1.0f),
	m_paragraphJustification(WPX_PARAGRAPH_JUSTIFICATION_LEFT),
	m_tempParagraphJustification(0),
*/
	m_isSectionOpened(false),

	m_isParagraphOpened(false),
	m_isParagraphClosed(false),
	m_isSpanOpened(false),
	m_numDeferredParagraphBreaks(0),
/*	m_numRemovedParagraphBreaks(0),

	m_currentTable(NULL),
	m_nextTableIndice(0),
	m_currentTableCol(0),
	m_currentTableRow(0),
	m_isTableOpened(false),
	m_isTableRowOpened(false),
	m_isTableCellOpened(false),
*/
	m_isPageSpanOpened(false),
	m_nextPageSpanIndice(0),
	m_numPagesRemainingInSpan(0),

	m_sectionAttributesChanged(sectionAttributesChanged),
	m_numColumns(1),

	m_pageFormLength(11.0f),
	m_pageFormWidth(8.5f),
	m_pageFormOrientation(PORTRAIT),

	m_pageMarginLeft(1.0f),
	m_pageMarginRight(1.0f),
	m_paragraphMarginLeft(0.0f),
	m_paragraphMarginRight(0.0f),
	m_paragraphTextIndent(0.0f),
	m_paragraphSpacingAfter(0.0f),
/*	m_currentRow(-1),
	m_currentColumn(-1),

	m_currentListLevel(0),
	m_putativeListElementHasParagraphNumber(false),
	m_putativeListElementHasDisplayReferenceNumber(false),

	m_noteTextPID(0),
	m_inSubDocument(false)
*/

	m_alignmentCharacter('.'),
	m_tabStops(NULL),
	m_numberOfTabStops(0),
	m_isTabPositionRelative(false)

{
}

_WPXParsingState::~_WPXParsingState()
{
	g_string_free(m_fontName, TRUE);
	DELETEP(m_fontColor);
	DELETEP(m_highlightColor);
	delete[] m_tabStops;
}

WPXHLListener::WPXHLListener(vector<WPXPageSpan *> *pageList, WPXHLListenerImpl *listenerImpl) :
	WPXLLListener(),
	m_pageList(pageList),
	m_listenerImpl(listenerImpl),
	m_ps(new WPXParsingState),
	m_isUndoOn(false)
{
}

WPXHLListener::~WPXHLListener()
{
	DELETEP(m_ps);
}

void WPXHLListener::startDocument()
{
	m_listenerImpl->setDocumentMetaData(m_metaData.m_author, m_metaData.m_subject,
					    m_metaData.m_publisher, m_metaData.m_category,
					    m_metaData.m_keywords, m_metaData.m_language,
					    m_metaData.m_abstract, m_metaData.m_descriptiveName,
					    m_metaData.m_descriptiveType);

	m_listenerImpl->startDocument();
	_openPageSpan();
}

void WPXHLListener::_openSection()
{
	_closeSection();
	if (m_ps->m_numColumns > 1)
		m_listenerImpl->openSection(m_ps->m_numColumns, 1.0f);
	else
		m_listenerImpl->openSection(m_ps->m_numColumns, 0.0f);

	m_ps->m_sectionAttributesChanged = false;
	m_ps->m_isSectionOpened = true;
}

void WPXHLListener::_closeSection()
{
	_closeParagraph();
	if (m_ps->m_isSectionOpened)
		m_listenerImpl->closeSection();

	m_ps->m_isSectionOpened = false;
}

void WPXHLListener::_openPageSpan()
{
	_closePageSpan();

	if ( !m_pageList ||
	     (m_pageList && m_ps->m_nextPageSpanIndice > (int)m_pageList->size() - 1)
	   )
	{
		throw ParseException();
	}

	WPXPageSpan *currentPage = (*m_pageList)[m_ps->m_nextPageSpanIndice];
	currentPage->makeConsistent(1);
	bool isLastPageSpan;
	(m_pageList->size() <= (m_ps->m_nextPageSpanIndice+1)) ? isLastPageSpan = true : isLastPageSpan = false;

	m_listenerImpl->openPageSpan(currentPage->getPageSpan(), isLastPageSpan,
	                 currentPage->getFormLength(), currentPage->getFormWidth(),
	                 currentPage->getFormOrientation(),
				     currentPage->getMarginLeft(), currentPage->getMarginRight(),
				     currentPage->getMarginTop(), currentPage->getMarginBottom());

	m_ps->m_pageFormWidth = currentPage->getFormWidth();
	m_ps->m_pageMarginLeft = currentPage->getMarginLeft();
	m_ps->m_pageMarginRight = currentPage->getMarginRight();

	const vector<WPXHeaderFooter> headerFooterList = currentPage->getHeaderFooterList();
	for (vector<WPXHeaderFooter>::const_iterator iter = headerFooterList.begin(); iter != headerFooterList.end(); iter++)
	{
		if (!currentPage->getHeaderFooterSuppression((*iter).getInternalType()))
		{
			m_listenerImpl->openHeaderFooter((*iter).getType(), (*iter).getOccurence());
			handleSubDocument((*iter).getTextPID(), true);
			m_listenerImpl->closeHeaderFooter((*iter).getType(), (*iter).getOccurence());
			WPD_DEBUG_MSG(("Header Footer Element: type: %i occurence: %i pid: %i\n",
				       (*iter).getType(), (*iter).getOccurence(), (*iter).getTextPID()));
		}
	}

	m_ps->m_pageFormLength = currentPage->getFormLength();
	m_ps->m_pageFormWidth = currentPage->getFormWidth();
	m_ps->m_pageFormOrientation = currentPage->getFormOrientation();
	m_ps->m_pageMarginLeft = currentPage->getMarginLeft();
	m_ps->m_pageMarginRight = currentPage->getMarginRight();
	m_ps->m_numPagesRemainingInSpan = (currentPage->getPageSpan() - 1);
	m_ps->m_nextPageSpanIndice++;
	m_ps->m_isPageSpanOpened = true;
}

void WPXHLListener::_closePageSpan()
{
	if (m_ps->m_isPageSpanOpened)
	{
		m_listenerImpl->closePageSpan();
		m_ps->m_isPageSpanOpened = false;
	}
}

void WPXHLListener::_closeParagraph()
{
	_closeSpan();
	if (m_ps->m_isParagraphOpened)
		m_listenerImpl->closeParagraph();

	m_ps->m_isParagraphOpened = false;
}

void WPXHLListener::_openSpan()
{
	_closeSpan();
	m_listenerImpl->openSpan(m_ps->m_textAttributeBits,
				 m_ps->m_fontName->str,
				 m_ps->m_fontSize, m_ps->m_fontColor, m_ps->m_highlightColor);

	m_ps->m_isSpanOpened = true;
}

void WPXHLListener::_closeSpan()
{
	if (m_ps->m_isSpanOpened)
		m_listenerImpl->closeSpan();

	m_ps->m_isSpanOpened = false;
}

/**
Creates an new document state. Saves the old state on a "stack".
*/
void WPXHLListener::handleSubDocument(guint16 textPID, const bool isHeaderFooter)
{
	// save our old parsing state on our "stack"
	WPXParsingState *oldPS = m_ps;
	m_ps = new WPXParsingState(false); // false: don't open a new section unless we must inside this type of sub-document
	// BEGIN: copy page properties into the new parsing state
	m_ps->m_pageFormWidth = oldPS->m_pageFormWidth;
	m_ps->m_pageMarginLeft = oldPS->m_pageMarginLeft;
	m_ps->m_pageMarginRight = oldPS->m_pageMarginRight;
	// END: copy page properties into the new parsing state
	_handleSubDocument(textPID, isHeaderFooter);

	// restore our old parsing state
	delete m_ps;
	m_ps = oldPS;
}

void WPXHLListener::insertBreak(const guint8 breakType)
{
	if (!isUndoOn())
	{
		_flushText();
		switch (breakType)
		{
		case WPX_COLUMN_BREAK:
			m_ps->m_numDeferredParagraphBreaks++;
			m_ps->m_isParagraphColumnBreak = true;
			break;
		case WPX_PAGE_BREAK:
			m_ps->m_numDeferredParagraphBreaks++;
			m_ps->m_isParagraphPageBreak = true;
			break;
			// TODO: (.. line break?)
		}
		switch (breakType)
		{
		case WPX_PAGE_BREAK:
		case WPX_SOFT_PAGE_BREAK:
			if (m_ps->m_numPagesRemainingInSpan > 0)
				m_ps->m_numPagesRemainingInSpan--;
			else
			{
				_openPageSpan();
			}
		default:
			break;
		}
	}
}
