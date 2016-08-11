#pragma once

#include <connorlib/serialization/json.h>
#include <connorlib/serialization/conversion.h>

class VsCode
{
public:
    static void Send(const JSON::ValueRef &data);
    static JSON::Value Recv();
};
