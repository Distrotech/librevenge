/* libwpd
 * Copyright (C) 2002-2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002-2004 Marc Maurer (uwog@uwog.net)
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
 
#include <stdio.h>
#include <string.h>
#include "libwpd.h"
#include "WPXStreamImplementation.h"
#include "TextListenerImpl.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Usage: wpd2text [--info] <WordPerfect Document>\n");
		printf("Use \"--info\" to get document metadata instead\n");
		printf("of the document itself\n");
		return -1;
	}

        char *szInputFile;
	bool isInfo;

        if (!strcmp(argv[1], "--info"))
	{
                isInfo = true;
		szInputFile = argv[2];
	}		
	else
	{
		isInfo = false;
                szInputFile = argv[1];
	}

	WPXInputStream* input = new WPXFileStream(szInputFile);

	WPDConfidence confidence = WPDocument::isFileFormatSupported(input, false);
	if (confidence == WPD_CONFIDENCE_NONE || confidence == WPD_CONFIDENCE_POOR)
	{
		printf("ERROR: Unsupported file format!\n");
		delete input;
		return 1;
	}
	
	TextListenerImpl listenerImpl(isInfo);
 	WPDResult error = WPDocument::parse(input, static_cast<WPXDocumentInterface *>(&listenerImpl));

	if (error == WPD_FILE_ACCESS_ERROR)
		fprintf(stderr, "ERROR: File Exception!\n");
	else if (error == WPD_PARSE_ERROR)
		fprintf(stderr, "ERROR: Parse Exception!\n");
	else if (error == WPD_UNSUPPORTED_ENCRYPTION_ERROR)
		fprintf(stderr, "ERROR: File is password protected!\n");
	else if (error == WPD_OLE_ERROR)
		fprintf(stderr, "ERROR: File is an OLE document, but does not contain a WordPerfect stream!\n");
	else if (error != WPD_OK)
		fprintf(stderr, "ERROR: Unknown Error!\n");

	delete input;

	if (error != WPD_OK)
		return 1;

	return 0;
}
