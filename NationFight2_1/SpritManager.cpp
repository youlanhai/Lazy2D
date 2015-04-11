#include "../G2DEngine/Commen.h"
#include "Object.h"
#include "SpritManager.h"

CSpritManager::CSpritManager(void)
{
}

CSpritManager::~CSpritManager(void)
{
	destroy();
}
bool CSpritManager::add(Object *pObj)
{
	lstObj.push_back(pObj);
	return true;
}
void CSpritManager::update(Object *obj,RECT* rc)
{
	std::list<Object*>::iterator it,itTemp;
	Object *pObj=NULL;
	it=lstObj.begin();
	while( it!=lstObj.end())
	{
		pObj = (*it);
		if (0 == pObj->state)
		{
			delete pObj;
			itTemp=it++;
			lstObj.erase(itTemp);
		}
		else
		{
			pObj->AI(obj,rc);
			++it;
		}
	}
}
void CSpritManager::render(HDC hdc)
{
	std::list<Object*>::iterator it,itEnd;
	it=lstObj.begin();
	itEnd=lstObj.end();
	Object *pObj=NULL;
	for (; it!=itEnd; ++it)
	{
		pObj = (*it);
		pObj->show(hdc);
	}
}
void CSpritManager::destroy()
{
	std::list<Object*>::iterator it,itEnd;
	it=lstObj.begin();
	itEnd=lstObj.end();
	for (; it!=itEnd; ++it)
	{
		delete (*it);
	}
	lstObj.clear();
}
Object* CSpritManager::getByID(int id)
{
	std::list<Object*>::iterator it,itEnd;
	it=lstObj.begin();
	itEnd=lstObj.end();
	for (; it!=itEnd; ++it)
	{
		if((*it)->getID() == id)
		{
			return (*it);
		}
	}
	return NULL;
}
Object* CSpritManager::getByName(char *szName)
{
	std::list<Object*>::iterator it,itEnd;
	it=lstObj.begin();
	itEnd=lstObj.end();
	for (; it!=itEnd; ++it)
	{
		if(strcmp((*it)->name,szName)==0)
		{
			return (*it);
		}
	}
	return NULL;
}