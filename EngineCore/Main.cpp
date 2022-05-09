// Game.cpp : Defines the entry point for the console application.
//

#include "MainOgl.h"

#include "gl/glew.h"
#include "gl/glut.h"

#include "MainWinOgl.h"
#ifdef _IS_EDITOR_
#include "MainGLFW.h"
#endif
#include <iostream>

#ifdef _DEBUG
void* operator new(size_t size)
{
    void* memory = malloc(size);

    if (size > (1024*1024 * 1))
    {
        //std::cout << " [+] Allocating new big chunk of [" << size / 1024 / 1024 << "] MBs" << endl;
    }

    return memory;
}

void* operator new[](size_t size)
{
    void* memory = malloc(size);

    if (size > (1024 * 1024 * 1))
    {
        //std::cout << " [+] Allocating new[] big chunk of [" << size / 1024 / 1024 << "] MBs" << endl;
    }

    return memory;
}

void operator delete(void* p)
{
    free(p);
}

void operator delete[](void* p)
{
    free(p);
}
#endif

int 
main(int argv, char** argc)
{
	std::unique_ptr<EngineCore::IMain> run_mode;
#ifdef _IS_EDITOR_
	run_mode = std::make_unique<MainGLFW>();
#else
	run_mode = std::make_unique<MainOgl>();
#endif
	// checks if there is another argument
	// then checks if the argument is explicitly OGL or WINOGL (case insensitive)
	if (argv == 2)
	{		
		if (strcmp(strupr(argc[1]), "OGL") == 0)
			run_mode = std::make_unique<MainOgl>();
		else if (strcmp(strupr(argc[1]), "WINOGL") == 0)
			run_mode = std::make_unique<MainWinOgl>();
	}

	// most of the time, the arguments are pointless
	// but for some implementations it's a must
	// let's keep it here
	run_mode->StartUp(argv, argc);

	// ... this point shouldn't be reached

	// close application
	exit(0);
}

