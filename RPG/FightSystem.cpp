#include "Commen.h"
#include "FightSystem.h"
#include "MySound.h"

/**************按钮消息组*********************/
void ButtonCallAttack(void* pParam)
{
	CFightSystem *pThis=(CFightSystem*)pParam;
	pThis->SetFightMode(FM_ATTACK);
}
void ButtonCallDeffence(void* pParam)
{
	CFightSystem *pThis=(CFightSystem*)pParam;
	pThis->SetFightMode(FM_DEFENCE);
}
void ButtonCallMagic(void* pParam)
{
	CFightSystem *pThis=(CFightSystem*)pParam;
	pThis->SetFightMode(FM_MAGIC);
}
void ButtonCallUseGoods(void* pParam)
{
	CFightSystem *pThis=(CFightSystem*)pParam;
	pThis->SetFightMode(FM_GOODS);
}
void ButtonCallEscape(void* pParam)
{
	CFightSystem *pThis=(CFightSystem*)pParam;
	pThis->SetFightMode(FM_ESCAPE);
}
/**************按钮消息组*********************/


/**************class CFightSystem*************/

CFightSystem::CFightSystem()
{
	nRound=0;
	bPlayerDecision=true;
	pPlayer=NULL;
	pEnemy=NULL;
	bFighting=false;
	fmFightMode=FM_NONE;
	nUseMagicID=-1;
	bSpritShowEnd=true;
	bMagicShowEnd=true;
	hWindowHandle=NULL;
	szShowPlayerInformation[0]=0;
	szShowEnemyInformation[0]=0;
    fightResult = 0;
}
CFightSystem::~CFightSystem()
{

}
bool CFightSystem::Initialize(HWND hWnd,HINSTANCE hInstance)
{
	this->hWindowHandle=hWnd;
	this->hInstance=hInstance;
	return true;
}
bool CFightSystem::SetBackGroundBitmap(std::string strFileName)
{
	if(!bmpBackground.CreateBitmapFromFileList(hInstance,strFileName))
		return false;
	LBitmapInfo bmi=bmpBackground.GetAt(0);
	if(bmi)
	{
		RECT rc;
		GetClientRect(hWindowHandle,&rc);
		rcBackGround.w=bmi->nWidth;
		rcBackGround.h=bmi->nHeight;
		rcBackGround.x=(rc.right - bmi->nWidth)/2;
		rcBackGround.y=(rc.bottom - bmi->nHeight)/2;
		if(rcBackGround.x<0)
		{
			rcBackGround.x=0;
			rcBackGround.w=rc.right;
		}
		if(rcBackGround.y<0)
		{
			rcBackGround.y=0;
			rcBackGround.h=rc.bottom;
		}
	}
	return true;
}
bool CFightSystem::SetFighterHeadBitmap(std::string strFileName)
{
	if(!bmpFighterHead.CreateBitmapFromFileList(hInstance,strFileName))
		return false;
	return true;
}
bool CFightSystem::SetFightFormBitmap(std::string strFileName)
{//战斗菜单布局格式为仙剑战斗菜单布局
	if(!fightBitmap.CreateBitmapFromFileList(hInstance,strFileName))
		return false;

	fightForm.InitForm("",FORM_POPUP);
	fightForm.SetBitmap(&fightBitmap,18);
	fightForm.SetRect(200,300);
	fightForm.SetState(FORM_SHOW);
	
	CGameButton* pButton=NULL;
	
	pButton=new CGameButton("",BUTTON_DRAW);//技
	pButton->SetBitmap(&fightBitmap,0);
	pButton->SetRect(54,0);
	fightForm.AddButton(pButton);
	
	pButton=new CGameButton("",BUTTON_DRAW,ButtonCallMagic);//术
	pButton->SetBitmap(&fightBitmap,3);
	pButton->SetRect(0,52);
	fightForm.AddButton(pButton);
	
	pButton=new CGameButton("",BUTTON_DRAW,ButtonCallAttack);//攻
	pButton->SetBitmap(&fightBitmap,6);
	pButton->SetRect(51,52);
	fightForm.AddButton(pButton);
	
	pButton=new CGameButton("",BUTTON_DRAW,ButtonCallDeffence);//防
	pButton->SetBitmap(&fightBitmap,9);
	pButton->SetRect(108,54);
	fightForm.AddButton(pButton);
	
	pButton=new CGameButton("",BUTTON_DRAW,ButtonCallUseGoods);//物
	pButton->SetBitmap(&fightBitmap,12);
	pButton->SetRect(54,108);
	fightForm.AddButton(pButton);
	
	pButton=new CGameButton("",BUTTON_DRAW,ButtonCallEscape);//逃
	pButton->SetBitmap(&fightBitmap,15);
	pButton->SetRect(86,86);
	fightForm.AddButton(pButton);

	return true;
}
bool CFightSystem::SetGoodsFormBitmap(std::string strFileName)
{
	if(!goodsBitmap.CreateBitmapFromFileList(hInstance,strFileName))
		return false;
	return true;
}
bool CFightSystem::SetUIBitmap(std::string strFileName)
{
	if(!uiBitmap.CreateBitmapFromFileList(hInstance,strFileName))
		return false;
	return true;
}
bool CFightSystem::SetMagicList(std::string strFileName)
{
	if(!magicSystem.ReadDataFromFile(strFileName))
		return false;
	return true;
}
void CFightSystem::SetFighter(CGameObject* player,CGameObject* enemy)
{
	pPlayer=player;
	pEnemy=enemy;
	//计算玩家的显示区域
	rcPlayer.x=rcBackGround.x + rcBackGround.w - player->nWidth - 100;
	rcPlayer.y=(rcBackGround.y + rcBackGround.h - player->nHeight)/2;
	rcPlayer.w=player->nWidth;
	rcPlayer.h=player->nHeight;
	//计算敌人的显示区域
	rcEnemy.x=rcBackGround.x + 100;
	rcEnemy.y=rcPlayer.y + player->nHeight - enemy->nHeight;//和玩家底y值相同
	rcEnemy.w=enemy->nWidth;
	rcEnemy.h=enemy->nHeight;
	//初始化数据
	bPlayerDecision=true;
	nRound=1;
	bFighting=false;
	bSpritShowEnd=true;
	bMagicShowEnd=true;
	playerEffect.Initialize(1,rcPlayer.x,rcPlayer.y);
	enemyEffect.Initialize(0,rcEnemy.x,rcEnemy.y);
	szShowPlayerInformation[0]=0;
	szShowEnemyInformation[0]=0;
}
void CFightSystem::SetBackGroundRect(int x,int y,int width,int height)
{
	rcBackGround=CGameRect(x,y,width,height);
}
void CFightSystem::ChoiceFightMode(HDC hDC)
{
	if(bPlayerDecision)//玩家决策
	{
		fightForm.SetState(FORM_SHOW);
		fightForm.ProcessMessage(hWindowHandle,this);
		fightForm.Show(hDC);
	}
	else
	{
		fmFightMode=(FightMode)(rand()%5);
	}
}
void CFightSystem::ChoiceMagic()//选择魔法
{
	if(bPlayerDecision)//玩家决策
	{
		nUseMagicID=1;
	}
	else
	{
		nUseMagicID=0;
	}
}

