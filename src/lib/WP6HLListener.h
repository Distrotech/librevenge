/* libwpd2
 * Copyright (C) 2002 William Lachance (wlach@interlog.com)
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

class WPXHLListenerImpl;

enum WP6ParagraphStyleState { notInStyle, beginBeforeNumbering,
			      beginNumberingBeforeDisplayReferencing, 
			      beginDisplayReferencing, 
			      beginNumberingAfterDisplayReferencing,
			      beginAfterNumbering, styleBody, end };

class WP6ParagraphStyleStateSequence
{
public:
	WP6ParagraphStyleStateSequence() { m_currentState = m_previousState = notInStyle; }
	void setCurrentState(WP6ParagraphStyleState state) { m_previousState = m_currentState; m_currentState = state; }
	const WP6ParagraphStyleState getCurrentState() const { return m_currentState; }
	const WP6ParagraphStyleState getPreviousState() const { return m_previousState; }
private:
	WP6ParagraphStyleState m_currentState;
	WP6ParagraphStyleState m_previousState;

};

class WP6OutlineDefinition
{
 public:
	WP6OutlineDefinition(const WP6OutlineLocation outlineLocation, const guint8 *numberingMethods, 
			  const guint8 tabBehaviourFlag);
	void update(const guint8 *numberingMethods, const guint8 tabBehaviourFlag);

	//void setListID(int level, UT_uint32 id) { m_listIDs[level] = id; }
	//UT_uint32 getListID(int level) { return m_listIDs[level]; }
	//UT_uint16 getListNumber(int level) { return m_currentListNumbers[level]; }
	WPXListType getListType(int level) { return m_listTypes[level]; }
	//bool isLevelNumbered(int level) { return m_bIsNumberedList[level]; }
	//UT_String getListDelim(int level) { return m_listDelim[level]; }
                    
	//void setListRightDelimText(int level, UT_String rightDelimText) { UT_String_sprintf(m_listDelim[level], "%%L%s", rightDelimText.c_str()); }
	//void resetListIDsAndNumbers() { for (unsigned int i=0; i<WP_NUM_LIST_LEVELS; i++) { m_listIDs[i]=0; m_currentListNumbers[i]=0; } }
	//void updateListTypes(UT_uint16 *numberingMethods); // update list types based on information from a wordperfect outlining group
	//void incrementLevelNumber(int level) { m_currentListNumbers[level]++; }
protected:
	void _updateNumberingMethods(const WP6OutlineLocation outlineLocation, const guint8 *numberingMethods);
                 
private:
	
	//UT_uint32 m_listIDs[WP6_NUM_LIST_LEVELS];
	WPXListType m_listTypes[WP6_NUM_LIST_LEVELS];
	//bool m_bIsNumberedList[WP6_NUM_LIST_LEVELS];
	//UT_uint16 m_currentListNumbers[WP6_NUM_LIST_LEVELS];
	//UT_String m_listDelim[WP6_NUM_LIST_LEVELS];
};

class WP6HLListener : public WP6LLListener
{
 public:
	WP6HLListener(WPXHLListenerImpl *listenerImpl);
	~WP6HLListener();

	// for getting low-level messages from the parser
	virtual void startDocument();
	virtual void insertCharacter(guint16 character);
	virtual void insertEOL();
 	virtual void insertBreak(guint8 breakType); 
	virtual void undoChange(const guint8 undoType, const guint16 undoLevel);
	virtual void fontChange(const guint16 matchedFontPointSize, const guint16 fontPID);
	virtual void attributeChange(gboolean isOn, guint8 attribute);
	virtual void justificationChange(guint8 justification);
	virtual void marginChange(guint8 side, guint16 margin);
	virtual void columnChange(guint8 numColumns); 
	virtual void updateOutlineDefinition(const WP6OutlineLocation outlineLocation, const guint16 outlineHash, 
					     const guint8 *numberingMethods, const guint8 tabBehaviourFlag);

	virtual void paragraphNumberOn(const guint16 outlineHash, const guint8 level, const guint8 flag);
	virtual void paragraphNumberOff();
	virtual void displayNumberReferenceGroupOn(const guint8 subGroup, const guint8 level);
	virtual void displayNumberReferenceGroupOff(const guint8 subGroup);
	virtual void styleGroupOn(const guint8 subGroup);	
	virtual void styleGroupOff(const guint8 subGroup);	
	virtual void endDocument();
 
	virtual void startTable();
 	virtual void insertRow();
 	virtual void insertCell(guint8 colSpan, guint8 rowSpan, gboolean boundFromLeft, gboolean boundFromAbove, RGBSColor * cellFgColor, RGBSColor * cellBgColor);
 	virtual void endTable(); 

 protected:
	void _handleLineBreakElementBegin();
	void _flushText();
	void _handleListChange(const guint16 outlineHash);
 private:
	WPXHLListenerImpl * m_listenerImpl;

	GArray *m_textArray;
	GArray *m_textBeforeNumber;
	GArray *m_textBeforeDisplayReference;
	GArray *m_numberText;
	GArray *m_textAfterDisplayReference;
	GArray *m_textAfterNumber;

	guint32 m_textAttributeBits;
	gboolean m_textAttributesChanged;

	gfloat m_currentFontSize;
	GString * m_currentFontName;

	gboolean m_isParagraphColumnBreak;
	gboolean m_isParagraphPageBreak;
	guint8 m_paragraphJustification;
 	gboolean m_paragraphJustificationChanged;
	gboolean m_isParagraphOpened;
	gboolean m_isParagraphClosed;
	guint m_numDeferredParagraphBreaks;
	guint m_numRemovedParagraphBreaks;
	
	gboolean m_isTableOpened;

	gboolean m_sectionAttributesChanged;
	guint m_numColumns;
	gboolean m_isLeftMarginSet;
	gboolean m_isRightMarginSet;
	gfloat m_marginLeft;
	gfloat m_marginRight;
	
	gint32 m_currentRow;
	gint32 m_currentColumn;

	GHashTable *m_outlineDefineHash;
	GQueue *m_listLevelStack;
	guint16 m_currentOutlineHash; // probably should replace Hash with Key in these sorts of cases
	guint8 m_oldListLevel;
	guint8 m_currentListLevel;
	WP6ParagraphStyleStateSequence m_paragraphStyleStateSequence;
	gboolean m_isPutativeListElementHasParagraphNumber;
	gboolean m_isPutativeListElementHasDisplayReferenceNumber;
	//gboolean m_isInListElement; // for handling an absolutely obscene case where the document only has one list element.

	gboolean m_isUndoOn;
};

#endif /* WP6HLLISTENER_H */
