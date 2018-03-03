#include "Win32Editor.h"

#define  ZY_EDITOR_DEFAULE 128
#define TEXTBOX_WIDTH       300      // �ı�����
#define TEXTBOX_HEIGHT      20       // �ı���߶�
#define  _TextBoxClass  _T("ZY_EDITOR")

#define TEXTBOX_MAXLENGTH   1024 // �ı������ı�����󳤶�

TCHAR	MyString[TEXTBOX_MAXLENGTH + 1] = _T("");     // �ı�
int    MyStringPosition = ::_tcslen(MyString);        // ����������ڵ�λ��

static CWin32Editor*		g_pEidtor = NULL;

LRESULT WINAPI TextMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return g_pEidtor->TextBoxWndProc(hWnd, msg, wParam, lParam);
}

CWin32Editor::CWin32Editor(void)
{
	g_pEidtor = this;
	m_pText = (TCHAR*)malloc(ZY_EDITOR_DEFAULE*sizeof(TCHAR));
	ZeroMemory(m_pText, ZY_EDITOR_DEFAULE*sizeof(TCHAR));
	m_bInitSuc = false;
}

CWin32Editor::~CWin32Editor(void)
{
	if (m_pText)
	{
		free(m_pText);
		m_pText= NULL;
	}
}

// ע��Ӧ�ó��򴰿���
ATOM CWin32Editor::RegClass(HINSTANCE hInst)
{
	WNDCLASSEX wc;
	::ZeroMemory(&wc, sizeof(wc));                 // ��Ϊһ����գ���Ϊ����δ��ֵ���ֶε�Ĭ��ֵΪ����NULL��

	wc.cbSize     = sizeof(wc);
	wc.style      = CS_HREDRAW | CS_VREDRAW;  // ָ�������ں��������ĳߴ緢���仯ʱ�����ػ洰��
	wc.hInstance  = hInst;
	wc.hbrBackground = (HBRUSH)( COLOR_APPWORKSPACE + 1);  // ָ�������ڱ���Ϊ����������ϵͳ��ɫ
	wc.lpszClassName = _TextBoxClass;          // ��ΪҪע�����������������ʱҪ�Դ�����Ϊ��ʶ��
	wc.lpfnWndProc = TextMsgProc;                      // ��Ϊ��������Ϣ�ĺ���

	return ::RegisterClassEx(&wc);                 // ����API����ע�ᴰ����
}

bool CWin32Editor::Create(HWND hParentWnd,HINSTANCE hInst)
{
	bool bRet = false;
	if(RegClass(hInst))
	{
		// ֮�´�����Ϊ�����ı�����ʾ�ڸ��������룬������λ��
		RECT parentWndRect;
		::GetClientRect(hParentWnd, &parentWndRect);  // ��ȡ�����ڿͻ�����λ��
		int left = (parentWndRect.right - TEXTBOX_WIDTH) / 2, top = (parentWndRect.bottom - TEXTBOX_HEIGHT) / 2;

		// �����ı���
		HWND hWnd = ::CreateWindow(_TextBoxClass, NULL, WS_CHILDWINDOW | WS_VISIBLE,
			left, top, TEXTBOX_WIDTH, TEXTBOX_HEIGHT,
			hParentWnd, NULL, hInst, NULL);
		DWORD dwErr = ::GetLastError();
		bRet = ::IsWindow(hWnd);
	}
	m_bInitSuc = bRet;
	return bRet;
}

bool CWin32Editor::IsOk()
{
	return m_bInitSuc;
}

HWND CWin32Editor::GetHwnd()
{
	return m_hWnd;
}

// ���´���
void CWin32Editor::MyUpdateWindow(HWND hWnd)
{
	RECT rect;
	::GetClientRect(hWnd, &rect);
	::InvalidateRect(hWnd, &rect, TRUE);
	::UpdateWindow(hWnd);
}

// �����ı�
void CWin32Editor::MyDrawText(HDC hDC)
{
	int len = ::_tcslen(MyString);
	::TextOut(hDC, 4, 2, MyString, len);
}

// ���ù��λ��
void CWin32Editor::MySetCaretPos(HWND hWnd)
{
	HDC hDC = ::GetDC(hWnd);

	SIZE size;
	::GetTextExtentPoint(hDC, MyString, MyStringPosition, &size);
	::SetCaretPos(4 + size.cx, 3);

	::ReleaseDC(hWnd, hDC);
}

