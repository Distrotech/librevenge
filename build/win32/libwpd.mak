# Microsoft Developer Studio Generated NMAKE File, Based on libwpd.dsp
!IF "$(CFG)" == ""
CFG=libwpd - Win32 Release
!MESSAGE No configuration specified. Defaulting to libwpd - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "libwpd - Win32 Release" && "$(CFG)" != "libwpd - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libwpd.mak" CFG="libwpd - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libwpd - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libwpd - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "libwpd - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\lib\libwpd-0.9.lib"


CLEAN :
	-@erase "$(INTDIR)\libwpd_internal.obj"
	-@erase "$(INTDIR)\libwpd_math.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WP1BottomMarginGroup.obj"
	-@erase "$(INTDIR)\WP1CenterTextGroup.obj"
	-@erase "$(INTDIR)\WP1ContentListener.obj"
	-@erase "$(INTDIR)\WP1ExtendedCharacterGroup.obj"
	-@erase "$(INTDIR)\WP1FileStructure.obj"
	-@erase "$(INTDIR)\WP1FixedLengthGroup.obj"
	-@erase "$(INTDIR)\WP1FlushRightGroup.obj"
	-@erase "$(INTDIR)\WP1FontIdGroup.obj"
	-@erase "$(INTDIR)\WP1FootnoteEndnoteGroup.obj"
	-@erase "$(INTDIR)\WP1HeaderFooterGroup.obj"
	-@erase "$(INTDIR)\WP1Heuristics.obj"
	-@erase "$(INTDIR)\WP1JustificationGroup.obj"
	-@erase "$(INTDIR)\WP1LeftIndentGroup.obj"
	-@erase "$(INTDIR)\WP1LeftRightIndentGroup.obj"
	-@erase "$(INTDIR)\WP1Listener.obj"
	-@erase "$(INTDIR)\WP1MarginReleaseGroup.obj"
	-@erase "$(INTDIR)\WP1MarginResetGroup.obj"
	-@erase "$(INTDIR)\WP1Parser.obj"
	-@erase "$(INTDIR)\WP1Part.obj"
	-@erase "$(INTDIR)\WP1PictureGroup.obj"
	-@erase "$(INTDIR)\WP1PointSizeGroup.obj"
	-@erase "$(INTDIR)\WP1SetTabsGroup.obj"
	-@erase "$(INTDIR)\WP1SpacingResetGroup.obj"
	-@erase "$(INTDIR)\WP1StylesListener.obj"
	-@erase "$(INTDIR)\WP1SubDocument.obj"
	-@erase "$(INTDIR)\WP1SuppressPageCharacteristicsGroup.obj"
	-@erase "$(INTDIR)\WP1TopMarginGroup.obj"
	-@erase "$(INTDIR)\WP1UnsupportedFixedLengthGroup.obj"
	-@erase "$(INTDIR)\WP1UnsupportedVariableLengthGroup.obj"
	-@erase "$(INTDIR)\WP1VariableLengthGroup.obj"
	-@erase "$(INTDIR)\WP3AttributeGroup.obj"
	-@erase "$(INTDIR)\WP3ContentListener.obj"
	-@erase "$(INTDIR)\WP3DefinitionGroup.obj"
	-@erase "$(INTDIR)\WP3DisplayGroup.obj"
	-@erase "$(INTDIR)\WP3DoubleByteScriptCharacterGroup.obj"
	-@erase "$(INTDIR)\WP3EndOfLinePageGroup.obj"
	-@erase "$(INTDIR)\WP3ExtendedCharacterGroup.obj"
	-@erase "$(INTDIR)\WP3FileStructure.obj"
	-@erase "$(INTDIR)\WP3FixedLengthGroup.obj"
	-@erase "$(INTDIR)\WP3FontGroup.obj"
	-@erase "$(INTDIR)\WP3FootnoteEndnoteGroup.obj"
	-@erase "$(INTDIR)\WP3Header.obj"
	-@erase "$(INTDIR)\WP3HeaderFooterGroup.obj"
	-@erase "$(INTDIR)\WP3IndentGroup.obj"
	-@erase "$(INTDIR)\WP3Listener.obj"
	-@erase "$(INTDIR)\WP3MiscellaneousGroup.obj"
	-@erase "$(INTDIR)\WP3PageFormatGroup.obj"
	-@erase "$(INTDIR)\WP3Parser.obj"
	-@erase "$(INTDIR)\WP3Part.obj"
	-@erase "$(INTDIR)\WP3Resource.obj"
	-@erase "$(INTDIR)\WP3ResourceFork.obj"
	-@erase "$(INTDIR)\WP3SingleByteFunction.obj"
	-@erase "$(INTDIR)\WP3StylesListener.obj"
	-@erase "$(INTDIR)\WP3SubDocument.obj"
	-@erase "$(INTDIR)\WP3TabGroup.obj"
	-@erase "$(INTDIR)\WP3TablesGroup.obj"
	-@erase "$(INTDIR)\WP3UndoGroup.obj"
	-@erase "$(INTDIR)\WP3UnsupportedFixedLengthGroup.obj"
	-@erase "$(INTDIR)\WP3UnsupportedVariableLengthGroup.obj"
	-@erase "$(INTDIR)\WP3VariableLengthGroup.obj"
	-@erase "$(INTDIR)\WP3WindowGroup.obj"
	-@erase "$(INTDIR)\WP42ContentListener.obj"
	-@erase "$(INTDIR)\WP42DefineColumnsGroup.obj"
	-@erase "$(INTDIR)\WP42ExtendedCharacterGroup.obj"
	-@erase "$(INTDIR)\WP42FileStructure.obj"
	-@erase "$(INTDIR)\WP42HeaderFooterGroup.obj"
	-@erase "$(INTDIR)\WP42Heuristics.obj"
	-@erase "$(INTDIR)\WP42Listener.obj"
	-@erase "$(INTDIR)\WP42MarginResetGroup.obj"
	-@erase "$(INTDIR)\WP42MultiByteFunctionGroup.obj"
	-@erase "$(INTDIR)\WP42Parser.obj"
	-@erase "$(INTDIR)\WP42Part.obj"
	-@erase "$(INTDIR)\WP42StylesListener.obj"
	-@erase "$(INTDIR)\WP42SubDocument.obj"
	-@erase "$(INTDIR)\WP42SuppressPageCharacteristicsGroup.obj"
	-@erase "$(INTDIR)\WP42UnsupportedMultiByteFunctionGroup.obj"
	-@erase "$(INTDIR)\WP5AttributeGroup.obj"
	-@erase "$(INTDIR)\WP5BoxGroup.obj"
	-@erase "$(INTDIR)\WP5ContentListener.obj"
	-@erase "$(INTDIR)\WP5DefinitionGroup.obj"
	-@erase "$(INTDIR)\WP5ExtendedCharacterGroup.obj"
	-@erase "$(INTDIR)\WP5FileStructure.obj"
	-@erase "$(INTDIR)\WP5FixedLengthGroup.obj"
	-@erase "$(INTDIR)\WP5FontGroup.obj"
	-@erase "$(INTDIR)\WP5FontNameStringPoolPacket.obj"
	-@erase "$(INTDIR)\WP5FootnoteEndnoteGroup.obj"
	-@erase "$(INTDIR)\WP5GeneralPacketData.obj"
	-@erase "$(INTDIR)\WP5GeneralPacketIndex.obj"
	-@erase "$(INTDIR)\WP5GraphicsInformationPacket.obj"
	-@erase "$(INTDIR)\WP5Header.obj"
	-@erase "$(INTDIR)\WP5HeaderFooterGroup.obj"
	-@erase "$(INTDIR)\WP5IndentGroup.obj"
	-@erase "$(INTDIR)\WP5Listener.obj"
	-@erase "$(INTDIR)\WP5ListFontsUsedPacket.obj"
	-@erase "$(INTDIR)\WP5PageFormatGroup.obj"
	-@erase "$(INTDIR)\WP5Parser.obj"
	-@erase "$(INTDIR)\WP5Part.obj"
	-@erase "$(INTDIR)\WP5PrefixData.obj"
	-@erase "$(INTDIR)\WP5SingleByteFunction.obj"
	-@erase "$(INTDIR)\WP5SpecialHeaderIndex.obj"
	-@erase "$(INTDIR)\WP5StylesListener.obj"
	-@erase "$(INTDIR)\WP5SubDocument.obj"
	-@erase "$(INTDIR)\WP5TabGroup.obj"
	-@erase "$(INTDIR)\WP5TableEOLGroup.obj"
	-@erase "$(INTDIR)\WP5TableEOPGroup.obj"
	-@erase "$(INTDIR)\WP5UnsupportedFixedLengthGroup.obj"
	-@erase "$(INTDIR)\WP5UnsupportedVariableLengthGroup.obj"
	-@erase "$(INTDIR)\WP5VariableLengthGroup.obj"
	-@erase "$(INTDIR)\WP60Header.obj"
	-@erase "$(INTDIR)\WP61Header.obj"
	-@erase "$(INTDIR)\WP6AttributeGroup.obj"
	-@erase "$(INTDIR)\WP6BoxGroup.obj"
	-@erase "$(INTDIR)\WP6CharacterGroup.obj"
	-@erase "$(INTDIR)\WP6ColumnGroup.obj"
	-@erase "$(INTDIR)\WP6CommentAnnotationPacket.obj"
	-@erase "$(INTDIR)\WP6ContentListener.obj"
	-@erase "$(INTDIR)\WP6DefaultInitialFontPacket.obj"
	-@erase "$(INTDIR)\WP6DisplayNumberReferenceGroup.obj"
	-@erase "$(INTDIR)\WP6EOLGroup.obj"
	-@erase "$(INTDIR)\WP6ExtendedCharacterGroup.obj"
	-@erase "$(INTDIR)\WP6ExtendedDocumentSummaryPacket.obj"
	-@erase "$(INTDIR)\WP6FileStructure.obj"
	-@erase "$(INTDIR)\WP6FillStylePacket.obj"
	-@erase "$(INTDIR)\WP6FixedLengthGroup.obj"
	-@erase "$(INTDIR)\WP6FontDescriptorPacket.obj"
	-@erase "$(INTDIR)\WP6FootnoteEndnoteGroup.obj"
	-@erase "$(INTDIR)\WP6GeneralTextPacket.obj"
	-@erase "$(INTDIR)\WP6GraphicsBoxStylePacket.obj"
	-@erase "$(INTDIR)\WP6GraphicsCachedFileDataPacket.obj"
	-@erase "$(INTDIR)\WP6GraphicsFilenamePacket.obj"
	-@erase "$(INTDIR)\WP6Header.obj"
	-@erase "$(INTDIR)\WP6HeaderFooterGroup.obj"
	-@erase "$(INTDIR)\WP6HighlightGroup.obj"
	-@erase "$(INTDIR)\WP6Listener.obj"
	-@erase "$(INTDIR)\WP6NumberingMethodGroup.obj"
	-@erase "$(INTDIR)\WP6OutlineStylePacket.obj"
	-@erase "$(INTDIR)\WP6PageGroup.obj"
	-@erase "$(INTDIR)\WP6ParagraphGroup.obj"
	-@erase "$(INTDIR)\WP6Parser.obj"
	-@erase "$(INTDIR)\WP6Part.obj"
	-@erase "$(INTDIR)\WP6PrefixData.obj"
	-@erase "$(INTDIR)\WP6PrefixDataPacket.obj"
	-@erase "$(INTDIR)\WP6PrefixIndice.obj"
	-@erase "$(INTDIR)\WP6SetNumberGroup.obj"
	-@erase "$(INTDIR)\WP6SingleByteFunction.obj"
	-@erase "$(INTDIR)\WP6StyleGroup.obj"
	-@erase "$(INTDIR)\WP6StylesListener.obj"
	-@erase "$(INTDIR)\WP6SubDocument.obj"
	-@erase "$(INTDIR)\WP6TabGroup.obj"
	-@erase "$(INTDIR)\WP6TableStylePacket.obj"
	-@erase "$(INTDIR)\WP6UndoGroup.obj"
	-@erase "$(INTDIR)\WP6UnsupportedFixedLengthGroup.obj"
	-@erase "$(INTDIR)\WP6UnsupportedVariableLengthGroup.obj"
	-@erase "$(INTDIR)\WP6VariableLengthGroup.obj"
	-@erase "$(INTDIR)\WPDocument.obj"
	-@erase "$(INTDIR)\WPXBinaryData.obj"
	-@erase "$(INTDIR)\WPXContentListener.obj"
	-@erase "$(INTDIR)\WPXEncryption.obj"
	-@erase "$(INTDIR)\WPXHeader.obj"
	-@erase "$(INTDIR)\WPXListener.obj"
	-@erase "$(INTDIR)\WPXMemoryStream.obj"
	-@erase "$(INTDIR)\WPXPageSpan.obj"
	-@erase "$(INTDIR)\WPXParser.obj"
	-@erase "$(INTDIR)\WPXProperty.obj"
	-@erase "$(INTDIR)\WPXPropertyList.obj"
	-@erase "$(INTDIR)\WPXPropertyListVector.obj"
	-@erase "$(INTDIR)\WPXString.obj"
	-@erase "$(INTDIR)\WPXStylesListener.obj"
	-@erase "$(INTDIR)\WPXSubDocument.obj"
	-@erase "$(INTDIR)\WPXTable.obj"
	-@erase "$(OUTDIR)\lib\libwpd-0.9.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GR /GX /O2 /D "NDEBUG" /D "WIN32" /D "_LIB" /D "_CRT_SECURE_NO_WARNINGS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libwpd.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\lib\libwpd-0.9.lib" 
