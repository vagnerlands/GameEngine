// Game.cpp : Defines the entry point for the console application.
//

#include "IGame.h"

class MainWinOgl : public EngineCore::IMain
{
public:
	MainWinOgl()
	{

	}
	virtual ~MainWinOgl()
	{

	}

	virtual void StartUp(int argv, char** argc);
};
