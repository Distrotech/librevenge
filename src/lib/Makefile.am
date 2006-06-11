if PLATFORM_WIN32
no_undefined = -no-undefined
else
if PLATFORM_DARWIN
no_undefined = -no-undefined
else
no_undefined =
endif
endif

if OS_WIN32
version_info = -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) -avoid-version

install-libtool-import-lib:
	$(INSTALL) .libs/libwpd*-0.8.dll.a $(DESTDIR)$(libdir)

uninstall-libtool-import-lib:
	-rm $(DESTDIR)$(libdir)/libwpd*-0.8.dll.a

else
version_info = -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)
install-libtool-import-lib:
uninstall-libtool-import-lib:
endif

if WITH_LIBWPD_STREAM
target_libwpd_stream = libwpd-stream-0.8.la
else
target_libwpd_stream =
endif

lib_LTLIBRARIES = libwpd-0.8.la $(target_libwpd_stream)
libwpd_0_8_includedir = $(includedir)/libwpd-0.8/libwpd
libwpd_0_8_include_HEADERS = libwpd.h libwpd_types.h WPXStream.h WPXProperty.h WPXPropertyList.h \
WPXString.h WPXPropertyListVector.h WPDocument.h WPXHLListenerImpl.h

INCLUDES = -I$(srcdir)

AM_CXXFLAGS = $(LIBWPD_CXXFLAGS) $(DEBUG_CXXFLAGS)

