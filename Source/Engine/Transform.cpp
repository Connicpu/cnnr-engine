#include "pch.h"
#include "Transform.h"
#include "GameData.h"

void Transform::Update(GameData &data)
{
    using namespace Math;

    if (!changed)
        return;
    if (updating)
        throw std::runtime_error{ "Entity transform dependency loop" };

    updating = true;

    transform =
        Matrix3x2::Rotation(rotation) *
        Matrix3x2::Scale(SizeF(size * scale)) *
        Matrix3x2::Translation(position);

    if (parent)
    {
        if (auto ptransform = data.components.transform.get(*parent))
        {
            if (ptransform->changed)
            {
                // Ensure the parent gets updated first
                ptransform->Update(data);
            }

            transform = ptransform->transform * transform;
        }
    }

    updating = false;
    changed = false;
}
