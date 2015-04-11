//MagicSystem.cpp
#include "Commen.h"
#include "MagicSystem.h"
CMagicSystem::CMagicSystem()
{
	nMagicCount=0;
	vecMagicList.resize(1);
}
CMagicSystem::~CMagicSystem()
{
	ClearData();
}
void CMagicSystem::ClearData()
{
	nMagicCount=0;
	vecMagicList.clear();
}
bool CMagicSystem::ReadDataFromFile(std::string strFileName)
{
	ClearData();
	std::ifstream fin(strFileName.c_str());
	if(fin.bad())
		return false;
	fin>>nMagicCount;
	vecMagicList.resize(nMagicCount+1);
	for(int i=0;i<nMagicCount;++i)
	{
		fin>>vecMagicList[i].strName;
		fin>>vecMagicList[i].nAttackHurt;
		fin>>vecMagicList[i].nAttackAdd;
		fin>>vecMagicList[i].nDeffenceAdd;
		fin>>vecMagicList[i].nMagicNeed;
		fin>>vecMagicList[i].nLastRound;
		fin>>vecMagicList[i].nEffectID;
	}
	fin.close();
	return true;
}
bool CMagicSystem::WriteDataFromFile(std::string strFileName)
{
	std::ofstream fout(strFileName.c_str());
	if(fout.bad())
		return false;
	fout<<nMagicCount<<std::endl;
	vecMagicList.resize(nMagicCount+1);
	for(int i=0;i<nMagicCount;++i)
	{
		fout<<vecMagicList[i].strName<<" ";
		fout<<vecMagicList[i].nAttackHurt<<" ";
		fout<<vecMagicList[i].nAttackAdd<<" ";
		fout<<vecMagicList[i].nDeffenceAdd<<" ";
		fout<<vecMagicList[i].nMagicNeed<<" ";
		fout<<vecMagicList[i].nLastRound<<" ";
		fout<<vecMagicList[i].nEffectID<<std::endl;
	}
	fout.close();
	return true;
}
CMagicNode* CMagicSystem::GetMagic(int nIndex)
{
	if(nIndex>=0 && nIndex<nMagicCount)
		return &vecMagicList[nIndex];
	return &vecMagicList[nMagicCount];
}