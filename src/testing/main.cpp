#include <stdio.h>
#include "libwpd2/libwpd2.h"
#include "TestListener.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("usage: libwpd2test <WordPerfect Document>\n");
		return -1;
	}
	
	FILE * stream = fopen(argv[1], "r");
	
	TestListener * listener = new TestListener();
	WPXParser * parser = WPXParser::constructParser(stream);
	parser->registerListener(listener);
	parser->parse();
	delete parser;
	delete listener;
	
	return 0;
}
