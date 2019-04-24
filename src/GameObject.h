#pragma once
#include "DisplayableObject.h"
#include "Psysl5Engine.h"
class GameObject :
	public DisplayableObject
{
protected:
	bool m_bIsToRemove;
	Psysl5Engine* m_pEngine;
	int m_iVel;
public:
	GameObject(Psysl5Engine* pEngine);
	~GameObject();
	inline Psysl5Engine* GetEngine() const { return m_pEngine; }
	bool IsToRemove() { return m_bIsToRemove; }
	virtual void SetIsToRemove(bool bIsToRemove) { m_bIsToRemove = bIsToRemove; }
	int GetM_iDrawWidth() { return m_iDrawWidth; }
	int GetM_iDrawHeight() { return m_iDrawHeight; }
	void RedrawBackground();
	int GetValidX(int iX);
	int GetValidY(int iY);
	int GetValidWidth(int iX, int iWidth);
	int GetValidHeight(int iY, int iHeight);
	int GetM_iPreviousScreenX() { return m_iPreviousScreenX; }
	int GetM_iPreviousScreenY() { return m_iPreviousScreenY; }
	int GetM_iCurrentScreenX() { return m_iCurrentScreenX; }
	int GetM_iCurrentScreenY() { return m_iCurrentScreenY; }
};

