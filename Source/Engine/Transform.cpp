#include "pch.h"
#include "Transform.h"
#include "GameData.h"
#include <LuaInterface/LuaMath.h>

void Transform::Update(GameData &data)
{
    ParallelCalculate();
    ParallelUpdate(data);
}

void Transform::ParallelCalculate()
{
    using namespace Math;

    parallel_calc =
        Matrix3x2::Rotation(rotation) *
        Matrix3x2::Scale(size * scale) *
        Matrix3x2::Translation(position);
}

void Transform::ParallelUpdate(const GameData &data)
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
            auto &self = *Transform::FromLua(L, 1);
            ("Transform{ pos: "_s + self.position +
                ", rot: "_s + self.rotation +
                ", scale: "_s + self.scale +
                ", size: "_s + self.size +
                " }"_s
                ).push_lua(L);
            return 1;
        }, 0);
        lua_setfield(L, -2, "__tostring");

        #pragma region Fields
        lua_pushcclosure(L, [](lua_State *L) -> int
        {
            auto oself = Transform::FromLua(L, 1);
            if (!oself)
                return luaL_typerror(L, 1, "Transform");
            auto &self = *oself;

            if (lua_type(L, 2) != LUA_TSTRING)
                return luaL_typerror(L, 2, "string");
            auto name = String::from_lua(L, 2);

            if (name == "position"_s)
                lua_pushpoint2f_ref(L, &self.position);
            else if (name == "rotation"_s)
                lua_pushnumber(L, self.rotation.rad);
            else if (name == "scale"_s)
                lua_pushnumber(L, self.scale);
            else if (name == "size"_s)
                lua_pushsize2f_ref(L, &self.size);
            else
            {
                lua_getmetatable(L, 1);
                lua_pushvalue(L, 2);
                lua_gettable(L, -2);
            }
            return 1;
        }, 0);
        lua_setfield(L, -2, "__index");

        lua_pushcclosure(L, [](lua_State *L) -> int
        {
            auto oself = Transform::FromLua(L, 1);
            if (!oself)
                return luaL_typerror(L, 1, "Transform");
            auto &self = *oself;

            if (lua_type(L, 2) != LUA_TSTRING)
                return luaL_typerror(L, 2, "string");
            auto name = String::from_lua(L, 2);

            if (name == "position"_s)
            {
                if (auto p = lua_topoint2f(L, 3))
                    self.position = *p;
                else
                    return luaL_typerror(L, 3, "math.Point2");
            }
            else if (name == "rotation"_s)
            {
                if (lua_type(L, 3) != LUA_TNUMBER)
                    return luaL_typerror(L, 3, "number");

                self.rotation = Math::Rads((float)lua_tonumber(L, 3));
            }
            else if (name == "scale"_s)
            {
                if (lua_type(L, 3) != LUA_TNUMBER)
                    return luaL_typerror(L, 3, "number");

                self.scale = (float)lua_tonumber(L, 3);
            }
            else if (name == "size"_s)
            {
                if (auto s = lua_tosize2f(L, 3))
                    self.size = *s;
                else
                    return luaL_typerror(L, 3, "math.Size2");
            }
            else
            {
                return luaL_error(L, "No field named %s exists on Transform", lua_tostring(L, 2));
            }

            return 0;
        }, 0);
        lua_setfield(L, -2, "__newindex");
        #pragma endregion
    });
}
