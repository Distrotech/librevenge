/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2005-2006 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WP6CONTENTLISTENER_H
#define WP6CONTENTLISTENER_H

#include "WP6Listener.h"
#include "WPXContentListener.h"
#include "WP6FileStructure.h"

#include <stack>
#include <map>
#include <vector>

class WPXDocumentInterface;
class WPXTable;

enum WP6StyleState { NORMAL, DOCUMENT_NOTE, DOCUMENT_NOTE_GLOBAL,
                     BEGIN_BEFORE_NUMBERING,
                     BEGIN_NUMBERING_BEFORE_DISPLAY_REFERENCING,
                     DISPLAY_REFERENCING,
                     BEGIN_NUMBERING_AFTER_DISPLAY_REFERENCING,
                     BEGIN_AFTER_NUMBERING, STYLE_BODY, STYLE_END
                   };

enum WP6ListType { ORDERED, UNORDERED };

const int STATE_MEMORY = 3;
class WP6StyleStateSequence
{
public:
	WP6StyleStateSequence() : m_stateSequence()
	{
		clear();
	}
	void setCurrentState(WP6StyleState state)
	{
		for (unsigned i=(STATE_MEMORY-1); i>0; i--) m_stateSequence[i] = m_stateSequence[i-1];
		m_stateSequence[0]=state;
	}
	WP6StyleState getCurrentState() const
	{
		return m_stateSequence[0];
	}
	WP6StyleState getPreviousState() const
	{
		return m_stateSequence[1];
	}
	void clear()
	{
		m_stateSequence.clear();
		for (int i=0; i<STATE_MEMORY; i++) m_stateSequence.push_back(NORMAL);
	}

private:
	std::vector<WP6StyleState> m_stateSequence;
};

typedef struct _WP6ContentParsingState WP6ContentParsingState;
struct _WP6ContentParsingState
{
	_WP6ContentParsingState(WPXTableList tableList, unsigned nextTableIndice = 0);
	~_WP6ContentParsingState();
	WPXString m_bodyText;
	WPXString m_textBeforeNumber;
	WPXString m_textBeforeDisplayReference;
	WPXString m_numberText;
	WPXString m_textAfterDisplayReference;
	WPXString m_textAfterNumber;

	double m_paragraphMarginBottomRelative;
	double m_paragraphMarginBottomAbsolute;

	unsigned m_numRemovedParagraphBreaks;

	unsigned m_numListExtraTabs;
	bool m_isListReference;

	WPXTableList m_tableList;
	WPXTable *m_currentTable;
	unsigned m_nextTableIndice;

	std::stack<unsigned> m_listLevelStack;
	std::stack<WP6ListType> m_listTypeStack;
	uint16_t m_currentOutlineHash; // probably should replace Hash with Key in these sorts of cases
	uint8_t m_oldListLevel;
	WP6StyleStateSequence m_styleStateSequence;
	bool m_putativeListElementHasParagraphNumber;
	bool m_putativeListElementHasDisplayReferenceNumber;

	unsigned m_noteTextPID;
	unsigned m_numNestedNotes;

	bool m_isFrameOpened;

	uint32_t m_leaderCharacter;
	uint8_t m_leaderNumSpaces;
	std::vector<WPXTabStop> m_tempTabStops;
	std::vector<bool> m_tempUsePreWP9LeaderMethod;

	WPXNumberingType m_currentPageNumberingType;

private:
	_WP6ContentParsingState(const _WP6ContentParsingState &);
	_WP6ContentParsingState &operator=(const _WP6ContentParsingState &);
};

struct _WP6ListLevel
{
	int m_level;
	bool m_isListElementOpenAtLevel;
};

class WP6OutlineDefinition
{
public:
	WP6OutlineDefinition();
	WP6OutlineDefinition(const WP6OutlineLocation outlineLocation, const uint8_t *numberingMethods,
	                     const uint8_t tabBehaviourFlag);
	void update(const uint8_t *numberingMethods, const uint8_t tabBehaviourFlag);

	WPXNumberingType getListType(int level)
	{
		return m_listTypes[level];
	}

protected:
	void _updateNumberingMethods(const WP6OutlineLocation outlineLocation, const uint8_t *numberingMethods);

private:
	WPXNumberingType m_listTypes[WP6_NUM_LIST_LEVELS];
};

class WP6ContentListener : public WP6Listener, protected WPXContentListener
{
public:
	WP6ContentListener(std::list<WPXPageSpan> &pageList, WPXTableList tableList, WPXDocumentInterface *documentInterface);
	~WP6ContentListener();

