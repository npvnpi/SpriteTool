#pragma once
class Window
{
private:
	HWND		m_hWnd;
	HINSTANCE   m_hInstance;
	int			m_width;
	int			m_height;
	wchar_t		m_className [30] = L"";
	wchar_t		m_title[30] = L"";


public:
	Window(HINSTANCE hInstance, int width, int height, const wchar_t* className, const wchar_t* title);
	~Window();

	bool Init(WNDPROC wndProc, int nCmdShow);
	void Run();
	LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
	HWND GetHWND() { return m_hWnd; }
	void SetHWND(HWND hWnd) { m_hWnd = hWnd; }
};

