# Microsoft Developer Studio Generated NMAKE File, Based on lsdsk.dsp
!IF "$(CFG)" == ""
CFG=lsdsk - Win32 Debug
!MESSAGE No configuration specified. Defaulting to lsdsk - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "lsdsk - Win32 Release" && "$(CFG)" != "lsdsk - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "lsdsk.mak" CFG="lsdsk - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "lsdsk - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "lsdsk - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "lsdsk - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\lsdsk.exe"


CLEAN :
	-@erase "$(INTDIR)\lsdsk.obj"
	-@erase "$(INTDIR)\lsdsk.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\lsdsk.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\lsdsk.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\lsdsk.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\lsdsk.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=shared.lib Setupapi.lib netapi32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\lsdsk.pdb" /machine:I386 /out:"$(OUTDIR)\lsdsk.exe" 
LINK32_OBJS= \
	"$(INTDIR)\lsdsk.obj" \
	"$(INTDIR)\lsdsk.res"

"$(OUTDIR)\lsdsk.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "lsdsk - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\lsdsk.exe" "$(OUTDIR)\lsdsk.bsc"


CLEAN :
	-@erase "$(INTDIR)\lsdsk.obj"
	-@erase "$(INTDIR)\lsdsk.res"
	-@erase "$(INTDIR)\lsdsk.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\lsdsk.bsc"
	-@erase "$(OUTDIR)\lsdsk.exe"
	-@erase "$(OUTDIR)\lsdsk.ilk"
	-@erase "$(OUTDIR)\lsdsk.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\lsdsk.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\lsdsk.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\lsdsk.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\lsdsk.sbr"

"$(OUTDIR)\lsdsk.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=sharedd.lib Setupapi.lib netapi32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\lsdsk.pdb" /debug /machine:I386 /out:"$(OUTDIR)\lsdsk.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\lsdsk.obj" \
	"$(INTDIR)\lsdsk.res"

"$(OUTDIR)\lsdsk.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("lsdsk.dep")
!INCLUDE "lsdsk.dep"
!ELSE 
!MESSAGE Warning: cannot find "lsdsk.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "lsdsk - Win32 Release" || "$(CFG)" == "lsdsk - Win32 Debug"
SOURCE=.\lsdsk.cpp

!IF  "$(CFG)" == "lsdsk - Win32 Release"


"$(INTDIR)\lsdsk.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "lsdsk - Win32 Debug"


"$(INTDIR)\lsdsk.obj"	"$(INTDIR)\lsdsk.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\lsdsk.rc

"$(INTDIR)\lsdsk.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

