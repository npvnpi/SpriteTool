#include "pch.h"
#include "Window.h"

Window::Window(HINSTANCE hInstance, int width, int height, const wchar_t* className, const wchar_t* title)
	:m_hInstance(hInstance), m_width(width), m_height(height)
{
	::memcpy(m_className, className, sizeof(wchar_t) * wcslen(className));
	::memcpy(m_title, title, sizeof(wchar_t) * wcslen(title));
}

Window::~Window()
{

}

bool Window::Init(WNDPROC wndProc, int nCmdShow)
{
	WNDCLASS wc = {};
	wc.lpfnWndProc = wndProc;       // 메시지 처리 함수 등록
	wc.hInstance = m_hInstance;
	wc.lpszClassName = m_className;

	RegisterClass(&wc);

	m_hWnd = CreateWindowEx(
		0,                   
		m_className,         
		m_title,      
		WS_OVERLAPPEDWINDOW,    
		CW_USEDEFAULT, CW_USEDEFAULT, m_width, m_height,
		nullptr, nullptr, m_hInstance, this);

	if (m_hWnd == nullptr)
		return false;

	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);
	
	return false;
}

void Window::Run()
{
	MSG msg = {};
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT Window::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) 
	{
	case WM_CREATE:
	{
		ShowWindow(m_hWnd, 10);
		UpdateWindow(m_hWnd);
		return 0;
	}
	

	case WM_PAINT: 
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(m_hWnd, &ps);
		TextOut(hdc, 50, 50, L"Hello, World!", 13);
		EndPaint(m_hWnd, &ps);
		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(m_hWnd, msg, wParam, lParam);
}

