#pragma once

#ifdef _WIN32
#define NO_MINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <atlbase.h>
#include <Shellapi.h>

template <typename T>
using ComPtr = ATL::CComPtr<T>;

#define unreachable() (__assume(0))

#else

#define unreachable() (__builtin_unreachable())

#endif
