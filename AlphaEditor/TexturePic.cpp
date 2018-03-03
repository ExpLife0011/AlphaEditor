#include "TexturePic.h"
#include "CGdiPlusBitmap.h"
#include "resource.h"

#define ZY_EMPTY_TEXT _T("�����������")

CTexturePic::CTexturePic(void)
{
	m_pBitmap = NULL;
	m_pBmpRes = new CGdiPlusBitmapResource();
	m_ptShowPos.x = 0;
	m_ptShowPos.y = 0;
	m_Angle = 0.0;
	m_Alpha = 0.8;
	m_bActive = false;
	m_bVisual = true;
	m_strText = _T("");
	m_state = TEXT_STATE_NORMAL;
}

CTexturePic::~CTexturePic(void)
{
	m_pBitmap = NULL;
	delete m_pBmpRes;
	m_pBmpRes = NULL;
	ZY_TRACEA("%s ����ok\n",__FUNCTION__);
}

void CTexturePic::Init(UINT nId)
{
	m_pBmpRes->Load(nId,_T("PNG"),NULL);
	m_pBitmap = *m_pBmpRes;
}

void CTexturePic::SetShowPos(int x,int y)
{
	m_ptShowPos.x = x;
	m_ptShowPos.y = y;
}

RECT CTexturePic::GetShowPos()
{
	RECT rt ={0};
	int x = m_ptShowPos.x;
	int y = m_ptShowPos.y;
	SetRect(&rt,x,y,x+m_pBitmap->GetWidth(),y+m_pBitmap->GetHeight());
	return rt;
}

int CTexturePic::OffSetShowPos(int nOffx,int nOffy)
{
	m_ptShowPos.x += nOffx;
	m_ptShowPos.y += nOffy;
	ZY_TRACEA("%s �ƶ�����\n",__FUNCTION__);
	return 0;
}

int CTexturePic::SetState(TEXT_STATE state)
{
	m_state = state;
	return 0;
}

TEXT_STATE CTexturePic::GetState()
{
	return m_state;
}

int CTexturePic::SetActiveState(bool bActive)
{
	m_bActive = bActive;
	return 0;
}

bool CTexturePic::GetActiveState()
{
	return m_bActive;
}

int CTexturePic::SetText(CString szText)
{
	m_strText = szText;
	return 0;
}

TCHAR* CTexturePic::GetText()
{
	return m_strText.GetBuffer(m_strText.GetLength());
}

RectF GetTextBoundsRect(const Font& font,const StringFormat& strFormat,const CString& szText)
{
	GraphicsPath graphicsPathObj;
	FontFamily fontfamily;
	font.GetFamily(&fontfamily);
	graphicsPathObj.AddString(szText,-1,&fontfamily,font.GetStyle(),font.GetSize(),\

		PointF(0,0),&strFormat);
	RectF rcBound;

	/// ��ȡ�߽緶Χ
	graphicsPathObj.GetBounds(&rcBound);
	return rcBound;
}


// GDIPlus��������ȵķ�����

SizeF GetTextBounds(const Font& font,const StringFormat& strFormat,const CString& szText)
{
	GraphicsPath graphicsPathObj;
	FontFamily fontfamily;
	font.GetFamily(&fontfamily);
	graphicsPathObj.AddString(szText,-1,&fontfamily,font.GetStyle(),font.GetSize(),\

	PointF(0,0),&strFormat);
	RectF rcBound;

	/// ��ȡ�߽緶Χ
	graphicsPathObj.GetBounds(&rcBound);
	/// �����ı��Ŀ��
	return SizeF(rcBound.Width,rcBound.Height);
}

void CTexturePic::Draw(Graphics &myGraphics)
{
	if (m_pBitmap)
	{
		// Notice the value 0.8 in row 4, column 4.
		ColorMatrix colorMatrix = {
			1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, m_Alpha, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
			// Create an ImageAttributes object and set its color matrix.
			ImageAttributes imageAtt;
			imageAtt.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault,
				ColorAdjustTypeBitmap);

			INT iWidth = m_pBitmap->GetWidth();
			INT iHeight = m_pBitmap->GetHeight();
			myGraphics.DrawImage(
				m_pBitmap, 
				Rect(m_ptShowPos.x, m_ptShowPos.y, iWidth, iHeight),  // Destination rectangle
				0,                             // Source rectangle X 
				0,                             // Source rectangle Y
				iWidth,                        // Source rectangle width
				iHeight,                       // Source rectangle height
				UnitPixel, 
				&imageAtt);
			
			FontFamily  fontFamily(L"Times New Roman");
			Font        font(&fontFamily, 24, FontStyleRegular, UnitPixel);
			PointF      pointF(m_ptShowPos.x + 20,m_ptShowPos.y +20);
			SolidBrush  solidBrush(Color(200, 0, 0, 0));

			//myGraphics.DrawString(m_strText, -1, &font, pointF, &solidBrush);
			RectF rectF(m_ptShowPos.x + 20, m_ptShowPos.y, iWidth - 20,iHeight);
			PointF pos(m_ptShowPos.x + 20, m_ptShowPos.y);
			StringFormat stringFormat;
			// �ھ��η�Χ�ڣ�ˮƽ����
			//stringFormat.SetAlignment(StringAlignmentCenter);
			// �ھ��η�Χ�ڣ����¾���
			stringFormat.SetLineAlignment(StringAlignmentCenter);
	
			CString  szText = m_strText;
			if (szText.IsEmpty())
				szText = ZY_EMPTY_TEXT;
			Pen *pPen = NULL;
			if (m_bActive)
			{
				if (szText.CompareNoCase(ZY_EMPTY_TEXT) == 0 || szText.IsEmpty()) // ����ʱ������ʾĳЩ�ַ�
					szText = _T("");

				// ����״̬��������
				pPen = new Pen(Color(250,129,190,235),2);
				ZY_TRACEA("���������\n");

				CGdiPlusBitmapResource bmp;
				if(bmp.Load(IDB_PNG_CURSOR,_T("PNG"),NULL))
				{
					Bitmap*pBmp = (Bitmap*)bmp;
					int nWidth = pBmp->GetWidth();
					// �����ڽ������ʾ��Ϣ
					CString  text=_T("��");
					
					RectF rect = GetTextBoundsRect(font,stringFormat,text);
					SizeF fontSize = GetTextBounds(font,stringFormat,text);
					myGraphics.DrawImage(pBmp,
						m_ptShowPos.x-rect.X,m_ptShowPos.y-rect.Y,
						(float)pBmp->GetWidth(),
						fontSize.Height);
				}
			}
			else if(m_state == TEXT_STATE_FOCUS)
			{
				// ��ǰ��꽹����������
				pPen = new Pen(Color(250,255,255,255),2);
				ZY_TRACEA("����������\n");
			}
			myGraphics.DrawString(szText, -1, &font, rectF, &stringFormat, &solidBrush);
			if (pPen)
			{
				//Rect rt(m_ptShowPos.x, m_ptShowPos.y,iWidth,iHeight);
				//myGraphics.DrawRectangle(pPen,rt);
				myGraphics.DrawRectangle(pPen,m_ptShowPos.x, m_ptShowPos.y,iWidth,iHeight);
				delete pPen;
			}
	}

}
