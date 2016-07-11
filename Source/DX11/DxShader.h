#pragma once

#include <Common/String.h>
#include <gsl.h>
#include "DxPlatform.h"

struct DxShaderDesc
{
    DxShaderDesc() = default;
    DxShaderDesc(const String &v, const String &p, gsl::span<const D3D11_INPUT_ELEMENT_DESC> i)
        : vertex_shader(v), pixel_shader(p), input_layout(i)
    {
    }

    String vertex_shader;
    String pixel_shader;
    gsl::span<const D3D11_INPUT_ELEMENT_DESC> input_layout;
};

struct DxShader
{
    void Load(ID3D11Device *device, const DxShaderDesc &desc);

    ComPtr<ID3D11VertexShader> vertex_shader;
    ComPtr<ID3D11PixelShader> pixel_shader;
    ComPtr<ID3D11InputLayout> input_layout;
};
