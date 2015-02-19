.PHONY:	all clean realclean

all:
	$(MAKE) /f lsdsk.mak CFG="lsdsk - Win32 Release"	all
	$(MAKE) /f lsdsk.mak CFG="lsdsk - Win32 Debug"		all

clean:
	$(MAKE) /f lsdsk.mak CFG="lsdsk - Win32 Release"	clean
	$(MAKE) /f lsdsk.mak CFG="lsdsk - Win32 Debug"		clean

realclean: clean
	-cmd /c del /s *.bak *.bsc *.idb *.pdb *.lib *.ncb *.obj *.opt *.pch *.plg *.sbr

