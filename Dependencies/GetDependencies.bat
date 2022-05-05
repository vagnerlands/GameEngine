conan install . -s arch=x86_64 -s compiler="Visual Studio" --build missing -s build_type=Release -s compiler.version=16 -g deploy
::git clone https://github.com/glfw/glfw
::cmake -S ./ -B ../glfw-bin/
::cmake --build "../glfw-bin/" --config Release
::cmake --build "../glfw-bin/" --config Debug