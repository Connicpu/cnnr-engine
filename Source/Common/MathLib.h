#pragma once

#ifdef MSVC

#include <DirectXMath.h>
#include <DirectXColors.h>

#else

#include <Vendor/Math/DirectXMath.h>
#include <Vendor/Math/DirectXColors.h>

#endif

namespace Math = DirectX;
