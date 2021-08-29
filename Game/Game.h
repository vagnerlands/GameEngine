#ifndef _GAME_H_
#define _GAME_H_

#include "IGame.h"
#include "CGameController.h"
#include "IvVector3.h" // DEBUG - delete me
#include "OpenGL/Window2DOGL.h"

class Game : 
    public EngineCore::IGame,
    public _Keys::Keyn,
    public _Keys::Keym,
    public _Keys::Keyv,
	public _Keys::Keyb,
    public _Keys::Keyt,
	public _Keys::Keyy,
	public _Keys::Keyg,
	public _Keys::Keyh,
    public _Keys::Keyz,
    public _Keys::Keyx,
    public _Keys::Keyc,
	public _Keys::Key0,
	public _Keys::Key1,
	public _Keys::Key2,
	public _Keys::Key3,
    public _Keys::KeyEscape
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
	void OnKeyEvent(const Keyt& e) override;
	void OnKeyEvent(const Keyy& e) override;
	void OnKeyEvent(const Keyg& e) override;
	void OnKeyEvent(const Keyh& e) override;
    void OnKeyEvent(const KeyEscape& e) override;
    void OnKeyEvent(const Keyv& e) override;
    void OnKeyEvent(const Keyb& e) override;
    void OnKeyEvent(const Keyn& e) override;
    void OnKeyEvent(const Keym& e) override;
    void OnKeyEvent(const Keyz& e) override;
    void OnKeyEvent(const Keyx& e) override;
    void OnKeyEvent(const Keyc& e) override;
protected:

	virtual void ExecuteBackground();
	virtual void UpdateObjects(float dt);
	virtual void Render(float dt);

private:
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;

	CGameController* mpGameInput;

    // Debug shit...
    Float m_lightAngle;
    Float m_bias;
    Float m_shadowDetailsFactor;
    IvVector3 m_ambient;
    Graphics::Window2DOGL wind;
};


#endif // _GAME_H_
