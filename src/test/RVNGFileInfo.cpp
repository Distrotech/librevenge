/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */

#include <assert.h>
#include <stdio.h>
#include <boost/scoped_ptr.hpp>
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
		const char *const name = input.subStreamName(i);
		assert(input.existsSubStream(name));
		printf("--> Stream %i: %s\n", i, name);

		boost::scoped_ptr<librevenge::RVNGInputStream> subStream(input.getSubStreamById(i));
		if (!subStream)
			printf("Failed to get the substream by id\n");

		boost::scoped_ptr<librevenge::RVNGInputStream> namedSubStream(input.getSubStreamByName(name));
		if (!namedSubStream)
			printf("Failed to get the substream by name\n");

		assert((bool(subStream) && bool(namedSubStream)) || (!subStream && !namedSubStream));
	}
	return 0;
}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
