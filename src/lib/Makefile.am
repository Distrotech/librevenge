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
else
version_info = -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)
endif

if DEF_FILES
export_symbols_librevenge = -export-symbols librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.def
export_symbols_librevenge_stream = -export-symbols librevenge-stream-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.def
else
export_symbols_librevenge =
export_symbols_librevenge_stream =
endif

if WITH_LIBREVENGE_STREAM
target_librevenge_stream = librevenge-stream-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la
else
target_librevenge_stream =
endif

lib_LTLIBRARIES = librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la $(target_librevenge_stream)
librevenge_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_includedir = $(includedir)/librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@/librevenge
librevenge_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_include_HEADERS = \
	$(top_srcdir)/inc/librevenge/librevenge.h \
	$(top_srcdir)/inc/librevenge/RVNGocument.h \
	$(top_srcdir)/inc/librevenge/RVNGBinaryData.h \
	$(top_srcdir)/inc/librevenge/RVNGDocumentInterface.h \
	$(top_srcdir)/inc/librevenge/RVNGProperty.h \
	$(top_srcdir)/inc/librevenge/RVNGPropertyList.h \
	$(top_srcdir)/inc/librevenge/RVNGPropertyListVector.h \
	$(top_srcdir)/inc/librevenge/RVNGString.h

AM_CXXFLAGS = -I$(top_srcdir)/inc $(DEBUG_CXXFLAGS)

