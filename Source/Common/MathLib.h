#pragma once

#ifdef MSVC

#include <DirectXMath.h>
#include <DirectXColors.h>

#else

#include <Vendor/Math/DirectXMath.h>

#endif

namespace Math = DirectX;
