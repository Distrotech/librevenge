#include <gsf/gsf-utils.h>
#include <gsf/gsf-input-stdio.h>
#include <stdio.h>
#include "libwpd.h"
#include "HtmlListenerImpl.h"

int main(int argc, char *argv[])
{
	gsf_init ();
	if (argc < 2)
	{
		printf("USAGE: wpd2html <WordPerfect Document>\n");
		return 1;
	}
	
	GError   *err;
	GsfInput * input;
	input = GSF_INPUT(gsf_input_stdio_new (argv[1], &err));
	if (input == NULL) 
		{
			g_return_val_if_fail (err != NULL, 1);
			
			g_warning ("'%s' error: %s", argv[1], err->message);
			g_error_free (err);
			return 1;
		}

	if (WP6LLParser::getFileType(input) != WP6_DOCUMENT)
	{
		printf("Not a WordPerfect 6/7/8/9/10/11 Document\n");
		return 1;
	}
	HtmlListenerImpl listenerImpl;
 	try 
 	  {
		  WP6HLParser::parse(input, static_cast<WPXHLListenerImpl *>(&listenerImpl));
 	  } 
 	catch (FileException)
 	  {
 	    printf("ERROR: File Exception!\n");
 	    return 1;
 	  }
	
	
	gsf_shutdown();
	g_object_unref (G_OBJECT (input));

	return 0;
}
