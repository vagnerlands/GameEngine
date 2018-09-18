#ifndef _GAME_H_
#define _GAME_H_

#include "IGame.h"

class Game : public EngineCore::IGame
{
public:
	Game();
	virtual ~Game();
	virtual bool PostRendererInitialize();

protected:

	virtual void UpdateObjects(float dt);
	virtual void Render();

private:
	Game(const Game& other);
	Game& operator=(const Game& other);


};


#endif // _GAME_H_