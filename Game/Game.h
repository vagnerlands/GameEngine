#ifndef _GAME_H_
#define _GAME_H_

#include "IGame.h"
#include "CGameController.h"

//class CGameController;

class Game : public EngineCore::IGame,
	public _Keys::KeyT,
	public _Keys::KeyY,
	public _Keys::KeyG,
	public _Keys::KeyH,
	public _Keys::Key0,
	public _Keys::Key1,
	public _Keys::Key2,
	public _Keys::Key3
{
public:
	Game();
	virtual ~Game();
	virtual bool PostRendererInitialize();

	virtual CGameController* GetGameController()
	{
		return mpGameInput;
	}

	void OnKeyEvent(const Key0& e) override;
	void OnKeyEvent(const Key1& e) override;
	void OnKeyEvent(const Key2& e) override;
	void OnKeyEvent(const Key3& e) override;
	void OnKeyEvent(const KeyT& e) override;
	void OnKeyEvent(const KeyY& e) override;
	void OnKeyEvent(const KeyG& e) override;
	void OnKeyEvent(const KeyH& e) override;

protected:

	virtual void ExecuteBackground();
	virtual void UpdateObjects(float dt);
	virtual void Render(float dt);

private:
	Game(const Game& other) = delete;
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
