EXTERNAL_WARNINGS_NOT_ERRORS := TRUE

PRJ=..$/..$/..$/..$/..$/..

PRJNAME=libwpd
TARGET=wpdlib
ENABLE_EXCEPTIONS=TRUE
LIBTARGET=NO

.INCLUDE :  settings.mk

.IF "$(GUI)$(COM)"=="WNTMSC"
CFLAGS+=-GR
.ENDIF
.IF "$(COM)"=="GCC"
CFLAGSCXX+=-frtti
.ENDIF

SLOFILES= \
	$(SLO)$/libwpd_internal.obj \
	$(SLO)$/libwpd_math.obj \
	$(SLO)$/WP1BottomMarginGroup.obj \
	$(SLO)$/WP1CenterTextGroup.obj \
	$(SLO)$/WP1ContentListener.obj \
	$(SLO)$/WP1ExtendedCharacterGroup.obj \
	$(SLO)$/WP1FileStructure.obj \
	$(SLO)$/WP1FixedLengthGroup.obj \
	$(SLO)$/WP1FlushRightGroup.obj \
	$(SLO)$/WP1FontIdGroup.obj \
	$(SLO)$/WP1FootnoteEndnoteGroup.obj \
	$(SLO)$/WP1HeaderFooterGroup.obj \
	$(SLO)$/WP1Heuristics.obj \
	$(SLO)$/WP1JustificationGroup.obj \
	$(SLO)$/WP1LeftIndentGroup.obj \
	$(SLO)$/WP1LeftRightIndentGroup.obj \
	$(SLO)$/WP1Listener.obj \
	$(SLO)$/WP1MarginReleaseGroup.obj \
	$(SLO)$/WP1MarginResetGroup.obj \
	$(SLO)$/WP1Parser.obj \
	$(SLO)$/WP1Part.obj \
	$(SLO)$/WP1PictureGroup.obj \
	$(SLO)$/WP1PointSizeGroup.obj \
	$(SLO)$/WP1SetTabsGroup.obj \
	$(SLO)$/WP1SpacingResetGroup.obj \
	$(SLO)$/WP1StylesListener.obj \
	$(SLO)$/WP1SubDocument.obj \
	$(SLO)$/WP1SuppressPageCharacteristicsGroup.obj \
	$(SLO)$/WP1TopMarginGroup.obj \
	$(SLO)$/WP1UnsupportedFixedLengthGroup.obj \
	$(SLO)$/WP1UnsupportedVariableLengthGroup.obj \
	$(SLO)$/WP1VariableLengthGroup.obj \
	$(SLO)$/WP3AttributeGroup.obj \
	$(SLO)$/WP3ContentListener.obj \
	$(SLO)$/WP3DefinitionGroup.obj \
	$(SLO)$/WP3DisplayGroup.obj \
	$(SLO)$/WP3DoubleByteScriptCharacterGroup.obj \
	$(SLO)$/WP3EndOfLinePageGroup.obj \
	$(SLO)$/WP3ExtendedCharacterGroup.obj \
	$(SLO)$/WP3FileStructure.obj \
	$(SLO)$/WP3FixedLengthGroup.obj \
	$(SLO)$/WP3FontGroup.obj \
	$(SLO)$/WP3FootnoteEndnoteGroup.obj \
	$(SLO)$/WP3Header.obj \
	$(SLO)$/WP3HeaderFooterGroup.obj \
	$(SLO)$/WP3IndentGroup.obj \
	$(SLO)$/WP3Listener.obj \
	$(SLO)$/WP3MiscellaneousGroup.obj \
	$(SLO)$/WP3PageFormatGroup.obj \
	$(SLO)$/WP3Parser.obj \
	$(SLO)$/WP3Part.obj \
	$(SLO)$/WP3Resource.obj \
	$(SLO)$/WP3ResourceFork.obj \
	$(SLO)$/WP3SingleByteFunction.obj \
	$(SLO)$/WP3StylesListener.obj \
	$(SLO)$/WP3SubDocument.obj \
	$(SLO)$/WP3TabGroup.obj \
	$(SLO)$/WP3TablesGroup.obj \
	$(SLO)$/WP3UndoGroup.obj \
	$(SLO)$/WP3UnsupportedFixedLengthGroup.obj \
	$(SLO)$/WP3UnsupportedVariableLengthGroup.obj \
	$(SLO)$/WP3VariableLengthGroup.obj \
	$(SLO)$/WP3WindowGroup.obj \
	$(SLO)$/WP42ContentListener.obj \
	$(SLO)$/WP42DefineColumnsGroup.obj \
	$(SLO)$/WP42ExtendedCharacterGroup.obj \
	$(SLO)$/WP42FileStructure.obj \
	$(SLO)$/WP42HeaderFooterGroup.obj \
	$(SLO)$/WP42Heuristics.obj \
	$(SLO)$/WP42Listener.obj \
	$(SLO)$/WP42MarginResetGroup.obj \
	$(SLO)$/WP42MultiByteFunctionGroup.obj \
	$(SLO)$/WP42Parser.obj \
	$(SLO)$/WP42Part.obj \
	$(SLO)$/WP42StylesListener.obj \
	$(SLO)$/WP42SubDocument.obj \
	$(SLO)$/WP42SuppressPageCharacteristicsGroup.obj \
	$(SLO)$/WP42UnsupportedMultiByteFunctionGroup.obj \
	$(SLO)$/WP5AttributeGroup.obj \
	$(SLO)$/WP5BoxGroup.obj \
	$(SLO)$/WP5ContentListener.obj \
	$(SLO)$/WP5DefinitionGroup.obj \
	$(SLO)$/WP5ExtendedCharacterGroup.obj \
	$(SLO)$/WP5FileStructure.obj \
	$(SLO)$/WP5FixedLengthGroup.obj \
	$(SLO)$/WP5FontGroup.obj \
	$(SLO)$/WP5FontNameStringPoolPacket.obj \
	$(SLO)$/WP5FootnoteEndnoteGroup.obj \
	$(SLO)$/WP5GeneralPacketData.obj \
	$(SLO)$/WP5GeneralPacketIndex.obj \
	$(SLO)$/WP5GraphicsInformationPacket.obj \
	$(SLO)$/WP5Header.obj \
	$(SLO)$/WP5HeaderFooterGroup.obj \
	$(SLO)$/WP5IndentGroup.obj \
	$(SLO)$/WP5Listener.obj \
	$(SLO)$/WP5ListFontsUsedPacket.obj \
	$(SLO)$/WP5PageFormatGroup.obj \
	$(SLO)$/WP5Parser.obj \
	$(SLO)$/WP5Part.obj \
	$(SLO)$/WP5PrefixData.obj \
	$(SLO)$/WP5SingleByteFunction.obj \
	$(SLO)$/WP5SpecialHeaderIndex.obj \
	$(SLO)$/WP5StylesListener.obj \
	$(SLO)$/WP5SubDocument.obj \
	$(SLO)$/WP5TabGroup.obj \
	$(SLO)$/WP5TableEOLGroup.obj \
	$(SLO)$/WP5TableEOPGroup.obj \
	$(SLO)$/WP5UnsupportedFixedLengthGroup.obj \
	$(SLO)$/WP5UnsupportedVariableLengthGroup.obj \
	$(SLO)$/WP5VariableLengthGroup.obj \
	$(SLO)$/WP60Header.obj \
	$(SLO)$/WP61Header.obj \
	$(SLO)$/WP6AttributeGroup.obj \
	$(SLO)$/WP6BoxGroup.obj \
	$(SLO)$/WP6CharacterGroup.obj \
	$(SLO)$/WP6ColumnGroup.obj \
	$(SLO)$/WP6CommentAnnotationPacket.obj \
	$(SLO)$/WP6ContentListener.obj \
	$(SLO)$/WP6DefaultInitialFontPacket.obj \
	$(SLO)$/WP6DisplayNumberReferenceGroup.obj \
	$(SLO)$/WP6EOLGroup.obj \
	$(SLO)$/WP6ExtendedCharacterGroup.obj \
	$(SLO)$/WP6ExtendedDocumentSummaryPacket.obj \
	$(SLO)$/WP6FileStructure.obj \
	$(SLO)$/WP6FillStylePacket.obj \
	$(SLO)$/WP6FixedLengthGroup.obj \
	$(SLO)$/WP6FontDescriptorPacket.obj \
	$(SLO)$/WP6FootnoteEndnoteGroup.obj \
	$(SLO)$/WP6GeneralTextPacket.obj \
	$(SLO)$/WP6GraphicsBoxStylePacket.obj \
	$(SLO)$/WP6GraphicsCachedFileDataPacket.obj \
	$(SLO)$/WP6GraphicsFilenamePacket.obj \
	$(SLO)$/WP6Header.obj \
	$(SLO)$/WP6HeaderFooterGroup.obj \
	$(SLO)$/WP6HighlightGroup.obj \
	$(SLO)$/WP6Listener.obj \
	$(SLO)$/WP6NumberingMethodGroup.obj \
	$(SLO)$/WP6OutlineStylePacket.obj \
	$(SLO)$/WP6PageGroup.obj \
	$(SLO)$/WP6ParagraphGroup.obj \
	$(SLO)$/WP6Parser.obj \
	$(SLO)$/WP6Part.obj \
	$(SLO)$/WP6PrefixData.obj \
	$(SLO)$/WP6PrefixDataPacket.obj \
	$(SLO)$/WP6PrefixIndice.obj \
	$(SLO)$/WP6SetNumberGroup.obj \
	$(SLO)$/WP6SingleByteFunction.obj \
	$(SLO)$/WP6StyleGroup.obj \
	$(SLO)$/WP6StylesListener.obj \
	$(SLO)$/WP6SubDocument.obj \
	$(SLO)$/WP6TabGroup.obj \
	$(SLO)$/WP6TableStylePacket.obj \
	$(SLO)$/WP6UndoGroup.obj \
	$(SLO)$/WP6UnsupportedFixedLengthGroup.obj \
	$(SLO)$/WP6UnsupportedVariableLengthGroup.obj \
	$(SLO)$/WP6VariableLengthGroup.obj \
	$(SLO)$/WPDocument.obj \
	$(SLO)$/WPXBinaryData.obj \
	$(SLO)$/WPXContentListener.obj \
	$(SLO)$/WPXEncryption.obj \
	$(SLO)$/WPXHeader.obj \
	$(SLO)$/WPXListener.obj \
	$(SLO)$/WPXMemoryStream.obj \
	$(SLO)$/WPXPageSpan.obj \
	$(SLO)$/WPXParser.obj \
	$(SLO)$/WPXProperty.obj \
	$(SLO)$/WPXPropertyList.obj \
	$(SLO)$/WPXPropertyListVector.obj \
	$(SLO)$/WPXString.obj \
	$(SLO)$/WPXStylesListener.obj \
	$(SLO)$/WPXSubDocument.obj \
	$(SLO)$/WPXTable.obj

LIB1ARCHIV=$(LB)$/libwpdlib.a
LIB1TARGET=$(SLB)$/$(TARGET).lib
LIB1OBJFILES= $(SLOFILES)

.INCLUDE :  target.mk
