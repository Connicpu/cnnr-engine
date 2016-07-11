#include "DxShader.h"
#include "DxException.h"
#include <Common/MMap.h>

void DxShader::Load(ID3D11Device *device, const DxShaderDesc &desc)
{
    MMap mmap;

    // Just in case we're reusing an instance
    vertex_shader.Release();
    pixel_shader.Release();
    input_layout.Release();

    // Open the vertex shader file
    if (!mmap.Open(desc.vertex_shader.path()))
        CheckHR(HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND));

    // Load the vertex shader
    CheckHR(device->CreateVertexShader(
        mmap.GetMemory(), mmap.GetLength(),
        nullptr, &vertex_shader));

    // Create the input layout (requires the vertex shader bytecode)
    CheckHR(device->CreateInputLayout(
        desc.input_layout.data(), (UINT)desc.input_layout.size(),
        mmap.GetMemory(), mmap.GetLength(), &input_layout));

    // Open the pixel shader file
    if (!mmap.Open(desc.pixel_shader.path()))
        CheckHR(HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND));

    // Load the pixel shader
    CheckHR(device->CreatePixelShader(
        mmap.GetMemory(), mmap.GetLength(),
        nullptr, &pixel_shader));
}
