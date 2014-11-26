
#include <stdio.h>
#include <librevenge-stream/librevenge-stream.h>

int main(int argc, char *argv[])
{
	if (argc < 2)
		return -1;
	librevenge::RVNGFileStream input(argv[1]);
	printf("Filename: %s\n", argv[1]);
	if (!input.isStructured())
	{
		printf("Not a structured file!\n");
		return 0;
	}
	unsigned ssc = input.subStreamCount();
	printf("subStreamCount %i\n", ssc);
	if (!ssc)
	{
		printf("Funny structured file!\n");
		return 0;
	}
	for (unsigned i = 0; i < ssc; ++i)
	{
		printf("--> Stream %i: %s\n", i, input.subStreamName(i));
	}
	return 0;
}
