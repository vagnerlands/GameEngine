#ifndef _GAME_H_
#define _GAME_H_

#include "IGame.h"
#include "CGameController.h"

//class CGameController;

class Game : public EngineCore::IGame,
	public KeyBinderT,
	public KeyBinderY,
	public KeyBinderG,
	public KeyBinderH
{
public:
	Game();
	virtual ~Game();
	virtual bool PostRendererInitialize();

	virtual CGameController* GetGameController()
	{
		return mpGameInput;
	}

	void OnKeyEvent(const KeyBinderT& e) override;
	void OnKeyEvent(const KeyBinderY& e) override;
	void OnKeyEvent(const KeyBinderG& e) override;
	void OnKeyEvent(const KeyBinderH& e) override;

protected:

	virtual void ExecuteBackground();
	virtual void UpdateObjects(float dt);
	virtual void Render(float dt);

private:
	Game(const Game& other);
	Game& operator=(const Game& other);

	CGameController* mpGameInput;

    Float m_lightAngle;

	inline Float clamp(Float number1, Float number2) const
	{
		if (number1 > number2)
			return number1;
		else
			return number2;
	}

};


#endif // _GAME_H_
