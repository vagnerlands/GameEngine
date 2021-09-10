#ifndef _GAMECONTROLS_H_
#define _GAMECONTROLS_H_

#include "CommonTypes.h"
#include "CGameController.h"
#include "IvVector3.h"

namespace EngineCore
{
    class IGame;
}

class GameControls : 
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
    friend class Game;
    GameControls(EngineCore::IGame* pGameEngine);
	virtual ~GameControls();
    CGameController* GetGameController();
    void Execute(float dt);

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
private:
    GameControls(const GameControls& other) = delete;
    GameControls& operator=(const GameControls& other) = delete;

    EngineCore::IGame* mpGameEngine;
	CGameController* mpGameInput;

    // Debug shit...
    Float m_lightAngle;
    Float m_bias;
    Float m_shadowDetailsFactor;
    IvVector3 m_ambient;
};


#endif // _GAME_H_
