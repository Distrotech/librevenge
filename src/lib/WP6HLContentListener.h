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

#ifndef WP6HLCONTENTLISTENER_H
#define WP6HLCONTENTLISTENER_H

#include "WP6HLListener.h"
#include "WPXHLListener.h"
#include "WP6FileStructure.h"

#include <stack>
#include <map>
#include <vector>

using namespace std;

class WPXHLListenerImpl;
class WP6LLParser;
class WPXTable;

enum WP6StyleState { NORMAL, DOCUMENT_NOTE, DOCUMENT_NOTE_GLOBAL,
		     BEGIN_BEFORE_NUMBERING,
		     BEGIN_NUMBERING_BEFORE_DISPLAY_REFERENCING,
		     DISPLAY_REFERENCING,
		     BEGIN_NUMBERING_AFTER_DISPLAY_REFERENCING,
		     BEGIN_AFTER_NUMBERING, STYLE_BODY, STYLE_END };

const int STATE_MEMORY = 3;
class WP6StyleStateSequence
{
public:
	WP6StyleStateSequence() { clear(); }
	void setCurrentState(WP6StyleState state) { for (int i=(STATE_MEMORY-1); i>0; i--) m_stateSequence[i] = m_stateSequence[i-1]; m_stateSequence[0]=state; }
	const WP6StyleState getCurrentState() const { return m_stateSequence[0]; /*currentState;*/ }
	const WP6StyleState getPreviousState() const { return m_stateSequence[1]; /*m_previousState;*/ }
	void clear() { m_stateSequence.clear(); for (int i=0; i<STATE_MEMORY; i++) m_stateSequence.push_back(NORMAL); }

private:
	vector<WP6StyleState> m_stateSequence;
	WP6StyleState m_currentState;
	WP6StyleState m_previousState;
};

typedef struct _WP6ParsingState WP6ParsingState;
struct _WP6ParsingState
{
	_WP6ParsingState(WPXTableList tableList, int nextTableIndice = 0);
	~_WP6ParsingState();
	WPXString m_bodyText;
	WPXString m_textBeforeNumber;
	WPXString m_textBeforeDisplayReference;
	WPXString m_numberText;
	WPXString m_textAfterDisplayReference;
	WPXString m_textAfterNumber;

	float m_paragraphMarginBottomRelative;
	float m_paragraphMarginBottomAbsolute;

	int m_numRemovedParagraphBreaks;

	WPXTableList m_tableList;
	WPXTable *m_currentTable;
	int m_nextTableIndice;

	stack<int> m_listLevelStack;
	uint16_t m_currentOutlineHash; // probably should replace Hash with Key in these sorts of cases
	uint8_t m_oldListLevel;
	uint8_t m_currentListLevel;
	WP6StyleStateSequence m_styleStateSequence;
	bool m_putativeListElementHasParagraphNumber;
	bool m_putativeListElementHasDisplayReferenceNumber;

	int m_noteTextPID;

	uint16_t m_leaderCharacter;
	uint8_t m_leaderNumSpaces;
	vector<WPXTabStop> m_tempTabStops;
	vector<bool> m_tempUsePreWP9LeaderMethod;
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

	WPXNumberingType getListType(int level) { return m_listTypes[level]; }

protected:
	void _updateNumberingMethods(const WP6OutlineLocation outlineLocation, const uint8_t *numberingMethods);

private:
	WPXNumberingType m_listTypes[WP6_NUM_LIST_LEVELS];
};

class WP6HLContentListener : public WP6HLListener
{
public:
	WP6HLContentListener(vector<WPXPageSpan *> *pageList, WPXTableList tableList, WPXHLListenerImpl *listenerImpl);
	virtual ~WP6HLContentListener();

