#include "header.h"
#include "templates.h"

#include "PlayState.h"
#include "SubmarineCommander.h"
#include "PauseState.h"
#include "GameObject.h"
#include "Battleship.h"


PlayState PlayState::m_PlayState;


void PlayState::Init(Psysl5Engine* pEngine)
{
	pEngine->m_pFont = pEngine->GetFont("Cornerstone Regular.ttf", 12);
	pEngine->ResetMousePositionClickedDown();
	pEngine->InitialiseVariables();
	pEngine->InitialiseObjects();
	cout << "play state inited." << endl;
}


void PlayState::Clean(Psysl5Engine* pEngine)
{
	pEngine->DestroyOldObjects(); /* clear game object vector */
	cout << "play state cleaned." << endl;
}


void PlayState::Pause()
{
}


void PlayState::Resume()
{
}


void PlayState::SetupBackgroundBuffer(Psysl5Engine* pEngine)
{
	pEngine->FillBackground(0x000000);
	unsigned int uiColor = 0x9fffff;
	/* draw sky from 0 to 100 */
	for (int iY = pEngine->GetM_iBackgroundY(); iY < pEngine->GetM_iBackgroundY() + 50; iY++)
	{
		pEngine->DrawBackgroundRectangle(pEngine->GetM_iBackgroundX(), pEngine->GetM_iBackgroundY() + iY, pEngine->GetM_iBackgroundX() + pEngine->GetScreenWidth(), pEngine->GetM_iBackgroundY() + iY + 1, uiColor);
		uiColor += 0x010000;
	}
	for (int iY = pEngine->GetM_iBackgroundY() + 50; iY < pEngine->GetM_iBackgroundY() + 100; iY++)
	{
		pEngine->DrawBackgroundRectangle(pEngine->GetM_iBackgroundX(), pEngine->GetM_iBackgroundY() + iY, pEngine->GetM_iBackgroundX() + pEngine->GetScreenWidth(), pEngine->GetM_iBackgroundY() + iY + 1, uiColor);
		uiColor -= 0x010000;
	}
	/* draw shallow sea from 100 to 200 (205) */
	uiColor = 0xffffff;
	for (int iY = pEngine->GetM_iBackgroundY() + 100; iY < pEngine->GetM_iBackgroundY() + 200; iY += 7)
	{
		pEngine->DrawBackgroundRectangle(pEngine->GetM_iBackgroundX(), pEngine->GetM_iBackgroundY() + iY, pEngine->GetM_iBackgroundX() + pEngine->GetScreenWidth(), pEngine->GetM_iBackgroundY() + iY + 7, uiColor);
		uiColor -= 0x111100;
	}
	/* draw deep ocean from 200 to 600 */
	for (int iStartLine = pEngine->GetM_iBackgroundY() + 200; iStartLine < pEngine->GetM_iBackgroundY() + 600; iStartLine += 120)
	{
		uiColor = 0x000fff;
		for (int iY = 0; iY < 60; iY++)
		{
			pEngine->DrawBackgroundRectangle(pEngine->GetM_iBackgroundX(), pEngine->GetM_iBackgroundY() + iStartLine + iY, pEngine->GetM_iBackgroundX() + pEngine->GetScreenWidth(), pEngine->GetM_iBackgroundY() + iStartLine + iY + 1, uiColor);
			pEngine->DrawBackgroundRectangle(pEngine->GetM_iBackgroundX(), pEngine->GetM_iBackgroundY() + iStartLine + 120 - iY, pEngine->GetM_iBackgroundX() + pEngine->GetScreenWidth(), pEngine->GetM_iBackgroundY() + iStartLine + 120 - iY + 1, uiColor);
			uiColor -= 0x000004;
		}
	}
	/* set background position to (0,0) */
	pEngine->SetM_iBackgroundX(0);
	pEngine->SetM_iBackgroundY(0);
	/* tiles */
	pEngine->GetM_oTiles().DrawAllTiles(pEngine, pEngine->GetBackground(), 0, 0, 9, 2);
}


void PlayState::DrawStringsOnTop(Psysl5Engine* pEngine)
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
}


void PlayState::UnDrawStrings(Psysl5Engine* pEngine)
{
	pEngine->CopyBackgroundPixels(0/*X*/, 0/*Y*/, pEngine->GetScreenWidth(), 30/*Height*/);
}


void PlayState::GameAction(Psysl5Engine* pEngine)
{
	SetupBackgroundBuffer(pEngine);
	pEngine->Redraw(true);
	if (pEngine->GetM_uiScore() > 10000 * (pEngine->GetM_iLevel() + 1))
	{
		pEngine->IncM_iLevel();
		pEngine->GetM_pCommander()->IncM_uiNumOfSub(2);
	}

	// Only tell objects to move when not paused etc
	pEngine->UpdateAllObjects(pEngine->GetModifiedTime());
	pEngine->GetM_pCommander()->SendSubmarine();
}


void PlayState::KeyDown(int iKeyCode, Psysl5Engine* pEngine)
{
	switch (iKeyCode)
	{
	case SDLK_ESCAPE:
		pEngine->SetExitWithCode(0);
		break;
	case SDLK_RETURN:
		pEngine->PushState(PauseState::Instance());
		pEngine->m_iPauseStarted = pEngine->GetTime();
		pEngine->SetupBackgroundBuffer();
		pEngine->Redraw(true);
		break;
	default:
		break;
	}
}


void PlayState::UndrawObjects(Psysl5Engine* pEngine)
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


void PlayState::DrawObjects(Psysl5Engine* pEngine)
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