libwpd_0_8_la_LIBADD  = @LIBWPD_WIN32_RESOURCE@
libwpd_0_8_la_DEPENDENCIES = @LIBWPD_WIN32_RESOURCE@
libwpd_0_8_la_LDFLAGS = $(version_info) -export-dynamic $(no_undefined)
libwpd_0_8_la_SOURCES = \
	libwpd_internal.cpp \
	libwpd_math.cpp \
	WP3AttributeGroup.cpp \
	WP3ContentListener.cpp \
	WP3DefinitionGroup.cpp \
	WP3DisplayGroup.cpp \
	WP3EndOfLinePageGroup.cpp \
	WP3ExtendedCharacterGroup.cpp \
	WP3FileStructure.cpp \
	WP3FixedLengthGroup.cpp \
	WP3FontGroup.cpp \
	WP3FootnoteEndnoteGroup.cpp \
	WP3Header.cpp \
	WP3HeaderFooterGroup.cpp \
	WP3Listener.cpp \
	WP3MiscellaneousGroup.cpp \
	WP3PageFormatGroup.cpp \
	WP3Parser.cpp \
	WP3Part.cpp \
	WP3SingleByteFunction.cpp \
	WP3StylesListener.cpp \
	WP3SubDocument.cpp \
	WP3TablesGroup.cpp \
	WP3UndoGroup.cpp \
	WP3UnsupportedFixedLengthGroup.cpp \
	WP3UnsupportedVariableLengthGroup.cpp \
	WP3VariableLengthGroup.cpp \
	WP42ContentListener.cpp \
	WP42FileStructure.cpp \
	WP42HeaderFooterGroup.cpp \
	WP42Heuristics.cpp \
	WP42Listener.cpp \
	WP42Parser.cpp \
	WP42Part.cpp \
	WP42StylesListener.cpp \
	WP42SubDocument.cpp \
	WP42UnsupportedMultiByteFunctionGroup.cpp \
	WP42MultiByteFunctionGroup.cpp \
	WP5AttributeGroup.cpp \
	WP5ContentListener.cpp \
	WP5DefinitionGroup.cpp \
	WP5ExtendedCharacterGroup.cpp \
	WP5FileStructure.cpp \
	WP5FixedLengthGroup.cpp \
	WP5FontGroup.cpp \
	WP5FontNameStringPoolPacket.cpp \
	WP5FootnoteEndnoteGroup.cpp \
	WP5GeneralPacketData.cpp \
	WP5GeneralPacketIndex.cpp \
	WP5Header.cpp \
	WP5HeaderFooterGroup.cpp \
	WP5Listener.cpp \
	WP5ListFontsUsedPacket.cpp \
	WP5PageFormatGroup.cpp \
	WP5Parser.cpp \
	WP5Part.cpp \
	WP5PrefixData.cpp \
	WP5SingleByteFunction.cpp \
	WP5SpecialHeaderIndex.cpp \
	WP5StylesListener.cpp \
	WP5SubDocument.cpp \
	WP5TableEOLGroup.cpp \
	WP5TableEOPGroup.cpp \
	WP5UnsupportedFixedLengthGroup.cpp \
	WP5UnsupportedVariableLengthGroup.cpp \
	WP5VariableLengthGroup.cpp \
	WP6AttributeGroup.cpp \
	WP6CharacterGroup.cpp \
	WP6ColumnGroup.cpp \
	WP6ContentListener.cpp \
	WP6DefaultInitialFontPacket.cpp \
	WP6DisplayNumberReferenceGroup.cpp \
	WP6EOLGroup.cpp \
	WP6ExtendedCharacterGroup.cpp \
	WP6ExtendedDocumentSummaryPacket.cpp \
	WP6FileStructure.cpp \
	WP6FillStylePacket.cpp \
	WP6FixedLengthGroup.cpp \
	WP6FontDescriptorPacket.cpp \
	WP6FootnoteEndnoteGroup.cpp \
	WP6GeneralTextPacket.cpp \
	WP6Header.cpp \
	WP60Header.cpp \
	WP61Header.cpp \
	WP6HeaderFooterGroup.cpp \
	WP6HighlightGroup.cpp \
	WP6Listener.cpp \
	WP6Parser.cpp \
	WP6OutlineStylePacket.cpp \
	WP6PageGroup.cpp \
	WP6ParagraphGroup.cpp \
	WP6Part.cpp \
	WP6PrefixData.cpp \
	WP6PrefixIndice.cpp \
	WP6PrefixDataPacket.cpp \
	WP6SingleByteFunction.cpp \
	WP6StyleGroup.cpp \
	WP6StylesListener.cpp \
	WP6SubDocument.cpp \
	WP6TabGroup.cpp \
	WP6UndoGroup.cpp \
	WP6UnsupportedFixedLengthGroup.cpp \
	WP6UnsupportedVariableLengthGroup.cpp \
	WP6VariableLengthGroup.cpp \
	WPDocument.cpp \
	WPXContentListener.cpp \
	WPXHeader.cpp \
	WPXListener.cpp \
	WPXMemoryStream.cpp \
	WPXPageSpan.cpp \
	WPXParser.cpp \
	WPXPart.cpp \
	WPXProperty.cpp \
	WPXPropertyList.cpp \
	WPXPropertyListVector.cpp \
	WPXString.cpp \
	WPXStylesListener.cpp \
	WPXSubDocument.cpp \
	WPXTable.cpp \
	libwpd.h \
	libwpd_internal.h \
	libwpd_math.h \
	libwpd_types.h \
	WP3AttributeGroup.h \
	WP3ContentListener.h \
	WP3DefinitionGroup.h \
	WP3DisplayGroup.h \
	WP3EndOfLinePageGroup.h \
	WP3ExtendedCharacterGroup.h \
	WP3FileStructure.h \
	WP3FixedLengthGroup.h \
	WP3FontGroup.h \
	WP3FootnoteEndnoteGroup.h \
	WP3Header.h \
	WP3HeaderFooterGroup.h \
	WP3Listener.h \
	WP3MiscellaneousGroup.h \
	WP3PageFormatGroup.h \
	WP3Parser.h \
	WP3Part.h \
	WP3SingleByteFunction.h \
	WP3StylesListener.h \
	WP3SubDocument.h \
	WP3TablesGroup.h \
	WP3UndoGroup.h \
	WP3UnsupportedFixedLengthGroup.h \
	WP3UnsupportedVariableLengthGroup.h \
	WP3VariableLengthGroup.h \
	WP42ContentListener.h \
	WP42FileStructure.h \
	WP42HeaderFooterGroup.h \
	WP42Heuristics.h \
	WP42Listener.h \
	WP42Part.h \
	WP42Parser.h \
	WP42StylesListener.h \
	WP42SubDocument.h \
	WP42UnsupportedMultiByteFunctionGroup.h \
	WP42MultiByteFunctionGroup.h \
	WP5AttributeGroup.h \
	WP5ContentListener.h \
	WP5DefinitionGroup.h \
	WP5ExtendedCharacterGroup.h \
	WP5FileStructure.h \
	WP5FixedLengthGroup.h \
	WP5FontGroup.h \
	WP5FontNameStringPoolPacket.h \
	WP5FootnoteEndnoteGroup.h \
	WP5GeneralPacketData.h \
	WP5GeneralPacketIndex.h \
	WP5Header.h \
	WP5HeaderFooterGroup.h \
	WP5Listener.h \
	WP5ListFontsUsedPacket.h \
	WP5PageFormatGroup.h \
	WP5Part.h \
	WP5Parser.h \
	WP5PrefixData.h \
	WP5SingleByteFunction.h \
	WP5SpecialHeaderIndex.h \
	WP5StylesListener.h \
	WP5SubDocument.h \
	WP5TableEOLGroup.h \
	WP5TableEOPGroup.h \
	WP5UnsupportedFixedLengthGroup.h \
	WP5UnsupportedVariableLengthGroup.h \
	WP5VariableLengthGroup.h \
	WP6FileStructure.h \
	WP6AttributeGroup.h \
	WP6CharacterGroup.h \
	WP6ColumnGroup.h \
	WP6ContentListener.h \
	WP6DefaultInitialFontPacket.h \
	WP6DisplayNumberReferenceGroup.h \
	WP6EOLGroup.h \
	WP6ExtendedCharacterGroup.h \
	WP6ExtendedDocumentSummaryPacket.h \
	WP6FillStylePacket.h \
	WP6FixedLengthGroup.h \
	WP6FontDescriptorPacket.h \
	WP6FootnoteEndnoteGroup.h \
	WP6GeneralTextPacket.h \
	WP6Header.h \
	WP60Header.h \
	WP61Header.h \
	WP6HeaderFooterGroup.h \
	WP6HighlightGroup.h \
	WP6Listener.h \
	WP6OutlineStylePacket.h \
	WP6PageGroup.h \
	WP6ParagraphGroup.h \
	WP6Parser.h \
	WP6Part.h \
	WP6PrefixData.h	 \
	WP6PrefixIndice.h \
	WP6PrefixDataPacket.h \
	WP6SingleByteFunction.h \
	WP6StyleGroup.h \
	WP6StylesListener.h \
	WP6SubDocument.h \
	WP6TabGroup.h	 \
	WP6TibetanMap.h \
	WP6UndoGroup.h \
	WP6UnsupportedFixedLengthGroup.h \
	WP6UnsupportedVariableLengthGroup.h \
	WP6VariableLengthGroup.h \
	WPDocument.h \
	WPXContentListener.h \
	WPXHeader.h \
	WPXFileStructure.h \
	WPXHLListenerImpl.h \
	WPXListener.h \
	WPXMemoryStream.h \
	WPXPageSpan.h \
	WPXParser.h \
	WPXPart.h \
	WPXProperty.h \
	WPXPropertyList.h \
	WPXPropertyListVector.h \
	WPXString.h \
	WPXStylesListener.h \
	WPXSubDocument.h \
	WPXTable.h

