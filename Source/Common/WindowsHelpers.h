#pragma once

#include "Platform.h"
#include <cassert>

template <typename FnPtr>
inline HINSTANCE GetHinstanceFromFn(FnPtr *pFn)
{
    HMODULE handle;
    BOOL result = GetModuleHandleExW(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
        GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        (LPCWSTR)pFn,
        &handle
    ); result;
    assert(result);
    return (HINSTANCE)handle;
}

