#include "pch.h"
#include "ModulePack.h"
#include <Common/MMap.h>

const size_t NAME_LEN_MAX = 128 - sizeof(uint64_t) - sizeof(uint32_t) - 1;

struct PackHeader
{
    uint32_t module_count;
};

struct ModuleListing
{
    uint64_t offset;
    uint32_t length;
    char name[NAME_LEN_MAX + 1];
};

ModulePack::ModulePack(const fs::path &path)
{
    if (!pack_.Open(path, true))
        throw std::runtime_error{ "Failed to open module pack located at " + path.generic_string() };

    auto offset = 0;
    auto header_view = pack_.ReadStruct<PackHeader>(offset);
    if (!header_view)
        throw std::runtime_error{ "Failed to open module pack located at " + path.generic_string() };
    offset += sizeof(PackHeader);

    auto listings = pack_.ReadArray<ModuleListing>(offset, header_view->module_count);
    if (!listings)
        throw std::runtime_error{ "Failed to open module pack located at " + path.generic_string() };

    for (auto &listing : *listings)
    {
        listings_[listing.name] = std::make_pair(listing.offset, listing.length);
    }
}

void ModulePackBuilder::AddFile(const String &module, const fs::path &path)
{
    if (module.size() > NAME_LEN_MAX)
        throw std::runtime_error{ "Module name too long (115 char max)" };
    files_.insert_or_assign(module, path);
}

void ModulePackBuilder::Build(const fs::path &output)
{
    uint64_t next_module = sizeof(PackHeader) + sizeof(ModuleListing) * files_.size();

    MMapManager manager;
    if (!manager.Open(output, false))
        throw std::runtime_error{ "Failed to write out the module pack" };

    PackHeader header;
    header.module_count = (uint32_t)files_.size();
    if (!manager.WriteStruct(0, header))
        throw std::runtime_error{ "Failed to write out the module pack" };

    lua_State *L = lua_open();
    try
    {
        uint64_t listing_off = sizeof(PackHeader);
        for (auto &pair : files_)
        {
            MMap lua_file;
            if (!lua_file.Open(pair.second))
                throw std::runtime_error{ "Failed to open " + pair.second.generic_string() + " while building pack" };

            if (luaL_loadbuffer(L, lua_file.GetMemory<char>(), lua_file.GetLength(), pair.first.clone().c_str()))
            {
                auto msg = ("Failed to compile "_s + pair.second.generic_string() +
                    ", "_s + String::from_lua(L, -1)).into_stdstring();
                throw std::runtime_error{ msg };
            }

            std::vector<char> dump_data;
            lua_dump(L, [](lua_State *, const void *data, size_t sz, void *ud) -> int
            {
                auto &dump_data = *(std::vector<char> *)ud;
                size_t begin = dump_data.size();
                dump_data.resize(begin + sz);
                memcpy(&dump_data[begin], data, sz);
                return 0;
            }, &dump_data);
            lua_pop(L, 1);

            ModuleListing listing;
            listing.offset = next_module;
            listing.length = (uint32_t)dump_data.size();
            memcpy(listing.name, pair.first.span().data(), pair.first.span().size());
            listing.name[pair.first.span().size()] = 0;
            manager.WriteStruct(listing_off, listing);
            manager.WriteArray(next_module, &dump_data[0], dump_data.size());

            listing_off += sizeof(ModuleListing);
            next_module += dump_data.size();
        }
    }
    catch (...)
    {
        lua_close(L);
        throw;
    }
    lua_close(L);
}
