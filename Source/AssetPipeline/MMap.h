#pragma once

#include <Common/Filesystem.h>
#include <Common/Platform.h>

class MMap
{
public:
    MMap();
    ~MMap();

    bool Open(const fs::path &path);
    
    size_t GetLength();
    const uint8_t *GetMemory();

    void Close();

private:
#ifdef MSVC
    HANDLE file = INVALID_HANDLE_VALUE;
    HANDLE file_view = nullptr;
    size_t length = 0;
    void *memory = nullptr;
#else
    int fd = -1;
    size_t length = 0;
    void *memory = MAP_FAILED;
#endif
};

