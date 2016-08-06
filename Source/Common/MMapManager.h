#pragma once

#include <stdint.h>

#include <Common/Filesystem.h>
#include <Common/Platform.h>
#include <Common/optional.h>
#include <vector>

class MMapView;

class MMapManager
{
public:
    inline MMapManager() = default;
    inline ~MMapManager();
    inline bool Open(const fs::path &path, bool readonly);
    inline void Close();
    inline std::optional<MMapView> Map(uint64_t offset, size_t length);
    // NOTE: Only does anything on windows
    inline bool Reserve(uint64_t size);

    template <typename T>
    inline std::optional<T> ReadStruct(uint64_t offset);
    template <typename T>
    inline std::optional<std::vector<T>> ReadArray(uint64_t offset, size_t count);

    template <typename T>
    inline bool WriteStruct(uint64_t offset, const T &data);
    template <typename T>
    inline bool WriteArray(uint64_t offset, const T *data, size_t len);

private:
    bool readonly = false;
#ifdef MSVC
    uint64_t size = 0;
    HANDLE file = INVALID_HANDLE_VALUE;
    HANDLE file_view = nullptr;
#else
    int fd = -1;
#endif
    std::optional<fs::path> path_;
};

class MMapView
{
public:
    inline MMapView();
    inline MMapView(MMapView &&other);
    inline ~MMapView();
    inline MMapView &operator=(MMapView &&other);
    template <typename T = uint8_t>
    inline const T *Memory() const;
    template <typename T = uint8_t>
    inline T *MemoryMut() const;
    inline size_t Length() const;
    inline bool IsValid() const;

    inline MMapView(const MMapView &) = delete;
    inline MMapView &operator=(const MMapView &) = delete;
private:
    friend class MMapManager;
    inline MMapView(void *real_addr, void *memory, size_t length, bool readonly)
        : real_addr(real_addr), memory(memory), length(length), readonly(readonly)
    {
    }

    void *real_addr;
    void *memory;
    size_t length;
    bool readonly;
};

inline MMapManager::~MMapManager()
{
    Close();
}

