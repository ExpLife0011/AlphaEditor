#pragma once
#include <Windows.h>
#include <tchar.h>


class CWin32Editor
{
public:
	CWin32Editor(void);
	~CWin32Editor(void);
public:
	bool Create(HWND hParentWnd,HINSTANCE hInst);
	bool IsOk();
	LRESULT  WINAPI TextBoxWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void OnKeyDown(HWND hWnd,WPARAM wParam, LPARAM lParam);

	HWND	GetHwnd();
	operator HWND() const { return m_hWnd; }
protected:
	void MyDrawText(HDC hDC);                           // �����ı�
	void MySetCaretPos(HWND hWnd);                      // ���ù��λ��
	void MyUpdateWindow(HWND hWnd);                     // ���´���

private:
	ATOM RegClass(HINSTANCE hInst);

private:
	HWND		m_hWnd;	
	TCHAR		*m_pText;
	bool		m_bInitSuc;
};
