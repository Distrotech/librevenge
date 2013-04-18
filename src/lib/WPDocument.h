/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
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
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WPDOCUMENT_H
#define WPDOCUMENT_H

/* The "WPD_CONFIDENCE_NONE=0" must not be removed for the type detection to work well */
enum WPDConfidence { WPD_CONFIDENCE_NONE=0, WPD_CONFIDENCE_UNSUPPORTED_ENCRYPTION, WPD_CONFIDENCE_SUPPORTED_ENCRYPTION, WPD_CONFIDENCE_EXCELLENT };
enum WPDResult { WPD_OK, WPD_FILE_ACCESS_ERROR, WPD_PARSE_ERROR, WPD_UNSUPPORTED_ENCRYPTION_ERROR, WPD_PASSWORD_MISSMATCH_ERROR, WPD_OLE_ERROR, WPD_UNKNOWN_ERROR };
enum WPDPasswordMatch { WPD_PASSWORD_MATCH_NONE, WPD_PASSWORD_MATCH_DONTKNOW, WPD_PASSWORD_MATCH_OK };
enum WPDFileFormat { WPD_FILE_FORMAT_WP6, WPD_FILE_FORMAT_WP5, WPD_FILE_FORMAT_WP42, WPD_FILE_FORMAT_WP3, WPD_FILE_FORMAT_WP1, WPD_FILE_FORMAT_UNKNOWN };

class WPXDocumentInterface;
class WPXInputStream;

/**
This class provides all the functions an application would need to parse
WordPerfect documents.
*/

class WPDocument
{
public:
	static WPDConfidence isFileFormatSupported(WPXInputStream *input);
	static WPDPasswordMatch verifyPassword(WPXInputStream *input, const char *password);
	static WPDResult parse(WPXInputStream *input, WPXDocumentInterface *documentInterface, const char *password);
	static WPDResult parseSubDocument(WPXInputStream *input, WPXDocumentInterface *documentInterface, WPDFileFormat fileFormat);
};

#endif /* WPDOCUMENT_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
