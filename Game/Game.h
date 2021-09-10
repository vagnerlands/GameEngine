#ifndef _GAME_H_
#define _GAME_H_

#include "IGame.h"
#include "CGameController.h"
#include "GameControls.h"
#include "IvVector3.h" // DEBUG - delete me

class Game : 
    public EngineCore::IGame
{
public:
	Game();
	virtual ~Game();
	virtual bool PostRendererInitialize();
	virtual CGameController* GetGameController()
	{
		return mControls.GetGameController();
	}

protected:

	virtual void ExecuteBackground() override;
	virtual void UpdateObjects(float dt) override;
	virtual void Render(float dt) override;

private:
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;

    GameControls mControls;
};


#endif // _GAME_H_
