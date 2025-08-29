#include "pch.h"
#include "DirectCore.h"
#include "Direct2DBitmap.h"

bool DirectCore::Init()
{
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
    
    if (hr != S_OK) 
    {
        return false;
    }

    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
    
    if (hr != S_OK) 
    {
        return false;
    }

    hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    if (hr != S_OK)
    {
        return false;
    }

    hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pWICFactory));

    if (hr != S_OK) 
    {
        return false;
    }

    return false;
}

void DirectCore::Clear()
{
    if (m_pD2DFactory) 
        m_pD2DFactory->Release();
    CoUninitialize();
    if (m_pWICFactory)
        m_pWICFactory->Release();
}

bool DirectCore::CreateTextFromat(IDWriteTextFormat** pTextFormat, const wchar_t* fontName, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch, float fontSize)
{
    HRESULT hr = m_pDWriteFactory->CreateTextFormat(fontName, NULL, fontWeight, fontStyle, fontStretch, fontSize, L"en-us", pTextFormat);
    return hr == S_OK;
}

bool DirectCore::CreateSolidColorBrush(ID2D1HwndRenderTarget* rt, const D2D1_COLOR_F color, ID2D1SolidColorBrush** brush)
{
    HRESULT hr = rt->CreateSolidColorBrush(color, brush);
    return hr == S_OK;
}

bool DirectCore::CreateRenderTarget(HWND hWnd, ID2D1HwndRenderTarget** rt)
{
    RECT rc;
    GetClientRect(hWnd, &rc);

    HRESULT hr = m_pD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(
            hWnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
        rt);

    return hr == S_OK;
}

bool DirectCore::CreateBitmapRenderTarget(HWND hWnd, ID2D1HwndRenderTarget* rt, ID2D1BitmapRenderTarget** brt)
{
    HRESULT hr = rt->CreateCompatibleRenderTarget(brt);
    return hr == S_OK;
}

Direct2DBitmap* DirectCore::Direct2DLoadBitmap(const wchar_t* filePath, ID2D1RenderTarget* rt)
{
	IWICBitmapDecoder* pDecoder = nullptr;
	IWICBitmapFrameDecode* pSource = nullptr;
	IWICStream* pStream = nullptr;
	IWICFormatConverter* pConverter = nullptr;
	IWICBitmapScaler* pScaler = nullptr;

	HRESULT hr = m_pWICFactory->CreateDecoderFromFilename(
		filePath,
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);
	
	if (hr != S_OK)
		return nullptr;

	hr = pDecoder->GetFrame(0, &pSource);
	
	if (hr != S_OK)
		return nullptr;

	IWICBitmapSource* bitmapSource = pSource;
	hr = WICConvertBitmapSource(GUID_WICPixelFormat32bppPBGRA, pSource, &bitmapSource);

	if (hr != S_OK)
		return nullptr;

	UINT width = 0;
	UINT height = 0;
	hr = bitmapSource->GetSize(&width, &height);
	
	if (hr != S_OK)
		return nullptr;


	const int buffSize = width * height * 4;
	BYTE* buffer = new BYTE[width * height * 4];
	hr = bitmapSource->CopyPixels(NULL, width * 4, buffSize, buffer);

	if (hr != S_OK)
		return nullptr;

	bitmapSource->Release();

	hr = m_pWICFactory->CreateFormatConverter(&pConverter);
	if (hr != S_OK)
		return nullptr;

	hr = pConverter->Initialize(
		pSource
		, GUID_WICPixelFormat32bppPBGRA
		, WICBitmapDitherTypeNone
		, nullptr
		, 0.0f
		, WICBitmapPaletteTypeCustom);
	if (hr != S_OK)
		return nullptr;

	ID2D1Bitmap* pBitmap = nullptr;

	hr = rt->CreateBitmapFromWicBitmap(pConverter, nullptr, &pBitmap);
	if (hr != S_OK)
		return nullptr;

	Direct2DBitmap* bitmap = new Direct2DBitmap(width, height, pBitmap, buffer);

	if (pDecoder) pDecoder->Release();
	if (pSource) pSource->Release();
	if (pConverter) pConverter->Release();

	return bitmap;
}

bool DirectCore::Direct2DSaveBitmapToPng(HWND hWnd, wchar_t* path, ID2D1RenderTarget* rt, Direct2DBitmap* targetBitmap)
{
	const D2D1_SIZE_U ps = targetBitmap->GetD2D1Bitmap()->GetPixelSize();
	
	IWICBitmap* wicBitmap = nullptr;
	HRESULT hr = m_pWICFactory->CreateBitmap(
		ps.width, ps.height,
		GUID_WICPixelFormat32bppPBGRA, // D2D ·»´õÅ¸°Ù°ú µ¿ÀÏÇÑ premultiplied BGRA
		WICBitmapCacheOnLoad,
		&wicBitmap
	);
	if (FAILED(hr)) 
		return false;

	return false;
}
