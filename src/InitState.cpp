#include "header.h"
#include "templates.h"

#include "InitState.h"
#include "MenuState.h"


InitState InitState::m_InitState;


void InitState::Init(Psysl5Engine* pEngine)
{
	m_uiColour = 0x000000;
	m_bIsToDarken = false;
	m_iBrightness = 100;
	m_bBackgroundIsToDarken = true;
	m_imInitStateBackground.LoadImage("init_state_background.png");
	cout << "init state inited." << endl;
}


void InitState::Clean(Psysl5Engine* pEngine)
{
}


void InitState::Pause()
{
}


void InitState::Resume()
{
}


void InitState::SetupBackgroundBuffer(Psysl5Engine* pEngine)
{
#if 0
	m_imInitStateBackground.RenderImage(
		pEngine->GetBackground()
		, 0, 0, 0, 0
		, m_imInitStateBackground.GetWidth()
		, m_imInitStateBackground.GetHeight());
#else
	pEngine->FillBackground(0x000000);
	m_imInitStateBackground.FlexibleRenderImageWithMask(
		pEngine->GetBackground(),
		0, 0, 0, 0,
		m_imInitStateBackground.GetWidth(),
		m_imInitStateBackground.GetHeight(),
		0, // Number of 90 degree rotations to apply
		400, 250, // Position of the transparency pixel
		m_iBrightness, // Percentage brightness to use
		-1, // An alternate pixel colour - set to -1 to not use otherwise you will get a crosshatch pattern
		-1, // Pixel colour for each third pixel - set to -1 to not use
		-1, // Pixel colour for each fourth pixel - set to -1 to not use
		-1 // Merge colour - set to -1 to not use, otherwise this will average with current pixels
		);
	if (m_bBackgroundIsToDarken)
		m_iBrightness-=0.5;
	else
		m_iBrightness+=0.5;
	if (m_iBrightness == 100)
		m_bBackgroundIsToDarken = true;
	if (m_iBrightness == 50)
		m_bBackgroundIsToDarken = false;
#endif
}


void InitState::DrawStringsOnTop(Psysl5Engine* pEngine)
{
	pEngine->DrawScreenString(150, 450, "Initialised and waiting for RETURN", m_uiColour, NULL);
	if (m_bIsToDarken)
		m_uiColour -= 0x010101;
	else
		m_uiColour += 0x010101;
	if (m_uiColour == 0xffffff)
		m_bIsToDarken = true;
	if (m_uiColour == 0x000000)
		m_bIsToDarken = false;
}


void InitState::UnDrawStrings(Psysl5Engine* pEngine)
{
	pEngine->CopyBackgroundPixels(0/*X*/, 450/*Y*/, pEngine->GetScreenWidth(), 30/*Height*/);
}


void InitState::GameAction(Psysl5Engine* pEngine)
{
	SetupBackgroundBuffer(pEngine);
	pEngine->Redraw(true);
}


void InitState::KeyDown(int iKeyCode, Psysl5Engine* pEngine)
{
	switch (iKeyCode)
	{
	case SDLK_ESCAPE:
		pEngine->SetExitWithCode(0);
		break;
	case SDLK_RETURN:
		pEngine->ChangeState(MenuState::Instance());
		pEngine->SetupBackgroundBuffer();
		pEngine->Redraw(true);
		break;
	default:
		pEngine->CompareCode(iKeyCode);
		break;
	}
}


void InitState::UndrawObjects(Psysl5Engine* pEngine)
{
}


void InitState::DrawObjects(Psysl5Engine* pEngine)
{
}