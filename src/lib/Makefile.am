EXTRA_DIST =			\
	libwpd.rc.in

# These may be in the builddir too
BUILD_EXTRA_DIST = 		\
	libwpd.rc	
	
if PLATFORM_WIN32
no_undefined = -no-undefined
version_info = -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) -avoid-version 
else
no_undefined =
version_info = -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)
endif

if OS_WIN32
install-libtool-import-lib:
	$(INSTALL) .libs/libwpd-1.dll.a $(DESTDIR)$(libdir)

uninstall-libtool-import-lib:
	-rm $(DESTDIR)$(libdir)/libwpd-1.dll.a
	
else
install-libtool-import-lib:
uninstall-libtool-import-lib:
endif

lib_LTLIBRARIES = libwpd-1.la
libwpd_1_includedir = $(includedir)/libwpd-1/libwpd
libwpd_1_include_HEADERS = libwpd.h libwpd_support.h GSFStream.h WPXStream.h WPDocument.h WPXHLListenerImpl.h

AM_CXXFLAGS = $(LIBWPD_CXXFLAGS) $(DEBUG_CXXFLAGS)

libwpd_1_la_LIBADD  = $(LIBWPD_LIBS) @LIBWPD_WIN32_RESOURCE@
libwpd_1_la_DEPENDENCIES = @LIBWPD_WIN32_RESOURCE@
libwpd_1_la_LDFLAGS = $(version_info) -export-dynamic $(no_undefined)
libwpd_1_la_SOURCES =				\
	libwpd_internal.cpp			\
	libwpd_math.cpp				\
	libwpd_support.cpp			\
	GSFStream.cpp				\
	WP3FileStructure.cpp			\
	WP3FixedLengthGroup.cpp			\
	WP3Header.cpp			\
	WP3HLListener.cpp			\
	WP3HLStylesListener.cpp			\
	WP3Parser.cpp			\
	WP3Part.cpp			\
	WP3UnsupportedFixedLengthGroup.cpp \
	WP3UnsupportedVariableLengthGroup.cpp \
	WP3VariableLengthGroup.cpp		\
	WP42FileStructure.cpp			\
	WP42Heuristics.cpp			\
	WP42HLListener.cpp			\
	WP42HLStylesListener.cpp		\
	WP42Parser.cpp				\
	WP42Part.cpp				\
	WP42UnsupportedVariableLengthGroup.cpp	\
	WP42VariableLengthGroup.cpp		\
	WP5AttributeGroup.cpp 			\
	WP5FileStructure.cpp			\
	WP5FixedLengthGroup.cpp			\
	WP5FormatGroup.cpp			\
	WP5Header.cpp				\
	WP5HLListener.cpp			\
	WP5HLStylesListener.cpp			\
	WP5Parser.cpp				\
	WP5Part.cpp				\
	WP5UnsupportedFixedLengthGroup.cpp	\
	WP5UnsupportedVariableLengthGroup.cpp	\
	WP5VariableLengthGroup.cpp		\
	WP6AttributeGroup.cpp 			\
	WP6CharacterGroup.cpp			\
	WP6ColumnGroup.cpp			\
	WP6DefaultInitialFontPacket.cpp		\
	WP6DisplayNumberReferenceGroup.cpp 	\
	WP6EOLGroup.cpp				\
	WP6ExtendedCharacterGroup.cpp 		\
	WP6ExtendedDocumentSummaryPacket.cpp 	\
	WP6FileStructure.cpp			\
	WP6FillStylePacket.cpp 			\
	WP6FixedLengthGroup.cpp			\
	WP6FontDescriptorPacket.cpp		\
	WP6FootnoteEndnoteGroup.cpp		\
	WP6GeneralTextPacket.cpp		\
	WP6Header.cpp				\
	WP60Header.cpp				\
	WP61Header.cpp				\
	WP6HeaderFooterGroup.cpp 		\
	WP6HighlightGroup.cpp			\
	WP6HLContentListener.cpp		\
	WP6HLListener.cpp			\
	WP6HLStylesListener.cpp			\
	WP6LLListener.cpp			\
	WP6Parser.cpp				\
	WP6OutlineStylePacket.cpp		\
	WP6PageGroup.cpp			\
	WP6ParagraphGroup.cpp			\
	WP6Part.cpp				\
	WP6PrefixData.cpp 			\
	WP6PrefixIndice.cpp 			\
	WP6PrefixDataPacket.cpp			\
	WP6SingleByteFunction.cpp		\
	WP6StyleGroup.cpp			\
	WP6TabGroup.cpp 			\
	WP6UndoGroup.cpp			\
	WP6UnsupportedFixedLengthGroup.cpp 	\
	WP6UnsupportedVariableLengthGroup.cpp 	\
	WP6VariableLengthGroup.cpp 		\
	WPXPageSpan.cpp				\
	WPXHeader.cpp 				\
	WPXHLListener.cpp 			\
	WPXMemoryStream.cpp			\
	WPXTable.cpp				\
	WPXParser.cpp 				\
	WPXPart.cpp				\
	WPDocument.cpp				\
	libwpd.h	        		\
	libwpd_internal.h			\
	libwpd_math.h				\
	WP3FileStructure.h			\
	libwpd_support.h        		\
	WP3FixedLengthGroup.h			\
	WP3Header.h			\
	WP3HLListener.h			\
	WP3HLStylesListener.h			\
	WP3LLListener.h			\
	WP3Parser.h			\
	WP3Part.h			\
	WP3VariableLengthGroup.h		\
	WP3UnsupportedFixedLengthGroup.h \
	WP3UnsupportedVariableLengthGroup.h	\
	WP42FileStructure.h			\
	WP42Heuristics.h			\
	WP42HLListener.h			\
	WP42HLStylesListener.h			\
	WP42LLListener.h			\
	WP42Part.h				\
	WP42Parser.h				\
	WP42UnsupportedVariableLengthGroup.h	\
	WP42VariableLengthGroup.h		\
	WP5AttributeGroup.h 			\
	WP5FileStructure.h			\
	WP5FixedLengthGroup.h			\
	WP5FormatGroup.h			\
	WP5Header.h				\
	WP5HLListener.h				\
	WP5HLStylesListener.h			\
	WP5LLListener.h				\
	WP5Part.h				\
	WP5Parser.h				\
	WP5UnsupportedFixedLengthGroup.h	\
	WP5UnsupportedVariableLengthGroup.h	\
	WP5VariableLengthGroup.h		\
	WP6FileStructure.h			\
	WP6AttributeGroup.h 			\
	WP6CharacterGroup.h			\
	WP6ColumnGroup.h			\
	WP6DefaultInitialFontPacket.h		\
	WP6DisplayNumberReferenceGroup.h	\
	WP6EOLGroup.h				\
	WP6ExtendedCharacterGroup.h 		\
	WP6ExtendedDocumentSummaryPacket.h 	\
	WP6FillStylePacket.h			\
	WP6FixedLengthGroup.h			\
	WP6FontDescriptorPacket.h		\
	WP6FootnoteEndnoteGroup.h		\
	WP6GeneralTextPacket.h			\
	WP6Header.h 				\
	WP60Header.h 				\
	WP61Header.h 				\
	WP6HeaderFooterGroup.h 			\
	WP6HighlightGroup.h			\
	WP6HLContentListener.h			\
	WP6HLListener.h				\
	WP6HLStylesListener.h			\
	WP6LLListener.h				\
	WP6OutlineStylePacket.h			\
	WP6PageGroup.h				\
	WP6ParagraphGroup.h			\
	WP6Parser.h				\
	WP6Part.h 				\
	WP6PrefixData.h	 			\
	WP6PrefixIndice.h			\
	WP6PrefixDataPacket.h			\
	WP6SingleByteFunction.h			\
	WP6StyleGroup.h				\
	WP6TabGroup.h	 			\
	WP6TibetanMap.h				\
	WP6UndoGroup.h				\
	WP6UnsupportedFixedLengthGroup.h	\
	WP6UnsupportedVariableLengthGroup.h 	\
	WP6VariableLengthGroup.h		\
	WPXHeader.h				\
	WPDocument.h				\
	WPXFileStructure.h			\
	WPXLLListener.h				\
	WPXHLListener.h				\
	WPXHLListenerImpl.h			\
	WPXMemoryStream.h			\
	WPXPageSpan.h				\
	WPXParser.h 				\
	WPXPart.h				\
	WPXTable.h
	
if OS_WIN32

@LIBWPD_WIN32_RESOURCE@ : libwpd.rc
	$(top_srcdir)/build/win32/lt-compile-resource libwpd.rc @LIBWPD_WIN32_RESOURCE@
endif


install-data-local: install-libtool-import-lib

uninstall-local: uninstall-libtool-import-lib
