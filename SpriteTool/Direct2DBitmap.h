#pragma once
class Direct2DBitmap
{
private:
	int					_width;
	int					_height;
	unsigned char*		_buffer;
	ID2D1Bitmap*		_d2d1Bitmap;
public:
	Direct2DBitmap();
	~Direct2DBitmap()
	{
		if (_d2d1Bitmap)
			_d2d1Bitmap->Release();

		if (_buffer)
			delete[] _buffer;
	}

	Direct2DBitmap(UINT width, UINT height, ID2D1Bitmap* d2d1Bitmap, BYTE* buffer) : _width(width), _height(height), _d2d1Bitmap(d2d1Bitmap), _buffer(buffer)
	{

	}

	unsigned char* RefBuffer() { return _buffer; }
	ID2D1Bitmap* GetD2D1Bitmap() { return _d2d1Bitmap; }
	int			 GetWidth() { return _width; }
	int			 GetHeight() { return _height; }
};

