#pragma once
#include "GameState.h"
#include "ScoreAndName.h"
#include <vector>
using namespace std;
class LoseState :
	public GameState
{
private:
	static LoseState m_LoseState;
	ImageData m_imLoseStateBackground;
	char m_charBuffer[5];
	int m_iCharCounter;
	vector<ScoreAndName> m_ScoreAndName_vector;
	bool m_bIsHighScore;
protected:
	LoseState() {}
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
	static LoseState* Instance() { return &m_LoseState; } /* singleton pattern */
	void ReadScoresFromFile();
	void WriteScoresToFile();
};

