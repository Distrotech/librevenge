/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002-2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002-2004 Marc Maurer (uwog@uwog.net)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://librevenge.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include <stdio.h>
#include <string.h>
#include <librevenge/librevenge.h>
#include <librevenge-stream/librevenge-stream.h>
#include "TextDocumentGenerator.h"

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
	printf("Usage: rvng2text [OPTION] <WordPerfect Document>\n");
	printf("\n");
	printf("Options:\n");
	printf("--info                Display document metadata instead of the text\n");
	printf("--help                Shows this help message\n");
	printf("--password <password> Try to decrypt password protected document\n");
	printf("--version             Output rvng2text version\n");
	return -1;
}

int printVersion()
{
	printf("rvng2text %s\n", VERSION);
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

	RVNGFileStream input(szInputFile);

	RVNGConfidence confidence = RVNGocument::isFileFormatSupported(&input);
	if (confidence != RVNG_CONFIDENCE_EXCELLENT && confidence != RVNG_CONFIDENCE_SUPPORTED_ENCRYPTION)
	{
		fprintf(stderr, "ERROR: Unsupported file format!\n");
		return 1;
	}

	if (confidence == RVNG_CONFIDENCE_SUPPORTED_ENCRYPTION && !password)
	{
		fprintf(stderr, "ERROR: File is password protected! Use \"--password\" option!\n");
		return 1;
	}

	if (confidence == RVNG_CONFIDENCE_SUPPORTED_ENCRYPTION && password && (RVNG_PASSWORD_MATCH_OK != RVNGocument::verifyPassword(&input, password)))
	{
		fprintf(stderr, "ERROR: The password does not match, or document is not encrypted!\n");
		return 1;
	}

	TextDocumentGenerator documentGenerator(isInfo);
	RVNGResult error = RVNGocument::parse(&input, &documentGenerator, password);

	if (error == RVNG_FILE_ACCESS_ERROR)
		fprintf(stderr, "ERROR: File Exception!\n");
	else if (error == RVNG_PARSE_ERROR)
		fprintf(stderr, "ERROR: Parse Exception!\n");
	else if (error == RVNG_UNSUPPORTED_ENCRYPTION_ERROR)
		fprintf(stderr, "ERROR: File is password protected! (Unsupported encryption)\n");
	else if (error == RVNG_OLE_ERROR)
		fprintf(stderr, "ERROR: File is an OLE document, but does not contain a WordPerfect stream!\n");
	else if (error != RVNG_OK)
		fprintf(stderr, "ERROR: Unknown Error!\n");

	if (error != RVNG_OK)
		return 1;

	return 0;
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
