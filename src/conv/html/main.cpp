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

#include <gsf/gsf-utils.h>
#include <gsf/gsf-input-stdio.h>
#include <stdio.h>
#include "HtmlListenerImpl.h"
#include "GSFStream.h"
#include "WPDocument.h"

int main(int argc, char *argv[])
{
	gsf_init ();
	if (argc < 2)
	{
		printf("Usage: wpd2html <WordPerfect Document>\n");
		return 1;
	}
	
	GError   *err = 0;
	GsfInput * input;
	input = GSF_INPUT(gsf_input_stdio_new (argv[1], &err));
	if (!input) 
	{
		g_return_val_if_fail (err != 0, 1);
		
		g_warning ("'%s' error: %s", argv[1], err->message);
		g_error_free (err);
		return 1;
	}
	
	GSFInputStream *gsfInput = new GSFInputStream(input);

	WPDConfidence confidence = WPDocument::isFileFormatSupported(gsfInput, false);
	if (confidence == WPD_CONFIDENCE_NONE || confidence == WPD_CONFIDENCE_POOR)
	{
		printf("ERROR: Unsupported file format!\n");
		delete gsfInput;
		return 1;
	}

	HtmlListenerImpl listenerImpl;
 	WPDResult error = WPDocument::parse(gsfInput, static_cast<WPXHLListenerImpl *>(&listenerImpl));

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

	delete gsfInput;
	g_object_unref (G_OBJECT (input));
	gsf_shutdown();

	if (error != WPD_OK)
		return 1;

	return 0;
}
