#pragma once

#include "DxPlatform.h"
#include "DxException.h"

template <typename Vert>
class DxVertexBuffer
{
public:
    struct UploadContext;

    void CreateImmutable(ID3D11Device *device, uint32_t count, const Vert *vertices);
    void Reserve(ID3D11Device *device, uint32_t count);

    UploadContext BeginUpload(ID3D11DeviceContext *context);
    void Push(const Vert &vert, UploadContext &upload_ctx);
    void Push(const Vert *verts, uint32_t count, UploadContext &upload_ctx);
    void EndUpload(const UploadContext &upload_ctx);

    ID3D11Buffer *Get() const;
    uint32_t Count() const;

private:
    ComPtr<ID3D11Buffer> buffer_;
    uint32_t size_ = 0;
    uint32_t requested_size_ = 0;
};

template <typename Vert>
struct DxVertexBuffer<Vert>::UploadContext
{
private:
    friend class DxVertexBuffer<Vert>;

    ID3D11DeviceContext *context;
    D3D11_MAPPED_SUBRESOURCE subres;
    uint32_t pos;
};

template<typename Vert>
inline void DxVertexBuffer<Vert>::CreateImmutable(ID3D11Device *device, uint32_t count, const Vert *vertices)
{
    buffer_.Release();

    D3D11_BUFFER_DESC desc;
    desc.ByteWidth = count * sizeof(Vert);
    desc.StructureByteStride = sizeof(Vert);
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA data = { 0 };
    data.pSysMem = vertices;

    CheckHR(device->CreateBuffer(&desc, &data, &buffer_));
    size_ = count;
    requested_size_ = 0;
}

template<typename Vert>
inline void DxVertexBuffer<Vert>::Reserve(ID3D11Device *device, uint32_t count)
{
    const uint32_t RIDICULOUS_EXCESS = 5; // If we're using this much more memory than we need
    const float CREATION_HEADROOM = 1.5f; // How much excess space we add when we have to resize

    requested_size_ = count;

    // Check if we should recreate this buffer
    // (too small or using a ridiculous amount of memory)
    if (count > size_ ||
        count > size_ / RIDICULOUS_EXCESS)
    {
        buffer_.Release();
        uint32_t creation_count = uint32_t(float(count) * CREATION_HEADROOM);
        D3D11_BUFFER_DESC desc;
        desc.ByteWidth = creation_count * sizeof(Vert);
        desc.StructureByteStride = sizeof(Vert);
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0;

        CheckHR(device->CreateBuffer(&desc, nullptr, &buffer_));
        size_ = creation_count;
    }
}

template<typename Vert>
inline typename DxVertexBuffer<Vert>::UploadContext DxVertexBuffer<Vert>::BeginUpload(ID3D11DeviceContext *context)
{
    UploadContext upload_ctx;
    CheckHR(context->Map(buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &upload_ctx.subres));
    upload_ctx.pos = 0;
    upload_ctx.context = context;
    return upload_ctx;
}

template<typename Vert>
inline void DxVertexBuffer<Vert>::Push(const Vert & vert, UploadContext &upload_ctx)
{
    this->Push(&vert, 1, upload_ctx);
}

template<typename Vert>
inline void DxVertexBuffer<Vert>::Push(const Vert *verts, uint32_t count, UploadContext &upload_ctx)
{
    assert(upload_ctx.pos + count <= requested_size_);
    memcpy(upload_ctx.subres.pData, verts, count * sizeof(Vert));
    upload_ctx.pos += count;
}

template<typename Vert>
inline void DxVertexBuffer<Vert>::EndUpload(const UploadContext &upload_ctx)
{
    upload_ctx.context->Unmap(buffer_, 0);
    assert(upload_ctx.pos == requested_size_);
}

template<typename Vert>
inline ID3D11Buffer * DxVertexBuffer<Vert>::Get() const
{
    return buffer_;
}

template<typename Vert>
inline uint32_t DxVertexBuffer<Vert>::Count() const
{
    return requested_size_;
}
