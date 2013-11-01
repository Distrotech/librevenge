/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2003-2005 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2003 Marc Maurer (uwog@uwog.net)
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

#ifndef RVNGOCUMENT_H
#define RVNGOCUMENT_H

/* The "RVNG_CONFIDENCE_NONE=0" must not be removed for the type detection to work well */
enum RVNGConfidence { RVNG_CONFIDENCE_NONE=0, RVNG_CONFIDENCE_UNSUPPORTED_ENCRYPTION, RVNG_CONFIDENCE_SUPPORTED_ENCRYPTION, RVNG_CONFIDENCE_EXCELLENT };
enum RVNGResult { RVNG_OK, RVNG_FILE_ACCESS_ERROR, RVNG_PARSE_ERROR, RVNG_UNSUPPORTED_ENCRYPTION_ERROR, RVNG_PASSWORD_MISSMATCH_ERROR, RVNG_OLE_ERROR, RVNG_UNKNOWN_ERROR };
enum RVNGPasswordMatch { RVNG_PASSWORD_MATCH_NONE, RVNG_PASSWORD_MATCH_DONTKNOW, RVNG_PASSWORD_MATCH_OK };
enum RVNGFileFormat { RVNG_FILE_FORMAT_WP6, RVNG_FILE_FORMAT_WP5, RVNG_FILE_FORMAT_WP42, RVNG_FILE_FORMAT_WP3, RVNG_FILE_FORMAT_WP1, RVNG_FILE_FORMAT_UNKNOWN };

class RVNGDocumentInterface;
class RVNGInputStream;

/**
This class provides all the functions an application would need to parse
WordPerfect documents.
*/

class RVNGocument
{
public:
	static RVNGConfidence isFileFormatSupported(RVNGInputStream *input);
	static RVNGPasswordMatch verifyPassword(RVNGInputStream *input, const char *password);
	static RVNGResult parse(RVNGInputStream *input, RVNGDocumentInterface *documentInterface, const char *password);
	static RVNGResult parseSubDocument(RVNGInputStream *input, RVNGDocumentInterface *documentInterface, RVNGFileFormat fileFormat);
};

#endif /* RVNGOCUMENT_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
