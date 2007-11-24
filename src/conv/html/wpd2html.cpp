/* libwpd
 * Copyright (C) 2002-2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002-2004 Marc Maurer (uwog@uwog.net)
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

#include <stdio.h>
#include "HtmlDocumentGenerator.h"
#include "WPXStreamImplementation.h"
#include "WPDocument.h"
#include <string.h>

int printUsage()
{
	printf("Usage: wpd2html [OPTION] <WordPerfect Document>\n");
	printf("\n");
	printf("Options:\n");
	printf("--help                Shows this help message\n");
	printf("--password <password> Try to decrypt password protected document\n");
	return -1;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
		return printUsage();
	
	char *file = 0;
	char *password = 0;
	
	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "--password"))
		{
		    if (i < argc - 1)
				password = argv[++i];
		}
		else if (!strncmp(argv[i], "--password=", 11))
			password = &argv[i][11];
		else if (!file && strncmp(argv[i], "--", 2))
			file = argv[i];
		else
			return printUsage();
	}

	if (!file)
		return printUsage();
		
	WPXInputStream* input = new WPXFileStream(file);

	WPDConfidence confidence = WPDocument::isFileFormatSupported(input, password);
	if (confidence != WPD_CONFIDENCE_EXCELLENT)
	{
		printf("ERROR: Unsupported file format!\n");
		delete input;
		return 1;
	}

	HtmlDocumentGenerator documentGenerator;
 	WPDResult error = WPDocument::parse(input, &documentGenerator, password);

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