LIB32_OBJS= \
	"$(INTDIR)\libwpd_internal.obj" \
	"$(INTDIR)\libwpd_math.obj" \
	"$(INTDIR)\WP1BottomMarginGroup.obj" \
	"$(INTDIR)\WP1CenterTextGroup.obj" \
	"$(INTDIR)\WP1ContentListener.obj" \
	"$(INTDIR)\WP1ExtendedCharacterGroup.obj" \
	"$(INTDIR)\WP1FileStructure.obj" \
	"$(INTDIR)\WP1FixedLengthGroup.obj" \
	"$(INTDIR)\WP1FlushRightGroup.obj" \
	"$(INTDIR)\WP1FontIdGroup.obj" \
	"$(INTDIR)\WP1FootnoteEndnoteGroup.obj" \
	"$(INTDIR)\WP1HeaderFooterGroup.obj" \
	"$(INTDIR)\WP1Heuristics.obj" \
	"$(INTDIR)\WP1JustificationGroup.obj" \
	"$(INTDIR)\WP1LeftIndentGroup.obj" \
	"$(INTDIR)\WP1LeftRightIndentGroup.obj" \
	"$(INTDIR)\WP1Listener.obj" \
	"$(INTDIR)\WP1MarginReleaseGroup.obj" \
	"$(INTDIR)\WP1MarginResetGroup.obj" \
	"$(INTDIR)\WP1Parser.obj" \
	"$(INTDIR)\WP1Part.obj" \
	"$(INTDIR)\WP1PictureGroup.obj" \
	"$(INTDIR)\WP1PointSizeGroup.obj" \
	"$(INTDIR)\WP1SetTabsGroup.obj" \
	"$(INTDIR)\WP1SpacingResetGroup.obj" \
	"$(INTDIR)\WP1StylesListener.obj" \
	"$(INTDIR)\WP1SubDocument.obj" \
	"$(INTDIR)\WP1SuppressPageCharacteristicsGroup.obj" \
	"$(INTDIR)\WP1TopMarginGroup.obj" \
	"$(INTDIR)\WP1UnsupportedFixedLengthGroup.obj" \
	"$(INTDIR)\WP1UnsupportedVariableLengthGroup.obj" \
	"$(INTDIR)\WP1VariableLengthGroup.obj" \
	"$(INTDIR)\WP3AttributeGroup.obj" \
	"$(INTDIR)\WP3ContentListener.obj" \
	"$(INTDIR)\WP3DefinitionGroup.obj" \
	"$(INTDIR)\WP3DisplayGroup.obj" \
	"$(INTDIR)\WP3DoubleByteScriptCharacterGroup.obj" \
	"$(INTDIR)\WP3EndOfLinePageGroup.obj" \
	"$(INTDIR)\WP3ExtendedCharacterGroup.obj" \
	"$(INTDIR)\WP3FileStructure.obj" \
	"$(INTDIR)\WP3FixedLengthGroup.obj" \
	"$(INTDIR)\WP3FontGroup.obj" \
	"$(INTDIR)\WP3FootnoteEndnoteGroup.obj" \
	"$(INTDIR)\WP3Header.obj" \
	"$(INTDIR)\WP3HeaderFooterGroup.obj" \
	"$(INTDIR)\WP3IndentGroup.obj" \
	"$(INTDIR)\WP3Listener.obj" \
	"$(INTDIR)\WP3MiscellaneousGroup.obj" \
	"$(INTDIR)\WP3PageFormatGroup.obj" \
	"$(INTDIR)\WP3Parser.obj" \
	"$(INTDIR)\WP3Part.obj" \
	"$(INTDIR)\WP3Resource.obj" \
	"$(INTDIR)\WP3ResourceFork.obj" \
	"$(INTDIR)\WP3SingleByteFunction.obj" \
	"$(INTDIR)\WP3StylesListener.obj" \
	"$(INTDIR)\WP3SubDocument.obj" \
	"$(INTDIR)\WP3TabGroup.obj" \
	"$(INTDIR)\WP3TablesGroup.obj" \
	"$(INTDIR)\WP3UndoGroup.obj" \
	"$(INTDIR)\WP3UnsupportedFixedLengthGroup.obj" \
	"$(INTDIR)\WP3UnsupportedVariableLengthGroup.obj" \
	"$(INTDIR)\WP3VariableLengthGroup.obj" \
	"$(INTDIR)\WP3WindowGroup.obj" \
	"$(INTDIR)\WP42ContentListener.obj" \
	"$(INTDIR)\WP42DefineColumnsGroup.obj" \
	"$(INTDIR)\WP42ExtendedCharacterGroup.obj" \
	"$(INTDIR)\WP42FileStructure.obj" \
	"$(INTDIR)\WP42HeaderFooterGroup.obj" \
	"$(INTDIR)\WP42Heuristics.obj" \
	"$(INTDIR)\WP42Listener.obj" \
	"$(INTDIR)\WP42MarginResetGroup.obj" \
	"$(INTDIR)\WP42MultiByteFunctionGroup.obj" \
	"$(INTDIR)\WP42Parser.obj" \
	"$(INTDIR)\WP42Part.obj" \
	"$(INTDIR)\WP42StylesListener.obj" \
	"$(INTDIR)\WP42SubDocument.obj" \
	"$(INTDIR)\WP42SuppressPageCharacteristicsGroup.obj" \
	"$(INTDIR)\WP42UnsupportedMultiByteFunctionGroup.obj" \
	"$(INTDIR)\WP5AttributeGroup.obj" \
	"$(INTDIR)\WP5BoxGroup.obj" \
	"$(INTDIR)\WP5ContentListener.obj" \
	"$(INTDIR)\WP5DefinitionGroup.obj" \
	"$(INTDIR)\WP5ExtendedCharacterGroup.obj" \
	"$(INTDIR)\WP5FileStructure.obj" \
	"$(INTDIR)\WP5FixedLengthGroup.obj" \
	"$(INTDIR)\WP5FontGroup.obj" \
	"$(INTDIR)\WP5FontNameStringPoolPacket.obj" \
	"$(INTDIR)\WP5FootnoteEndnoteGroup.obj" \
	"$(INTDIR)\WP5GeneralPacketData.obj" \
	"$(INTDIR)\WP5GeneralPacketIndex.obj" \
	"$(INTDIR)\WP5GraphicsInformationPacket.obj" \
	"$(INTDIR)\WP5Header.obj" \
	"$(INTDIR)\WP5HeaderFooterGroup.obj" \
	"$(INTDIR)\WP5IndentGroup.obj" \
	"$(INTDIR)\WP5Listener.obj" \
	"$(INTDIR)\WP5ListFontsUsedPacket.obj" \
	"$(INTDIR)\WP5PageFormatGroup.obj" \
	"$(INTDIR)\WP5Parser.obj" \
	"$(INTDIR)\WP5Part.obj" \
	"$(INTDIR)\WP5PrefixData.obj" \
	"$(INTDIR)\WP5SingleByteFunction.obj" \
	"$(INTDIR)\WP5SpecialHeaderIndex.obj" \
	"$(INTDIR)\WP5StylesListener.obj" \
	"$(INTDIR)\WP5SubDocument.obj" \
	"$(INTDIR)\WP5TabGroup.obj" \
	"$(INTDIR)\WP5TableEOLGroup.obj" \
	"$(INTDIR)\WP5TableEOPGroup.obj" \
	"$(INTDIR)\WP5UnsupportedFixedLengthGroup.obj" \
	"$(INTDIR)\WP5UnsupportedVariableLengthGroup.obj" \
	"$(INTDIR)\WP5VariableLengthGroup.obj" \
	"$(INTDIR)\WP60Header.obj" \
	"$(INTDIR)\WP61Header.obj" \
	"$(INTDIR)\WP6AttributeGroup.obj" \
	"$(INTDIR)\WP6BoxGroup.obj" \
	"$(INTDIR)\WP6CharacterGroup.obj" \
	"$(INTDIR)\WP6ColumnGroup.obj" \
	"$(INTDIR)\WP6CommentAnnotationPacket.obj" \
	"$(INTDIR)\WP6ContentListener.obj" \
	"$(INTDIR)\WP6DefaultInitialFontPacket.obj" \
	"$(INTDIR)\WP6DisplayNumberReferenceGroup.obj" \
	"$(INTDIR)\WP6EOLGroup.obj" \
	"$(INTDIR)\WP6ExtendedCharacterGroup.obj" \
	"$(INTDIR)\WP6ExtendedDocumentSummaryPacket.obj" \
	"$(INTDIR)\WP6FileStructure.obj" \
	"$(INTDIR)\WP6FillStylePacket.obj" \
	"$(INTDIR)\WP6FixedLengthGroup.obj" \
	"$(INTDIR)\WP6FontDescriptorPacket.obj" \
	"$(INTDIR)\WP6FootnoteEndnoteGroup.obj" \
	"$(INTDIR)\WP6GeneralTextPacket.obj" \
	"$(INTDIR)\WP6GraphicsBoxStylePacket.obj" \
	"$(INTDIR)\WP6GraphicsCachedFileDataPacket.obj" \
	"$(INTDIR)\WP6GraphicsFilenamePacket.obj" \
	"$(INTDIR)\WP6Header.obj" \
	"$(INTDIR)\WP6HeaderFooterGroup.obj" \
	"$(INTDIR)\WP6HighlightGroup.obj" \
	"$(INTDIR)\WP6Listener.obj" \
	"$(INTDIR)\WP6NumberingMethodGroup.obj" \
	"$(INTDIR)\WP6OutlineStylePacket.obj" \
	"$(INTDIR)\WP6PageGroup.obj" \
	"$(INTDIR)\WP6ParagraphGroup.obj" \
	"$(INTDIR)\WP6Parser.obj" \
	"$(INTDIR)\WP6Part.obj" \
	"$(INTDIR)\WP6PrefixData.obj" \
	"$(INTDIR)\WP6PrefixDataPacket.obj" \
	"$(INTDIR)\WP6PrefixIndice.obj" \
	"$(INTDIR)\WP6SetNumberGroup.obj" \
	"$(INTDIR)\WP6SingleByteFunction.obj" \
	"$(INTDIR)\WP6StyleGroup.obj" \
	"$(INTDIR)\WP6StylesListener.obj" \
	"$(INTDIR)\WP6SubDocument.obj" \
	"$(INTDIR)\WP6TabGroup.obj" \
	"$(INTDIR)\WP6TableStylePacket.obj" \
	"$(INTDIR)\WP6UndoGroup.obj" \
	"$(INTDIR)\WP6UnsupportedFixedLengthGroup.obj" \
	"$(INTDIR)\WP6UnsupportedVariableLengthGroup.obj" \
	"$(INTDIR)\WP6VariableLengthGroup.obj" \
	"$(INTDIR)\WPDocument.obj" \
	"$(INTDIR)\WPXBinaryData.obj" \
	"$(INTDIR)\WPXContentListener.obj" \
	"$(INTDIR)\WPXEncryption.obj" \
	"$(INTDIR)\WPXHeader.obj" \
	"$(INTDIR)\WPXListener.obj" \
	"$(INTDIR)\WPXMemoryStream.obj" \
	"$(INTDIR)\WPXPageSpan.obj" \
	"$(INTDIR)\WPXParser.obj" \
	"$(INTDIR)\WPXProperty.obj" \
	"$(INTDIR)\WPXPropertyList.obj" \
	"$(INTDIR)\WPXPropertyListVector.obj" \
	"$(INTDIR)\WPXString.obj" \
	"$(INTDIR)\WPXStylesListener.obj" \
	"$(INTDIR)\WPXSubDocument.obj" \
	"$(INTDIR)\WPXTable.obj"

