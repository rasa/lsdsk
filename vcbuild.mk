all:
	VCBuild.exe /nologo lsdsk.vcproj /rebuild

clean:
	VCBuild.exe /nologo lsdsk.vcproj /clean
	
upgrade:
	devenv lsdsk.sln /upgrade

.PHONY:	all clean upgrade

