#include <stdio.h>
#include "libwpd2/libwpd2.h"
#include "HtmlListener.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("usage: wpd2html <WordPerfect Document>\n");
		return -1;
	}
	
	FILE * stream = fopen(argv[1], "r");
	if (!stream)
	{
	    printf("Unable to open file!\n");
	    return -1;
	}
	
	HtmlListener * listener = new HtmlListener();
	WPXParser * parser = WPXParser::constructParser(stream);
	parser->registerListener(listener);
	parser->parse();
	delete parser;
	delete listener;
	
	return 0;
}