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

typedef struct _WP6TableDefinition WP6TableDefinition;
struct _WP6TableDefinition
{
	guint8 m_positionBits;
	float m_leftOffset;
	vector < WPXColumnDefinition > columns;
};

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
	_WP6ParsingState();
	~_WP6ParsingState();
	UCSString m_bodyText;
	UCSString m_textBeforeNumber;
	UCSString m_textBeforeDisplayReference;
	UCSString m_numberText;
	UCSString m_textAfterDisplayReference;
	UCSString m_textAfterNumber;

	bool m_isParagraphColumnBreak;
	bool m_isParagraphPageBreak;
	guint8 m_paragraphJustification;
	guint8 m_tempParagraphJustification; // TODO: wouldn't a better name be m_tabParagraphJustification ? - MARCM
	float m_paragraphLineSpacing;

	guint m_numRemovedParagraphBreaks;

	WPXTable *m_currentTable;
	int m_nextTableIndice;
	int m_currentTableCol;
	int m_currentTableRow;
	bool m_isTableOpened;
	bool m_isTableRowOpened;
	bool m_isTableColumnOpened;
	bool m_isTableCellOpened;
	gint32 m_currentRow;
	gint32 m_currentColumn;

	stack<int> m_listLevelStack;
	guint16 m_currentOutlineHash; // probably should replace Hash with Key in these sorts of cases
	guint8 m_oldListLevel;
	guint8 m_currentListLevel;
	WP6StyleStateSequence m_styleStateSequence;
	bool m_putativeListElementHasParagraphNumber;
	bool m_putativeListElementHasDisplayReferenceNumber;

	int m_noteTextPID;
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
	WP6OutlineDefinition(const WP6OutlineLocation outlineLocation, const guint8 *numberingMethods, 
			  const guint8 tabBehaviourFlag);
	void update(const guint8 *numberingMethods, const guint8 tabBehaviourFlag);

	WPXNumberingType getListType(int level) { return m_listTypes[level]; }

protected:
	void _updateNumberingMethods(const WP6OutlineLocation outlineLocation, const guint8 *numberingMethods);
                 
private:	
	WPXNumberingType m_listTypes[WP6_NUM_LIST_LEVELS];
};

class WP6HLContentListener : public WP6HLListener
{
public:
	WP6HLContentListener(vector<WPXPageSpan *> *pageList, vector<WPXTable *> *tableList, WPXHLListenerImpl *listenerImpl);
	virtual ~WP6HLContentListener();
		
	// for getting low-level messages from the parser
	virtual void setDate(const guint16 year, const guint8 month, const guint8 day, 
						const guint8 hour, const guint8 minute, const guint8 second,
						const guint8 dayOfWeek, const guint8 timeZone, const guint8 unused) {}
	virtual void setExtendedInformation(const guint16 type, const UCSString &data);
	virtual void insertCharacter(const guint16 character);
	virtual void insertTab(const guint8 tabType);
	virtual void insertEOL();
 	virtual void insertBreak(const guint8 breakType); 
	virtual void fontChange(const guint16 matchedFontPointSize, const guint16 fontPID);
 	virtual void attributeChange(const bool isOn, const guint8 attribute);
	virtual void lineSpacingChange(const float lineSpacing);
	virtual void justificationChange(const guint8 justification);
	virtual void pageMarginChange(const guint8 side, const guint16 margin) {}
	virtual void marginChange(const guint8 side, const guint16 margin);
	virtual void columnChange(const guint8 numColumns); 
	virtual void updateOutlineDefinition(const WP6OutlineLocation outlineLocation, const guint16 outlineHash, 
						const guint8 *numberingMethods, const guint8 tabBehaviourFlag);

	virtual void paragraphNumberOn(const guint16 outlineHash, const guint8 level, const guint8 flag);
	virtual void paragraphNumberOff();
	virtual void displayNumberReferenceGroupOn(const guint8 subGroup, const guint8 level);
	virtual void displayNumberReferenceGroupOff(const guint8 subGroup);
	virtual void styleGroupOn(const guint8 subGroup);	
	virtual void styleGroupOff(const guint8 subGroup);	
	virtual void globalOn(const guint8 systemStyle);
	virtual void globalOff();
	virtual void noteOn(const guint16 textPID);
	virtual void noteOff(const WPXNoteType noteType);
	virtual void headerFooterGroup(const guint8 headerFooterType, const guint8 occurenceBits, const guint16 textPID) {}
	virtual void suppressPageCharacteristics(const guint8 suppressCode) {}
	virtual void endDocument();
 
 	virtual void defineTable(guint8 position, guint16 leftOffset);
	virtual void addTableColumnDefinition(guint32 width, guint32 leftGutter, guint32 rightGutter);
	virtual void startTable();
 	virtual void insertRow();
 	virtual void insertCell(const guint8 colSpan, const guint8 rowSpan, const bool boundFromLeft, const bool boundFromAbove, 
						const guint8 borderBits, 
						const RGBSColor * cellFgColor, const RGBSColor * cellBgColor);
 	virtual void endTable(); 


protected:
	virtual void _handleSubDocument(guint16 textPID);

	//void _handleLineBreakElementBegin();
	void _paragraphNumberOn(const guint16 outlineHash, const guint8 level);
	void _flushText(const bool fakeText=false);
	void _handleListChange(const guint16 outlineHash);
	    
	void _openListElement();

	void _openTable();
	void _closeTable();
	void _openTableRow();
	void _closeTableRow();
	void _openTableCell(const guint8 colSpan, const guint8 rowSpan, 
			    const bool boundFromLeft, const bool boundFromAbove,
				const guint8 borderBits, 
			    const RGBSColor * cellFgColor, const RGBSColor * cellBgColor);
	void _closeTableCell();

	void _openParagraph();

private:
	WP6ParsingState *m_parseState;	

	WP6TableDefinition m_tableDefinition;
	vector<WPXTable *> *m_tableList;

	map<int,WP6OutlineDefinition *> m_outlineDefineHash;
};

#endif /* WP6HLCONTENTLISTENER_H */