libwpd_stream_headers = GSFStream.h
libwpd_stream_sources = GSFStream.cpp

if WITH_LIBWPD_STREAM

libwpd_stream_0_8_includedir = $(includedir)/libwpd-0.8/libwpd
libwpd_stream_0_8_include_HEADERS = $(libwpd_stream_headers)

libwpd_stream_0_8_la_LIBADD  = $(LIBWPD_LIBS) @LIBWPD_STREAM_WIN32_RESOURCE@
libwpd_stream_0_8_la_DEPENDENCIES = @LIBWPD_STREAM_WIN32_RESOURCE@
libwpd_stream_0_8_la_LDFLAGS = $(version_info) -export-dynamic $(no_undefined)
libwpd_stream_0_8_la_SOURCES = $(libwpd_stream_sources)

endif

if OS_WIN32

@LIBWPD_WIN32_RESOURCE@ : libwpd.rc
	$(top_srcdir)/build/win32/lt-compile-resource libwpd.rc @LIBWPD_WIN32_RESOURCE@

@LIBWPD_STREAM_WIN32_RESOURCE@ : libwpd-stream.rc
	$(top_srcdir)/build/win32/lt-compile-resource libwpd-stream.rc @LIBWPD_STREAM_WIN32_RESOURCE@
endif


install-data-local: install-libtool-import-lib

uninstall-local: uninstall-libtool-import-lib

# Include the GSFStream.* in case we build a tarball without stream
EXTRA_DIST = \
	$(libwpd_stream_sources) \
	$(libwpd_stream_headers) \
	makefile.mk \
	libwpd.rc.in \
	libwpd-stream.rc.in

# These may be in the builddir too
BUILD_EXTRA_DIST = \
	libwpd.rc \
	libwpd-stream.rc