	// for getting low-level messages from the parser
	virtual void setDate(const uint16_t year, const uint8_t month, const uint8_t day,
						const uint8_t hour, const uint8_t minute, const uint8_t second,
						const uint8_t dayOfWeek, const uint8_t timeZone, const uint8_t unused) {}
	virtual void setExtendedInformation(const uint16_t type, const WPXString &data);
	virtual void setAlignmentCharacter(const uint16_t character);
	virtual void setLeaderCharacter(const uint16_t character, const uint8_t numSpaces);
	virtual void defineTabStops(const bool isRelative, const vector<WPXTabStop> &tabStops, 
				    const vector<bool> &usePreWP9LeaderMethods);
	virtual void insertCharacter(const uint16_t character);
	virtual void insertTab(const uint8_t tabType, const float tabPosition);
	virtual void handleLineBreak();
	virtual void insertEOL();
	virtual void characterColorChange(const uint8_t red, const uint8_t green, const uint8_t blue);
	virtual void characterShadingChange(const uint8_t shading);
	virtual void highlightChange(const bool isOn, const RGBSColor color);
	virtual void fontChange(const uint16_t matchedFontPointSize, const uint16_t fontPID);
 	virtual void attributeChange(const bool isOn, const uint8_t attribute);
	virtual void spacingAfterParagraphChange(const float spacingRelative, const float spacingAbsolute);
	virtual void pageMarginChange(const uint8_t side, const uint16_t margin) {}
	virtual void pageFormChange(const uint16_t length, const uint16_t width, const WPXFormOrientation orientation, const bool isPersistent) {}
	virtual void marginChange(const uint8_t side, const uint16_t margin);
	virtual void paragraphMarginChange(const uint8_t side, const int16_t margin);
	virtual void indentFirstLineChange(const int16_t offset);
	virtual void columnChange(const WPXTextColumnType columnType, const uint8_t numColumns, const vector<float> &columnWidth,
				  const vector<bool> &isFixedWidth);
	virtual void updateOutlineDefinition(const WP6OutlineLocation outlineLocation, const uint16_t outlineHash,
					     const uint8_t *numberingMethods, const uint8_t tabBehaviourFlag);

	virtual void paragraphNumberOn(const uint16_t outlineHash, const uint8_t level, const uint8_t flag);
	virtual void paragraphNumberOff();
	virtual void displayNumberReferenceGroupOn(const uint8_t subGroup, const uint8_t level);
	virtual void displayNumberReferenceGroupOff(const uint8_t subGroup);
	virtual void styleGroupOn(const uint8_t subGroup);
	virtual void styleGroupOff(const uint8_t subGroup);
	virtual void globalOn(const uint8_t systemStyle);
	virtual void globalOff();
	virtual void noteOn(const uint16_t textPID);
	virtual void noteOff(const WPXNoteType noteType);
	virtual void headerFooterGroup(const uint8_t headerFooterType, const uint8_t occurenceBits, const uint16_t textPID) {}
	virtual void suppressPageCharacteristics(const uint8_t suppressCode) {}
	virtual void endDocument();

 	virtual void defineTable(uint8_t position, uint16_t leftOffset);
	virtual void addTableColumnDefinition(uint32_t width, uint32_t leftGutter, uint32_t rightGutter);
	virtual void startTable();
 	virtual void insertRow(const uint16_t rowHeight, const bool isMinimumHeight, const bool isHeaderRow);
 	virtual void insertCell(const uint8_t colSpan, const uint8_t rowSpan, const bool boundFromLeft, const bool boundFromAbove,
				const uint8_t borderBits, const RGBSColor * cellFgColor, const RGBSColor * cellBgColor,
				const RGBSColor * cellBorderColor, 
				const WPXVerticalAlignment cellVerticalAlignment, const uint32_t cellAttributes);
 	virtual void endTable();


protected:
	virtual void _handleSubDocument(uint16_t textPID, const bool isHeaderFooter, WPXTableList tableList, int nextTableIndice = 0);

	//void _handleLineBreakElementBegin();
	void _paragraphNumberOn(const uint16_t outlineHash, const uint8_t level);
	void _flushText(const bool fakeText=false);
	void _handleListChange(const uint16_t outlineHash);

	void _flushList();

private:
	WP6ParsingState *m_parseState;

	map<int,WP6OutlineDefinition *> m_outlineDefineHash;
};

#endif /* WP6HLCONTENTLISTENER_H */
