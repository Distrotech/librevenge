#include <gsf/gsf-utils.h>
#include <gsf/gsf-input-stdio.h>
#include <stdio.h>
#include "libwpd.h"
#include "TextListener.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("usage: wpd2text <WordPerfect Document>\n");
		return -1;
	}
	gsf_init();

	GError   *err;
	GsfInput * input = gsf_input_stdio_new (argv[1], &err);
	if (input == NULL) 
		{
			g_return_val_if_fail (err != NULL, 1);
			
			g_warning ("'%s' error: %s", argv[1], err->message);
			g_error_free (err);
			return 1;
		}
	
	WPXLLListener * listener = new TextListener();
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
	
	return 0;
}
