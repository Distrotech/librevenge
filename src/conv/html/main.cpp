#include <stdio.h>
#include "libwpd.h"
#include "HtmlListenerImpl.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("USAGE: wpd2html <WordPerfect Document>\n");
		return -1;
	}
	
	FILE * stream = fopen(argv[1], "r");
	if (!stream)
	{
	    printf("ERROR: Unable to open file!\n");
	    return -1;
	}
	
 	WPXLLListener * listener = new WP6HLListener(new HtmlListenerImpl());
 	WPXParser * parser = new WP6Parser(stream, listener);
	try 
	  {
		  parser->parse();
	  } 
	catch (FileException)
	  {
	    printf("ERROR: File Exception!\n");
	    return -1;
	  }
	
	
 	delete parser;
 	delete listener;
	
	return 0;
}
