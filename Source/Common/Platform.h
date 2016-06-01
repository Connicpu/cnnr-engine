#pragma once

#ifdef _WIN32
#define NO_MINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <atlbase.h>

template <typename T>
using ComPtr = ATL::CComPtr<T>;

#else

#endif
