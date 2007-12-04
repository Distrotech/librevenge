/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
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

#ifndef WPXPARSER_H
#define WPXPARSER_H

class WPXDocumentInterface;
class WPXHeader;
class WPXInputStream;
class WPXEncryption;

class WPXParser
{
public:
	WPXParser(WPXInputStream *input, WPXHeader *header, WPXEncryption *encryption);
	virtual ~WPXParser() {}

	virtual void parse(WPXDocumentInterface *documentInterface) = 0;
	virtual void parseSubDocument(WPXDocumentInterface *documentInterface) = 0;

protected:
	WPXHeader * getHeader() { return m_header; }
	WPXInputStream * getInput() { return m_input; }
	WPXEncryption * getEncryption() { return m_encryption; }
	
private:
	WPXParser(const WPXParser&);
	WPXParser& operator=(const WPXParser&);
	WPXInputStream * m_input;
	WPXHeader * m_header;
	WPXEncryption * m_encryption;
};

#endif /* WPXPARSER_H */
