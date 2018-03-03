#pragma once
#include "WtlRect.h"
#include "WtlString.h"
#include <GdiPlus.h>
#include "BasicDebugInfo.h"
#include "TextureDef.h"
using namespace Gdiplus;
class CGdiPlusBitmapResource;


class CTexturePic
{
public:
	CTexturePic(void);
	~CTexturePic(void);
	// ��ʼ��������λͼ
	void Init(UINT nId);
	void SetShowPos(int x,int y);
	RECT GetShowPos();
	int OffSetShowPos(int nOffx,int nOffy);
	CSize GetTextureSize();
	// �ļ���ʾ״̬
	int SetState(TEXT_STATE state);
	TEXT_STATE GetState();
	// ������״̬
	int SetActiveState(bool bActive=true);
	bool GetActiveState();
	// ����
	void Draw(Graphics &myGraphics);
	// ������ʾ��λ��
	int SetText(CString szText);
	TCHAR* GetText();
private:
	CGdiPlusBitmapResource*		m_pBmpRes;
	Gdiplus::Bitmap*	m_pBitmap;
	bool		m_bActive;		// ������ˣ�һ��ֻ����һ�������
	bool		m_bVisual;
	double		m_Alpha;		// ͸����
	double		m_Angle;		// ��ת�Ƕ�
	POINT		m_ptShowPos;	// ��ʾ�����Ͻ�λ��
	CString		m_strText;		// ���������
	TEXT_STATE	m_state;
};
