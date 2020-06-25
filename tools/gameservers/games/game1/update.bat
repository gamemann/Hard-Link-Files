REM This basically updates a game install and applies it to all the hard linked servers. 

@echo off

REM Start the updater.
echo Updating the base installation...
echo.
echo.
"C:\Path\To\steamcmd.exe" +runscript C:/Path/To/update.txt

REM Now relink all the servers.
echo Relinking servers...\n
for /F "tokens=*" %%A in (../csgo.txt) do (
	echo Removing files from %%A...
	echo.
	"C:\Path\To\HardLinkFiles.exe" --name %%A --config C:\Path\To\config.ini -remove -force	
	
	echo Relinking files for %%A...
	echo.
	"C:\Path\To\HardLinkFiles.exe" --name %%A --config C:\Path\To\config.ini -force
	
	echo %%A Updated...
	echo.
)
echo Game servers updated...
echo.
pause
exit