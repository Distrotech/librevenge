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
	
	WPXHLListenerImpl * listenerImpl = new HtmlListenerImpl();
 	WPXLLListener * listener = new WP6HLListener(listenerImpl);
 	WPXParser * parser = new WP6Parser(input, listener);
	try 
	  {
		  parser->parse();
	  } 
	catch (FileException)
	  {
	    printf("ERROR: File Exception!\n");
	    return 1;
	  }
	
	
	gsf_shutdown();
 	delete parser;
 	delete listener;
	delete listenerImpl;
	g_object_unref (G_OBJECT (input));
	
	return 0;
}
