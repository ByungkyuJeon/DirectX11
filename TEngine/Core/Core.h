#pragma once

#define NOMINMAX

#include "Framework/Logger/ErrorLogger/TErrorLogger.h"
#include "Framework/Logger/StatLogger/StatLogger.h"
#include "TFrameTimer.h"
#include "Helper/StringHelper.h"
#include <Windows.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include <vector>
#include <memory>
#include <string>
#include <DirectXMath.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/material.h>


// TYPE DEFINES
#define TComPtr Microsoft::WRL::ComPtr
#define TWString std::wstring
#define TString std::string

typedef UINT Length;
// TYPE DEFINES

// DEFAULT VALUES
#define DEFAULT_WINDOW_WIDTH (UINT)1920
#define DEFAULT_WINDOW_HEIGHT (UINT)1080
// DEFAULT VALUES

// INVALID VALUES
#define INVALID_HANDLE 0
#define INVALID_LENGTH -1
#define INVALID_INDEX -1
// INVALID VALUES


// Globals
static TComPtr<ID3D11Device> GDevice;
static TComPtr<ID3D11DeviceContext> GDeviceContext;
static HWND GWindowHandle;
static HINSTANCE GWindowInstance;
// Globals