"$(OUTDIR)\lib\libwpd-0.9.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "libwpd - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\lib\libwpd-0.9.lib"


CLEAN :
	-@erase "$(INTDIR)\libwpd_internal.obj"
	-@erase "$(INTDIR)\libwpd_math.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\WP1BottomMarginGroup.obj"
	-@erase "$(INTDIR)\WP1CenterTextGroup.obj"
	-@erase "$(INTDIR)\WP1ContentListener.obj"
	-@erase "$(INTDIR)\WP1ExtendedCharacterGroup.obj"
	-@erase "$(INTDIR)\WP1FileStructure.obj"
	-@erase "$(INTDIR)\WP1FixedLengthGroup.obj"
	-@erase "$(INTDIR)\WP1FlushRightGroup.obj"
	-@erase "$(INTDIR)\WP1FontIdGroup.obj"
	-@erase "$(INTDIR)\WP1FootnoteEndnoteGroup.obj"
	-@erase "$(INTDIR)\WP1HeaderFooterGroup.obj"
	-@erase "$(INTDIR)\WP1Heuristics.obj"
	-@erase "$(INTDIR)\WP1JustificationGroup.obj"
	-@erase "$(INTDIR)\WP1LeftIndentGroup.obj"
	-@erase "$(INTDIR)\WP1LeftRightIndentGroup.obj"
	-@erase "$(INTDIR)\WP1Listener.obj"
	-@erase "$(INTDIR)\WP1MarginReleaseGroup.obj"
	-@erase "$(INTDIR)\WP1MarginResetGroup.obj"
	-@erase "$(INTDIR)\WP1Parser.obj"
	-@erase "$(INTDIR)\WP1Part.obj"
	-@erase "$(INTDIR)\WP1PictureGroup.obj"
	-@erase "$(INTDIR)\WP1PointSizeGroup.obj"
	-@erase "$(INTDIR)\WP1SetTabsGroup.obj"
	-@erase "$(INTDIR)\WP1SpacingResetGroup.obj"
	-@erase "$(INTDIR)\WP1StylesListener.obj"
	-@erase "$(INTDIR)\WP1SubDocument.obj"
	-@erase "$(INTDIR)\WP1SuppressPageCharacteristicsGroup.obj"
	-@erase "$(INTDIR)\WP1TopMarginGroup.obj"
	-@erase "$(INTDIR)\WP1UnsupportedFixedLengthGroup.obj"
	-@erase "$(INTDIR)\WP1UnsupportedVariableLengthGroup.obj"
	-@erase "$(INTDIR)\WP1VariableLengthGroup.obj"
	-@erase "$(INTDIR)\WP3AttributeGroup.obj"
	-@erase "$(INTDIR)\WP3ContentListener.obj"
	-@erase "$(INTDIR)\WP3DefinitionGroup.obj"
	-@erase "$(INTDIR)\WP3DisplayGroup.obj"
	-@erase "$(INTDIR)\WP3DoubleByteScriptCharacterGroup.obj"
	-@erase "$(INTDIR)\WP3EndOfLinePageGroup.obj"
	-@erase "$(INTDIR)\WP3ExtendedCharacterGroup.obj"
	-@erase "$(INTDIR)\WP3FileStructure.obj"
	-@erase "$(INTDIR)\WP3FixedLengthGroup.obj"
	-@erase "$(INTDIR)\WP3FontGroup.obj"
	-@erase "$(INTDIR)\WP3FootnoteEndnoteGroup.obj"
	-@erase "$(INTDIR)\WP3Header.obj"
	-@erase "$(INTDIR)\WP3HeaderFooterGroup.obj"
	-@erase "$(INTDIR)\WP3IndentGroup.obj"
	-@erase "$(INTDIR)\WP3Listener.obj"
	-@erase "$(INTDIR)\WP3MiscellaneousGroup.obj"
	-@erase "$(INTDIR)\WP3PageFormatGroup.obj"
	-@erase "$(INTDIR)\WP3Parser.obj"
	-@erase "$(INTDIR)\WP3Part.obj"
	-@erase "$(INTDIR)\WP3Resource.obj"
	-@erase "$(INTDIR)\WP3ResourceFork.obj"
	-@erase "$(INTDIR)\WP3SingleByteFunction.obj"
	-@erase "$(INTDIR)\WP3StylesListener.obj"
	-@erase "$(INTDIR)\WP3SubDocument.obj"
	-@erase "$(INTDIR)\WP3TabGroup.obj"
	-@erase "$(INTDIR)\WP3TablesGroup.obj"
	-@erase "$(INTDIR)\WP3UndoGroup.obj"
	-@erase "$(INTDIR)\WP3UnsupportedFixedLengthGroup.obj"
	-@erase "$(INTDIR)\WP3UnsupportedVariableLengthGroup.obj"
	-@erase "$(INTDIR)\WP3VariableLengthGroup.obj"
	-@erase "$(INTDIR)\WP3WindowGroup.obj"
	-@erase "$(INTDIR)\WP42ContentListener.obj"
	-@erase "$(INTDIR)\WP42DefineColumnsGroup.obj"
	-@erase "$(INTDIR)\WP42ExtendedCharacterGroup.obj"
	-@erase "$(INTDIR)\WP42FileStructure.obj"
	-@erase "$(INTDIR)\WP42HeaderFooterGroup.obj"
	-@erase "$(INTDIR)\WP42Heuristics.obj"
	-@erase "$(INTDIR)\WP42Listener.obj"
	-@erase "$(INTDIR)\WP42MarginResetGroup.obj"
	-@erase "$(INTDIR)\WP42MultiByteFunctionGroup.obj"
	-@erase "$(INTDIR)\WP42Parser.obj"
	-@erase "$(INTDIR)\WP42Part.obj"
	-@erase "$(INTDIR)\WP42StylesListener.obj"
	-@erase "$(INTDIR)\WP42SubDocument.obj"
	-@erase "$(INTDIR)\WP42SuppressPageCharacteristicsGroup.obj"
	-@erase "$(INTDIR)\WP42UnsupportedMultiByteFunctionGroup.obj"
	-@erase "$(INTDIR)\WP5AttributeGroup.obj"
	-@erase "$(INTDIR)\WP5BoxGroup.obj"
	-@erase "$(INTDIR)\WP5ContentListener.obj"
	-@erase "$(INTDIR)\WP5DefinitionGroup.obj"
	-@erase "$(INTDIR)\WP5ExtendedCharacterGroup.obj"
	-@erase "$(INTDIR)\WP5FileStructure.obj"
	-@erase "$(INTDIR)\WP5FixedLengthGroup.obj"
	-@erase "$(INTDIR)\WP5FontGroup.obj"
	-@erase "$(INTDIR)\WP5FontNameStringPoolPacket.obj"
	-@erase "$(INTDIR)\WP5FootnoteEndnoteGroup.obj"
	-@erase "$(INTDIR)\WP5GeneralPacketData.obj"
	-@erase "$(INTDIR)\WP5GeneralPacketIndex.obj"
	-@erase "$(INTDIR)\WP5GraphicsInformationPacket.obj"
	-@erase "$(INTDIR)\WP5Header.obj"
	-@erase "$(INTDIR)\WP5HeaderFooterGroup.obj"
	-@erase "$(INTDIR)\WP5IndentGroup.obj"
	-@erase "$(INTDIR)\WP5Listener.obj"
	-@erase "$(INTDIR)\WP5ListFontsUsedPacket.obj"
	-@erase "$(INTDIR)\WP5PageFormatGroup.obj"
	-@erase "$(INTDIR)\WP5Parser.obj"
	-@erase "$(INTDIR)\WP5Part.obj"
	-@erase "$(INTDIR)\WP5PrefixData.obj"
	-@erase "$(INTDIR)\WP5SingleByteFunction.obj"
	-@erase "$(INTDIR)\WP5SpecialHeaderIndex.obj"
	-@erase "$(INTDIR)\WP5StylesListener.obj"
	-@erase "$(INTDIR)\WP5SubDocument.obj"
	-@erase "$(INTDIR)\WP5TabGroup.obj"
	-@erase "$(INTDIR)\WP5TableEOLGroup.obj"
	-@erase "$(INTDIR)\WP5TableEOPGroup.obj"
	-@erase "$(INTDIR)\WP5UnsupportedFixedLengthGroup.obj"
	-@erase "$(INTDIR)\WP5UnsupportedVariableLengthGroup.obj"
	-@erase "$(INTDIR)\WP5VariableLengthGroup.obj"
	-@erase "$(INTDIR)\WP60Header.obj"
	-@erase "$(INTDIR)\WP61Header.obj"
	-@erase "$(INTDIR)\WP6AttributeGroup.obj"
	-@erase "$(INTDIR)\WP6BoxGroup.obj"
	-@erase "$(INTDIR)\WP6CharacterGroup.obj"
	-@erase "$(INTDIR)\WP6ColumnGroup.obj"
	-@erase "$(INTDIR)\WP6CommentAnnotationPacket.obj"
	-@erase "$(INTDIR)\WP6ContentListener.obj"
	-@erase "$(INTDIR)\WP6DefaultInitialFontPacket.obj"
	-@erase "$(INTDIR)\WP6DisplayNumberReferenceGroup.obj"
	-@erase "$(INTDIR)\WP6EOLGroup.obj"
	-@erase "$(INTDIR)\WP6ExtendedCharacterGroup.obj"
	-@erase "$(INTDIR)\WP6ExtendedDocumentSummaryPacket.obj"
	-@erase "$(INTDIR)\WP6FileStructure.obj"
	-@erase "$(INTDIR)\WP6FillStylePacket.obj"
	-@erase "$(INTDIR)\WP6FixedLengthGroup.obj"
	-@erase "$(INTDIR)\WP6FontDescriptorPacket.obj"
	-@erase "$(INTDIR)\WP6FootnoteEndnoteGroup.obj"
	-@erase "$(INTDIR)\WP6GeneralTextPacket.obj"
	-@erase "$(INTDIR)\WP6GraphicsBoxStylePacket.obj"
	-@erase "$(INTDIR)\WP6GraphicsCachedFileDataPacket.obj"
	-@erase "$(INTDIR)\WP6GraphicsFilenamePacket.obj"
	-@erase "$(INTDIR)\WP6Header.obj"
	-@erase "$(INTDIR)\WP6HeaderFooterGroup.obj"
	-@erase "$(INTDIR)\WP6HighlightGroup.obj"
	-@erase "$(INTDIR)\WP6Listener.obj"
	-@erase "$(INTDIR)\WP6NumberingMethodGroup.obj"
	-@erase "$(INTDIR)\WP6OutlineStylePacket.obj"
	-@erase "$(INTDIR)\WP6PageGroup.obj"
	-@erase "$(INTDIR)\WP6ParagraphGroup.obj"
	-@erase "$(INTDIR)\WP6Parser.obj"
	-@erase "$(INTDIR)\WP6Part.obj"
	-@erase "$(INTDIR)\WP6PrefixData.obj"
	-@erase "$(INTDIR)\WP6PrefixDataPacket.obj"
	-@erase "$(INTDIR)\WP6PrefixIndice.obj"
	-@erase "$(INTDIR)\WP6SetNumberGroup.obj"
	-@erase "$(INTDIR)\WP6SingleByteFunction.obj"
	-@erase "$(INTDIR)\WP6StyleGroup.obj"
	-@erase "$(INTDIR)\WP6StylesListener.obj"
	-@erase "$(INTDIR)\WP6SubDocument.obj"
	-@erase "$(INTDIR)\WP6TabGroup.obj"
	-@erase "$(INTDIR)\WP6TableStylePacket.obj"
	-@erase "$(INTDIR)\WP6UndoGroup.obj"
	-@erase "$(INTDIR)\WP6UnsupportedFixedLengthGroup.obj"
	-@erase "$(INTDIR)\WP6UnsupportedVariableLengthGroup.obj"
	-@erase "$(INTDIR)\WP6VariableLengthGroup.obj"
	-@erase "$(INTDIR)\WPDocument.obj"
	-@erase "$(INTDIR)\WPXBinaryData.obj"
	-@erase "$(INTDIR)\WPXContentListener.obj"
	-@erase "$(INTDIR)\WPXEncryption.obj"
	-@erase "$(INTDIR)\WPXHeader.obj"
	-@erase "$(INTDIR)\WPXListener.obj"
	-@erase "$(INTDIR)\WPXMemoryStream.obj"
	-@erase "$(INTDIR)\WPXPageSpan.obj"
	-@erase "$(INTDIR)\WPXParser.obj"
	-@erase "$(INTDIR)\WPXProperty.obj"
	-@erase "$(INTDIR)\WPXPropertyList.obj"
	-@erase "$(INTDIR)\WPXPropertyListVector.obj"
	-@erase "$(INTDIR)\WPXString.obj"
	-@erase "$(INTDIR)\WPXStylesListener.obj"
	-@erase "$(INTDIR)\WPXSubDocument.obj"
	-@erase "$(INTDIR)\WPXTable.obj"
	-@erase "$(OUTDIR)\lib\libwpd-0.9.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /GR /GX /ZI /Od /D "_DEBUG" /D "DEBUG" /D "WIN32" /D "_LIB" /D "_CRT_SECURE_NO_WARNINGS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libwpd.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\lib\libwpd-0.9.lib" 
