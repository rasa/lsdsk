all:
	MSBuild.exe /nologo lsdsk.sln /p:Configuration=Debug
	MSBuild.exe /nologo lsdsk.sln /p:Configuration=Release

clean:
	MSBuild.exe /nologo lsdsk.sln /p:Configuration=Debug   /t:clean
	MSBuild.exe /nologo lsdsk.sln /p:Configuration=Release /t:clean

upgrade:
	devenv lsdsk.sln /upgrade
	
.PHONY:	all clean upgrade


