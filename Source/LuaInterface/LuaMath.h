#pragma once

#include "IncludeLua.h"
#include <Common/optional.h>
#include <Common/MathLib.h>

void lua_math_init(lua_State *L);

void lua_pushvec2f(lua_State *L, Math::Vec2F v);
void lua_pushvec2f_ref(lua_State *L, Math::Vec2F *v);
std::optional<Math::Vec2F &> lua_tovec2f(lua_State *L, int idx);

void lua_pushpoint2f(lua_State *L, Math::Point2F p);
void lua_pushpoint2f_ref(lua_State *L, Math::Point2F *p);
std::optional<Math::Point2F &> lua_topoint2f(lua_State *L, int idx);

void lua_pushsize2f(lua_State *L, Math::Size2F s);
void lua_pushsize2f_ref(lua_State *L, Math::Size2F *s);
std::optional<Math::Size2F &> lua_tosize2f(lua_State *L, int idx);

void lua_pushsize2u(lua_State *L, Math::Size2U s);
void lua_pushsize2u_ref(lua_State *L, Math::Size2U *s);
std::optional<Math::Size2U &> lua_tosize2u(lua_State *L, int idx);

void lua_pushrectf(lua_State *L, Math::RectF r);
void lua_pushrectf_ref(lua_State *L, Math::RectF *r);
std::optional<Math::RectF &> lua_torectf(lua_State *L, int idx);

void lua_pushmatrix(lua_State *L, Math::Matrix3x2F m);
void lua_pushmatrix_ref(lua_State *L, Math::Matrix3x2F *m);
std::optional<Math::Matrix3x2F &> lua_tomatrix(lua_State *L, int idx);