LIB32_OBJS= \
	"$(INTDIR)\libwpd_internal.obj" \
	"$(INTDIR)\libwpd_math.obj" \
	"$(INTDIR)\WP1BottomMarginGroup.obj" \
	"$(INTDIR)\WP1CenterTextGroup.obj" \
	"$(INTDIR)\WP1ContentListener.obj" \
	"$(INTDIR)\WP1ExtendedCharacterGroup.obj" \
	"$(INTDIR)\WP1FileStructure.obj" \
	"$(INTDIR)\WP1FixedLengthGroup.obj" \
	"$(INTDIR)\WP1FlushRightGroup.obj" \
	"$(INTDIR)\WP1FontIdGroup.obj" \
	"$(INTDIR)\WP1FootnoteEndnoteGroup.obj" \
	"$(INTDIR)\WP1HeaderFooterGroup.obj" \
	"$(INTDIR)\WP1Heuristics.obj" \
	"$(INTDIR)\WP1JustificationGroup.obj" \
	"$(INTDIR)\WP1LeftIndentGroup.obj" \
	"$(INTDIR)\WP1LeftRightIndentGroup.obj" \
	"$(INTDIR)\WP1Listener.obj" \
	"$(INTDIR)\WP1MarginReleaseGroup.obj" \
	"$(INTDIR)\WP1MarginResetGroup.obj" \
	"$(INTDIR)\WP1Parser.obj" \
	"$(INTDIR)\WP1Part.obj" \
	"$(INTDIR)\WP1PictureGroup.obj" \
	"$(INTDIR)\WP1PointSizeGroup.obj" \
	"$(INTDIR)\WP1SetTabsGroup.obj" \
	"$(INTDIR)\WP1SpacingResetGroup.obj" \
	"$(INTDIR)\WP1StylesListener.obj" \
	"$(INTDIR)\WP1SubDocument.obj" \
	"$(INTDIR)\WP1SuppressPageCharacteristicsGroup.obj" \
	"$(INTDIR)\WP1TopMarginGroup.obj" \
	"$(INTDIR)\WP1UnsupportedFixedLengthGroup.obj" \
	"$(INTDIR)\WP1UnsupportedVariableLengthGroup.obj" \
	"$(INTDIR)\WP1VariableLengthGroup.obj" \
	"$(INTDIR)\WP3AttributeGroup.obj" \
	"$(INTDIR)\WP3ContentListener.obj" \
	"$(INTDIR)\WP3DefinitionGroup.obj" \
	"$(INTDIR)\WP3DisplayGroup.obj" \
	"$(INTDIR)\WP3DoubleByteScriptCharacterGroup.obj" \
	"$(INTDIR)\WP3EndOfLinePageGroup.obj" \
	"$(INTDIR)\WP3ExtendedCharacterGroup.obj" \
	"$(INTDIR)\WP3FileStructure.obj" \
	"$(INTDIR)\WP3FixedLengthGroup.obj" \
	"$(INTDIR)\WP3FontGroup.obj" \
	"$(INTDIR)\WP3FootnoteEndnoteGroup.obj" \
	"$(INTDIR)\WP3Header.obj" \
	"$(INTDIR)\WP3HeaderFooterGroup.obj" \
	"$(INTDIR)\WP3IndentGroup.obj" \
	"$(INTDIR)\WP3Listener.obj" \
	"$(INTDIR)\WP3MiscellaneousGroup.obj" \
	"$(INTDIR)\WP3PageFormatGroup.obj" \
	"$(INTDIR)\WP3Parser.obj" \
	"$(INTDIR)\WP3Part.obj" \
	"$(INTDIR)\WP3Resource.obj" \
	"$(INTDIR)\WP3ResourceFork.obj" \
	"$(INTDIR)\WP3SingleByteFunction.obj" \
	"$(INTDIR)\WP3StylesListener.obj" \
	"$(INTDIR)\WP3SubDocument.obj" \
	"$(INTDIR)\WP3TabGroup.obj" \
	"$(INTDIR)\WP3TablesGroup.obj" \
	"$(INTDIR)\WP3UndoGroup.obj" \
	"$(INTDIR)\WP3UnsupportedFixedLengthGroup.obj" \
	"$(INTDIR)\WP3UnsupportedVariableLengthGroup.obj" \
	"$(INTDIR)\WP3VariableLengthGroup.obj" \
	"$(INTDIR)\WP3WindowGroup.obj" \
	"$(INTDIR)\WP42ContentListener.obj" \
	"$(INTDIR)\WP42DefineColumnsGroup.obj" \
	"$(INTDIR)\WP42ExtendedCharacterGroup.obj" \
	"$(INTDIR)\WP42FileStructure.obj" \
	"$(INTDIR)\WP42HeaderFooterGroup.obj" \
	"$(INTDIR)\WP42Heuristics.obj" \
	"$(INTDIR)\WP42Listener.obj" \
	"$(INTDIR)\WP42MarginResetGroup.obj" \
	"$(INTDIR)\WP42MultiByteFunctionGroup.obj" \
	"$(INTDIR)\WP42Parser.obj" \
	"$(INTDIR)\WP42Part.obj" \
	"$(INTDIR)\WP42StylesListener.obj" \
	"$(INTDIR)\WP42SubDocument.obj" \
	"$(INTDIR)\WP42SuppressPageCharacteristicsGroup.obj" \
	"$(INTDIR)\WP42UnsupportedMultiByteFunctionGroup.obj" \
	"$(INTDIR)\WP5AttributeGroup.obj" \
	"$(INTDIR)\WP5BoxGroup.obj" \
	"$(INTDIR)\WP5ContentListener.obj" \
	"$(INTDIR)\WP5DefinitionGroup.obj" \
	"$(INTDIR)\WP5ExtendedCharacterGroup.obj" \
	"$(INTDIR)\WP5FileStructure.obj" \
	"$(INTDIR)\WP5FixedLengthGroup.obj" \
	"$(INTDIR)\WP5FontGroup.obj" \
	"$(INTDIR)\WP5FontNameStringPoolPacket.obj" \
	"$(INTDIR)\WP5FootnoteEndnoteGroup.obj" \
	"$(INTDIR)\WP5GeneralPacketData.obj" \
	"$(INTDIR)\WP5GeneralPacketIndex.obj" \
	"$(INTDIR)\WP5GraphicsInformationPacket.obj" \
	"$(INTDIR)\WP5Header.obj" \
	"$(INTDIR)\WP5HeaderFooterGroup.obj" \
	"$(INTDIR)\WP5IndentGroup.obj" \
	"$(INTDIR)\WP5Listener.obj" \
	"$(INTDIR)\WP5ListFontsUsedPacket.obj" \
	"$(INTDIR)\WP5PageFormatGroup.obj" \
	"$(INTDIR)\WP5Parser.obj" \
	"$(INTDIR)\WP5Part.obj" \
	"$(INTDIR)\WP5PrefixData.obj" \
	"$(INTDIR)\WP5SingleByteFunction.obj" \
	"$(INTDIR)\WP5SpecialHeaderIndex.obj" \
	"$(INTDIR)\WP5StylesListener.obj" \
	"$(INTDIR)\WP5SubDocument.obj" \
	"$(INTDIR)\WP5TabGroup.obj" \
	"$(INTDIR)\WP5TableEOLGroup.obj" \
	"$(INTDIR)\WP5TableEOPGroup.obj" \
	"$(INTDIR)\WP5UnsupportedFixedLengthGroup.obj" \
	"$(INTDIR)\WP5UnsupportedVariableLengthGroup.obj" \
	"$(INTDIR)\WP5VariableLengthGroup.obj" \
	"$(INTDIR)\WP60Header.obj" \
	"$(INTDIR)\WP61Header.obj" \
	"$(INTDIR)\WP6AttributeGroup.obj" \
	"$(INTDIR)\WP6BoxGroup.obj" \
	"$(INTDIR)\WP6CharacterGroup.obj" \
	"$(INTDIR)\WP6ColumnGroup.obj" \
	"$(INTDIR)\WP6CommentAnnotationPacket.obj" \
	"$(INTDIR)\WP6ContentListener.obj" \
	"$(INTDIR)\WP6DefaultInitialFontPacket.obj" \
	"$(INTDIR)\WP6DisplayNumberReferenceGroup.obj" \
	"$(INTDIR)\WP6EOLGroup.obj" \
	"$(INTDIR)\WP6ExtendedCharacterGroup.obj" \
	"$(INTDIR)\WP6ExtendedDocumentSummaryPacket.obj" \
	"$(INTDIR)\WP6FileStructure.obj" \
	"$(INTDIR)\WP6FillStylePacket.obj" \
	"$(INTDIR)\WP6FixedLengthGroup.obj" \
	"$(INTDIR)\WP6FontDescriptorPacket.obj" \
	"$(INTDIR)\WP6FootnoteEndnoteGroup.obj" \
	"$(INTDIR)\WP6GeneralTextPacket.obj" \
	"$(INTDIR)\WP6GraphicsBoxStylePacket.obj" \
	"$(INTDIR)\WP6GraphicsCachedFileDataPacket.obj" \
	"$(INTDIR)\WP6GraphicsFilenamePacket.obj" \
	"$(INTDIR)\WP6Header.obj" \
	"$(INTDIR)\WP6HeaderFooterGroup.obj" \
	"$(INTDIR)\WP6HighlightGroup.obj" \
	"$(INTDIR)\WP6Listener.obj" \
	"$(INTDIR)\WP6NumberingMethodGroup.obj" \
	"$(INTDIR)\WP6OutlineStylePacket.obj" \
	"$(INTDIR)\WP6PageGroup.obj" \
	"$(INTDIR)\WP6ParagraphGroup.obj" \
	"$(INTDIR)\WP6Parser.obj" \
	"$(INTDIR)\WP6Part.obj" \
	"$(INTDIR)\WP6PrefixData.obj" \
	"$(INTDIR)\WP6PrefixDataPacket.obj" \
	"$(INTDIR)\WP6PrefixIndice.obj" \
	"$(INTDIR)\WP6SetNumberGroup.obj" \
	"$(INTDIR)\WP6SingleByteFunction.obj" \
	"$(INTDIR)\WP6StyleGroup.obj" \
	"$(INTDIR)\WP6StylesListener.obj" \
	"$(INTDIR)\WP6SubDocument.obj" \
	"$(INTDIR)\WP6TabGroup.obj" \
	"$(INTDIR)\WP6TableStylePacket.obj" \
	"$(INTDIR)\WP6UndoGroup.obj" \
	"$(INTDIR)\WP6UnsupportedFixedLengthGroup.obj" \
	"$(INTDIR)\WP6UnsupportedVariableLengthGroup.obj" \
	"$(INTDIR)\WP6VariableLengthGroup.obj" \
	"$(INTDIR)\WPDocument.obj" \
	"$(INTDIR)\WPXBinaryData.obj" \
	"$(INTDIR)\WPXContentListener.obj" \
	"$(INTDIR)\WPXEncryption.obj" \
	"$(INTDIR)\WPXHeader.obj" \
	"$(INTDIR)\WPXListener.obj" \
	"$(INTDIR)\WPXMemoryStream.obj" \
	"$(INTDIR)\WPXPageSpan.obj" \
	"$(INTDIR)\WPXParser.obj" \
	"$(INTDIR)\WPXProperty.obj" \
	"$(INTDIR)\WPXPropertyList.obj" \
	"$(INTDIR)\WPXPropertyListVector.obj" \
	"$(INTDIR)\WPXString.obj" \
	"$(INTDIR)\WPXStylesListener.obj" \
	"$(INTDIR)\WPXSubDocument.obj" \
	"$(INTDIR)\WPXTable.obj"

