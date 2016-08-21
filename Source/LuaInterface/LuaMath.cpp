#include "pch.h"
#include "LuaMath.h"

using namespace Math;

template <typename T>
struct MathObj
{
    enum Tag { Obj, Ptr };
    union Data { T obj; T *ptr; };
    Tag tag;
    Data data;

    MathObj(T obj)
        : tag(Obj)
    {
        data.obj = obj;
    }

    MathObj(T *ptr)
        : tag(Ptr)
    {
        data.ptr = ptr;
    }

    T &get()
    {
        if (tag == Obj)
            return data.obj;
        return *data.ptr;
    }
};

template <typename T>
inline void PushMathObj(lua_State *L, T obj)
{
    *(MathObj<T> *)lua_newuserdata(L, sizeof(MathObj<T>)) = MathObj<T>(obj);
}

template <typename T>
inline void PushMathPtr(lua_State *L, T *ptr)
{
    *(MathObj<T> *)lua_newuserdata(L, sizeof(MathObj<T>)) = MathObj<T>(ptr);
}

template <typename T>
inline T &DerefObj(lua_State *L, int idx)
{
    return ((MathObj<T> *)lua_touserdata(L, idx))->get();
}

template <typename Func>
inline void BuildMeta(lua_State *L, const char *type, Func &&f)
{
    lua_createtable(L, 0, 0);
    int top = lua_gettop(L);

    f(top);
    assert(lua_gettop(L) == top);

    lua_setfield(L, -2, type);
}

static void GetMathMeta(lua_State *L, const char *type)
{
    lua_getfield(L, LUA_GLOBALSINDEX, "math");
    lua_getfield(L, -1, type);
    lua_remove(L, -2);

    assert(lua_type(L, -1) == LUA_TTABLE);
}

static bool EnsureMeta(lua_State *L, int idx, const char *type)
{
    if (lua_type(L, idx) != LUA_TUSERDATA)
        return false;
    if (!lua_getmetatable(L, idx))
        return false;
    GetMathMeta(L, type);
    bool result = !!lua_rawequal(L, -1, -2);
    lua_pop(L, 2);
    return result;
}

static void Def(lua_State *L, int meta, const char *name, int(*fn)(lua_State*))
{
    lua_pushcclosure(L, fn, 0);
    lua_setfield(L, meta, name);
}

template <typename T>
T &GetVal(lua_State *L, int idx, const char *type)
{
    if (EnsureMeta(L, idx, type))
        return DerefObj<T>(L, idx);
    luaL_typerror(L, idx, type);
    unreachable();
}

