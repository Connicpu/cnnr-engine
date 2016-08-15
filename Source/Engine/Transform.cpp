#include "pch.h"
#include "Transform.h"
#include "GameData.h"

void Transform::Update(GameData &data)
{
    ParallelCalculate();
    ParallelUpdate(data);
}

void Transform::ParallelCalculate()
{
    using namespace Math;

    if (!changed)
        return;

    parallel_calc =
        Matrix3x2::Rotation(rotation) *
        Matrix3x2::Scale(size * scale) *
        Matrix3x2::Translation(position);

    changed = false;
}

void Transform::ParallelUpdate(GameData &data)
{
    auto result = parallel_calc;
    auto par = parent;

    int i = 0;
    while (par)
    {
        if (++i > 128)
            throw std::runtime_error{ "Recursion depth of transform parents exceeded" };

        if (auto ptransform = data.components.transform.get(*par))
        {
            result = ptransform->parallel_calc * result;
            par = ptransform->parent;

            continue;
        }
        break;
    }

    transform = result;
}

String Transform::GetName() const
{
    return "transform"_s;
}

void Transform::PushLuaBinding(lua_State *L)
{
    *(Transform **)lua_newuserdata(L, sizeof(Transform *)) = this;
    PushTransformMetatable(L);
    lua_setmetatable(L, -2);
}

std::optional<Transform &> Transform::FromLua(lua_State *L, int idx)
{
    lua_getmetatable(L, idx);
    PushTransformMetatable(L);
    if (!lua_rawequal(L, -2, -1))
    {
        lua_pop(L, 2);
        return std::nullopt;
    }

    lua_pop(L, 2);
    return **(Transform **)lua_touserdata(L, idx);
}

void Transform::PushTransformMetatable(lua_State *L)
{
    Component::SetupMetatable<Transform*>(L, [L]()
    {
        lua_pushcclosure(L, [](lua_State *L) -> int
        {
            auto self = *Transform::FromLua(L, 1);
            ("Transform{ pos: "_s + self.position +
                ", rot: "_s + self.rotation +
                ", scale: "_s + self.scale +
                ", size: "_s + self.size +
                " }"_s
                ).push_lua(L);
            return 1;
        }, 0);
        lua_setfield(L, -2, "__tostring");
    });
}
