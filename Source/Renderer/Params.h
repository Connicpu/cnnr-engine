#pragma once

#include <Common/MathLib.h>

class ITexture;

struct SpriteSetParams
{
    // Streaming affects whether the texture buffer will
    // be created as accepting dynamic data or immutable.
    // If streaming is false, buffers must not be null
    bool streaming;
    // The number of sprites stored in the SpriteSet
    uint32_t sprite_count;
    // The width of each sprite in the set
    uint32_t sprite_width;
    // The height of each sprite in the set
    uint32_t sprite_height;
    // A pointer to an array of [sprite_count] RGBA buffer arrays.
    // This may be null if streaming is true, and the textures will
    // not be immediately initialized.
    const uint8_t *const *buffers;
};

struct SpriteObjectParams
{
    // Translucent sprites are sorted by layer, specify this flag
    // if and only if it's needed, and always if it will ever be needed.
    bool is_translucent = false;

    // If this is true, the renderer may optimize the fact that this sprite
    // is expected to never have its properties changed until it is destroyed.
    // If you specify this flag, using any of the Set* functions is undefined
    // what the renderer may do. Destroy and recreate this sprite if something
    // has to be changed. Destroying and Creating static sprites often may
    // degrade performance.
    bool is_static = false;

    // Layering order of the sprites, affects actual draw order for
    // translucent sprites.
    float layer = 0.0f;

    // The texture this sprite will render with
    ITexture *texture = nullptr;

    // A [0,1] subrect of the texture to use for rendering
    Math::RectF window = Math::Rect(0, 0, 1, 1);

    // The transformation applied to this sprite. You must account for
    // the aspect ratio of the texture yourself. An identity matrix will
    // render a 1x1 sprite in the center of the viewport.
    Math::Matrix3x2F transform = Math::Matrix3x2::Identity();

    // The tint applied to this sprite
    Math::ColorF tint = Math::Color(1.0f, 1.0f, 1.0f);
};
