#!/bin/sh

# Script to create the BUILDNUMBER used by compile-resource. This script
# needs the script createBuildNumber.pl to be in the same directory.

{ perl ./createBuildNumber.pl \
	src/lib/librevenge-build.stamp \
	src/lib/librevenge-stream-build.stamp \
	src/conv/html/rvng2html-build.stamp \
	src/conv/raw/rvng2raw-build.stamp \
	src/conv/text/rvng2text-build.stamp
#Success
exit 0
}
#Unsucessful
exit 1
