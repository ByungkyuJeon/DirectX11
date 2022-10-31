#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX

// defines
#include "TEngineDefines.h"
#include "TEngineTypes.h"
#include "TEngineDefaults.h"

// Logger
#include "TErrorLogger.h"
#include "TStat.h"

// data
#include "TDataIOWrapper.h"

// COM
#include <wrl/client.h>

#include "TFrameTimer.h"
#include "StringHelper.h"
#include <Windows.h>

// directx
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <d3d11.h>

extern Microsoft::WRL::ComPtr<ID3D11Device> gDevice;
extern Microsoft::WRL::ComPtr<ID3D11DeviceContext> gDeviceContext;
extern HWND gWindowHandle;
extern HINSTANCE gWindowInstance;
