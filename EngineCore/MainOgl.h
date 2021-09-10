// Game.cpp : Defines the entry point for the console application.
//

#include "IGame.h"

class MainOgl : public EngineCore::IMain
{
public:
	MainOgl()
	{

	}
    virtual ~MainOgl();

	virtual void StartUp(int argv, char** argc);
};