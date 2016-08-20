#include "pch.h"
#include "SceneDraw.h"
#include "GameData.h"

SceneDraw::SceneDraw()
    : System(1500)
{
}

String SceneDraw::GetName() const
{
    return "SceneDraw"_s;
}

void SceneDraw::Process(GameData &data)
{
    auto display = data.services.graphics->GetDisplay();

    auto clear = Math::Color(0.5f, 0, 1);
    display->Clear(&clear.r);

    for (auto &name : data.services.scene_order)
    {
        auto &scene = data.services.scenes[name];
        display->DrawScene(scene.p, data.services.camera.p);
    }

    display->Present();
}

