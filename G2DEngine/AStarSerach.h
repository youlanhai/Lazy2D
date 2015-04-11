//AStarSearch.h
#pragma once

class CGameMap;
/*******************************************
�ķ��������Ĺ��ۺ�������:
	�����Ĺ���nValueToStart:���Ĺ���Ϊ0�������Ĺ���Ϊ�丸�ڵ�Ĺ���ֵ+1��
	���յ�Ĺ���nValueToEnd:Ϊ��ǰ�㵽�յ����ķ�������
���ܵĹ���value��value=nValueToStart+nValueToEnd��
����nValueToEnd�Ƕ�̬�����ֵ���ʿɺ��ԣ����ô���������
*********************************************/
typedef struct PathNode //·�����
{
	int nXIndex;		//���±�
	int nYIndex;		//���±�
	int nMark;			//���λ����Ǹý���״̬0-δ���ʣ�1-λ��open������ʣ�2-�Ѿ����ʹ�
	int nValueToStart;	//�����Ĺ���ֵ
	int nTotalValue;	//���ܹ���ֵnTotalValue=nValueToStart+nValueToEnd,��Ŀ����Ĺ���ֵ��̬���㡣
	PathNode* pParent;//�ý��ĸ��ڵ�
}PathNode,*LPathNode;

/*******************
CAStarSearch��A*Ѱ·��
�÷���
1.CreatePath����·���ṹ������ͼ������С��������ݽ�����
2.SearchPath��ʼѰ·���ɹ����ش��յ㵽���·�������ͷָ�룬ʧ�ܷ���NULL��
3.Ѱ·ʱ��Ҫ��ϵ�ͼ(map)�����ϰ�����ʱ�����޸�SerchPath����Ӧλ�á�
*******************/
class CAStarSearch
{
public:
	CAStarSearch();
	~CAStarSearch();
	void ClearPathData();	//���·���ṹ����
	void Destroy();			//����·ϵͳ
	bool CreatePath(int nXCount,int nYCount);//����·���ṹ��nYCountΪ����,nXCountΪ����
	LPathNode	SearchPath//Ѱ·
	(
		int nStartXIndex,	//��ʼ�����±�
		int nStartYIndex,	//��ʼ�����±�
		int nEndXindex,		//�յ����±�
		int nEndYIndex,		//������±�
		CGameMap* mapSystem		//��ͼϵͳ�����ڼ���ͨ��
	);
private:
	int nXCount;			//��ͼx���򷽸������������
	int nYCount;			//��ͼy���򷽸������������
	LPathNode* ppPath;		//������ͼ������Ϣ�Ķ�ά�ṹ������
	std::list<LPathNode> open;//open����Ŵ����õ�
};
