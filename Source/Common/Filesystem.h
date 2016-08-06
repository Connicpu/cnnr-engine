#pragma once

#ifdef _MSC_VER
#include <filesystem>
namespace fs = std::tr2::sys;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

