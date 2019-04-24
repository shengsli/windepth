#include "header.h"
#include "templates.h"

#include "SubLaunchedObject.h"


SubLaunchedObject::SubLaunchedObject(Psysl5Engine* pEngine, Submarine* pSubmarine)
	:GameObject(pEngine)
	, m_pSubmarine(pSubmarine)
{
}


SubLaunchedObject::~SubLaunchedObject()
{
}
