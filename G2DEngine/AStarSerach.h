//AStarSearch.h
#pragma once

class CGameMap;
/*******************************************
四方向搜索的估价函数计算:
	到起点的估价nValueToStart:起点的估价为0，其余点的估价为其父节点的估价值+1。
	到终点的估价nValueToEnd:为当前点到终点相差的方格数。
则总的估价value：value=nValueToStart+nValueToEnd。
由于nValueToEnd是动态计算的值，故可忽略，不用存贮起来。
*********************************************/
typedef struct PathNode //路径结点
{
	int nXIndex;		//列下标
	int nYIndex;		//行下标
	int nMark;			//标记位。标记该结点的状态0-未访问，1-位于open表待访问，2-已经访问过
	int nValueToStart;	//到起点的估价值
	int nTotalValue;	//到总估价值nTotalValue=nValueToStart+nValueToEnd,到目标结点的估价值动态计算。
	PathNode* pParent;//该结点的父节点
}PathNode,*LPathNode;

/*******************
CAStarSearch，A*寻路类
用法：
1.CreatePath创建路径结构。将地图方格的行、列数传递进来。
2.SearchPath开始寻路。成功返回从终点到起点路径链表的头指针，失败返回NULL。
3.寻路时若要结合地图(map)数组障碍进行时，需修改SerchPath的相应位置。
*******************/
class CAStarSearch
{
public:
	CAStarSearch();
	~CAStarSearch();
	void ClearPathData();	//清空路径结构数据
	void Destroy();			//销毁路系统
	bool CreatePath(int nXCount,int nYCount);//创建路径结构，nYCount为行数,nXCount为列数
	LPathNode	SearchPath//寻路
	(
		int nStartXIndex,	//起始点列下标
		int nStartYIndex,	//起始点行下标
		int nEndXindex,		//终点列下标
		int nEndYIndex,		//起点行下标
		CGameMap* mapSystem		//地图系统，用于检测可通行
	);
private:
	int nXCount;			//地图x方向方格个数，即列数
	int nYCount;			//地图y方向方格个数，即行数
	LPathNode* ppPath;		//存贮地图方格信息的二维结构体数组
	std::list<LPathNode> open;//open表，存放待检测得点
};
