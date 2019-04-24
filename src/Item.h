#pragma once
#include "SubLaunchedObject.h"
#include "Submarine.h"
class Psysl5Engine;
class Item :
	public SubLaunchedObject
{
protected:
	int m_iTimeStamp;
	int m_iCoolingTime;
	int m_iBlinkTimeStamp;
	int m_iBlinkPeriod;
	unsigned int m_uiColour;
	int m_iBlink;
public:
	Item(Psysl5Engine* pEngine, Submarine* pSubmarine, unsigned int iColour);
	~Item();
	void Draw();
	void DoUpdate(int iCurrentTime);
	void RedrawBackground();
	void Blink(int iCurrentTime);
	virtual void Trigger();
};

