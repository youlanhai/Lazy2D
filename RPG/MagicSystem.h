//MagicSystem.h
#pragma once
class CMagicNode
{
public:
	std::string strName;	//技能名称
	int		nAttackHurt;	//攻击伤害
	int		nAttackAdd;		//攻击增加
	int		nDeffenceAdd;	//防御增加
	int		nMagicNeed;		//需要的魔法值
	int		nLastRound;		//持续的回合,仅对增益属性起作用
	int		nEffectID;		//特效索引
	CMagicNode()
	{
		nAttackHurt=0;
		nAttackAdd=nDeffenceAdd=0;
		nLastRound=nEffectID=0;
		nMagicNeed=0;
	}
	CMagicNode(const CMagicNode& magic)
	{
		strName=magic.strName;
		nAttackHurt=magic.nAttackHurt;
		nAttackAdd=magic.nAttackAdd;
		nDeffenceAdd=magic.nDeffenceAdd;
		nLastRound=magic.nLastRound;
		nEffectID=magic.nEffectID;
		nMagicNeed=magic.nMagicNeed;
	}
};

class CMagicSystem
{
	int		nMagicCount;
	std::vector<CMagicNode>	vecMagicList;
public:
	CMagicSystem();
	~CMagicSystem();
	void ClearData();
	bool ReadDataFromFile(std::string strFileName);
	bool WriteDataFromFile(std::string strFileName);
	int  GetCount(){return nMagicCount;}
	CMagicNode* GetMagic(int nIndex);
};