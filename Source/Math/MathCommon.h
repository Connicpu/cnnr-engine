#pragma once

#ifdef MSVC
#define MCONV __vectorcall
#else
#define MCONV
#endif
