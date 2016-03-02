@echo Preparing to deploy the "Rebound" project...
@echo off
rmdir Deploy /s /q
mkdir Deploy
mkdir Deploy\Core\Assets
mkdir Deploy\Projects\Rebound\
mkdir Deploy\Projects\Rebound\Resources
mkdir Deploy\Projects\Rebound\Assets

xcopy DCEngine\Projects\Rebound\Assets\*.* Deploy\Projects\Rebound\Assets /E
xcopy DCEngine\Projects\Rebound\Resources\*.* Deploy\Projects\Rebound\Resources /E
xcopy DCEngine\Projects\Rebound\Rebound.dceproj Deploy\Projects\Rebound\ 
xcopy DCEngine\Core\Assets\*.* Deploy\Core\Assets /E

xcopy DCEngine\Dependencies\lib\MSVC\x64\fmodstudio64.dll Deploy
xcopy DCEngine\Dependencies\lib\MSVC\x64\fmod64.dll Deploy
xcopy DCEngine\Dependencies\lib\MSVC\x64\Release\sfml-graphics-2.dll Deploy
xcopy DCEngine\Dependencies\lib\MSVC\x64\Release\sfml-system-2.dll Deploy
xcopy DCEngine\Dependencies\lib\MSVC\x64\Release\sfml-window-2.dll Deploy

xcopy DCEngine\Daisy.cfg Deploy
xcopy DCEngine\x64\Production\DCEngine.exe Deploy
@echo Done deploying "Rebound"!