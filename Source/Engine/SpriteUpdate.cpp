#include "pch.h"
#include "SpriteUpdate.h"
#include "ParallelProcess.h"

SpriteUpdate::SpriteUpdate()
    : System(1100)
{
}

void SpriteUpdate::Process(GameData &data)
{
    HotProcess(data, [](GameData &data, Entity e)
    {
        if (auto sprite = data.components.sprite.get(e))
        {
            if (auto transform = data.components.transform.get(e))
            {
                sprite->Update(data, transform->transform);
            }
        }
    });
}

String SpriteUpdate::GetName() const
{
    return "SpriteUpdate"_s;
}
