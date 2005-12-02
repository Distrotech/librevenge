# Microsoft Developer Studio Project File - Name="libwpd" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libwpd - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libwpd.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libwpd.mak" CFG="libwpd - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libwpd - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libwpd - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libwpd - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release\lib\libwpd-0.8.lib"

!ELSEIF  "$(CFG)" == "libwpd - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\lib\libwpd-0.8.lib"

!ENDIF 

# Begin Target

# Name "libwpd - Win32 Release"
# Name "libwpd - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\lib\libwpd_internal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\libwpd_math.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3AttributeGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3DefinitionGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3DisplayGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3EndOfLinePageGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3ExtendedCharacterGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3FileStructure.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3FixedLengthGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3FontGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3FootnoteEndnoteGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3Header.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3Listener.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3MiscellaneousGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3PageFormatGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3Parser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3Part.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3SingleByteFunction.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3StylesListener.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3SubDocument.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3TablesGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3UndoGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3UnsupportedFixedLengthGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3UnsupportedVariableLengthGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3VariableLengthGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP42FileStructure.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP42Heuristics.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP42Listener.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP42Parser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP42Part.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP42StylesListener.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP42UnsupportedVariableLengthGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP42VariableLengthGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5AttributeGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5DefinitionGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5ExtendedCharacterGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5FileStructure.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5FixedLengthGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5FormatGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5Header.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5Listener.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5PageFormatGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5Parser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5Part.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5SingleByteFunction.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5StylesListener.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5TableEOLGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5TableEOPGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5UnsupportedFixedLengthGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5UnsupportedVariableLengthGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5VariableLengthGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP60Header.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP61Header.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6AttributeGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6CharacterGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6ColumnGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6ContentListener.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6DefaultInitialFontPacket.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6DisplayNumberReferenceGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6EOLGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6ExtendedCharacterGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6ExtendedDocumentSummaryPacket.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6FileStructure.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6FillStylePacket.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6FixedLengthGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6FontDescriptorPacket.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6FootnoteEndnoteGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6GeneralTextPacket.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6Header.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6HeaderFooterGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6HighlightGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6Listener.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6OutlineStylePacket.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6PageGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6ParagraphGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6Parser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6Part.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6PrefixData.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6PrefixDataPacket.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6PrefixIndice.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6SingleByteFunction.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6StyleGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6StylesListener.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6SubDocument.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6TabGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6UndoGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6UnsupportedFixedLengthGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6UnsupportedVariableLengthGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6VariableLengthGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPDocument.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXHeader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXListener.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXMemoryStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXPageSpan.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXParser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXPart.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXProperty.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXPropertyList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXPropertyListVector.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXSubDocument.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXTable.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\src\lib\libwpd.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\libwpd_internal.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\libwpd_math.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\libwpd_types.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3AttributeGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3DefinitionGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3DisplayGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3EndOfLinePageGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3ExtendedCharacterGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3FileStructure.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3FixedLengthGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3FontGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3FootnoteEndnoteGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3Header.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3Listener.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3MiscellaneousGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3PageFormatGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3Parser.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3Part.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3SingleByteFunction.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3StylesListener.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3SubDocument.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3TablesGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3UndoGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3UnsupportedFixedLengthGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3UnsupportedVariableLengthGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP3VariableLengthGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP42FileStructure.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP42Heuristics.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP42Listener.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP42Parser.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP42Part.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP42StylesListener.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP42UnsupportedVariableLengthGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP42VariableLengthGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5AttributeGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5DefinitionGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5ExtendedCharacterGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5FileStructure.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5FixedLengthGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5FormatGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5Header.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5Listener.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5PageFormatGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5Parser.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5Part.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5SingleByteFunction.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5StylesListener.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5TableEOLGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5TableEOPGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5UnsupportedFixedLengthGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5UnsupportedVariableLengthGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP5VariableLengthGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP60Header.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP61Header.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6AttributeGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6CharacterGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6ColumnGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6ContentListener.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6DefaultInitialFontPacket.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6DisplayNumberReferenceGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6EOLGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6ExtendedCharacterGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6ExtendedDocumentSummaryPacket.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6FileStructure.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6FillStylePacket.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6FixedLengthGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6FontDescriptorPacket.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6FootnoteEndnoteGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6GeneralTextPacket.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6Header.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6HeaderFooterGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6HighlightGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6Listener.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6OutlineStylePacket.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6PageGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6ParagraphGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6Parser.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6Part.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6PrefixData.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6PrefixDataPacket.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6PrefixIndice.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6SingleByteFunction.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6StyleGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6StylesListener.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6SubDocument.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6TabGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6TibetanMap.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6UndoGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6UnsupportedFixedLengthGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6UnsupportedVariableLengthGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6VariableLengthGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPDocument.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXFileStructure.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXHeader.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXHLListenerImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXListener.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXMemoryStream.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXPageSpan.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXParser.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXPart.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXProperty.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXPropertyList.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXPropertyListVector.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXStream.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXString.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXSubDocument.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXTable.h
# End Source File
# End Group
# End Target
# End Project
