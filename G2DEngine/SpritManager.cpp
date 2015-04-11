//CSpritManager.cpp
#include "Commen.h"
#include "SpritManager.h"
#include "Sprit.h"
#include <fstream>
CSpritManager::CSpritManager()
{
}
CSpritManager::~CSpritManager()
{
	Clear();
}
void CSpritManager::Clear()
{
	std::list<CSprit*>::iterator it,itEnd=spritList.end();
	for(it=spritList.begin();it!=itEnd;++it)
	{
		delete (*it);
	}
	spritList.clear();
}
bool CSpritManager::ReadFromFile(std::string fileName)
{
	Clear();
	std::ifstream fin(fileName.c_str());
	if(fin.fail())
		return false;
	CSprit *obj=NULL;
	while(!fin.eof())
	{
		obj=new CSprit;
		fin>>obj->nShowID;
		fin>>obj->nState;
		fin>>obj->strName;
		if(fin.eof())
		{
			delete obj;
			break;
		}
		fin>>obj->nMaxLife>>obj->nMaxMagic;
		fin>>obj->ptPosition.x>>obj->ptPosition.y>>obj->nWidth>>obj->nHeight;
		fin>>obj->actionRect.w>>obj->actionRect.h>>obj->actionRect.x>>obj->actionRect.y;
		fin>>obj->nAttack>>obj->nDefence>>obj->nSpeed>>obj->nExperence>>obj->nLevel;
		obj->nLife=obj->nMaxLife;
		obj->nMagic=obj->nMaxMagic;
		spritList.push_back(obj);
	}
	fin.close();
	return true;
}
bool CSpritManager::WriteToFile(std::string fileName)
{
	std::ofstream fout(fileName.c_str());
	if(fout.fail())
		return false;
	CGameObject *obj=NULL;
	std::list<CSprit*>::iterator it,itEnd=spritList.end();
	for(it=spritList.begin();it!=itEnd;++it)
	{
		obj=*it;
		fout<<obj->nShowID<<" "<<obj->nState<<" "<<obj->strName.c_str()<<std::endl;
		fout<<obj->nMaxLife<<" "<<obj->nMaxMagic<<std::endl;
		fout<<obj->ptPosition.x<<" "<<obj->ptPosition.y<<" "<<obj->nWidth<<" "<<obj->nHeight<<std::endl;
		fout<<obj->actionRect.h<<" "<<obj->actionRect.w<<" "
			<<obj->actionRect.x<<" "<<obj->actionRect.y<<std::endl;
		fout<<obj->nAttack<<" "<<obj->nDefence<<" "
			<<obj->nSpeed<<" "<<obj->nExperence<<" "<<obj->nLevel<<std::endl;
	}
	fout.close();
	return true;
}