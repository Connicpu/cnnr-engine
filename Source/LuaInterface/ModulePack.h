#pragma once

#include <Common/Filesystem.h>
#include <Common/String.h>
#include <Common/Hash.h>
#include <Common/MMapManager.h>

class ModulePack
{
public:
    ModulePack(const fs::path &path);

private:
    MMapManager pack_;
    HashMap<String, std::pair<uint64_t, uint32_t>> listings_;
};

class ModulePackBuilder
{
public:
    void AddFile(const String &module, const fs::path &path);
    void Build(const fs::path &output);

private:
    HashMap<String, fs::path> files_;
};
