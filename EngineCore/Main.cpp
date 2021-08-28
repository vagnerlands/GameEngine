// Game.cpp : Defines the entry point for the console application.
//

#include "MainOgl.h"

#include "gl/glew.h"
#include "gl/glut.h"

#include "MainWinOgl.h"

#include <iostream>

#ifndef IRIS_DEBUG_MEM_ALLOC
void* operator new(size_t size)
{
    void* memory = malloc(size);

    if (size > (1024*1024 * 1))
    {
        std::cout << " [+] Allocating new big chunk of [" << size / 1024 / 1024 << "] MBs" << endl;
    }

    return memory;
}

void* operator new[](size_t size)
{
    void* memory = malloc(size);

    if (size > (1024 * 1024 * 1))
    {
        std::cout << " [+] Allocating new[] big chunk of [" << size / 1024 / 1024 << "] MBs" << endl;
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
	// starts defaultively on OGL
	int initializationMode = 1;

	// checks if there is another argument
	// then checks if the argument is explicitly OGL or WINOGL (case insensitive)
	if (argv == 2)
	{		
		if (strcmp(strupr(argc[1]), "OGL") == 0)
			initializationMode = 1;
		else if (strcmp(strupr(argc[1]), "WINOGL") == 0)
			initializationMode = 2;
	}

	EngineCore::IMain* obj;
	if (initializationMode == 1)
	{
		obj = new MainOgl();
	}
	else if (initializationMode == 2)
	{
		obj = new MainWinOgl();
	}

	// most of the time, the arguments are pointless
	// but for some implementations it's a must
	// let's keep it here
	obj->StartUp(argv, argc);

	// ... this point shouldn't be reached

	// release allocated object
	delete obj;
	obj = nullptr;

	// close application
	exit(0);
}

