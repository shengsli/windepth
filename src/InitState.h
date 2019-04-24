#pragma once
#include "GameState.h"
class InitState :
	public GameState
{
private:
	static InitState m_InitState;
	ImageData m_imInitStateBackground;
	unsigned int m_uiColour;
	bool m_bIsToDarken;
	double m_iBrightness;
	bool m_bBackgroundIsToDarken;
protected:
	InitState() {}
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
	static InitState* Instance() { return &m_InitState; } /* singleton pattern */
};

