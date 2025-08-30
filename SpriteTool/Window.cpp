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

	DirectCore::GetInstance()->CreateRenderTarget(m_hWnd, &m_rt);
	DirectCore::GetInstance()->CreateBitmapRenderTarget(m_hWnd, m_rt, &m_brt);
	DirectCore::GetInstance()->CreateTextFromat(&m_pTextFormat, L"나눔고딕", DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 15.0f);
	DirectCore::GetInstance()->CreateSolidColorBrush(m_rt, D2D1::ColorF(D2D1::ColorF::Black), &m_blackColor);
	DirectCore::GetInstance()->CreateSolidColorBrush(m_rt, D2D1::ColorF(D2D1::ColorF::White), &m_whiteColor);
	SetMenu(m_hWnd, CreateAppMenu());
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

HMENU Window::CreateAppMenu()
{
	HMENU hMenuBar = CreateMenu();
	HMENU hFile = CreatePopupMenu();
	HMENU hTool = CreatePopupMenu();

	AppendMenuW(hFile, MF_STRING, ID_FILE_OPEN, L"&Open");
	AppendMenuW(hFile, MF_STRING, ID_FILE_SAVE, L"&Save");
	AppendMenuW(hTool, MF_STRING, ID_REMOVE_COLOR, L"&Remove Color");

	AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hFile, L"&File");
	AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hTool, L"&Tool");
	return hMenuBar;
}

LRESULT Window::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_FILE_OPEN:
		{
			wchar_t path[MAX_PATH]{};
			if (FileUtils::OpenFile(m_hWnd, path))
			{
				if (m_targetBitmap)
					delete m_targetBitmap;


				m_targetBitmap = DirectCore::GetInstance()->Direct2DLoadBitmap(path, m_rt);
				InvalidateRect(m_hWnd, NULL, TRUE);
			}
			return 0;
		}

		case ID_FILE_SAVE:
		{
			if (m_targetBitmap == nullptr)
				return 0;

			wchar_t path[MAX_PATH] = L"";
			FileUtils::SaveFile(m_hWnd, path);
			DirectCore::GetInstance()->Direct2DSaveBitmapToPng(m_hWnd, path, m_rt, m_targetBitmap);

			return 0;
		}

		case ID_REMOVE_COLOR:
		{
			if (m_targetBitmap == nullptr) break;
			UINT width = m_targetBitmap->GetWidth();
			UINT height = m_targetBitmap->GetHeight();

			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					int index = (x + (y * m_targetBitmap->GetWidth()));
					DWORD* pixlePtr = &(reinterpret_cast<DWORD*>(m_targetBitmap->RefBuffer()))[index];
					DWORD pixel = *pixlePtr;
					if (pixel == m_keyColor)
						::memset(pixlePtr, 0, sizeof(DWORD));
				}
			}
			D2D1_RECT_U rect = { 0,0,width * sizeof(DWORD), height * sizeof(DWORD) };
			m_targetBitmap->GetD2D1Bitmap()->CopyFromMemory(&rect, (void*)m_targetBitmap->RefBuffer(), width * sizeof(DWORD));
			InvalidateRect(m_hWnd, NULL, TRUE);
			break;
		}
		return 0;
		}
	}

	case WM_LBUTTONDOWN:
	{
		if (m_targetBitmap == nullptr)
			return 0;

		::memset(m_pixelText, 0, sizeof(m_pixelText));
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		if (m_targetBitmap->GetWidth() < x || m_targetBitmap->GetHeight() < y || x < 0 || y < 0)
			return 0;

		UINT index = (x + (y * m_targetBitmap->GetWidth()));
		m_keyColor = (reinterpret_cast<DWORD*>(m_targetBitmap->RefBuffer()))[index];
		swprintf_s(m_pixelText, sizeof(m_pixelText) / sizeof(wchar_t), L"KeyColor: %0.8X", m_keyColor);
		InvalidateRect(m_hWnd, NULL, TRUE);
		return 0;
	}

	case WM_CREATE:
	{
		return 0;
	}

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(m_hWnd, &ps);
	
		RECT rc;
		GetClientRect(m_hWnd, &rc);

		m_brt->BeginDraw();
		m_brt->Clear(D2D1::ColorF(D2D1::ColorF::DarkGray));
		
		if (m_targetBitmap)
		{
			m_brt->DrawBitmap(m_targetBitmap->GetD2D1Bitmap(), D2D1::RectF(
				0,
				0,
				m_targetBitmap->GetWidth(),
				m_targetBitmap->GetHeight()));
			D2D1_RECT_F textRect = D2D1::RectF(20.0f, rc.bottom - 30.0f, 300.0f, (FLOAT)rc.bottom);
			m_brt->FillRectangle(textRect, m_whiteColor);  // m_bgBrush: 반투명 등 원하는 색 브러시
			m_brt->DrawText(m_pixelText, wcslen(m_pixelText), m_pTextFormat, D2D1::RectF(20, rc.bottom - 30, 300, rc.bottom),m_blackColor);
		}

		m_brt->EndDraw();
		m_rt->BeginDraw();
		ID2D1Bitmap* pBitmap = nullptr;
		m_brt->GetBitmap(&pBitmap);
		m_rt->DrawBitmap(pBitmap);
		pBitmap->Release();
		m_rt->EndDraw();
		EndPaint(m_hWnd, &ps);
		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(m_hWnd, msg, wParam, lParam);
}

