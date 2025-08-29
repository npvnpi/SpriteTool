#pragma once
class FileUtils
{
public:
	static bool OpenFile(HWND hWnd, wchar_t* filePath);
	static void SaveFile(HWND hWnd, wchar_t* filePath);
};