"$(OUTDIR)\lib\libwpd-0.9.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("libwpd.dep")
!INCLUDE "libwpd.dep"
!ELSE 
!MESSAGE Warning: cannot find "libwpd.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "libwpd - Win32 Release" || "$(CFG)" == "libwpd - Win32 Debug"
SOURCE=..\..\src\lib\libwpd_internal.cpp

"$(INTDIR)\libwpd_internal.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\libwpd_math.cpp

"$(INTDIR)\libwpd_math.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1BottomMarginGroup.cpp

"$(INTDIR)\WP1BottomMarginGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1CenterTextGroup.cpp

"$(INTDIR)\WP1CenterTextGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1ContentListener.cpp

"$(INTDIR)\WP1ContentListener.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1ExtendedCharacterGroup.cpp

"$(INTDIR)\WP1ExtendedCharacterGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1FileStructure.cpp

"$(INTDIR)\WP1FileStructure.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1FixedLengthGroup.cpp

"$(INTDIR)\WP1FixedLengthGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1FlushRightGroup.cpp

"$(INTDIR)\WP1FlushRightGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1FontIdGroup.cpp

"$(INTDIR)\WP1FontIdGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1FootnoteEndnoteGroup.cpp

"$(INTDIR)\WP1FootnoteEndnoteGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1HeaderFooterGroup.cpp

