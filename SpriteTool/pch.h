#pragma once
#include <windows.h>
#include <shobjidl.h>    // IFileOpenDialog
#include <commctrl.h>
#include <d2d1.h>        // Direct2D
#include <dwrite.h>      // (텍스트 필요하면)
#include <wincodec.h>    // WIC (이미지 디코더/인코더)
#include <wrl/client.h>
#include <vector>
#include <shlwapi.h>
#include "Type.h"
#include "Direct2DBitmap.h"
#include "DirectCore.h"
#include "FileUtils.h"