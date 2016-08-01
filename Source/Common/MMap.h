#pragma once

#include <Common/Filesystem.h>
#include <Common/Platform.h>

class MMap
{
public:
    inline MMap();
    inline ~MMap();

    inline bool Open(const fs::path &path);

    inline size_t GetLength();
    template <typename T = uint8_t>
    inline const T *GetMemory();

    inline void Close();

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

inline MMap::MMap()
{
}

inline MMap::~MMap()
{
    Close();
}

inline bool MMap::Open(const fs::path &path)
{
    Close();

#ifdef MSVC

    auto wpath = path.generic_wstring();
    file = CreateFileW(
        wpath.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr
    );

    if (file == INVALID_HANDLE_VALUE)
        return false;

    file_view = CreateFileMappingW(
        file, nullptr, PAGE_READONLY, 0, 0, nullptr
    );

    if (file_view == nullptr)
        return false;

    LARGE_INTEGER file_size;
    if (!GetFileSizeEx(file, &file_size))
        return false;
    length = (SIZE_T)file_size.QuadPart;

    memory = MapViewOfFile(file_view, FILE_MAP_READ, 0, 0, length);

    if (memory == nullptr)
        return false;

#else

    auto spath = path.generic_string();
    fd = open(spath.c_str(), O_RDONLY, 0);

    if (fd == -1)
        return false;

    struct stat st;
    int fs_res = fstat(fd, &st);
    if (fs_res != 0)
        return false;
    length = (size_t)st.st_size;

    memory = mmap(nullptr, length, PROT_READ, MAP_PRIVATE | MAP_FILE, fd, 0);
    if (memory == MAP_FAILED)
        return false;

#endif

    return true;
}

inline size_t MMap::GetLength()
{
    return length;
}

template <typename T>
inline const T *MMap::GetMemory()
{
    return (const T *)memory;
}

inline void MMap::Close()
{
#ifdef MSVC

    if (memory != nullptr)
        UnmapViewOfFile(memory);
    memory = nullptr;

    if (file_view != nullptr)
        CloseHandle(file_view);
    file_view = nullptr;

    if (file != INVALID_HANDLE_VALUE)
        CloseHandle(file);
    file = INVALID_HANDLE_VALUE;

#else

    if (memory != MAP_FAILED)
        munmap(memory, length);
    memory = MAP_FAILED;

    if (fd != -1)
        close(fd);
    fd = -1;

#endif
}


