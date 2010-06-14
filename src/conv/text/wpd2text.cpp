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
#include "libwpd-stream.h"
#include "TextDocumentGenerator.h"

namespace {

int printUsage()
{
	printf("Usage: wpd2text [OPTION] <WordPerfect Document>\n");
	printf("\n");
	printf("Options:\n");
	printf("--info                Display document metadata instead of the text\n");
	printf("--help                Shows this help message\n");
	printf("--password <password> Try to decrypt password protected document\n");
	printf("--version             Output wpd2text version\n");
	return -1;
}

int printVersion()
{
	printf("wpd2text %s\n", LIBWPD_VERSION_STRING);
	return 0;
}

} // anonymous namespace

int main(int argc, char *argv[])
{
	if (argc < 2)
		return printUsage();

	char *szInputFile = 0;
	bool isInfo = false;
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
		else if (!strcmp(argv[i], "--info"))
			isInfo = true;
		else if (!strcmp(argv[i], "--version"))
			return printVersion();
		else if (!szInputFile && strncmp(argv[i], "--", 2))
			szInputFile = argv[i];
		else 
			return printUsage();
	}
	
	if (!szInputFile)
		return printUsage();
	
	WPXFileStream input(szInputFile);

	WPDConfidence confidence = WPDocument::isFileFormatSupported(&input);
	if (confidence != WPD_CONFIDENCE_EXCELLENT && confidence != WPD_CONFIDENCE_SUPPORTED_ENCRYPTION)
	{
		fprintf(stderr, "ERROR: Unsupported file format!\n");
		return 1;
	}
	
	if (confidence == WPD_CONFIDENCE_SUPPORTED_ENCRYPTION && !password)
	{
		fprintf(stderr, "ERROR: File is password protected! Use \"--password\" option!\n");
		return 1;
	}
	
	if (confidence == WPD_CONFIDENCE_SUPPORTED_ENCRYPTION && password && (WPD_PASSWORD_MATCH_OK != WPDocument::verifyPassword(&input, password)))
	{
		fprintf(stderr, "ERROR: The password does not match, or document is not encrypted!\n");
		return 1;
	}
	
	TextDocumentGenerator documentGenerator(isInfo);
 	WPDResult error = WPDocument::parse(&input, &documentGenerator, password);

	if (error == WPD_FILE_ACCESS_ERROR)
		fprintf(stderr, "ERROR: File Exception!\n");
	else if (error == WPD_PARSE_ERROR)
		fprintf(stderr, "ERROR: Parse Exception!\n");
	else if (error == WPD_UNSUPPORTED_ENCRYPTION_ERROR)
		fprintf(stderr, "ERROR: File is password protected! (Unsupported encryption)\n");
	else if (error == WPD_OLE_ERROR)
		fprintf(stderr, "ERROR: File is an OLE document, but does not contain a WordPerfect stream!\n");
	else if (error != WPD_OK)
		fprintf(stderr, "ERROR: Unknown Error!\n");

	if (error != WPD_OK)
		return 1;

	return 0;
}
