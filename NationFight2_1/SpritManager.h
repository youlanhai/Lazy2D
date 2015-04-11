#pragma once

#include <list>

class Object;

class CSpritManager
{
public:
	CSpritManager(void);
	~CSpritManager(void);
	bool add(Object *pObj);
	void update(Object *obj,RECT* rc);
	void render(HDC hdc);
	void destroy();
	Object* getByID(int id);
	Object* getByName(char *szName);

	std::list<Object*> lstObj;
};