"$(INTDIR)\WP1HeaderFooterGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1Heuristics.cpp

"$(INTDIR)\WP1Heuristics.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1JustificationGroup.cpp

"$(INTDIR)\WP1JustificationGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1LeftIndentGroup.cpp

"$(INTDIR)\WP1LeftIndentGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1LeftRightIndentGroup.cpp

"$(INTDIR)\WP1LeftRightIndentGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1Listener.cpp

"$(INTDIR)\WP1Listener.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1MarginReleaseGroup.cpp

"$(INTDIR)\WP1MarginReleaseGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1MarginResetGroup.cpp

"$(INTDIR)\WP1MarginResetGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1Parser.cpp

"$(INTDIR)\WP1Parser.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1Part.cpp

"$(INTDIR)\WP1Part.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1PictureGroup.cpp

"$(INTDIR)\WP1PictureGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1PointSizeGroup.cpp

"$(INTDIR)\WP1PointSizeGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1SetTabsGroup.cpp

"$(INTDIR)\WP1SetTabsGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1SpacingResetGroup.cpp

"$(INTDIR)\WP1SpacingResetGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1StylesListener.cpp

"$(INTDIR)\WP1StylesListener.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1SubDocument.cpp

"$(INTDIR)\WP1SubDocument.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1SuppressPageCharacteristicsGroup.cpp

