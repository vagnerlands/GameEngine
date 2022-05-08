#pragma once

#include "IGame.h"
#include "CGameController.h"
#include "EditorGameControls.h"
#include "IvVector3.h" // DEBUG - delete me

class Editor : public EngineCore::IGame
{
public:
	Editor();
	virtual ~Editor();
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
	Editor(const Editor& other) = delete;
	Editor& operator=(const Editor& other) = delete;
    // all the game input controls are handled here...
	EditorGameControls mControls;
};