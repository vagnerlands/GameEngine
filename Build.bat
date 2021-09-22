@Echo On
@Cls
call Setup.bat

"C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\MSBuild\15.0\Bin\msbuild.exe" "%PROJECT_ROOT%"\Game.sln /t:Rebuild /p:Platform=x64 /p:Configuration=Release