"$(INTDIR)\WP1SuppressPageCharacteristicsGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1TopMarginGroup.cpp

"$(INTDIR)\WP1TopMarginGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1UnsupportedFixedLengthGroup.cpp

"$(INTDIR)\WP1UnsupportedFixedLengthGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1UnsupportedVariableLengthGroup.cpp

"$(INTDIR)\WP1UnsupportedVariableLengthGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP1VariableLengthGroup.cpp

"$(INTDIR)\WP1VariableLengthGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3AttributeGroup.cpp

"$(INTDIR)\WP3AttributeGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3ContentListener.cpp

"$(INTDIR)\WP3ContentListener.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3DefinitionGroup.cpp

"$(INTDIR)\WP3DefinitionGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3DisplayGroup.cpp

"$(INTDIR)\WP3DisplayGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3DoubleByteScriptCharacterGroup.cpp

"$(INTDIR)\WP3DoubleByteScriptCharacterGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3EndOfLinePageGroup.cpp

"$(INTDIR)\WP3EndOfLinePageGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3ExtendedCharacterGroup.cpp

"$(INTDIR)\WP3ExtendedCharacterGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3FileStructure.cpp

"$(INTDIR)\WP3FileStructure.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3FixedLengthGroup.cpp

"$(INTDIR)\WP3FixedLengthGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3FontGroup.cpp

"$(INTDIR)\WP3FontGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3FootnoteEndnoteGroup.cpp

"$(INTDIR)\WP3FootnoteEndnoteGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3Header.cpp

"$(INTDIR)\WP3Header.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3HeaderFooterGroup.cpp

"$(INTDIR)\WP3HeaderFooterGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3IndentGroup.cpp

"$(INTDIR)\WP3IndentGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3Listener.cpp

"$(INTDIR)\WP3Listener.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3MiscellaneousGroup.cpp

"$(INTDIR)\WP3MiscellaneousGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3PageFormatGroup.cpp

"$(INTDIR)\WP3PageFormatGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3Parser.cpp

"$(INTDIR)\WP3Parser.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3Part.cpp

"$(INTDIR)\WP3Part.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3Resource.cpp

"$(INTDIR)\WP3Resource.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3ResourceFork.cpp

"$(INTDIR)\WP3ResourceFork.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3SingleByteFunction.cpp

"$(INTDIR)\WP3SingleByteFunction.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3StylesListener.cpp

"$(INTDIR)\WP3StylesListener.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3SubDocument.cpp

"$(INTDIR)\WP3SubDocument.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3TabGroup.cpp

"$(INTDIR)\WP3TabGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3TablesGroup.cpp

"$(INTDIR)\WP3TablesGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3UndoGroup.cpp

"$(INTDIR)\WP3UndoGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3UnsupportedFixedLengthGroup.cpp

"$(INTDIR)\WP3UnsupportedFixedLengthGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3UnsupportedVariableLengthGroup.cpp

"$(INTDIR)\WP3UnsupportedVariableLengthGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3VariableLengthGroup.cpp

"$(INTDIR)\WP3VariableLengthGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP3WindowGroup.cpp

"$(INTDIR)\WP3WindowGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP42ContentListener.cpp

"$(INTDIR)\WP42ContentListener.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP42DefineColumnsGroup.cpp

"$(INTDIR)\WP42DefineColumnsGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP42ExtendedCharacterGroup.cpp

"$(INTDIR)\WP42ExtendedCharacterGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP42FileStructure.cpp

"$(INTDIR)\WP42FileStructure.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP42HeaderFooterGroup.cpp

"$(INTDIR)\WP42HeaderFooterGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP42Heuristics.cpp

"$(INTDIR)\WP42Heuristics.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP42Listener.cpp

"$(INTDIR)\WP42Listener.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP42MarginResetGroup.cpp

"$(INTDIR)\WP42MarginResetGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP42MultiByteFunctionGroup.cpp

"$(INTDIR)\WP42MultiByteFunctionGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP42Parser.cpp

"$(INTDIR)\WP42Parser.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP42Part.cpp

"$(INTDIR)\WP42Part.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP42StylesListener.cpp

"$(INTDIR)\WP42StylesListener.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP42SubDocument.cpp

"$(INTDIR)\WP42SubDocument.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP42SuppressPageCharacteristicsGroup.cpp

