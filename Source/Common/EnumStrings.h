#pragma once

#include "Hash.h"
#include "CxHash.h"
#include "String.h"

#define BEGIN_STRING_TO_ENUM(str) \
    using namespace cx; \
    switch (StdHash<Fnv1A_64>{}(str)) \
    {

#define RETURN_STRING_ENUM(str, value, result) \
        case value##_fnv1a_64: \
            if (value##_s == str) \
                return result; \
            break;

#define END_STRING_TO_ENUM() \
        default: break; \
    } \
    return std::nullopt;