void lua_math_init(lua_State *L)
{
    lua_getfield(L, LUA_GLOBALSINDEX, "math");

    #pragma region Vec2
    BuildMeta(L, "Vec2", [L](int meta)
    {
        #pragma region Constructor
        lua_createtable(L, 0, 1);
        Def(L, lua_gettop(L), "__call", [](lua_State *L)
        {
            int argc = lua_gettop(L);
            if (argc == 1)
            {
                lua_pushvec2f(L, Vec2());
            }
            else if (argc == 3)
            {
                if (lua_type(L, 2) != LUA_TNUMBER)
                    return luaL_typerror(L, 2, "number");
                if (lua_type(L, 3) != LUA_TNUMBER)
                    return luaL_typerror(L, 3, "number");
                auto x = (float)lua_tonumber(L, 2);
                auto y = (float)lua_tonumber(L, 3);
                lua_pushvec2f(L, Vec2(x, y));
            }
            else
            {
                return luaL_error(L, "Unknown number of arguments passed to math.Vec2()");
            }
            return 1;
        });
        lua_setmetatable(L, meta);
        #pragma endregion

        #pragma region Get fields
        Def(L, meta, "__index", [](lua_State *L)
        {
            auto &self = *lua_tovec2f(L, 1);

            if (lua_type(L, 2) != LUA_TSTRING)
                return luaL_typerror(L, 2, "string");
            auto field = String::from_lua(L, 2);

            if (field == "x"_s)
                lua_pushnumber(L, self.x);
            else if (field == "y"_s)
                lua_pushnumber(L, self.y);
            else
            {
                lua_getmetatable(L, 1);
                lua_pushvalue(L, 2);
                lua_gettable(L, -2);
            }
            return 1;
        });
        #pragma endregion

        #pragma region Set fields
        Def(L, meta, "__newindex", [](lua_State *L)
        {
            auto &self = *lua_tovec2f(L, 1);

            if (lua_type(L, 2) != LUA_TSTRING)
                return luaL_typerror(L, 2, "string");
            auto field = String::from_lua(L, 2);

            if (lua_type(L, 3) != LUA_TNUMBER)
                return luaL_typerror(L, 3, "number");
            auto value = (float)lua_tonumber(L, 3);

            if (field == "x"_s)
                self.x = value;
            else if (field == "y"_s)
                self.y = value;
            else
                return luaL_error(L, "Vec2 has no field `%s`", field.c_str());
            return 0;
        });
        #pragma endregion

        #pragma region tostring
        Def(L, meta, "__tostring", [](lua_State *L)
        {
            auto &self = *lua_tovec2f(L, 1);
            tostring(self).push_lua(L);
            return 1;
        });
        #pragma endregion

        #pragma region operator+
        Def(L, meta, "__add", [](lua_State *L)
        {
            if (auto self = lua_tovec2f(L, 1)) // __add(Vec2, _)
            {
                if (auto rhs = lua_tovec2f(L, 2)) // __add(Vec2, Vec2)
                    lua_pushvec2f(L, *self + *rhs);
                else if (auto rhs = lua_topoint2f(L, 2)) // __add(Vec2, Point2)
                    lua_pushpoint2f(L, *self + *rhs);
                else
                    return luaL_typerror(L, 2, "Vec2 or Point2");

                return 1;
            }

            return luaL_typerror(L, 1, "Vec2");
        });
        #pragma endregion

        #pragma region operator-
        Def(L, meta, "__sub", [](lua_State *L)
        {
            if (auto self = lua_tovec2f(L, 1)) // __sub(Vec2, _)
            {
                if (auto rhs = lua_tovec2f(L, 2)) // __sub(Vec2, Vec2)
                    lua_pushvec2f(L, *self - *rhs);
                else
                    return luaL_typerror(L, 2, "Vec2");

                return 1;
            }

            return luaL_typerror(L, 1, "Vec2");
        });
        #pragma endregion

        #pragma region operator*
        Def(L, meta, "__mul", [](lua_State *L)
        {
            if (auto self = lua_tovec2f(L, 1)) // __mul(Vec2, _)
            {
                if (auto rhs = lua_tosize2f(L, 2)) // __mul(Vec2, Size2)
                    lua_pushvec2f(L, *self * *rhs);
                else if (lua_type(L, 2) == LUA_TNUMBER) // __mul(Vec2, number)
                    lua_pushvec2f(L, *self * (float)lua_tonumber(L, 2));
                else
                    return luaL_typerror(L, 2, "Size2 or number");

                return 1;
            }
            else if (lua_type(L, 1) == LUA_TNUMBER) // __mul(number, Vec2)
            {
                auto lhs = (float)lua_tonumber(L, 1);
                auto rhs = *lua_tovec2f(L, 2);
                lua_pushvec2f(L, lhs * rhs);
                return 1;
            }

            return luaL_typerror(L, 1, "Vec2");
        });
        #pragma endregion

        #pragma region operator/
        Def(L, meta, "__div", [](lua_State *L)
        {
            if (auto self = lua_tovec2f(L, 1)) // __div(Vec2, _)
            {
                if (lua_type(L, 2) == LUA_TNUMBER) // __div(Vec2, number)
                {
                    auto rhs = (float)lua_tonumber(L, 2);
                    lua_pushvec2f(L, *self / rhs);
                    return 1;
                }

                return luaL_typerror(L, 2, "Vec2");
            }
            else if (lua_type(L, 1) == LUA_TNUMBER) // __div(number, Vec2)
            {
                auto lhs = (float)lua_tonumber(L, 1);
                auto rhs = *lua_tovec2f(L, 2);
                lua_pushvec2f(L, lhs / rhs);
                return 1;
            }

            return luaL_typerror(L, 1, "Vec2");
        });
        #pragma endregion

        #pragma region operator- (unary)
        Def(L, meta, "__unm", [](lua_State *L)
        {
            auto self = *lua_tovec2f(L, 1);
            lua_pushvec2f(L, -self);
            return 1;
        });
        #pragma endregion

        #pragma region topoint
        Def(L, meta, "topoint", [](lua_State *L)
        {
            auto self = *lua_tovec2f(L, 1);
            lua_pushpoint2f(L, Point2(self));
            return 1;
        });
        #pragma endregion

        #pragma region tosize
        Def(L, meta, "tosize", [](lua_State *L)
        {
            auto self = *lua_tovec2f(L, 1);
            lua_pushsize2f(L, SizeF(self));
            return 1;
        });
        #pragma endregion
    });
    #pragma endregion

    #pragma region Point2
    BuildMeta(L, "Point2", [L](int meta)
    {
        #pragma region Constructor
        lua_createtable(L, 0, 1);
        Def(L, lua_gettop(L), "__call", [](lua_State *L)
        {
            int argc = lua_gettop(L);
            if (argc == 1)
            {
                lua_pushpoint2f(L, Point2());
            }
            else if (argc == 3)
            {
                if (lua_type(L, 2) != LUA_TNUMBER)
                    return luaL_typerror(L, 2, "number");
                if (lua_type(L, 3) != LUA_TNUMBER)
                    return luaL_typerror(L, 3, "number");
                auto x = (float)lua_tonumber(L, 2);
                auto y = (float)lua_tonumber(L, 3);
                lua_pushpoint2f(L, Point2(x, y));
            }
            else
            {
                return luaL_error(L, "Unknown number of arguments passed to math.Point2()");
            }
            return 1;
        });
        lua_setmetatable(L, meta);
        #pragma endregion

        #pragma region Get fields
        Def(L, meta, "__index", [](lua_State *L)
        {
            auto &self = *lua_topoint2f(L, 1);

            if (lua_type(L, 2) != LUA_TSTRING)
                return luaL_typerror(L, 2, "string");
            auto field = String::from_lua(L, 2);

            if (field == "x"_s)
                lua_pushnumber(L, self.x);
            else if (field == "y"_s)
                lua_pushnumber(L, self.y);
            else
            {
                lua_getmetatable(L, 1);
                lua_pushvalue(L, 2);
                lua_gettable(L, -2);
            }
            return 1;
        });
        #pragma endregion

        #pragma region Set fields
        Def(L, meta, "__newindex", [](lua_State *L)
        {
            auto &self = *lua_topoint2f(L, 1);

            if (lua_type(L, 2) != LUA_TSTRING)
                return luaL_typerror(L, 2, "string");
            auto field = String::from_lua(L, 2);

            if (lua_type(L, 3) != LUA_TNUMBER)
                return luaL_typerror(L, 3, "number");
            auto value = (float)lua_tonumber(L, 3);

            if (field == "x"_s)
                self.x = value;
            else if (field == "y"_s)
                self.y = value;
            else
                return luaL_error(L, "Point2 has no field `%s`", field.c_str());
            return 0;
        });
        #pragma endregion

        #pragma region tostring
        Def(L, meta, "__tostring", [](lua_State *L)
        {
            auto &self = *lua_topoint2f(L, 1);
            tostring(self).push_lua(L);
            return 1;
        });
        #pragma endregion

        #pragma region operator+
        Def(L, meta, "__add", [](lua_State *L)
        {
            if (auto self = lua_topoint2f(L, 1)) // __add(Point2, _)
            {
                if (auto rhs = lua_tovec2f(L, 2)) // __add(Point2, Vec2)
                    lua_pushpoint2f(L, *self + *rhs);
                else
                    return luaL_typerror(L, 2, "Vec2");

                return 1;
            }

            return luaL_typerror(L, 1, "Vec2");
        });
        #pragma endregion

        #pragma region operator-
        Def(L, meta, "__sub", [](lua_State *L)
        {
            if (auto self = lua_topoint2f(L, 1)) // __sub(Point2, _)
            {
                if (auto rhs = lua_tovec2f(L, 2)) // __add(Point2, Vec2)
                    lua_pushpoint2f(L, *self - *rhs);
                else if (auto rhs = lua_topoint2f(L, 2)) // __add(Point2, Point2)
                    lua_pushvec2f(L, *self - *rhs);
                else
                    return luaL_typerror(L, 2, "Vec2 or Point2");

                return 1;
            }

            return luaL_typerror(L, 1, "Vec2");
        });
        #pragma endregion

        #pragma region operator- (unary)
        Def(L, meta, "__unm", [](lua_State *L)
        {
            auto self = *lua_topoint2f(L, 1);
            lua_pushpoint2f(L, -self);
            return 1;
        });
        #pragma endregion

        #pragma region tovec
        Def(L, meta, "tovec", [](lua_State *L)
        {
            auto self = *lua_topoint2f(L, 1);
            lua_pushvec2f(L, Vec2(self));
            return 1;
        });
        #pragma endregion

        #pragma region tosize
        Def(L, meta, "tosize", [](lua_State *L)
        {
            auto self = *lua_topoint2f(L, 1);
            lua_pushsize2f(L, SizeF(Vec2(self)));
            return 1;
        });
        #pragma endregion
    });
    #pragma endregion

    #pragma region Size2
    BuildMeta(L, "Size2", [L](int meta)
    {
        #pragma region Constructor
        lua_createtable(L, 0, 1);
        Def(L, lua_gettop(L), "__call", [](lua_State *L)
        {
            int argc = lua_gettop(L);
            if (argc == 1)
            {
                lua_pushsize2f(L, SizeF());
            }
            else if (argc == 3)
            {
                if (lua_type(L, 2) != LUA_TNUMBER)
                    return luaL_typerror(L, 2, "number");
                if (lua_type(L, 3) != LUA_TNUMBER)
                    return luaL_typerror(L, 3, "number");
                auto x = (float)lua_tonumber(L, 2);
                auto y = (float)lua_tonumber(L, 3);
                lua_pushsize2f(L, SizeF(x, y));
            }
            else
            {
                return luaL_error(L, "Unknown number of arguments passed to math.Size2()");
            }
            return 1;
        });
        lua_setmetatable(L, meta);
        #pragma endregion

        #pragma region Get fields
        Def(L, meta, "__index", [](lua_State *L)
        {
            auto &self = *lua_tosize2f(L, 1);

            if (lua_type(L, 2) != LUA_TSTRING)
                return luaL_typerror(L, 2, "string");
            auto field = String::from_lua(L, 2);

            if (field == "width"_s)
                lua_pushnumber(L, self.width);
            else if (field == "height"_s)
                lua_pushnumber(L, self.height);
            else
            {
                lua_getmetatable(L, 1);
                lua_pushvalue(L, 2);
                lua_gettable(L, -2);
            }
            return 1;
        });
        #pragma endregion

        #pragma region Set fields
        Def(L, meta, "__newindex", [](lua_State *L)
        {
            auto &self = *lua_tosize2f(L, 1);

            if (lua_type(L, 2) != LUA_TSTRING)
                return luaL_typerror(L, 2, "string");
            auto field = String::from_lua(L, 2);

            if (lua_type(L, 3) != LUA_TNUMBER)
                return luaL_typerror(L, 3, "number");
            auto value = (float)lua_tonumber(L, 3);

            if (field == "width"_s)
                self.width = value;
            else if (field == "height"_s)
                self.height = value;
            else
                return luaL_error(L, "Size2 has no field `%s`", field.c_str());
            return 0;
        });
        #pragma endregion

        #pragma region tostring
        #pragma endregion

        #pragma region operator+

        #pragma endregion

        #pragma region operator-
        #pragma endregion

        #pragma region operator*
        #pragma endregion

        #pragma region operator/
        #pragma endregion

        #pragma region operator- (unary)
        #pragma endregion

        #pragma region tovec
        #pragma endregion

        #pragma region topoint
        #pragma endregion
    });
    #pragma endregion

    #pragma region Size2U
    BuildMeta(L, "Size2U", [L](int meta)
    {
    });
    #pragma endregion

    #pragma region Rect
    BuildMeta(L, "Rect", [L](int meta)
    {
    });
    #pragma endregion

    #pragma region Matrix
    BuildMeta(L, "Matrix", [L](int meta)
    {
    });
    #pragma endregion

    lua_pop(L, 1);
}

