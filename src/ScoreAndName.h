#pragma once
#include <string>
using namespace std;
struct ScoreAndName
{
	unsigned int uiScore; /* key */
	string strName; /* value */

	ScoreAndName(unsigned int uiScore, const string& strName) : uiScore(uiScore), strName(strName) {}

	bool operator > (const ScoreAndName& scoreAndName) const /* descending */
	{
		return (uiScore > scoreAndName.uiScore);
	}
};

