#pragma once
#include "DisplayableObject.h"
class DemoBObject :
	public DisplayableObject
{
public:
	DemoBObject(BaseEngine* pEngine);
	~DemoBObject(void);
	void Draw();
	void DemoBObject::DoUpdate(int iCurrentTime);
};