void lua_pushvec2f(lua_State *L, Vec2F v)
{
    PushMathObj(L, v);
    GetMathMeta(L, "Vec2");
    lua_setmetatable(L, -2);
}

void lua_pushvec2f_ref(lua_State *L, Vec2F *v)
{
    PushMathPtr(L, v);
    GetMathMeta(L, "Vec2");
    lua_setmetatable(L, -2);
}

std::optional<Vec2F &> lua_tovec2f(lua_State *L, int idx)
{
    if (EnsureMeta(L, idx, "Vec2"))
        return DerefObj<Vec2F>(L, idx);
    return std::nullopt;
}

void lua_pushpoint2f(lua_State *L, Point2F v)
{
    PushMathObj(L, v);
    GetMathMeta(L, "Point2");
    lua_setmetatable(L, -2);
}

void lua_pushpoint2f_ref(lua_State *L, Point2F *v)
{
    PushMathPtr(L, v);
    GetMathMeta(L, "Point2");
    lua_setmetatable(L, -2);
}

std::optional<Point2F&> lua_topoint2f(lua_State *L, int idx)
{
    if (EnsureMeta(L, idx, "Point2"))
        return DerefObj<Point2F>(L, idx);
    return std::nullopt;
}

void lua_pushsize2f(lua_State *L, Size2F v)
{
}

void lua_pushsize2f_ref(lua_State *L, Size2F *v)
{
}

std::optional<Size2F&> lua_tosize2f(lua_State *L, int idx)
{
    return std::optional<Size2F&>();
}

void lua_pushsize2u(lua_State *L, Size2U v)
{
}

void lua_pushsize2u_ref(lua_State *L, Size2U *v)
{
}

std::optional<Size2U&> lua_tosize2u(lua_State *L, int idx)
{
    return std::optional<Size2U&>();
}

void lua_pushrectf(lua_State *L, RectF v)
{
}

void lua_pushrectf_ref(lua_State *L, RectF *v)
{
}

std::optional<RectF&> lua_torectf(lua_State *L, int idx)
{
    return std::optional<RectF&>();
}

void lua_pushmatrix(lua_State * L, Math::Matrix3x2F m)
{
}

void lua_pushmatrix_ref(lua_State * L, Math::Matrix3x2F * m)
{
}

std::optional<Math::Matrix3x2F&> lua_tomatrix(lua_State * L, int idx)
{
    return std::optional<Math::Matrix3x2F&>();
}