"$(INTDIR)\WP42SuppressPageCharacteristicsGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP42UnsupportedMultiByteFunctionGroup.cpp

"$(INTDIR)\WP42UnsupportedMultiByteFunctionGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5AttributeGroup.cpp

"$(INTDIR)\WP5AttributeGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5BoxGroup.cpp

"$(INTDIR)\WP5BoxGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5ContentListener.cpp

"$(INTDIR)\WP5ContentListener.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5DefinitionGroup.cpp

"$(INTDIR)\WP5DefinitionGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5ExtendedCharacterGroup.cpp

"$(INTDIR)\WP5ExtendedCharacterGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5FileStructure.cpp

"$(INTDIR)\WP5FileStructure.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5FixedLengthGroup.cpp

"$(INTDIR)\WP5FixedLengthGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5FontGroup.cpp

"$(INTDIR)\WP5FontGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5FontNameStringPoolPacket.cpp

"$(INTDIR)\WP5FontNameStringPoolPacket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5FootnoteEndnoteGroup.cpp

"$(INTDIR)\WP5FootnoteEndnoteGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5GeneralPacketData.cpp

"$(INTDIR)\WP5GeneralPacketData.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5GeneralPacketIndex.cpp

"$(INTDIR)\WP5GeneralPacketIndex.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5GraphicsInformationPacket.cpp

"$(INTDIR)\WP5GraphicsInformationPacket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5Header.cpp

"$(INTDIR)\WP5Header.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5HeaderFooterGroup.cpp

"$(INTDIR)\WP5HeaderFooterGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5IndentGroup.cpp

"$(INTDIR)\WP5IndentGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5Listener.cpp

"$(INTDIR)\WP5Listener.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5ListFontsUsedPacket.cpp

"$(INTDIR)\WP5ListFontsUsedPacket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5PageFormatGroup.cpp

"$(INTDIR)\WP5PageFormatGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5Parser.cpp

"$(INTDIR)\WP5Parser.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5Part.cpp

"$(INTDIR)\WP5Part.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5PrefixData.cpp

"$(INTDIR)\WP5PrefixData.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5SingleByteFunction.cpp

"$(INTDIR)\WP5SingleByteFunction.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5SpecialHeaderIndex.cpp

"$(INTDIR)\WP5SpecialHeaderIndex.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5StylesListener.cpp

"$(INTDIR)\WP5StylesListener.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5SubDocument.cpp

"$(INTDIR)\WP5SubDocument.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5TabGroup.cpp

"$(INTDIR)\WP5TabGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5TableEOLGroup.cpp

"$(INTDIR)\WP5TableEOLGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5TableEOPGroup.cpp

"$(INTDIR)\WP5TableEOPGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5UnsupportedFixedLengthGroup.cpp

"$(INTDIR)\WP5UnsupportedFixedLengthGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5UnsupportedVariableLengthGroup.cpp

"$(INTDIR)\WP5UnsupportedVariableLengthGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP5VariableLengthGroup.cpp

"$(INTDIR)\WP5VariableLengthGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP60Header.cpp

"$(INTDIR)\WP60Header.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP61Header.cpp

"$(INTDIR)\WP61Header.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6AttributeGroup.cpp

"$(INTDIR)\WP6AttributeGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6BoxGroup.cpp

"$(INTDIR)\WP6BoxGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6CharacterGroup.cpp

"$(INTDIR)\WP6CharacterGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6ColumnGroup.cpp

"$(INTDIR)\WP6ColumnGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6CommentAnnotationPacket.cpp

"$(INTDIR)\WP6CommentAnnotationPacket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6ContentListener.cpp

"$(INTDIR)\WP6ContentListener.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6DefaultInitialFontPacket.cpp

"$(INTDIR)\WP6DefaultInitialFontPacket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6DisplayNumberReferenceGroup.cpp

"$(INTDIR)\WP6DisplayNumberReferenceGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6EOLGroup.cpp

"$(INTDIR)\WP6EOLGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6ExtendedCharacterGroup.cpp

"$(INTDIR)\WP6ExtendedCharacterGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6ExtendedDocumentSummaryPacket.cpp

"$(INTDIR)\WP6ExtendedDocumentSummaryPacket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6FileStructure.cpp

"$(INTDIR)\WP6FileStructure.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6FillStylePacket.cpp

"$(INTDIR)\WP6FillStylePacket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6FixedLengthGroup.cpp

"$(INTDIR)\WP6FixedLengthGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6FontDescriptorPacket.cpp

"$(INTDIR)\WP6FontDescriptorPacket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6FootnoteEndnoteGroup.cpp

"$(INTDIR)\WP6FootnoteEndnoteGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6GeneralTextPacket.cpp

"$(INTDIR)\WP6GeneralTextPacket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6GraphicsBoxStylePacket.cpp

"$(INTDIR)\WP6GraphicsBoxStylePacket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6GraphicsCachedFileDataPacket.cpp

"$(INTDIR)\WP6GraphicsCachedFileDataPacket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6GraphicsFilenamePacket.cpp

"$(INTDIR)\WP6GraphicsFilenamePacket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6Header.cpp

"$(INTDIR)\WP6Header.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6HeaderFooterGroup.cpp

"$(INTDIR)\WP6HeaderFooterGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6HighlightGroup.cpp

"$(INTDIR)\WP6HighlightGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6Listener.cpp

"$(INTDIR)\WP6Listener.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6NumberingMethodGroup.cpp

"$(INTDIR)\WP6NumberingMethodGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6OutlineStylePacket.cpp

"$(INTDIR)\WP6OutlineStylePacket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6PageGroup.cpp

"$(INTDIR)\WP6PageGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6ParagraphGroup.cpp

"$(INTDIR)\WP6ParagraphGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6Parser.cpp

"$(INTDIR)\WP6Parser.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6Part.cpp

"$(INTDIR)\WP6Part.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6PrefixData.cpp

"$(INTDIR)\WP6PrefixData.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6PrefixDataPacket.cpp

"$(INTDIR)\WP6PrefixDataPacket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6PrefixIndice.cpp

"$(INTDIR)\WP6PrefixIndice.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6SetNumberGroup.cpp

"$(INTDIR)\WP6SetNumberGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6SingleByteFunction.cpp

"$(INTDIR)\WP6SingleByteFunction.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6StyleGroup.cpp

"$(INTDIR)\WP6StyleGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6StylesListener.cpp

"$(INTDIR)\WP6StylesListener.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6SubDocument.cpp

"$(INTDIR)\WP6SubDocument.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6TabGroup.cpp

"$(INTDIR)\WP6TabGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6TableStylePacket.cpp

"$(INTDIR)\WP6TableStylePacket.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6UndoGroup.cpp

"$(INTDIR)\WP6UndoGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6UnsupportedFixedLengthGroup.cpp

"$(INTDIR)\WP6UnsupportedFixedLengthGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6UnsupportedVariableLengthGroup.cpp

"$(INTDIR)\WP6UnsupportedVariableLengthGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WP6VariableLengthGroup.cpp

"$(INTDIR)\WP6VariableLengthGroup.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WPDocument.cpp

"$(INTDIR)\WPDocument.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WPXBinaryData.cpp

"$(INTDIR)\WPXBinaryData.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WPXContentListener.cpp

"$(INTDIR)\WPXContentListener.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WPXEncryption.cpp

"$(INTDIR)\WPXEncryption.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WPXHeader.cpp

"$(INTDIR)\WPXHeader.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WPXListener.cpp

"$(INTDIR)\WPXListener.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WPXMemoryStream.cpp

"$(INTDIR)\WPXMemoryStream.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WPXPageSpan.cpp

"$(INTDIR)\WPXPageSpan.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WPXParser.cpp

"$(INTDIR)\WPXParser.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WPXProperty.cpp

"$(INTDIR)\WPXProperty.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WPXPropertyList.cpp

"$(INTDIR)\WPXPropertyList.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WPXPropertyListVector.cpp

"$(INTDIR)\WPXPropertyListVector.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WPXString.cpp

"$(INTDIR)\WPXString.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WPXStylesListener.cpp

"$(INTDIR)\WPXStylesListener.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WPXSubDocument.cpp

"$(INTDIR)\WPXSubDocument.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\lib\WPXTable.cpp

"$(INTDIR)\WPXTable.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

