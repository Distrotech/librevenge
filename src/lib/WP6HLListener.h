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
 
#ifndef WP6HLLISTENER_H
#define WP6HLLISTENER_H
 
#include "WP6LLListener.h"
#include "WPXHLListener.h"
#include "WP6FileStructure.h"

#include <stack>
#include <map>
#include <vector>
using namespace std;

class WPXHLListenerImpl;

enum WP6StyleState { NORMAL, DOCUMENT_NOTE, DOCUMENT_NOTE_GLOBAL, 
		     BEGIN_BEFORE_NUMBERING,
		     BEGIN_NUMBERING_BEFORE_DISPLAY_REFERENCING, 
		     DISPLAY_REFERENCING, 
		     BEGIN_NUMBERING_AFTER_DISPLAY_REFERENCING,
		     BEGIN_AFTER_NUMBERING, STYLE_BODY, STYLE_END };

typedef struct _WP6DocumentMetaData WP6DocumentMetaData;
struct _WP6DocumentMetaData
{
	UCSString m_author;
	UCSString m_subject;
	UCSString m_publisher; 
	UCSString m_category;
	UCSString m_keywords; 
	UCSString m_language;
	UCSString m_abstract; 
	UCSString m_descriptiveName;
	UCSString m_descriptiveType;
};

typedef struct _WP6TableDefinition WP6TableDefinition;
struct _WP6TableDefinition
{
	guint8 m_positionBits;
	gfloat m_leftOffset;
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
	UCSString m_bodyText;
	UCSString m_textBeforeNumber;
	UCSString m_textBeforeDisplayReference;
	UCSString m_numberText;
	UCSString m_textAfterDisplayReference;
	UCSString m_textAfterNumber;

	guint32 m_textAttributeBits;
	gboolean m_textAttributesChanged;
	gfloat m_fontSize;
	GString * m_fontName;

	gboolean m_isParagraphColumnBreak;
	gboolean m_isParagraphPageBreak;
	guint8 m_paragraphJustification;
	float m_paragraphLineSpacing;
 	gboolean m_paragraphJustificationChanged;

	gboolean m_isSectionOpened;

	gboolean m_isParagraphOpened;
	gboolean m_isParagraphClosed;
	gboolean m_isSpanOpened;
	guint m_numDeferredParagraphBreaks;
	guint m_numRemovedParagraphBreaks;
	
	gboolean m_isTableOpened;
	gboolean m_isTableRowOpened;
	gboolean m_isTableColumnOpened;
	gboolean m_isTableCellOpened;

	gboolean m_sectionAttributesChanged;
	guint m_numColumns;
	gboolean m_isLeftMarginSet;
	gboolean m_isRightMarginSet;
	gfloat m_marginLeft;
	gfloat m_marginRight;
	
	gint32 m_currentRow;
	gint32 m_currentColumn;

	stack<int> m_listLevelStack;
	guint16 m_currentOutlineHash; // probably should replace Hash with Key in these sorts of cases
	guint8 m_oldListLevel;
	guint8 m_currentListLevel;
	WP6StyleStateSequence m_styleStateSequence;
	gboolean m_putativeListElementHasParagraphNumber;
	gboolean m_putativeListElementHasDisplayReferenceNumber;

	int m_noteTextPID;
	gboolean m_inSubDocument;

	gboolean m_isUndoOn;
};

class WP6OutlineDefinition
{
 public:
	WP6OutlineDefinition();
	WP6OutlineDefinition(const WP6OutlineLocation outlineLocation, const guint8 *numberingMethods, 
			  const guint8 tabBehaviourFlag);
	void update(const guint8 *numberingMethods, const guint8 tabBehaviourFlag);

	NumberingType getListType(int level) { return m_listTypes[level]; }

protected:
	void _updateNumberingMethods(const WP6OutlineLocation outlineLocation, const guint8 *numberingMethods);
                 
private:	
	NumberingType m_listTypes[WP6_NUM_LIST_LEVELS];
};

class WP6HLListener : public WP6LLListener
{
public:
	WP6HLListener(WPXHLListenerImpl *listenerImpl);
	~WP6HLListener();

	// for getting low-level messages from the parser
	virtual void setDate(const guint16 year, const guint8 month, const guint8 day, 
			     const guint8 hour, const guint8 minute, const guint8 second,
			     const guint8 dayOfWeek, const guint8 timeZone, const guint8 unused) {}
	virtual void setExtendedInformation(const guint16 type, const UCSString &data);
	virtual void startDocument();
	virtual void insertCharacter(const guint16 character);
	virtual void insertTab();
	virtual void insertEOL();
 	virtual void insertBreak(const guint8 breakType); 
	virtual void undoChange(const guint8 undoType, const guint16 undoLevel);
	virtual void fontChange(const guint16 matchedFontPointSize, const guint16 fontPID);
	virtual void attributeChange(const gboolean isOn, const guint8 attribute);
	virtual void lineSpacingChange(const gfloat lineSpacing);
	virtual void justificationChange(const guint8 justification);
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
	virtual void noteOff(const NoteType noteType);
	virtual void endDocument();
 
 	virtual void defineTable(guint8 position, guint16 leftOffset);
	virtual void addTableColumnDefintion(guint32 width, guint32 leftGutter, guint32 rightGutter);
	virtual void startTable();
 	virtual void insertRow();
 	virtual void insertCell(const guint8 colSpan, const guint8 rowSpan, const gboolean boundFromLeft, const gboolean boundFromAbove, const RGBSColor * cellFgColor, const RGBSColor * cellBgColor);
 	virtual void endTable(); 

protected:
	void _handleLineBreakElementBegin();
	void _flushText(const gboolean forceInitialParagraph=FALSE);
	void _handleListChange(const guint16 outlineHash);

	void _openSection();
	void _closeSection();

	void _openTable();
	void _closeTable();
	void _openTableRow();
	void _closeTableRow();
	void _openTableCell(const guint8 colSpan, const guint8 rowSpan, 
			    const gboolean boundFromLeft, const gboolean boundFromAbove, 
			    const RGBSColor * cellFgColor, const RGBSColor * cellBgColor);
	void _closeTableCell();

	void _openParagraph();
	void _closeParagraph();
	void _openSpan();
	void _closeSpan();

private:
	WPXHLListenerImpl * m_listenerImpl;

	WP6DocumentMetaData m_metaData;
	WP6ParsingState *m_parseState;
	WP6TableDefinition m_tableDefinition;

	map<int,WP6OutlineDefinition *> m_outlineDefineHash;
};

#endif /* WP6HLLISTENER_H */