	void startDocument()
	{
		WPXContentListener::startDocument();
	}
	void startSubDocument()
	{
		WPXContentListener::startSubDocument();
	}
	void setDate(const uint16_t type, const uint16_t year,
	             const uint8_t month, const uint8_t day,
	             const uint8_t hour, const uint8_t minute,
	             const uint8_t second, const uint8_t dayOfWeek,
	             const uint8_t timeZone, const uint8_t unused);
	void setExtendedInformation(const uint16_t type, const WPXString &data);
	void setAlignmentCharacter(const uint32_t character);
	void setLeaderCharacter(const uint32_t character, const uint8_t numSpaces);
	void defineTabStops(const bool isRelative, const std::vector<WPXTabStop> &tabStops,
	                    const std::vector<bool> &usePreWP9LeaderMethods);
	void insertCharacter(uint32_t character);
	void insertTab(const uint8_t tabType, double tabPosition);
	void handleLineBreak();
	void insertEOL();
	void insertBreak(const uint8_t breakType)
	{
		WPXContentListener::insertBreak(breakType);
	}
	void lineSpacingChange(const double lineSpacing)
	{
		WPXContentListener::lineSpacingChange(lineSpacing);
	}
	void justificationChange(const uint8_t justification)
	{
		WPXContentListener::justificationChange(justification);
	}
	void characterColorChange(const uint8_t red, const uint8_t green, const uint8_t blue);
	void characterShadingChange(const uint8_t shading);
	void highlightChange(const bool isOn, const RGBSColor &color);
	void fontChange(const uint16_t matchedFontPointSize, const uint16_t fontPID, const WPXString &fontName);
	void attributeChange(const bool isOn, const uint8_t attribute);
	void spacingAfterParagraphChange(const double spacingRelative, const double spacingAbsolute);
	void pageNumberingChange(const WPXPageNumberPosition /* page numbering position */, const uint16_t /* matchedFontPointSize */, const uint16_t /* fontPID */) {}
	void pageMarginChange(const uint8_t /* side */, const uint16_t /* margin */) {}
	void pageFormChange(const uint16_t /* length */, const uint16_t /* width */, const WPXFormOrientation /* orientation */) {}
	void marginChange(const uint8_t side, const uint16_t margin);
	void paragraphMarginChange(const uint8_t side, const int16_t margin);
	void indentFirstLineChange(const int16_t offset);
	void columnChange(const WPXTextColumnType columnType, const uint8_t numColumns, const std::vector<double> &columnWidth,
	                  const std::vector<bool> &isFixedWidth);
	void updateOutlineDefinition(const WP6OutlineLocation outlineLocation, const uint16_t outlineHash,
	                             const uint8_t *numberingMethods, const uint8_t tabBehaviourFlag);

	void paragraphNumberOn(const uint16_t outlineHash, const uint8_t level, const uint8_t flag);
	void paragraphNumberOff();
	void displayNumberReferenceGroupOn(const uint8_t subGroup, const uint8_t level);
	void displayNumberReferenceGroupOff(const uint8_t subGroup);
	void styleGroupOn(const uint8_t subGroup);
	void styleGroupOff(const uint8_t subGroup);
	void globalOn(const uint8_t systemStyle);
	void globalOff();
	void noteOn(const uint16_t textPID);
	void noteOff(const WPXNoteType noteType);
	void headerFooterGroup(const uint8_t /* headerFooterType */, const uint8_t /* occurenceBits */, const uint16_t /* textPID */) {}
	void suppressPageCharacteristics(const uint8_t /* suppressCode */) {}
	void setPageNumber(const uint16_t /* pageNumber */) {}
	void setPageNumberingType(const WPXNumberingType pageNumberingType);
	void endDocument()
	{
		WPXContentListener::endDocument();
	}
	void endSubDocument()
	{
		WPXContentListener::endSubDocument();
	}

	void defineTable(const uint8_t position, const uint16_t leftOffset);
	void addTableColumnDefinition(const uint32_t width, const uint32_t leftGutter, const uint32_t rightGutter,
	                              const uint32_t attributes, const uint8_t alignment);
	void startTable();
	void insertRow(const uint16_t rowHeight, const bool isMinimumHeight, const bool isHeaderRow);
	void insertCell(const uint8_t colSpan, const uint8_t rowSpan, const uint8_t borderBits,
	                const RGBSColor *cellFgColor, const RGBSColor *cellBgColor,
	                const RGBSColor *cellBorderColor, const WPXVerticalAlignment cellVerticalAlignment,
	                const bool useCellAttributes, const uint32_t cellAttributes);
	void endTable();
	void boxOn(const uint8_t anchoringType, const uint8_t generalPositioningFlags, const uint8_t horizontalPositioningFlags,
	           const int16_t horizontalOffset, const uint8_t leftColumn, const uint8_t rightColumn,
	           const uint8_t verticalPositioningFlags, const int16_t verticalOffset, const uint8_t widthFlags, const uint16_t width,
	           const uint8_t heightFlags, const uint16_t height, const uint8_t boxContentType, const uint16_t nativeWidth,
	           const uint16_t nativeHeight);
	void boxOff();
	void insertGraphicsData(const uint16_t packetId);
	void insertTextBox(const WP6SubDocument *subDocument);
	void commentAnnotation(const uint16_t textPID);

	void undoChange(const uint8_t undoType, const uint16_t undoLevel);

protected:
	void _handleSubDocument(const WPXSubDocument *subDocument, WPXSubDocumentType subDocumentType, WPXTableList tableList, unsigned nextTableIndice = 0);

	//void _handleLineBreakElementBegin();
	void _paragraphNumberOn(const uint16_t outlineHash, const uint8_t level);
	void _flushText();
	void _handleListChange(const uint16_t outlineHash);

	void _changeList();

private:
	WP6ContentListener(const WP6ContentListener &);
	WP6ContentListener &operator=(const WP6ContentListener &);
	WP6ContentParsingState *m_parseState;

	std::map<uint16_t,WP6OutlineDefinition *> m_outlineDefineHash;
};

#endif /* WP6CONTENTLISTENER_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
