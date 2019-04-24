#include "header.h"
#include "templates.h"

#include "LoseState.h"
#include "HighScoresState.h"
#include <algorithm>
#include <functional>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;


LoseState LoseState::m_LoseState;


void LoseState::Init(Psysl5Engine* pEngine)
{
	m_imLoseStateBackground.LoadImage("lose_state_background.png");
	m_bIsHighScore = false;
	m_iCharCounter = 0; /* initialise char counter for name input */
	ReadScoresFromFile(); /* read file and store scores to vector, then sort it in decending order */
	WriteScoresToFile(); /* write sorted scores from vector to file */
	cout << "lose state inited." << endl;
}


void LoseState::Clean(Psysl5Engine* pEngine)
{
	m_bIsHighScore = false;
	m_ScoreAndName_vector.clear();
	for (int i = 0; i < sizeof(m_charBuffer); i++)
		m_charBuffer[i] = NULL;
	cout << "lose state cleaned." << endl;
}


void LoseState::Pause()
{
}


void LoseState::Resume()
{
}


void LoseState::SetupBackgroundBuffer(Psysl5Engine* pEngine)
{
	m_imLoseStateBackground.RenderImage(
		pEngine->GetBackground()
		, 0, 0, 0, 0
		, m_imLoseStateBackground.GetWidth()
		, m_imLoseStateBackground.GetHeight());
}


void LoseState::DrawStringsOnTop(Psysl5Engine* pEngine)
{
	char buffer[128];
	sprintf(buffer, "your score: %u", pEngine->GetM_uiScore());
	pEngine->DrawScreenString(200, 150, buffer, 0xffffff, NULL);
	if (m_ScoreAndName_vector.size() < 10 
		|| pEngine->GetM_uiScore() > m_ScoreAndName_vector.back().uiScore)
	{
		m_bIsHighScore = true;
		pEngine->DrawScreenString(200, 210, "new high score", 0xffffff, NULL);
		pEngine->DrawScreenString(200, 240, "please enter your name: ", 0xffffff, NULL);
		pEngine->DrawScreenString(200, 270, m_charBuffer, 0xffffff, NULL);
	}
	pEngine->DrawScreenString(400, 570, "press return to continue", 0xffffff, NULL);
}


void LoseState::UnDrawStrings(Psysl5Engine* pEngine)
{
}


void LoseState::GameAction(Psysl5Engine* pEngine)
{
	SetupBackgroundBuffer(pEngine);
	pEngine->Redraw(true);
}


void LoseState::KeyDown(int iKeyCode, Psysl5Engine* pEngine)
{
	switch (iKeyCode)
	{
	case SDLK_ESCAPE:
		pEngine->SetExitWithCode(0);
		break;
	case SDLK_RETURN:
		if (m_iCharCounter > 0)
		{
			m_ScoreAndName_vector.push_back(ScoreAndName{ pEngine->GetM_uiScore(), m_charBuffer });
			WriteScoresToFile();
			pEngine->ChangeState(HighScoresState::Instance());
			pEngine->SetupBackgroundBuffer();
			pEngine->Redraw(true);
		}
		else if (!m_bIsHighScore)
		{
			pEngine->ChangeState(HighScoresState::Instance());
			pEngine->SetupBackgroundBuffer();
			pEngine->Redraw(true);
		}
		break;
	case SDLK_BACKSPACE:
		if (m_iCharCounter > 0)
		{
			m_charBuffer[m_iCharCounter - 1] = NULL;
			m_iCharCounter--;
		}
		break;
	default:
		if (iKeyCode >= SDLK_a && iKeyCode <= SDLK_z && m_iCharCounter < sizeof(m_charBuffer))
		{
			m_charBuffer[m_iCharCounter] = iKeyCode;
			m_iCharCounter++;
		}
		break;
	}
}


void LoseState::UndrawObjects(Psysl5Engine* pEngine)
{

}


void LoseState::DrawObjects(Psysl5Engine* pEngine)
{

}


void LoseState::ReadScoresFromFile()
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


void LoseState::WriteScoresToFile()
{
	std::sort(m_ScoreAndName_vector.begin(), m_ScoreAndName_vector.end(), greater<ScoreAndName>());
	ofstream file;
	file.open("top_ten_scores.txt");

	int iRankCounter = 0;
	for (ScoreAndName san : m_ScoreAndName_vector)
	{
		if (iRankCounter < 10)
		{
			file << san.uiScore << " " << san.strName << endl;
			iRankCounter++;
		}
	}
	file.close();
}