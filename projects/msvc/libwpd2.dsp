# Microsoft Developer Studio Project File - Name="libwpd2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libwpd2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libwpd2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libwpd2.mak" CFG="libwpd2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libwpd2 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libwpd2 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libwpd2 - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\..\..\include\glib-2.0" /I "..\..\..\lib\glib-2.0\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libwpd2 - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\include\glib-2.0" /I "..\..\..\lib\glib-2.0\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "libwpd2 - Win32 Release"
# Name "libwpd2 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
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

SOURCE=..\..\src\lib\WP6EOLGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6ExtendedCharacterGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6FixedEOLGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6FixedLengthGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6FixedSpaceGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6Header.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6HLListener.cpp
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

SOURCE=..\..\src\lib\WP6UndoGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6UnsupportedVariableLengthGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6VariableLengthGroup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXHeader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXParser.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\src\lib\libwpd2.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\UT_libwpd2.h
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

SOURCE=..\..\src\lib\WP6EOLGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6ExtendedCharacterGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6FileStructure.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6FixedEOLGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6FixedLengthGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6FixedSpaceGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6Header.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6HLListener.h
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

SOURCE=..\..\src\lib\WP6UndoGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6UnsupportedVariableLengthGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WP6VariableLengthGroup.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXFileStructure.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXHeader.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXHLListener.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXHLListenerImpl.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXLLListener.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\WPXParser.h
# End Source File
# End Group
# End Target
# End Project
