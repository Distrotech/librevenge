#include <stdio.h>
#include "libwpd2/libwpd2.h"
#include "TextListener.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("usage: wpd2text <WordPerfect Document>\n");
		return -1;
	}
	
	FILE * stream = fopen(argv[1], "r");
	if (!stream)
	{
	    printf("Unable to open file!\n");
	    return -1;
	}
	
	TextListener * listener = new TextListener();
	WPXParser * parser = WPXParser::constructParser(stream);
	parser->registerListener(listener);
	parser->parse();
	delete parser;
	delete listener;
	
	return 0;
}
