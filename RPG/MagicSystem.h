//MagicSystem.h
#pragma once
class CMagicNode
{
public:
	std::string strName;	//��������
	int		nAttackHurt;	//�����˺�
	int		nAttackAdd;		//��������
	int		nDeffenceAdd;	//��������
	int		nMagicNeed;		//��Ҫ��ħ��ֵ
	int		nLastRound;		//�����Ļغ�,������������������
	int		nEffectID;		//��Ч����
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