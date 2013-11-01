# Microsoft Developer Studio Project File - Name="librevenge" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=librevenge - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "librevenge.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "librevenge.mak" CFG="librevenge - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "librevenge - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "librevenge - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "librevenge - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /I "..\..\inc" /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_CRT_SECURE_NO_WARNINGS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "..\..\inc" /D "NDEBUG" /D "WIN32" /D "_LIB" /D "_CRT_SECURE_NO_WARNINGS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release\lib\librevenge-0.0.lib"

!ELSEIF  "$(CFG)" == "librevenge - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\inc" /D "WIN32" /D "_DEBUG" /D "DEBUG" /D "_LIB" /D "_CRT_SECURE_NO_WARNINGS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /GR /GX /ZI /Od /I "..\..\inc" /D "_DEBUG" /D "DEBUG" /D "WIN32" /D "_LIB" /D "_CRT_SECURE_NO_WARNINGS" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\lib\librevenge-0.0.lib"

!ENDIF 

# Begin Target

# Name "librevenge - Win32 Release"
# Name "librevenge - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\lib\RVNGBinaryData.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\RVNGMemoryStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\RVNGProperty.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\RVNGPropertyList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\RVNGPropertyListVector.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\RVNGString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\RVNGStringVector.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\inc\librevenge\librevenge.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\librevenge_internal.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\librevenge\RVNGBinaryData.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\librevenge\RVNGDrawingInterface.h
# End Source File
# Begin Source File

SOURCE=..\..\src\lib\RVNGMemoryStream.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\librevenge\RVNGPresentationInterface.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\librevenge\RVNGProperty.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\librevenge\RVNGPropertyList.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\librevenge\RVNGPropertyListVector.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\librevenge\RVNGString.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\librevenge\RVNGStringVector.h
# End Source File
# Begin Source File

SOURCE=..\..\inc\librevenge\RVNGTextInterface.h
# End Source File
# End Group
# End Target
# End Project
