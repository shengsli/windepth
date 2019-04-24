#pragma once
#include "GameState.h"
class PauseState :
	public GameState
{
private:
	static PauseState m_PauseState;
protected:
	PauseState() {}
public:
	void Init(Psysl5Engine* pEngine);
	void Clean(Psysl5Engine* pEngine);
	void Pause();
	void Resume();
	void SetupBackgroundBuffer(Psysl5Engine* pEngine);
	void DrawStringsOnTop(Psysl5Engine* pEngine);
	void UnDrawStrings(Psysl5Engine* pEngine);
	void GameAction(Psysl5Engine* pEngine);
	void KeyDown(int iKeyCode, Psysl5Engine* pEngine);
	void UndrawObjects(Psysl5Engine* pEngine);
	void DrawObjects(Psysl5Engine* pEngine);
	static PauseState* Instance() { return &m_PauseState; } /* singleton pattern */
};

