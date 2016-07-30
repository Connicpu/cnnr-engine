#include "Transform.h"
#include "GameData.h"

void Transform::Update(GameData &data)
{
    using namespace Math;

    transform =
        Matrix3x2::Rotation(rotation) *
        Matrix3x2::Scale(SizeF(size * scale)) *
        Matrix3x2::Translation(position);

    if (parent)
    {
        if (auto ptransform = data.components.transform.get(*parent))
        {
            transform = ptransform->transform * transform;
        }
    }
}
