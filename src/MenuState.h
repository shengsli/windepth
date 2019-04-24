#pragma once
#include "GameState.h"
class MenuState :
	public GameState
{
private:
	static MenuState m_MenuState;
	ImageData m_imMenuStateBackground;
	unsigned int m_uiStringColour_start;
	unsigned int m_uiRectangleColour_start;
	unsigned int m_uiStringColour_highscores;
	unsigned int m_uiRectangleColour_highscores;
	unsigned int m_uiStringColour_quit;
	unsigned int m_uiRectangleColour_quit;
protected:
	MenuState() {}
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
	static MenuState* Instance() { return &m_MenuState; } /* singleton pattern */
};

