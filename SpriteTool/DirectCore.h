#pragma once
class DirectCore
{
private:
	ID2D1Factory* m_pD2DFactory;
	IWICImagingFactory* m_pWICFactory;
	IDWriteFactory* m_pDWriteFactory;

public:
	static DirectCore* GetInstance() 
	{
		static DirectCore s_instance;
		return &s_instance;
	}

public:
	bool Init();
	void Clear();

	bool  CreateTextFromat(IDWriteTextFormat** pTextFormat, const wchar_t* fontName, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch, float fontSize);
	bool  CreateSolidColorBrush(ID2D1HwndRenderTarget* rt, const D2D1_COLOR_F color, ID2D1SolidColorBrush** brush);
	bool  CreateRenderTarget(HWND hWnd, ID2D1HwndRenderTarget** rt);
	bool  CreateBitmapRenderTarget(HWND hWnd, ID2D1HwndRenderTarget* rt, ID2D1BitmapRenderTarget** brt);
	class Direct2DBitmap* Direct2DLoadBitmap(const wchar_t* filePath, ID2D1RenderTarget* rt);
	bool Direct2DSaveBitmapToPng(HWND hWnd, wchar_t* path, ID2D1RenderTarget* rt, Direct2DBitmap* targetBitmap);
};