int	CFightSystem::FightLoop(HDC hDC)
{
	//在此绘制背景
	DrawBackGround(hDC);
	if(bFighting)
	{//战斗进行中
		if(bPlayerDecision)
		{//玩家决策
			if(bSpritShowEnd && bMagicShowEnd)
			{//显示完毕
				if(pEnemy->nLife<=0)
				{
					StopSound(0);
					DeleteSound(0);
					LoadWave(0,"sound/dead.wav");
					PlaySound(0);
					pEnemy->nState=0;
                    fightResult = 1;
					return 1;
				}
				++nRound;//增加回合计数
				bFighting=false;//回合结束
				bPlayerDecision=!bPlayerDecision;//切换
				//恢复数据
				fmFightMode=FM_NONE;
				nUseMagicID=-1;
				playerEffect.Initialize(1,rcPlayer.x,rcPlayer.y);
				szShowPlayerInformation[0]=0;
				szShowEnemyInformation[0]=0;
			}
			else
			{
				if(false==bSpritShowEnd)
				{
					bSpritShowEnd=playerEffect.Show(hDC);
					if(bSpritShowEnd)
						playerEffect.Initialize(1,rcPlayer.x,rcPlayer.y);//改成常规模式
				}
				else
				{
					playerEffect.Show(hDC);	
				}
				
			}
			enemyEffect.Show(hDC);
		}
		else
		{//敌人决策
			if(bSpritShowEnd && bMagicShowEnd)
			{
                if (pPlayer->nLife <= 0)
                {
                    fightResult = 2;
                    return 2;
                }
				++nRound;//增加回合计数
				bFighting=false;//回合结束
				bPlayerDecision=!bPlayerDecision;//切换
				//恢复数据
				fmFightMode=FM_NONE;
				nUseMagicID=-1;
				enemyEffect.Initialize(0,rcEnemy.x,rcEnemy.y);//改成常规模式
				szShowPlayerInformation[0]=0;
				szShowEnemyInformation[0]=0;
			}
			else
			{
				if(false==bSpritShowEnd)
				{
					bSpritShowEnd=enemyEffect.Show(hDC);
					if(bSpritShowEnd)
						enemyEffect.Initialize(0,rcEnemy.x,rcEnemy.y);//改成常规模式
				}
				else
				{
					enemyEffect.Show(hDC);
				}
			}
			playerEffect.Show(hDC);
		}
		if(false==bMagicShowEnd)
		{
			bMagicShowEnd=magicEffect.Show(hDC);
		}
	}
	else
	{//战斗选择
		playerEffect.Show(hDC);
		enemyEffect.Show(hDC);
		if(fmFightMode==FM_NONE)//如果没有做出战斗选择，则进入战斗方式选择界面
		{
			ChoiceFightMode(hDC);	
			if(fmFightMode==FM_NONE)//如果仍然没有做出选择，则返回，等待下次继续选择
				goto CFIGHTSYSTEM_FIGHTLOOP_END;
			else
			{
				fightForm.SetState(FORM_HIDE);
			}
		}
		if(fmFightMode==FM_MAGIC)
		{
			if(nUseMagicID==-1)//如果没有做出选择，则进入选择界面选择。
				ChoiceMagic();
			if(nUseMagicID==-1)//如果仍然没有做出选择，则退出，等待下次继续选择。
				goto CFIGHTSYSTEM_FIGHTLOOP_END;
		}
		switch(fmFightMode)
		{
		case FM_ATTACK:	//攻击
			{
				Attack();
			}
			break;
		case FM_MAGIC:	//法术
			{
				Magic();
			}
			break;
		case FM_GOODS:	//物品
			{
				UseGoods();
			}
			break;
		case FM_ESCAPE:	//逃跑
			{
				Escape();
			}
			break;
		case FM_DEFENCE:
			{
				Deffence();
			}
			break;
		default:
			goto CFIGHTSYSTEM_FIGHTLOOP_END;
		}
		bFighting=true;
	}
CFIGHTSYSTEM_FIGHTLOOP_END:

	//在此绘制精灵状态
	DrawFighterState(hDC);

	return 0;
}
void CFightSystem::DrawBackGround(HDC hDC)
{
	LBitmapInfo bmi=bmpBackground.GetAt(0);
	if(bmi)
	{
		HDC hMemDC=CreateCompatibleDC(0);
		SelectObject(hMemDC,bmi->hBitmap);
		BitBlt(hDC,rcBackGround.x,rcBackGround.y,
			rcBackGround.w,rcBackGround.h,
			hMemDC,0,0,SRCCOPY);
		DeleteObject(hMemDC);
	}
}
void CFightSystem::DrawFighterState(HDC hDC)
{
	if(0!=szShowPlayerInformation[0])
	{
		TextOut(hDC,rcPlayer.x,rcPlayer.y-40,szShowPlayerInformation,
			strlen(szShowPlayerInformation));
	}
	if(0!=szShowEnemyInformation[0])
	{
		TextOut(hDC,rcEnemy.x,rcEnemy.y-40,szShowEnemyInformation,
			strlen(szShowEnemyInformation));
	}
	LBitmapInfo bmi=uiBitmap.GetAt(0);
	if(NULL!=bmi)
	{
		RECT rc;
		GetClientRect(hWindowHandle,&rc);
		int nStateX=rc.right - bmi->nWidth;
		int nStateY=rc.bottom - bmi->nHeight;
		DrawFighterState2(hDC,nStateX,nStateY,pPlayer);
		DrawFighterState2(hDC,0,nStateY,pEnemy);
	}
}
void CFightSystem::DrawFighterState2(HDC hDC,int nStateX,int nStateY,CGameObject* pSprit)
{
	LBitmapInfo bmi=uiBitmap.GetAt(0);
	if(NULL!=bmi)
	{
		int nBloodLength=(96*pSprit->nLife)/pSprit->nMaxLife;
		if(nBloodLength<0)nBloodLength=0;
		int nMagicLenght=(96*pSprit->nMagic)/pSprit->nMaxMagic;
		if(nMagicLenght<0)nMagicLenght=0;
		HDC hMemDC=CreateCompatibleDC(0);
		SelectObject(hMemDC,bmi->hBitmap);
		//绘制到屏幕
		TransparentBlt(hDC,nStateX,nStateY,bmi->nWidth,bmi->nHeight,
			hMemDC,0,0,bmi->nWidth,bmi->nHeight,GetPixel(hMemDC,0,0));
		bmi=uiBitmap.GetAt(pSprit->nShowID+1);
		if(bmi)
		{
			SelectObject(hMemDC,bmi->hBitmap);
			TransparentBlt(hDC,nStateX+5,nStateY+5,50,50,
				hMemDC,0,0,bmi->nWidth,bmi->nHeight,GetPixel(hMemDC,0,0));
		}
		//绘制血槽
		HBRUSH hRedBrush=CreateSolidBrush(RGB(255,0,0));
		HBRUSH hOldBrush=(HBRUSH)SelectObject(hDC,hRedBrush);
		Rectangle(hDC,nStateX+54,nStateY+80,nStateX+54+nBloodLength,nStateY+84);
		//绘制气槽
		HBRUSH hBlueBrush=CreateSolidBrush(RGB(0,0,255));
		SelectObject(hDC,hBlueBrush);
		Rectangle(hDC,nStateX+54,nStateY+96,nStateX+54+nMagicLenght,nStateY+100);
		//绘制名称
		RECT rc;
		rc.left=nStateX+62;
		rc.top=nStateY+22;
		rc.right=nStateX+156;
		rc.bottom=nStateY+34;
		COLORREF crOld=SetTextColor(hDC,RGB(255,0,0));
		int nOldBkMode=SetBkMode(hDC,TRANSPARENT);
		DrawText(hDC,pSprit->strName.c_str(),pSprit->strName.size(),
			&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		SelectObject(hDC,hOldBrush);
		SetTextColor(hDC,crOld);
		SetBkMode(hDC,nOldBkMode);
		DeleteDC(hMemDC);
		DeleteObject(hRedBrush);
		DeleteObject(hBlueBrush);
	}
}
int	CFightSystem::Attack()
{
	if(bPlayerDecision)
	{
		int nDLife=pPlayer->nAttack - pEnemy->nDefence+rand()%(pPlayer->nLevel*10+10);
		if(nDLife<=0)
			nDLife=1;
		pEnemy->nLife-=nDLife;
		szShowPlayerInformation[0]=0;
		sprintf(szShowEnemyInformation,"生命-%d",nDLife);
		playerEffect.Initialize(3,rcEnemy.x,rcEnemy.y);
	}
	else
	{
		int nDLife=pEnemy->nAttack - pPlayer->nDefence+rand()%(pEnemy->nLevel*10+10);
		if(nDLife<=0)
			nDLife=1;
		pPlayer->nLife-=nDLife;
		szShowEnemyInformation[0]=0;
		sprintf(szShowPlayerInformation,"生命-%d",nDLife);
		enemyEffect.Initialize(2,rcPlayer.x,rcPlayer.y);
	}
	StopSound(0);
	DeleteSound(0);
	LoadWave(0,"sound/girlhurt.wav");
	PlaySound(0);
	bSpritShowEnd=false;
	bMagicShowEnd=true;
	return 0;
}
int	CFightSystem::Deffence()
{
	bSpritShowEnd=true;
	bMagicShowEnd=true;
	return 0;
}
int	CFightSystem::Magic()
{
	CMagicNode* pMagicNode=magicSystem.GetMagic(nUseMagicID);
	if(bPlayerDecision)
	{
		int nDLife=pPlayer->nAttack+pMagicNode->nAttackHurt - pEnemy->nDefence+rand()%(pPlayer->nLevel*10+10);
		if(nDLife<=0)
			nDLife=1;
		pEnemy->nLife-=nDLife;
		szShowPlayerInformation[0]=0;
		sprintf(szShowEnemyInformation,"生命-%d",nDLife);
		magicEffect.Initialize(pMagicNode->nEffectID,rcEnemy.x,rcEnemy.y);
	}
	else
	{
		int nDLife=pEnemy->nAttack+pMagicNode->nAttackHurt - pPlayer->nDefence+rand()%(pEnemy->nLevel*10+10);
		if(nDLife<=0)
			nDLife=1;
		pPlayer->nLife-=nDLife;
		szShowEnemyInformation[0]=0;
		sprintf(szShowPlayerInformation,"生命-%d",nDLife);
		magicEffect.Initialize(pMagicNode->nEffectID,rcPlayer.x,rcPlayer.y);
	}
	playerEffect.Initialize(1,rcPlayer.x,rcPlayer.y);
	enemyEffect.Initialize(0,rcEnemy.x,rcEnemy.y);
	bSpritShowEnd=false;
	bMagicShowEnd=false;
	StopSound(1);
	DeleteSound(1);
	LoadWave(1,"sound/lighting.wav");
	PlaySound(1);
	return 0;
}
int	CFightSystem::UseGoods()
{
	bSpritShowEnd=false;
	bMagicShowEnd=true;
	return 0;
}
int	CFightSystem::Escape()
{
	bSpritShowEnd=false;
	bMagicShowEnd=true;
	return 0;
}