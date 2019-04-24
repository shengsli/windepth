#pragma once
#include "GameState.h"
class PlayState :
	public GameState
{
private:
	static PlayState m_PlayState;
protected:
	PlayState() {}
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
	static PlayState* Instance() { return &m_PlayState; } /* singleton pattern */
};

