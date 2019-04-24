#include "header.h"
#include "templates.h"

#include "GameObject.h"
#include "Explosion.h"


GameObject::GameObject(Psysl5Engine* pEngine)
	: DisplayableObject(pEngine)
	, m_bIsToRemove(false)
	, m_pEngine(pEngine)
{
}


GameObject::~GameObject()
{
}


void GameObject::RedrawBackground()
{
	m_pEngine->CopyBackgroundPixels(
		GetValidX(m_iPreviousScreenX + m_iStartDrawPosX),
		GetValidY(m_iPreviousScreenY + m_iStartDrawPosY),
		GetValidWidth(m_iPreviousScreenX + m_iStartDrawPosX, m_iDrawWidth),
		GetValidHeight(m_iPreviousScreenY + m_iStartDrawPosY, m_iDrawHeight)
		);
}


int GameObject::GetValidX(int iX)
{
	if (iX < 0)
		return 0;
	else if (iX > GetEngine()->GetScreenWidth())
		return GetEngine()->GetScreenWidth();
	else
		return iX;
}


int GameObject::GetValidY(int iY)
{
	if (iY < 0)
		return 0;
	else if (iY > GetEngine()->GetScreenHeight())
		return GetEngine()->GetScreenHeight();
	else
		return iY;
}


int GameObject::GetValidWidth(int iX, int iWidth)
{
	if (iX < 0)
		return iWidth - abs(iX);
	else if (iX > GetEngine()->GetScreenWidth() - iWidth)
		return GetEngine()->GetScreenWidth() - iX;
	else
		return iWidth;
}


int GameObject::GetValidHeight(int iY, int iHeight)
{
	if (iY < 0)
		return iHeight - abs(iY);
	else if (iY > GetEngine()->GetScreenHeight() - iHeight)
		return GetEngine()->GetScreenHeight() - iY;
	else
		return iHeight;
}


