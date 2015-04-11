//Scene.cpp
#include "Commen.h"
#include "Scene.h"

CScene::CScene()
{
	ptPlayerPosition.x=0;
	ptPlayerPosition.y=0;
}
CScene::~CScene()
{

}
bool CScene::ReadFromFile(std::string fileName)
{
	std::ifstream fin(fileName.c_str());
	if(fin.bad())
		return false;
	fin>>mapName>>spritName>>speakName>>backMusic;
	for(int i=0;i<4;++i)
	{
		fin>>swField[i].bUsefull;
		fin>>swField[i].newPos.x>>swField[i].newPos.y;
		fin>>swField[i].rect.x>>swField[i].rect.y
			>>swField[i].rect.w>>swField[i].rect.h;
		fin>>swField[i].newSceneName;
	}
	fin.close();
	return true;
}
bool CScene::WriteToFile(std::string fileName)
{
	std::ofstream fout(fileName.c_str());
	if(fout.bad())
		return false;
	fout<<mapName<<std::endl;
	fout<<spritName<<std::endl;
	fout<<speakName<<std::endl;
	fout<<backMusic<<std::endl;
	for(int i=0;i<4;++i)
	{
		fout<<swField[i].bUsefull<<std::endl;
		fout<<swField[i].newPos.x<<" "<<swField[i].newPos.y<<std::endl;
		fout<<swField[i].rect.x<<" "<<swField[i].rect.y<<" "
			<<swField[i].rect.w<<" "<<swField[i].rect.h<<std::endl;
		fout<<swField[i].newSceneName.c_str()<<std::endl;
	}
	fout.close();
	return true;
}
bool CScene::CheckSwitchScene(POINT* ptPosition)
{
	for(int i=0;i<4;++i)
		if(swField[i].bUsefull && swField[i].rect.IsIn(ptPosition))
		{
			ptPlayerPosition.x=swField[i].newPos.x;
			ptPlayerPosition.y=swField[i].newPos.y;
			ReadFromFile(swField[i].newSceneName);
			return true;
		}
	return false;
}