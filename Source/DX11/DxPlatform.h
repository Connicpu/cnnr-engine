#pragma once

#include <Common/Platform.h>
#include <Common/String.h>
#include <Common/MathLib.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <codecvt>

using namespace Math;

inline std::wstring widen(const String &str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    auto span = str.span();
    return converter.from_bytes(&*span.begin(), &*span.end());
}