librevenge_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_LIBADD  = @LIBREVENGE_WIN32_RESOURCE@
librevenge_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_DEPENDENCIES = @LIBREVENGE_WIN32_RESOURCE@
librevenge_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_LDFLAGS = $(version_info) -export-dynamic $(no_undefined) $(export_symbols_librevenge)
librevenge_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_SOURCES = \
	librevenge_internal.cpp \
	librevenge_math.cpp \
	WP1BottomMarginGroup.cpp \
	WP1CenterTextGroup.cpp \
	WP1ContentListener.cpp \
	WP1ExtendedCharacterGroup.cpp \
	WP1FileStructure.cpp \
	WP1FixedLengthGroup.cpp \
	WP1FlushRightGroup.cpp \
	WP1FontIdGroup.cpp \
	WP1FootnoteEndnoteGroup.cpp \
	WP1HeaderFooterGroup.cpp \
	WP1Heuristics.cpp \
	WP1JustificationGroup.cpp \
	WP1LeftIndentGroup.cpp \
	WP1LeftRightIndentGroup.cpp \
	WP1Listener.cpp \
	WP1MarginReleaseGroup.cpp \
	WP1MarginResetGroup.cpp \
	WP1Parser.cpp \
	WP1Part.cpp \
	WP1PictureGroup.cpp \
	WP1PointSizeGroup.cpp \
	WP1SetTabsGroup.cpp \
	WP1SpacingResetGroup.cpp \
	WP1StylesListener.cpp \
	WP1SubDocument.cpp \
	WP1SuppressPageCharacteristicsGroup.cpp \
	WP1TopMarginGroup.cpp \
	WP1UnsupportedFixedLengthGroup.cpp \
	WP1UnsupportedVariableLengthGroup.cpp \
	WP1VariableLengthGroup.cpp \
	WP3AttributeGroup.cpp \
	WP3ContentListener.cpp \
	WP3DefinitionGroup.cpp \
	WP3DisplayGroup.cpp \
	WP3DoubleByteScriptCharacterGroup.cpp \
	WP3EndOfLinePageGroup.cpp \
	WP3ExtendedCharacterGroup.cpp \
	WP3FileStructure.cpp \
	WP3FixedLengthGroup.cpp \
	WP3FontGroup.cpp \
	WP3FootnoteEndnoteGroup.cpp \
	WP3Header.cpp \
	WP3HeaderFooterGroup.cpp \
	WP3IndentGroup.cpp \
	WP3Listener.cpp \
	WP3MiscellaneousGroup.cpp \
	WP3PageFormatGroup.cpp \
	WP3Parser.cpp \
	WP3Part.cpp \
	WP3ResourceFork.cpp \
	WP3Resource.cpp \
	WP3SingleByteFunction.cpp \
	WP3StylesListener.cpp \
	WP3SubDocument.cpp \
	WP3TabGroup.cpp \
	WP3TablesGroup.cpp \
	WP3UndoGroup.cpp \
	WP3UnsupportedFixedLengthGroup.cpp \
	WP3UnsupportedVariableLengthGroup.cpp \
	WP3VariableLengthGroup.cpp \
	WP3WindowGroup.cpp \
	WP42ContentListener.cpp \
	WP42DefineColumnsGroup.cpp \
	WP42ExtendedCharacterGroup.cpp \
	WP42FileStructure.cpp \
	WP42HeaderFooterGroup.cpp \
	WP42Heuristics.cpp \
	WP42Listener.cpp \
	WP42MarginResetGroup.cpp \
	WP42MultiByteFunctionGroup.cpp \
	WP42Parser.cpp \
	WP42Part.cpp \
	WP42StylesListener.cpp \
	WP42SubDocument.cpp \
	WP42SuppressPageCharacteristicsGroup.cpp \
	WP42UnsupportedMultiByteFunctionGroup.cpp \
	WP5AttributeGroup.cpp \
	WP5BoxGroup.cpp \
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
	WP5GraphicsInformationPacket.cpp \
	WP5Header.cpp \
	WP5HeaderFooterGroup.cpp \
	WP5IndentGroup.cpp \
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
	WP5TabGroup.cpp \
	WP5TableEOLGroup.cpp \
	WP5TableEOPGroup.cpp \
	WP5UnsupportedFixedLengthGroup.cpp \
	WP5UnsupportedVariableLengthGroup.cpp \
	WP5VariableLengthGroup.cpp \
	WP6AttributeGroup.cpp \
	WP6BoxGroup.cpp \
	WP6CharacterGroup.cpp \
	WP6ColumnGroup.cpp \
	WP6CommentAnnotationPacket.cpp \
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
	WP6GraphicsBoxStylePacket.cpp \
	WP6GraphicsCachedFileDataPacket.cpp \
	WP6GraphicsFilenamePacket.cpp \
	WP6Header.cpp \
	WP60Header.cpp \
	WP61Header.cpp \
	WP6HeaderFooterGroup.cpp \
	WP6HighlightGroup.cpp \
	WP6Listener.cpp \
	WP6NumberingMethodGroup.cpp \
	WP6Parser.cpp \
	WP6OutlineStylePacket.cpp \
	WP6PageGroup.cpp \
	WP6ParagraphGroup.cpp \
	WP6Part.cpp \
	WP6PrefixData.cpp \
	WP6PrefixIndice.cpp \
	WP6PrefixDataPacket.cpp \
	WP6SetNumberGroup.cpp \
	WP6SingleByteFunction.cpp \
	WP6StyleGroup.cpp \
	WP6StylesListener.cpp \
	WP6SubDocument.cpp \
	WP6TabGroup.cpp \
	WP6TableStylePacket.cpp \
	WP6UndoGroup.cpp \
	WP6UnsupportedFixedLengthGroup.cpp \
	WP6UnsupportedVariableLengthGroup.cpp \
	WP6VariableLengthGroup.cpp \
	RVNGocument.cpp \
	RVNGBinaryData.cpp \
	RVNGContentListener.cpp \
	RVNGEncryption.cpp \
	RVNGHeader.cpp \
	RVNGListener.cpp \
	RVNGMemoryStream.cpp \
	RVNGPageSpan.cpp \
	RVNGParser.cpp \
	RVNGProperty.cpp \
	RVNGPropertyList.cpp \
	RVNGPropertyListVector.cpp \
	RVNGString.cpp \
	RVNGStylesListener.cpp \
	RVNGSubDocument.cpp \
	RVNGTable.cpp \
	librevenge_internal.h \
	librevenge_math.h \
	librevenge_types.h \
	WP1BottomMarginGroup.h \
	WP1CenterTextGroup.h \
	WP1ContentListener.h \
	WP1ExtendedCharacterGroup.h \
	WP1FileStructure.h \
	WP1FixedLengthGroup.h \
	WP1FlushRightGroup.h \
	WP1FontIdGroup.h \
	WP1FootnoteEndnoteGroup.h \
	WP1HeaderFooterGroup.h \
	WP1Heuristics.h \
	WP1JustificationGroup.h \
	WP1LeftIndentGroup.h \
	WP1LeftRightIndentGroup.h \
	WP1Listener.h \
	WP1MarginReleaseGroup.h \
	WP1MarginResetGroup.h \
	WP1Parser.h \
	WP1Part.h \
	WP1PictureGroup.h \
	WP1PointSizeGroup.h \
	WP1SetTabsGroup.h \
	WP1SpacingResetGroup.h \
	WP1StylesListener.h \
	WP1SubDocument.h \
	WP1SuppressPageCharacteristicsGroup.h \
	WP1TopMarginGroup.h \
	WP1UnsupportedFixedLengthGroup.h \
	WP1UnsupportedVariableLengthGroup.h \
	WP1VariableLengthGroup.h \
	WP3AttributeGroup.h \
	WP3ContentListener.h \
	WP3DefinitionGroup.h \
	WP3DisplayGroup.h \
	WP3DoubleByteScriptCharacterGroup.h \
	WP3EndOfLinePageGroup.h \
	WP3ExtendedCharacterGroup.h \
	WP3FileStructure.h \
	WP3FixedLengthGroup.h \
	WP3FontGroup.h \
	WP3FootnoteEndnoteGroup.h \
	WP3Header.h \
	WP3HeaderFooterGroup.h \
	WP3IndentGroup.h \
	WP3Listener.h \
	WP3MiscellaneousGroup.h \
	WP3PageFormatGroup.h \
	WP3Parser.h \
	WP3Part.h \
	WP3ResourceFork.h \
	WP3Resource.h \
	WP3SingleByteFunction.h \
	WP3StylesListener.h \
	WP3SubDocument.h \
	WP3TabGroup.h \
	WP3TablesGroup.h \
	WP3UndoGroup.h \
	WP3UnsupportedFixedLengthGroup.h \
	WP3UnsupportedVariableLengthGroup.h \
	WP3VariableLengthGroup.h \
	WP3WindowGroup.h \
	WP42ContentListener.h \
	WP42DefineColumnsGroup.h \
	WP42ExtendedCharacterGroup.h \
	WP42FileStructure.h \
	WP42HeaderFooterGroup.h \
	WP42Heuristics.h \
	WP42Listener.h \
	WP42MarginResetGroup.h \
	WP42MultiByteFunctionGroup.h \
	WP42Part.h \
	WP42Parser.h \
	WP42StylesListener.h \
	WP42SubDocument.h \
	WP42SuppressPageCharacteristicsGroup.h \
	WP42UnsupportedMultiByteFunctionGroup.h \
	WP5AttributeGroup.h \
	WP5BoxGroup.h \
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
	WP5GraphicsInformationPacket.h \
	WP5Header.h \
	WP5HeaderFooterGroup.h \
	WP5IndentGroup.h \
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
	WP5TabGroup.h \
	WP5TableEOLGroup.h \
	WP5TableEOPGroup.h \
	WP5UnsupportedFixedLengthGroup.h \
	WP5UnsupportedVariableLengthGroup.h \
	WP5VariableLengthGroup.h \
	WP6FileStructure.h \
	WP6AttributeGroup.h \
	WP6BoxGroup.h \
	WP6CharacterGroup.h \
	WP6ColumnGroup.h \
	WP6CommentAnnotationPacket.h \
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
	WP6GraphicsBoxStylePacket.h \
	WP6GraphicsCachedFileDataPacket.h \
	WP6GraphicsFilenamePacket.h \
	WP6Header.h \
	WP60Header.h \
	WP61Header.h \
	WP6HeaderFooterGroup.h \
	WP6HighlightGroup.h \
	WP6Listener.h \
	WP6NumberingMethodGroup.h \
	WP6OutlineStylePacket.h \
	WP6PageGroup.h \
	WP6ParagraphGroup.h \
	WP6Parser.h \
	WP6Part.h \
	WP6PrefixData.h	 \
	WP6PrefixIndice.h \
	WP6PrefixDataPacket.h \
	WP6SetNumberGroup.h \
	WP6SingleByteFunction.h \
	WP6StyleGroup.h \
	WP6StylesListener.h \
	WP6SubDocument.h \
	WP6TabGroup.h \
	WP6TableStylePacket.h \
	WP6TibetanMap.h \
	WP6UndoGroup.h \
	WP6UnsupportedFixedLengthGroup.h \
	WP6UnsupportedVariableLengthGroup.h \
	WP6VariableLengthGroup.h \
	RVNGContentListener.h \
	RVNGEncryption.h \
	RVNGHeader.h \
	RVNGFileStructure.h \
	RVNGListener.h \
	RVNGMemoryStream.h \
	RVNGPageSpan.h \
	RVNGParser.h \
	RVNGStylesListener.h \
	RVNGSubDocument.h \
	RVNGTable.h

