#include <gsf/gsf-utils.h>
#include <gsf/gsf-input-stdio.h>
#include <stdio.h>
#include "libwpd.h"
#include "RawListener.h"

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

	if (WP6LLParser::getFileType(input) != WP6_DOCUMENT)
	{
		printf("ERROR: Not a WordPerfect 6/7/8/9/10/11 Document\n");
		return 1;
	}
	
	RawListener listener;

	GsfInput *document = NULL;
	WP6Header * header = NULL;
	bool isDocumentOLE = false;

	try 
	  {		
		document =  WP6LLParser::getDocument(input);
		if (document != NULL) {
			isDocumentOLE = true;
		}
		else
			document = input;

		gsf_input_seek(document, 0, G_SEEK_SET);			
		header = WP6LLParser::getHeader(document);
		
		WP6LLParser::parse(document, header, static_cast<WP6LLListener *>(&listener));
	  } 
	catch (FileException)
	  {
	    printf("ERROR: File Exception!\n");
	    return 1;
	  }

	gsf_shutdown();
	if (document != NULL && isDocumentOLE)
		g_object_unref(G_OBJECT(document));
	delete header;
	g_object_unref (G_OBJECT (input));

	return 0;
}
