/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */

#include <assert.h>
#include <stdio.h>
#include <boost/scoped_ptr.hpp>
#include <librevenge-stream/librevenge-stream.h>

namespace
{

static unsigned long getLength(librevenge::RVNGInputStream *const input)
{
	assert(input);

	const long pos = input->tell();
	if (0 > pos)
		return 0;

	long end = 0;

	if (0 == input->seek(0, librevenge::RVNG_SEEK_END))
		end = input->tell();
	if ((0 != input->seek(pos, librevenge::RVNG_SEEK_SET)) || (0 > end))
		return 0;

	assert(pos <= end);

	return static_cast<unsigned long>(end);
}

}

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

		boost::scoped_ptr<librevenge::RVNGInputStream> subStream(input.getSubStreamById(i));
		boost::scoped_ptr<librevenge::RVNGInputStream> namedSubStream(input.getSubStreamByName(name));
		bool subStreamIsStructured = false;
		bool namedSubStreamIsStructured = false;
		unsigned long subStreamSize = 0;
		unsigned long namedSubStreamSize = 0;

		if (bool(subStream))
		{
			subStreamIsStructured = subStream->isStructured();
			subStreamSize = getLength(subStream.get());
		}

		if (bool(namedSubStream))
		{
			namedSubStreamIsStructured = namedSubStream->isStructured();
			namedSubStreamSize = getLength(namedSubStream.get());
		}
		(void) namedSubStreamIsStructured;
		(void) namedSubStreamSize;

		if (subStreamIsStructured)
			printf("--> Stream %i: %s\n", i, name);
		else
			printf("--> Stream %i: %s (size: %lu)\n", i, name, subStreamSize);

		if (!subStream)
			printf("Failed to get the substream by id\n");
		if (!namedSubStream)
			printf("Failed to get the substream by name\n");

		assert((bool(subStream) && bool(namedSubStream)) || (!subStream && !namedSubStream));
		assert(subStreamIsStructured == namedSubStreamIsStructured);
		assert(subStreamSize == namedSubStreamSize);
	}
	return 0;
}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
