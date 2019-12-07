#pragma once
// Windows Includes
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <memory>
#include <assert.h>
#include <wrl.h>
// DirectX 11 Includes
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
// STL
#include <iostream>
#include <vector>
#include <map>
#include <array>
#include <algorithm>
#include <functional>
// Custom Includes
#include "RenderTypes.h"