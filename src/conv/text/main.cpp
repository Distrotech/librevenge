/* libwpd
 * Copyright (C) 2002-2003 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002-2003 Marc Maurer (j.m.maurer@student.utwente.nl)
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */
 
#include <gsf/gsf-utils.h>
#include <gsf/gsf-input-stdio.h>
#include <stdio.h>
#include "libwpd.h"
#include "TextListenerImpl.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("usage: wpd2text <WordPerfect Document>\n");
		return -1;
	}
	gsf_init();

	GError   *err;
	GsfInput * input = GSF_INPUT(gsf_input_stdio_new (argv[1], &err));
	if (input == NULL) 
	{
		g_return_val_if_fail (err != NULL, 1);
		
		g_warning ("'%s' error: %s", argv[1], err->message);
		g_error_free (err);
		return 1;
	}

	if (!WPDocument::isFileFormatSupported(input, false))
	{
		printf("ERROR: Unsupported file format!\n");
		return 1;
	}
	
	TextListenerImpl listenerImpl;
 	try 
	{
		WPDocument::parse(input, static_cast<WPXHLListenerImpl *>(&listenerImpl));
	} 
 	catch (FileException)
	{
 	    printf("ERROR: File Exception!\n");
 	    return 1;
	}	
 	catch (ParseException)
	{
 	    printf("ERROR: Parse Exception!\n");
 	    return 1;
	}
	catch (UnsupportedEncryptionException)
	{
 	    printf("ERROR: File is password protected!\n");
 	    return 1;
	}	
 	catch (...)
	{
 	    printf("ERROR: Unknown Error!\n");
 	    return 1;
	}

	gsf_shutdown();
	
	return 0;
}
