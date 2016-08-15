#include "pch.h"
#include "TransformUpdate.h"
#include "EntityFilter.h"
#include "ComponentManager.h"
#include "GameData.h"
#include "ParallelProcess.h"

TransformUpdate::TransformUpdate()
    : System(1000)
{
    
}

void TransformUpdate::Process(GameData &data)
{
    ParallelProcess(data, [this](GameData &data, Entity e)
    {
        if (auto transform = data.components.transform.get(e))
        {
            transform->ParallelCalculate();
        }
    });

    ParallelProcess(data, [this](GameData &data, Entity e)
    {
        if (auto transform = data.components.transform.get(e))
        {
            transform->ParallelUpdate(data);
        }
    });
}

String TransformUpdate::GetName() const
{
    return "TransformUpdate"_s;
}

