//CSpritManager.h
#pragma once
class CSprit;

class CSpritManager
{
public:
	CSpritManager();
	~CSpritManager();
	void Clear();
	bool ReadFromFile(std::string fileName);
	bool WriteToFile(std::string fileName);
	std::list<CSprit*> spritList;//¾«Áé
};