inline bool MMapManager::Open(const fs::path &path, bool rdonly)
{
    Close();
    path_ = path;

    readonly = rdonly;
#ifdef MSVC
    auto wpath = path.generic_wstring();
    file = CreateFileW(
        wpath.c_str(),
        readonly ? GENERIC_READ : (GENERIC_READ | GENERIC_WRITE),
        readonly ? FILE_SHARE_READ : 0,
        nullptr,
        readonly ? OPEN_EXISTING : OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (file == INVALID_HANDLE_VALUE)
        return false;

    LARGE_INTEGER file_size;
    if (!GetFileSizeEx(file, &file_size))
        return false;
    size = (SIZE_T)file_size.QuadPart;

    if (size == 0)
        file_size.QuadPart = 1;

    file_view = CreateFileMappingW(
        file, nullptr,
        readonly ? PAGE_READONLY : PAGE_READWRITE,
        file_size.HighPart, file_size.LowPart,
        nullptr
    );
    if (file_view == nullptr)
        return false;

#else
    auto spath = path.generic_string();
    fd = open(
        spath.c_str(),
        readonly ? O_RDONLY : (O_RDWR | O_CREAT),
        S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH
    );

    if (fd == -1)
        return false;
#endif

    return true;
}

inline void MMapManager::Close()
{
#ifdef MSVC
    if (file_view != nullptr)
        CloseHandle(file_view);
    file_view = nullptr;

    if (file != INVALID_HANDLE_VALUE)
        CloseHandle(file);
    file = INVALID_HANDLE_VALUE;
#else
    if (fd != -1)
        close(fd);
    fd = -1;
#endif
}


inline std::optional<MMapView> MMapManager::Map(uint64_t offset, size_t length)
{
#ifdef MSVC
    if (file == INVALID_HANDLE_VALUE)
        return std::nullopt;

    if (offset + length > size)
    {
        if (!Reserve(offset + length))
            return std::nullopt;
    }

    SYSTEM_INFO sys_info;
    GetSystemInfo(&sys_info);
    size_t align_off = offset % sys_info.dwAllocationGranularity;

    LARGE_INTEGER off;
    off.QuadPart = offset - align_off;
    void *real_addr = MapViewOfFile(
        file_view,
        readonly ? FILE_MAP_READ : (FILE_MAP_READ | FILE_MAP_WRITE),
        off.HighPart, off.LowPart,
        length + align_off
    );
    if (real_addr == nullptr)
        return std::nullopt;
#else
    if (fd == -1)
        return std::nullopt;

    size_t align_off = offset % sysconf(_SC_PAGE_SIZE);

    void *real_addr = mmap(
        nullptr, length + align_off,
        readonly ? PROT_READ : (PROT_READ | PROT_WRITE),
        MAP_PRIVATE | MAP_FILE, fd,
        (off_t)(offset - align_off)
    );
    if (real_addr == MAP_FAILED)
        return std::nullopt;
#endif

    void *memory = ((char *)real_addr) + align_off;
    return MMapView(real_addr, memory, length, readonly);
}

inline bool MMapManager::Reserve(uint64_t new_size)
{
    auto fsize = fs::file_size(*path_);
    if (fsize >= new_size)
        return true;

    auto path = std::move(*path_);
    Close();
    fs::resize_file(path, new_size);
    return Open(path, readonly);
}

template<typename T>
inline std::optional<T> MMapManager::ReadStruct(uint64_t offset)
{
    if (auto view = Map(offset, sizeof(T)))
    {
        return *view->Memory<T>();
    }
    return std::nullopt;
}

template<typename T>
inline std::optional<std::vector<T>> MMapManager::ReadArray(uint64_t offset, size_t count)
{
    if (auto view = Map(offset, sizeof(T) * count))
    {
        auto mem = view->Memory<T>();
        return std::vector<T>(&mem[0], &mem[count]);
    }
    return std::nullopt;
}

template<typename T>
inline bool MMapManager::WriteStruct(uint64_t offset, const T &data)
{
    if (readonly) return false;
    if (auto view = Map(offset, sizeof(T)))
    {
        memcpy(view->MemoryMut<void>(), &data, sizeof(T));
        return true;
    }
    return false;
}

template<typename T>
inline bool MMapManager::WriteArray(uint64_t offset, const T *data, size_t len)
{
    if (readonly) return false;
    if (auto view = Map(offset, sizeof(T) * len))
    {
        memcpy(view->MemoryMut<void>(), &data, sizeof(T) * len);
        return true;
    }
    return false;
}

inline MMapView::MMapView()
    : real_addr(nullptr)
{
}

inline MMapView::MMapView(MMapView &&other)
    : real_addr(other.real_addr), memory(other.memory), length(other.length), readonly(other.readonly)
{
    other.real_addr = nullptr;
}

inline MMapView::~MMapView()
{
    if (real_addr)
    {
#ifdef MSVC
        UnmapViewOfFile(real_addr);
#else
        size_t align_off = (const char *)memory - (const char *)real_addr;
        munmap(real_addr, length + align_off);
#endif
    }
}

inline MMapView &MMapView::operator=(MMapView &&other)
{
    real_addr = other.real_addr;
    memory = other.memory;
    length = other.length;
    readonly = other.readonly;

    other.real_addr = nullptr;

    return *this;
}

template<typename T>
inline const T *MMapView::Memory() const
{
    assert(real_addr);
    return (const T *)memory;
}

template<typename T>
inline T *MMapView::MemoryMut() const
{
    assert(real_addr);
    assert(!readonly);
    return (T *)memory;
}

inline size_t MMapView::Length() const
{
    if (!real_addr) throw std::logic_error{ "Tried to access invalid MMapView" };
    return length;
}

inline bool MMapView::IsValid() const
{
    return !!real_addr;
}
