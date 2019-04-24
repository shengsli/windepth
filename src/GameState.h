#pragma once
#include "Psysl5Engine.h"
class GameState
{
protected:
	GameState() {}
public:
	virtual void Init(Psysl5Engine* pEngine) = 0;
	virtual void Clean(Psysl5Engine* pEngine) = 0;
	virtual void Pause() = 0;
	virtual void Resume() = 0;
	virtual void SetupBackgroundBuffer(Psysl5Engine* pEngine) = 0;
	virtual void DrawStringsOnTop(Psysl5Engine* pEngine) = 0;
	virtual void UnDrawStrings(Psysl5Engine* pEngine) = 0;
	virtual void GameAction(Psysl5Engine* pEngine) = 0;
	virtual void KeyDown(int iKeyCode, Psysl5Engine* pEngine) = 0;
	virtual void UndrawObjects(Psysl5Engine* pEngine) = 0;
	virtual void DrawObjects(Psysl5Engine* pEngine) = 0;
	void ChangeState(Psysl5Engine* pEngine, GameState* pGameState)
	{
		pEngine->ChangeState(pGameState);
	}
};

