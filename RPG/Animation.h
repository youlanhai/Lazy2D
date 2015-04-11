//Animation.h
#pragma once
class CBitmapManager;

//������
class CAnimation
{
public:
	CAnimation();
	~CAnimation();

	bool	Initialize(HINSTANCE hInstance,std::string strBitmapListName);//��ʼ��������Դ
	int		DrawHeader(HDC hBackDC,RECT* rcClient);//��ͷ����,���Ž�������1
	int		DrawAnimation(HDC hBackDC,RECT* rcClient);//������Ч
	void	ClearAnimation();				//���������Դ

    bool    IsFinished() const { return m_index >= 256 * 2; }
private:
	CBitmapManager  bmpAnimManager;//����λͼ������
    int             m_index;
};