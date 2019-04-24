#pragma once
#include "BaseEngine.h"
class DemoAMain :
	public BaseEngine
{
public:
	DemoAMain();
	~DemoAMain();
	void SetupBackgroundBuffer();
	void MouseDown(int iButton, int iX, int iY);
	void KeyDown(int iKeyCode);
	int InitialiseObjects();
};

