@Echo On
@Cls
@Set PROJECT_ROOT=%CD%
@Set BIN_TARGET=%CD%\Binaries

Set LIB_DEPENDENCY=^
glew32.lib;^
freeglut.lib;^
Graphics.lib;^
Math.lib;^
EngineCore.lib;^
UtilitiesCore.lib;^
libpngd.lib;^
assimp-vc140-mt.lib;^
DevIL.lib;^
ILU.lib;^
ILUT.lib;^
SDL2.lib;^
SDL2main.lib;^
shell32.lib;^
zlibd.lib



:: kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;

Set LIB_PATHS_X86_DEBUG=^
%PROJECT_ROOT%\Libs;^
%PROJECT_ROOT%\Libs\Win32\Debug;

Set LIB_PATHS_X86_RELEASE=^
%PROJECT_ROOT%\Libs;^
%PROJECT_ROOT%\Libs\Win32\Release;

Set LIB_PATHS_X64_DEBUG=^
%PROJECT_ROOT%\Libs;^
%PROJECT_ROOT%\Libs\x64\Debug;

Set LIB_PATHS_X64_RELEASE=^
%PROJECT_ROOT%\Libs;^
%PROJECT_ROOT%\Libs\x64\Release;

Set INCLUDE_PATHS=^
%PROJECT_ROOT%\EngineCore;^
%PROJECT_ROOT%\Game;^
%PROJECT_ROOT%\GameDLL;^
%PROJECT_ROOT%\Game-Editor;^
%PROJECT_ROOT%\Graphics;^
%PROJECT_ROOT%\Graphics\Scene;^
%PROJECT_ROOT%\Includes;^
%PROJECT_ROOT%\Includes\assimp;^
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
%PROJECT_ROOT%\UtilitiesCore\zlib;
%PROJECT_ROOT%\UtilitiesCore\Rendered;


start "C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional\Common7\IDE\devenv.exe" "%PROJECT_ROOT%\Game.sln"