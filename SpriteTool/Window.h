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
	wchar_t		m_pixelText[256] = L"";
	ID2D1HwndRenderTarget* m_rt;
	ID2D1BitmapRenderTarget* m_brt;
	Direct2DBitmap* m_targetBitmap;
	LONG			m_keyColor;
	IDWriteTextFormat* m_pTextFormat;
	ID2D1SolidColorBrush* m_blackColor;
	ID2D1SolidColorBrush* m_whiteColor;
public:
	Window(HINSTANCE hInstance, int width, int height, const wchar_t* className, const wchar_t* title);
	~Window();

	bool Init(WNDPROC wndProc, int nCmdShow);
	void Run();
	HMENU CreateAppMenu();
	LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
	HWND GetHWND() { return m_hWnd; }
	void SetHWND(HWND hWnd) { m_hWnd = hWnd; }
};