librevenge_stream_headers = \
	$(top_srcdir)/inc/librevenge-stream/librevenge-stream.h \
	$(top_srcdir)/inc/librevenge-stream/RVNGStream.h \
	$(top_srcdir)/inc/librevenge-stream/RVNGStreamImplementation.h

librevenge_stream_sources = \
	RVNGStreamImplementation.cpp \
	RVNGOLEStream.cpp \
	RVNGZipStream.cpp \
	RVNGOLEStream.h \
	RVNGZipStream.h

if WITH_LIBREVENGE_STREAM

librevenge_stream_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_includedir = $(includedir)/librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@/librevenge-stream
librevenge_stream_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_include_HEADERS = $(librevenge_stream_headers)

librevenge_stream_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_LIBADD  = $(ZLIB_LIBS) @LIBREVENGE_STREAM_WIN32_RESOURCE@
librevenge_stream_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_DEPENDENCIES = @LIBREVENGE_STREAM_WIN32_RESOURCE@
librevenge_stream_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_LDFLAGS = $(version_info) -export-dynamic $(no_undefined) $(export_symbols_librevenge_stream)
librevenge_stream_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_SOURCES = $(librevenge_stream_sources)

endif

if OS_WIN32

@LIBREVENGE_WIN32_RESOURCE@ : librevenge.rc $(librevenge_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_OBJECTS)
	chmod +x $(top_srcdir)/build/win32/*compile-resource
	WINDRES=@WINDRES@ $(top_srcdir)/build/win32/lt-compile-resource librevenge.rc @LIBREVENGE_WIN32_RESOURCE@

@LIBREVENGE_STREAM_WIN32_RESOURCE@ : librevenge-stream.rc $(librevenge_stream_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_OBJECTS)
	chmod +x $(top_srcdir)/build/win32/*compile-resource && \
	WINDRES=@WINDRES@ $(top_srcdir)/build/win32/lt-compile-resource librevenge-stream.rc @LIBREVENGE_STREAM_WIN32_RESOURCE@
endif


# Include the GSFStream.* in case we build a tarball without stream
EXTRA_DIST = \
	$(librevenge_stream_sources) \
	$(librevenge_stream_headers) \
	makefile.mk \
	librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.def \
	librevenge.rc.in \
	librevenge-stream-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.def \
	librevenge-stream.rc.in

# These may be in the builddir too
BUILD_EXTRA_DIST = \
	librevenge.rc \
	librevenge-stream.rc
