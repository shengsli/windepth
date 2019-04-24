#pragma once
#include "GameState.h"
#include "ScoreAndName.h"
#include <vector>
using namespace std;
class HighScoresState :
	public GameState
{
private:
	static HighScoresState m_HighScoresState;
	ImageData m_imHighScoresStateBackground;
	vector<ScoreAndName> m_ScoreAndName_vector;
protected:
	HighScoresState() {}
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
	static HighScoresState* Instance() { return &m_HighScoresState; } /* singleton pattern */
	void ReadScoresFromFile();
	void WriteScoresToFile();
};

