#include "WP6TableListener.h"
#include "WPXTable.h"
#include "WP6FileStructure.h"

// WP6TableListener: creates an intermediate table representation, given a
// sequence of messages passed to it by the parser.

void WP6TableListener::undoChange(const guint8 undoType, const guint16 undoLevel)
{
	if (undoType == WP6_UNDO_GROUP_INVALID_TEXT_START)
		m_isUndoOn = true;
	else if (undoType == WP6_UNDO_GROUP_INVALID_TEXT_END)
		m_isUndoOn = false;		
}

void WP6TableListener::insertRow()
{
	if (!m_isUndoOn)
		m_table->insertRow();
}

void WP6TableListener::insertCell(const guint8 colSpan, const guint8 rowSpan, const bool boundFromLeft, const bool boundFromAbove, 
				  const guint8 borderBits, 
				  const RGBSColor * cellFgColor, const RGBSColor * cellBgColor)
{
	if (!m_isUndoOn)
		m_table->insertCell(colSpan, rowSpan, boundFromLeft, boundFromAbove, borderBits);
}
