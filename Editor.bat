@Echo On
@Cls
call Setup.bat
set PRE_COMP=_IS_EDITOR_

start "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\Common7\IDE\devenv.exe" "%PROJECT_ROOT%\Game.sln"