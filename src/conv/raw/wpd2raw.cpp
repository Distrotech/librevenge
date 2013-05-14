/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002,2004 Marc Maurer (uwog@uwog.net)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include <stdio.h>
#include <libwpd/libwpd.h>
#include <libwpd-stream/libwpd-stream.h>
#include "RawDocumentGenerator.h"
#include <string.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef VERSION
#define VERSION "UNKNOWN VERSION"
#endif

namespace
{

int printUsage()
{
	printf("Usage: wpd2raw [OPTION] <WordPerfect Document>\n");
	printf("\n");
	printf("Options:\n");
	printf("--callgraph           Display the call graph nesting level\n");
	printf("--help                Shows this help message\n");
	printf("--password <password> Try to decrypt password protected document\n");
	printf("--version             Output wpd2raw version \n");
	return -1;
}

int printVersion()
{
	printf("wpd2raw %s\n", VERSION);
	return 0;
}

} // anonymous namespace

int main(int argc, char *argv[])
{
	bool printIndentLevel = false;
	char *file = 0;
	char *password = 0;

	if (argc < 2)
		return printUsage();

	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "--password"))
		{
			if (i < argc - 1)
				password = argv[++i];
		}
		else if (!strncmp(argv[i], "--password=", 11))
			password = &argv[i][11];
		else if (!strcmp(argv[i], "--callgraph"))
			printIndentLevel = true;
		else if (!strcmp(argv[i], "--version"))
			return printVersion();
		else if (!file && strncmp(argv[i], "--", 2))
			file = argv[i];
		else
			return printUsage();
	}

	if (!file)
		return printUsage();

	WPXFileStream input(file);

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

	RawDocumentGenerator documentGenerator(printIndentLevel);
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
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
