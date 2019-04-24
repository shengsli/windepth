#include "header.h"
#include "templates.h"

#include "PauseState.h"
#include "GameObject.h"
#include "Battleship.h"


PauseState PauseState::m_PauseState;


void PauseState::Init(Psysl5Engine* pEngine)
{
}


void PauseState::Clean(Psysl5Engine* pEngine)
{
}


void PauseState::Resume()
{
}


void PauseState::Pause()
{
}


void PauseState::SetupBackgroundBuffer(Psysl5Engine* pEngine)
{
}


void PauseState::DrawStringsOnTop(Psysl5Engine* pEngine)
{
	char buffer[128];
	sprintf(buffer, "score: %u", pEngine->GetM_uiScore());
	pEngine->DrawScreenString(0, 0, buffer, 0x000000, NULL);
	if (pEngine->GetM_iLives() > 0)
	{
		pEngine->DrawScreenString(
			pEngine->GetM_pShip()->GetM_iCurrentScreenX() + 45,
			pEngine->GetM_pShip()->GetM_iCurrentScreenY() + 30,
			"anti-sub", 0x000000, pEngine->m_pFont
			);
		sprintf(buffer, "level%d", pEngine->GetM_iLevel());
		pEngine->DrawScreenString(
			pEngine->GetM_pShip()->GetM_iCurrentScreenX(),
			pEngine->GetM_pShip()->GetM_iCurrentScreenY(),
			buffer, 0x000000, pEngine->m_pFont
			);
	}

	pEngine->DrawScreenString(700, 0, "Paused", 0x000000, NULL);
	pEngine->DrawScreenString(400, 570, "press return to continue", 0xffffff, NULL);
}


void PauseState::UnDrawStrings(Psysl5Engine* pEngine)
{
}


void PauseState::GameAction(Psysl5Engine* pEngine)
{
}


void PauseState::KeyDown(int iKeyCode, Psysl5Engine* pEngine)
{
	switch (iKeyCode)
	{
	case SDLK_ESCAPE:
		pEngine->SetExitWithCode(0);
		break;
	case SDLK_RETURN:
		pEngine->PopState();
		pEngine->IncreaseTimeOffset(pEngine->m_iPauseStarted - pEngine->GetTime());
		pEngine->SetupBackgroundBuffer();
		pEngine->Redraw(true);
		break;
	default:
		pEngine->CompareCode(iKeyCode);
		break;
	}
}


void PauseState::UndrawObjects(Psysl5Engine* pEngine)
{
	pEngine->SetM_iDrawableObjectsChanged(0);
	// This effectively un-draws the old positions of the objects
	for (int i = 0; i < pEngine->GetM_pGameObjects_vector().size(); i++)
	{
		pEngine->GetM_pGameObjects_vector()[i]->RedrawBackground();
		if (pEngine->GetM_iDrawableObjectsChanged())
			return; // Abort! Something changed
	}
}


void PauseState::DrawObjects(Psysl5Engine* pEngine)
{
	pEngine->RemoveObjectsFromVector();
	pEngine->SetM_iDrawableObjectsChanged(0);
	// And this re-draws the new positions
	for (int i = 0; i < pEngine->GetM_pGameObjects_vector().size(); i++)
	{
		pEngine->GetM_pGameObjects_vector()[i]->Draw();
		if (pEngine->GetM_iDrawableObjectsChanged())
			return; // Abort! Something changed in the array
	}
}