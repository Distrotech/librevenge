 /* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002-2003 Marc Maurer (uwog@uwog.net)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
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

#ifndef TEXTLISTENERIMPL_H
#define TEXTLISTENERIMPL_H

#include "WPXDocumentInterface.h"

class TextListenerImpl : public WPXDocumentInterface
{
public:
	TextListenerImpl(const bool isInfo=false);
	virtual ~TextListenerImpl();

 	virtual void setDocumentMetaData(const WPXPropertyList &propList);

	virtual void startDocument() {}
	virtual void endDocument() {}

	virtual void openPageSpan(const WPXPropertyList & /* propList */) {}
	virtual void closePageSpan() {}
	virtual void openHeader(const WPXPropertyList & /* propList */) {}
	virtual void closeHeader() {}
	virtual void openFooter(const WPXPropertyList & /* propList */) {}
	virtual void closeFooter() {}

	virtual void openSection(const WPXPropertyList & /* propList */, const WPXPropertyListVector & /* columns */) {}
	virtual void closeSection() {}
	virtual void openParagraph(const WPXPropertyList & /* propList */, const WPXPropertyListVector & /* tabStops */) {}
	virtual void closeParagraph();
	virtual void openSpan(const WPXPropertyList & /* propList */) {}
	virtual void closeSpan() {}

	virtual void insertTab();
	virtual void insertText(const WPXString &text);
	virtual void insertLineBreak();

	virtual void defineOrderedListLevel(const WPXPropertyList & /* propList */) {}
	virtual void defineUnorderedListLevel(const WPXPropertyList & /* propList */) {}
	virtual void openOrderedListLevel(const WPXPropertyList & /* propList */) {}
	virtual void openUnorderedListLevel(const WPXPropertyList & /* propList */) {}
	virtual void closeOrderedListLevel() {}
	virtual void closeUnorderedListLevel() {}
	virtual void openListElement(const WPXPropertyList & /* propList */, const WPXPropertyListVector & /* tabStops */) {}
	virtual void closeListElement() {}

	virtual void openFootnote(const WPXPropertyList & /* propList */) {}
	virtual void closeFootnote() {}
	virtual void openEndnote(const WPXPropertyList & /* propList */) {}
	virtual void closeEndnote() {}

	virtual void openTable(const WPXPropertyList & /* propList */, const WPXPropertyListVector & /* columns */) {}
	virtual void openTableRow(const WPXPropertyList & /* propList */) {}
	virtual void closeTableRow() {}
	virtual void openTableCell(const WPXPropertyList & /* propList */) {}
	virtual void closeTableCell() {}
	virtual void insertCoveredTableCell(const WPXPropertyList & /* propList */) {}
	virtual void closeTable() {}

	virtual void openBox(const WPXPropertyList & /* propList */) {}
	virtual void closeBox() {}
	
	virtual void insertBinaryObject(const WPXPropertyList & /* propList */, const WPXBinaryData * /* object */) {}

private:
	unsigned int m_currentListLevel;
	bool m_isInfo;
};

#endif /* TEXTLISTENERIMPL_H */
