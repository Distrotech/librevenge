# Microsoft Developer Studio Generated NMAKE File, Based on wpd2text.dsp
!IF "$(CFG)" == ""
CFG=wpd2text - Win32 Release
!MESSAGE No configuration specified. Defaulting to wpd2text - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "wpd2text - Win32 Release" && "$(CFG)" != "wpd2text - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wpd2text.mak" CFG="wpd2text - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wpd2text - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "wpd2text - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "wpd2text - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\bin\wpd2text.exe"

!ELSE 

ALL : "libwpdstream - Win32 Release" "libwpd - Win32 Release" "$(OUTDIR)\bin\wpd2text.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"libwpd - Win32 ReleaseCLEAN" "libwpdstream - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\TextDocumentGenerator.obj"
	-@erase "$(INTDIR)\wpd2text.obj"
	-@erase "$(OUTDIR)\bin\wpd2text.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /I "..\..\src\lib" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /Fo"$(INTDIR)\\" /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\wpd2text.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=libwpd-stream-0.9.lib libwpd-0.9.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\wpd2text.pdb" /machine:IX86 /out:"$(OUTDIR)\bin\wpd2text.exe" /libpath:"Release\lib" 
LINK32_OBJS= \
	"$(INTDIR)\TextDocumentGenerator.obj" \
	"$(INTDIR)\wpd2text.obj" \
	"$(OUTDIR)\lib\libwpd-0.9.lib" \
	"$(OUTDIR)\lib\libwpd-stream-0.9.lib"

"$(OUTDIR)\bin\wpd2text.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "wpd2text - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\bin\wpd2text.exe"

!ELSE 

ALL : "libwpdstream - Win32 Debug" "libwpd - Win32 Debug" "$(OUTDIR)\bin\wpd2text.exe"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"libwpd - Win32 DebugCLEAN" "libwpdstream - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\TextDocumentGenerator.obj"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\wpd2text.obj"
	-@erase "$(OUTDIR)\bin\wpd2text.exe"
	-@erase "$(OUTDIR)\bin\wpd2text.ilk"
	-@erase "$(OUTDIR)\wpd2text.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

MTL=midl.exe
CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /GX /ZI /Od /I "..\..\src\lib" /D "_DEBUG" /D "DEBUG" /D "WIN32" /D "_CONSOLE" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\wpd2text.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=libwpd-0.9.lib libwpd-stream-0.9.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\wpd2text.pdb" /debug /machine:IX86 /out:"$(OUTDIR)\bin\wpd2text.exe" /libpath:"Debug\lib" 
LINK32_OBJS= \
	"$(INTDIR)\TextDocumentGenerator.obj" \
	"$(INTDIR)\wpd2text.obj" \
	"$(OUTDIR)\lib\libwpd-0.9.lib" \
	"$(OUTDIR)\lib\libwpd-stream-0.9.lib"

"$(OUTDIR)\bin\wpd2text.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("wpd2text.dep")
!INCLUDE "wpd2text.dep"
!ELSE 
!MESSAGE Warning: cannot find "wpd2text.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "wpd2text - Win32 Release" || "$(CFG)" == "wpd2text - Win32 Debug"
SOURCE=..\..\src\conv\text\TextDocumentGenerator.cpp

"$(INTDIR)\TextDocumentGenerator.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\..\src\conv\text\wpd2text.cpp

"$(INTDIR)\wpd2text.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!IF  "$(CFG)" == "wpd2text - Win32 Release"

"libwpd - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\libwpd.mak" CFG="libwpd - Win32 Release" 
   cd "."

"libwpd - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\libwpd.mak" CFG="libwpd - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "wpd2text - Win32 Debug"

"libwpd - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\libwpd.mak" CFG="libwpd - Win32 Debug" 
   cd "."

"libwpd - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\libwpd.mak" CFG="libwpd - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 

!IF  "$(CFG)" == "wpd2text - Win32 Release"

"libwpdstream - Win32 Release" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\libwpdstream.mak" CFG="libwpdstream - Win32 Release" 
   cd "."

"libwpdstream - Win32 ReleaseCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\libwpdstream.mak" CFG="libwpdstream - Win32 Release" RECURSE=1 CLEAN 
   cd "."

!ELSEIF  "$(CFG)" == "wpd2text - Win32 Debug"

"libwpdstream - Win32 Debug" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\libwpdstream.mak" CFG="libwpdstream - Win32 Debug" 
   cd "."

"libwpdstream - Win32 DebugCLEAN" : 
   cd "."
   $(MAKE) /$(MAKEFLAGS) /F ".\libwpdstream.mak" CFG="libwpdstream - Win32 Debug" RECURSE=1 CLEAN 
   cd "."

!ENDIF 


!ENDIF 

