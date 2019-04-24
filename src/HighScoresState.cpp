#include "header.h"
#include "templates.h"

#include "HighScoresState.h"
#include "MenuState.h"
#include "ScoreAndName.h"
#include <algorithm>
#include <functional>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;


HighScoresState HighScoresState::m_HighScoresState;


void HighScoresState::Init(Psysl5Engine* pEngine)
{
	m_imHighScoresStateBackground.LoadImage("lose_state_background.png");
	ReadScoresFromFile();
	cout << "high scores state inited." << endl;
}


void HighScoresState::Clean(Psysl5Engine* pEngine)
{
	m_ScoreAndName_vector.clear();
	cout << "high scores state cleaned." << endl;
}


void HighScoresState::Pause()
{
}


void HighScoresState::Resume()
{
}


void HighScoresState::SetupBackgroundBuffer(Psysl5Engine* pEngine)
{
	m_imHighScoresStateBackground.RenderImage(
		pEngine->GetBackground()
		, 0, 0, 0, 0
		, m_imHighScoresStateBackground.GetWidth()
		, m_imHighScoresStateBackground.GetHeight());
}


void HighScoresState::DrawStringsOnTop(Psysl5Engine* pEngine)
{
	char buffer[128];
	int iScreenX = 300;
	int iScreenY = 150;
	pEngine->DrawScreenString(iScreenX + 20, iScreenY, "high scores", 0xffffff, NULL);
	iScreenY += 30;
	pEngine->DrawScreenRectangle(iScreenX, iScreenY, 500, iScreenY + 5, 0xffffff);
	for (ScoreAndName san : m_ScoreAndName_vector)
	{
		iScreenY += 30;
		sprintf(buffer, "%u", san.uiScore);
		pEngine->DrawScreenString(iScreenX - 100, iScreenY, buffer, 0xffffff, NULL);
		sprintf(buffer, "%s", san.strName.c_str());
		pEngine->DrawScreenString(iScreenX + 200, iScreenY, buffer, 0xffffff, NULL);
	}
	pEngine->DrawScreenString(400, 570, "press return to continue", 0xffffff, NULL);
}


void HighScoresState::UnDrawStrings(Psysl5Engine* pEngine)
{
}


void HighScoresState::GameAction(Psysl5Engine* pEngine)
{
}


void HighScoresState::KeyDown(int iKeyCode, Psysl5Engine* pEngine)
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
		break;
	}
}


void HighScoresState::UndrawObjects(Psysl5Engine* pEngine)
{

}


void HighScoresState::DrawObjects(Psysl5Engine* pEngine)
{

}


void HighScoresState::ReadScoresFromFile()
{
	ifstream file("top_ten_scores.txt");
	unsigned int uiScore;
	string strName;
	if (file)
	{
		while (file >> uiScore >> strName)
		{
			m_ScoreAndName_vector.push_back(ScoreAndName{ uiScore, strName });
		}
		file.close();
		std::sort(m_ScoreAndName_vector.begin(), m_ScoreAndName_vector.end(), greater<ScoreAndName>());
	}
	else cout << "Unable to open file." << endl;
}

