#include "pch.h"
#include "FileUtils.h"

bool FileUtils::OpenFile(HWND hWnd, wchar_t* filePath)
{
    OPENFILENAME  ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = filePath;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


    if (GetOpenFileName(&ofn) == TRUE)
        return true;
    else
    {
        DWORD error = CommDlgExtendedError();
        if (error != 0)
        {
            wchar_t errorMessage[256];
            swprintf_s(errorMessage, 256, L"GetOpenFileName failed with error code: %lu", error);
            MessageBox(hWnd, errorMessage, L"Error", MB_OK | MB_ICONERROR);
        }
        else
        {
            return false;
        }
    }

    return false;
}

void FileUtils::SaveFile(HWND hWnd, wchar_t* filePath)
{
    OPENFILENAME ofn = { 0 };
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"PNG Files\0*.png\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // 저장 대화상자
    if (GetSaveFileName(&ofn)) {
        MessageBoxW(hWnd, filePath, L"저장 경로", MB_OK);
    }
}
