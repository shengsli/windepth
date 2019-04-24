#include "header.h"
#include "templates.h"

#include "MenuState.h"
#include "PlayState.h"
#include "HighScoresState.h"


MenuState MenuState::m_MenuState;


void MenuState::Init(Psysl5Engine* pEngine)
{
	m_imMenuStateBackground.LoadImage("menu_state_background.png");
	pEngine->ResetMousePositionClickedDown();
	m_uiStringColour_start = 0x000000;
	m_uiRectangleColour_start = 0xffffff;
	m_uiStringColour_highscores = 0x000000;
	m_uiRectangleColour_highscores = 0xffffff;
	m_uiStringColour_quit = 0x000000;
	m_uiRectangleColour_quit = 0xffffff;
	cout << "menu state inited." << endl;
}


void MenuState::Clean(Psysl5Engine* pEngine)
{
}


void MenuState::Pause()
{
}


void MenuState::Resume()
{
}


void MenuState::SetupBackgroundBuffer(Psysl5Engine* pEngine)
{
	pEngine->FillBackground(0x000000);
	m_imMenuStateBackground.RenderImage(
		pEngine->GetBackground()
		, 0, 0, 0, 0
		, m_imMenuStateBackground.GetWidth()
		, m_imMenuStateBackground.GetHeight());
}


void MenuState::DrawStringsOnTop(Psysl5Engine* pEngine)
{
	pEngine->DrawScreenRectangle(325, 300, 475, 330, m_uiRectangleColour_start);
	pEngine->DrawScreenString(365, 300, "start", m_uiStringColour_start);
	pEngine->DrawScreenRectangle(325, 350, 475, 380, m_uiRectangleColour_highscores);
	pEngine->DrawScreenString(325, 350, "high score", m_uiStringColour_highscores);
	pEngine->DrawScreenRectangle(325, 400, 475, 430, m_uiRectangleColour_quit);
	pEngine->DrawScreenString(370, 400, "quit", m_uiStringColour_quit);
}


void MenuState::UnDrawStrings(Psysl5Engine* pEngine)
{
}


void MenuState::GameAction(Psysl5Engine* pEngine)
{
	/* menu buttons (string and rectangle) change colour when mouse hovers
	   go to corresponding states by clicking */
	/* start */
	if (pEngine->GetCurrentMouseX() > 325 && pEngine->GetCurrentMouseX() < 475
		&& pEngine->GetCurrentMouseY() > 300 && pEngine->GetCurrentMouseY() < 330)
	{
		m_uiRectangleColour_start = 0x000000;
		m_uiStringColour_start = 0xffffff;
		if (pEngine->GetMouseXClickedDown() > 325 && pEngine->GetMouseXClickedDown() < 475
			&& pEngine->GetMouseYClickedDown() > 300 && pEngine->GetMouseYClickedDown() < 330)
		{
			pEngine->ChangeState(PlayState::Instance());
			pEngine->SetupBackgroundBuffer();
			pEngine->Redraw(true);
		}
	}
	else
	{
		m_uiRectangleColour_start = 0xffffff;
		m_uiStringColour_start = 0x000000;
	}
	/* high scores */
	if (pEngine->GetCurrentMouseX() > 325 && pEngine->GetCurrentMouseX() < 475
		&& pEngine->GetCurrentMouseY() > 350 && pEngine->GetCurrentMouseY() < 380)
	{
		m_uiRectangleColour_highscores = 0x000000;
		m_uiStringColour_highscores = 0xffffff;
		if (pEngine->GetMouseXClickedDown() > 325 && pEngine->GetMouseXClickedDown() < 475
			&& pEngine->GetMouseYClickedDown() > 350 && pEngine->GetMouseYClickedDown() < 380)
		{
			pEngine->ChangeState(HighScoresState::Instance());
			pEngine->SetupBackgroundBuffer();
			pEngine->Redraw(true);
		}
	}
	else
	{
		m_uiRectangleColour_highscores = 0xffffff;
		m_uiStringColour_highscores = 0x000000;
	}
	/* quit */
	if (pEngine->GetCurrentMouseX() > 325 && pEngine->GetCurrentMouseX() < 475
		&& pEngine->GetCurrentMouseY() > 400 && pEngine->GetCurrentMouseY() < 430)
	{
		m_uiRectangleColour_quit = 0x000000;
		m_uiStringColour_quit = 0xffffff;
		if (pEngine->GetMouseXClickedDown() > 325 && pEngine->GetMouseXClickedDown() < 475
			&& pEngine->GetMouseYClickedDown() > 400 && pEngine->GetMouseYClickedDown() < 430)
		{
			pEngine->SetExitWithCode(0);
		}
	}
	else
	{
		m_uiRectangleColour_quit = 0xffffff;
		m_uiStringColour_quit = 0x000000;
	}

	pEngine->Redraw(true);
}


void MenuState::KeyDown(int iKeyCode, Psysl5Engine* pEngine)
{
	switch (iKeyCode)
	{
	case SDLK_ESCAPE:
		pEngine->SetExitWithCode(0);
		break;
	case SDLK_RETURN:
		pEngine->ChangeState(PlayState::Instance());
		pEngine->SetupBackgroundBuffer();
		pEngine->Redraw(true);
		break;
	default:
		pEngine->CompareCode(iKeyCode);
		break;
	}
}


void MenuState::UndrawObjects(Psysl5Engine* pEngine)
{
}


void MenuState::DrawObjects(Psysl5Engine* pEngine)
{
}

