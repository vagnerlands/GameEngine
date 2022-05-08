// Game.cpp : Defines the entry point for the console application.
//
#pragma once
#include "IGame.h"

class MainGLFW : public EngineCore::IMain
{
public:
	MainGLFW()
	{

	}
    virtual ~MainGLFW();

	virtual void StartUp(int argv, char** argc);
};