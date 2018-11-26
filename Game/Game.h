#ifndef _GAME_H_
#define _GAME_H_

#include "IGame.h"

class CGameController;

class Game : public EngineCore::IGame
{
public:
	Game();
	virtual ~Game();
	virtual bool PostRendererInitialize();

	virtual CGameController* GetGameController()
	{
		return mpGameInput;
	}

protected:

	virtual void UpdateObjects(float dt);
	virtual void Render();

private:
	Game(const Game& other);
	Game& operator=(const Game& other);

	CGameController* mpGameInput;

	inline Float clamp(Float number1, Float number2) const
	{
		if (number1 > number2)
			return number1;
		else
			return number2;
	}

};


#endif // _GAME_H_