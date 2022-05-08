@Echo On
@Cls
@Set PROJECT_ROOT=%CD%
@Set BIN_TARGET=%CD%\Binaries

Set LIB_DEPENDENCY=^
Graphics.lib;^
Math.lib;^
EngineCore.lib;^
UtilitiesCore.lib;^
glew32.lib;^
glfw3.lib;^
freeglut.lib;^
assimp-vc142-mt.lib;^
DevIL.lib;^
ILU.lib;^
ILUT.lib;^
SDL2.lib;^
SDL2main.lib;^
shell32.lib;^
brotlicommon-static.lib;^
brotlidec-static.lib;^
brotlienc-static.lib;^
bz2.lib;^
freetype.lib;^
libpng16.lib;^
zlib.lib;


:: kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;

Set LIB_PATHS_X86_DEBUG=^
%PROJECT_ROOT%\Libs;^
%PROJECT_ROOT%\Libs\Win32\Debug;

Set LIB_PATHS_X86_RELEASE=^
%PROJECT_ROOT%\Libs;^
%PROJECT_ROOT%\Libs\Win32\Release;

Set LIB_PATHS_X64_DEBUG=^
%PROJECT_ROOT%\Libs;^
%PROJECT_ROOT%\Dependencies\Libs\x64;^
%PROJECT_ROOT%\Dependencies\glfw-bin\src\Debug;^
%PROJECT_ROOT%\Libs\x64\Debug;

Set LIB_PATHS_X64_RELEASE=^
%PROJECT_ROOT%\Libs;^
%PROJECT_ROOT%\Dependencies\Libs\x64;^
%PROJECT_ROOT%\Dependencies\glfw-bin\src\Release;^
%PROJECT_ROOT%\Libs\x64\Release;

Set INCLUDE_PATHS=^
%PROJECT_ROOT%\EngineCore;^
%PROJECT_ROOT%\Game;^
%PROJECT_ROOT%\GameDLL;^
%PROJECT_ROOT%\Game-Editor;^
%PROJECT_ROOT%\Graphics;^
%PROJECT_ROOT%\Graphics\Scene;^
%PROJECT_ROOT%\Dependencies;^
%PROJECT_ROOT%\Dependencies\Includes;^
::%PROJECT_ROOT%\Dependencies\glfw\include;^
%PROJECT_ROOT%\Dependencies\GL;^
%PROJECT_ROOT%\Dependencies\assimp;^
%PROJECT_ROOT%\Dependencies\freetype\include\freetype2;^
%PROJECT_ROOT%\Math;^
%PROJECT_ROOT%\UtilitiesCore;^
%PROJECT_ROOT%\EngineCore\Cache;^
%PROJECT_ROOT%\EngineCore\Debug;^
%PROJECT_ROOT%\EngineCore\Holders;^
%PROJECT_ROOT%\EngineCore\Input;^
%PROJECT_ROOT%\EngineCore\Loaders;^
%PROJECT_ROOT%\EngineCore\Loaders\Assimp;^
%PROJECT_ROOT%\EngineCore\Loaders\Assimp\Compiler;^
%PROJECT_ROOT%\EngineCore\OS;^
%PROJECT_ROOT%\EngineCore\Cache\ResourceCache;^
%PROJECT_ROOT%\EngineCore\Loaders\Images;^
%PROJECT_ROOT%\EngineCore\Loaders\Images\Windows;^
%PROJECT_ROOT%\EngineCore\Loaders\Model3D;^
%PROJECT_ROOT%\EngineCore\Loaders\Model3D\Assimp;^
%PROJECT_ROOT%\EngineCore\Loaders\Model3D\objParser;^
%PROJECT_ROOT%\EngineCore\OS\Abstract;^
%PROJECT_ROOT%\EngineCore\OS\Windows;^
%PROJECT_ROOT%\Includes\GL;^
%PROJECT_ROOT%\Includes\lpng1626;^
%PROJECT_ROOT%\UtilitiesCore\Shaders;^
%PROJECT_ROOT%\UtilitiesCore\zlib;^
%PROJECT_ROOT%\UtilitiesCore\Rendered;


::"C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\MSBuild\15.0\Bin\msbuild.exe" "%PROJECT_ROOT%"\Game.sln /t:Rebuild /p:Platform=x64 /p:Configuration=Release
