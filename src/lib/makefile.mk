PRJ=..$/..$/..$/..$/..$/..

PRJNAME=libwpd
TARGET=wpdlib
ENABLE_EXCEPTIONS=TRUE
LIBTARGET=NO

.INCLUDE :  svpre.mk
.INCLUDE :  settings.mk
.INCLUDE :  sv.mk

.IF "$(GUI)"=="WNT"
CFLAGS+=-GR
.ENDIF
.IF "$(COM)"=="GCC"
CFLAGSCXX+=-frtti
.ENDIF

SLOFILES= \
	$(SLO)$/libwpd_internal.obj			\
	$(SLO)$/libwpd_math.obj				\
	$(SLO)$/WP3AttributeGroup.obj			\
	$(SLO)$/WP3EndOfLinePageGroup.obj		\
	$(SLO)$/WP3ExtendedCharacterGroup.obj		\
	$(SLO)$/WP3FileStructure.obj			\
	$(SLO)$/WP3FixedLengthGroup.obj			\
	$(SLO)$/WP3Header.obj				\
	$(SLO)$/WP3HLListener.obj			\
	$(SLO)$/WP3HLStylesListener.obj			\
	$(SLO)$/WP3MiscellaneousGroup.obj		\
	$(SLO)$/WP3PageFormatGroup.obj			\
	$(SLO)$/WP3Parser.obj				\
	$(SLO)$/WP3Part.obj	    			\
	$(SLO)$/WP3SingleByteFunction.obj		\
	$(SLO)$/WP3UndoGroup.obj			\
	$(SLO)$/WP3UnsupportedFixedLengthGroup.obj	\
	$(SLO)$/WP3UnsupportedVariableLengthGroup.obj	\
	$(SLO)$/WP3VariableLengthGroup.obj		\
	$(SLO)$/WP42FileStructure.obj			\
	$(SLO)$/WP42Heuristics.obj			\
	$(SLO)$/WP42HLListener.obj			\
	$(SLO)$/WP42HLStylesListener.obj		\
	$(SLO)$/WP42Parser.obj				\
	$(SLO)$/WP42Part.obj				\
	$(SLO)$/WP42UnsupportedVariableLengthGroup.obj	\
	$(SLO)$/WP42VariableLengthGroup.obj		\
	$(SLO)$/WP5AttributeGroup.obj 			\
	$(SLO)$/WP5ExtendedCharacterGroup.obj 		\
	$(SLO)$/WP5FileStructure.obj			\
	$(SLO)$/WP5FixedLengthGroup.obj			\
	$(SLO)$/WP5FormatGroup.obj			\
	$(SLO)$/WP5Header.obj				\
	$(SLO)$/WP5HLListener.obj			\
	$(SLO)$/WP5HLStylesListener.obj			\
	$(SLO)$/WP5PageFormatGroup.obj			\
	$(SLO)$/WP5Parser.obj				\
	$(SLO)$/WP5Part.obj				\
	$(SLO)$/WP5SingleByteFunction.obj		\
	$(SLO)$/WP5UnsupportedFixedLengthGroup.obj	\
	$(SLO)$/WP5UnsupportedVariableLengthGroup.obj	\
	$(SLO)$/WP5VariableLengthGroup.obj		\
	$(SLO)$/WP6AttributeGroup.obj 			\
	$(SLO)$/WP6CharacterGroup.obj			\
	$(SLO)$/WP6ColumnGroup.obj			\
	$(SLO)$/WP6DefaultInitialFontPacket.obj		\
	$(SLO)$/WP6DisplayNumberReferenceGroup.obj 	\
	$(SLO)$/WP6EOLGroup.obj				\
	$(SLO)$/WP6ExtendedCharacterGroup.obj 		\
	$(SLO)$/WP6ExtendedDocumentSummaryPacket.obj 	\
	$(SLO)$/WP6FileStructure.obj			\
	$(SLO)$/WP6FillStylePacket.obj 			\
	$(SLO)$/WP6FixedLengthGroup.obj			\
	$(SLO)$/WP6FontDescriptorPacket.obj		\
	$(SLO)$/WP6FootnoteEndnoteGroup.obj		\
	$(SLO)$/WP6GeneralTextPacket.obj		\
	$(SLO)$/WP6Header.obj				\
	$(SLO)$/WP60Header.obj				\
	$(SLO)$/WP61Header.obj				\
	$(SLO)$/WP6HeaderFooterGroup.obj 		\
	$(SLO)$/WP6HighlightGroup.obj			\
	$(SLO)$/WP6HLContentListener.obj		\
	$(SLO)$/WP6HLListener.obj			\
	$(SLO)$/WP6HLStylesListener.obj			\
	$(SLO)$/WP6LLListener.obj			\
	$(SLO)$/WP6Parser.obj				\
	$(SLO)$/WP6OutlineStylePacket.obj		\
	$(SLO)$/WP6PageGroup.obj			\
	$(SLO)$/WP6ParagraphGroup.obj			\
	$(SLO)$/WP6Part.obj				\
	$(SLO)$/WP6PrefixData.obj 			\
	$(SLO)$/WP6PrefixIndice.obj 			\
	$(SLO)$/WP6PrefixDataPacket.obj			\
	$(SLO)$/WP6SingleByteFunction.obj		\
	$(SLO)$/WP6StyleGroup.obj			\
	$(SLO)$/WP6TabGroup.obj 			\
	$(SLO)$/WP6UndoGroup.obj			\
	$(SLO)$/WP6UnsupportedFixedLengthGroup.obj 	\
	$(SLO)$/WP6UnsupportedVariableLengthGroup.obj 	\
	$(SLO)$/WP6VariableLengthGroup.obj 		\
	$(SLO)$/WPXPageSpan.obj				\
	$(SLO)$/WPXHeader.obj 				\
	$(SLO)$/WPXHLListener.obj 			\
	$(SLO)$/WPXMemoryStream.obj			\
	$(SLO)$/WPXTable.obj				\
	$(SLO)$/WPXParser.obj 				\
	$(SLO)$/WPXPart.obj				\
	$(SLO)$/WPXProperty.obj				\
	$(SLO)$/WPXString.obj				\
	$(SLO)$/WPDocument.obj

LIB1ARCHIV=$(LB)$/libwpdlib.a
LIB1TARGET=$(SLB)$/$(TARGET).lib
LIB1OBJFILES= $(SLOFILES)

.INCLUDE :  target.mk