LRESULT WINAPI CWin32Editor::TextBoxWndProc(HWND hWnd,UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT: 
	{  // ��������֮���Լ�һ�Դ����ţ���Ϊ����֮�¶���ı����ֲ���

		static PAINTSTRUCT ps;
		static RECT rect;
		HDC hDC = ::BeginPaint(hWnd, &ps);  // ��ʼ���Ʋ���

		::GetClientRect(hWnd, &rect);        // ��ȡ�ͻ����ĳߴ�
		::DrawEdge(hDC, &rect, EDGE_SUNKEN, BF_RECT);  // ���Ʊ߿�EDGE_SUNKEN��ʾ������ʽΪ��Ƕ��ʽ��BF_RECT��ʾ���ƾ��α߿�
		MyDrawText(hDC);                      // �����ı�
		::EndPaint(hWnd, &ps);               // �������Ʋ���
	} 
	break;

	case WM_SETFOCUS:
	{    // ��ý���
		::CreateCaret(hWnd, (HBITMAP)NULL, 1, TEXTBOX_HEIGHT-5);     // �������
		MySetCaretPos(hWnd);                            // ���ù��λ��
		::ShowCaret(hWnd);                   // ��ʾ���
	} 
	break;
	case WM_KILLFOCUS: // ʧȥ����
		::HideCaret(hWnd);                   // ���ع��
		::DestroyCaret();                    // ���ٹ��
		break;

	case WM_SETCURSOR: 
	{  // ���ù����״
		static HCURSOR hCursor = ::LoadCursor(NULL, IDC_IBEAM);
		::SetCursor(hCursor);	   
	} break;

	case WM_CHAR: 
	{    // �ַ���Ϣ
		TCHAR code = (TCHAR)wParam;
		int len = ::_tcslen(MyString);
		if(code < (TCHAR)' ' || len >= TEXTBOX_MAXLENGTH)
			return 0;

		::MoveMemory(MyString + MyStringPosition + 1, MyString + MyStringPosition, (len - MyStringPosition + 1) * sizeof(TCHAR));
		MyString[MyStringPosition ++] = code;

		MyUpdateWindow(hWnd);
		MySetCaretPos(hWnd);

	} 
	break;

	case WM_KEYDOWN: 
	{  // ��������Ϣ
		OnKeyDown(hWnd,wParam, lParam);
	} 
	break;

	case WM_LBUTTONDOWN: 
	{  // ��굥�������ù��λ��
		int x = LOWORD(lParam);
		HDC hDc = ::GetDC(hWnd);

		int strLen = ::_tcslen(MyString), strPos = 0;
		SIZE size;

		for (strPos=0; strPos<strLen; strPos++)
		{
			::GetTextExtentPoint(hDc, MyString, strPos, &size);

			if(size.cx + 4 >= x)
				break;
		}
		// �ַ��ĳ���
		MyStringPosition = strPos;
		::GetTextExtentPoint(hDc, MyString, strPos, &size);
		::SetCaretPos(size.cx + 4, 3);

		::ReleaseDC(hWnd, hDc);		
	}
	break;

	default:
		return ::DefWindowProc(hWnd, message, wParam, lParam);
	}

	return (LRESULT)0;
}

void CWin32Editor::OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	TCHAR code = (TCHAR)wParam;

	switch (code)
	{
	case VK_LEFT: // �����
		if (MyStringPosition > 0)
			MyStringPosition--;
		break;

	case VK_RIGHT:     // �ҹ���
		if (MyStringPosition < (int)::_tcslen(MyString))
			MyStringPosition++;
		break;

	case VK_HOME: // HOME ��
		MyStringPosition = 0;
		break;

	case VK_END:  // END ��
		MyStringPosition = ::_tcslen(MyString);
		break;

	case VK_BACK: // �˸��
		if (MyStringPosition > 0)
		{
			::MoveMemory(MyString + MyStringPosition - 1, MyString + MyStringPosition, (::_tcslen(MyString) - MyStringPosition + 1) * sizeof(TCHAR));
			MyStringPosition--;
			MyUpdateWindow(hWnd);
		}
		break;

	case VK_DELETE: {  // ɾ����
		int len = ::_tcslen(MyString);
		if (MyStringPosition < len)
		{
			::MoveMemory(MyString + MyStringPosition, MyString + MyStringPosition + 1, (::_tcslen(MyString) - MyStringPosition + 1) * sizeof(TCHAR));
			MyUpdateWindow(hWnd);
		}
	}
					break;
	}

	MySetCaretPos(hWnd);
}
