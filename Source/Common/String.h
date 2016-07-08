#pragma once

#include <Common/Platform.h>
#include <Common/Filesystem.h>
#include <string>
#include <gsl.h>
#include "Hash.h"

class String
{
    enum Tag { Owned, Ref };
    union Data
    {
        inline Data(std::string &&owned);
        inline Data(gsl::cstring_span<> ref);
        inline ~Data() {}

        std::string owned;
        gsl::cstring_span<> ref;
    };

    Tag tag;
    Data data;

public:
    inline ~String();
    inline String();

    inline String(const String &other);
    inline String(String &&other);

    // Copy a reference
    inline String(gsl::cstring_span<> str);
    // Takes ownership
    inline String(std::string &&owned);
    // String literals
    template <size_t len>
    inline String(const char str[len]);

    inline String &operator=(const String &other);
    inline String &operator=(String &&other);

    inline String &operator+=(const String &rhs);
    
    inline void reset();
    static inline String to_owned(String &&str);
    inline std::string &as_owned();
    inline gsl::cstring_span<> span() const;
    inline String &append(const String &rhs);

    inline bool empty() const;
    inline size_t length() const;
    inline size_t size() const;

    inline operator gsl::cstring_span<>() const;
};

inline String::Data::Data(std::string &&owned)
    : owned(std::move(owned))
{
}

inline String::Data::Data(gsl::cstring_span<> ref)
    : ref(ref)
{
}

inline String operator ""_s(const char *str, size_t len)
{
    return gsl::cstring_span<>{ str, (ptrdiff_t)len };
}

inline String::~String()
{
    if (tag == Owned)
        data.owned.~basic_string();
}

inline String::String()
    : tag(Ref), data(gsl::cstring_span<>{ "" })
{
}

inline String::String(const String &other)
    : tag(Ref), data(other)
{
    if (other.tag == Owned)
    {
        new (&data.owned) std::string(other.data.owned);
    }
}

inline String::String(String &&other)
    : String()
{
    tag = other.tag;
    if (tag == Owned)
        new (&data.owned) std::string(std::move(other.data.owned));
    else
        data.ref = other.data.ref;
}

inline String::String(gsl::cstring_span<> str)
    : tag(Ref), data(str)
{
}

inline String::String(std::string &&owned)
    : tag(Owned), data(std::move(owned))
{
}

inline String &String::operator=(const String &other)
{
    if (tag == Owned && other.tag == Owned)
    {
        data.owned = other.data.owned;
    }
    else
    {
        this->String::~String();
        this->String::String(other);
    }
    return *this;
}

inline String &String::operator=(String &&other)
{
    this->String::~String();
    this->String::String(std::move(other));
    return *this;
}

inline String &String::operator+=(const String &rhs)
{
    return append(rhs);
}

inline void String::reset()
{
    this->String::~String();
    this->String::String();
}

inline String String::to_owned(String &&str)
{
    if (str.tag == Owned)
        return std::move(str);

    gsl::cstring_span<> span = str;
    return String(std::string{ span.begin(), span.end() });
}

inline std::string &String::as_owned()
{
    *this = to_owned(std::move(*this));
    return data.owned;
}

inline gsl::cstring_span<> String::span() const
{
    return *this;
}

inline String &String::append(const String &rhs)
{
    gsl::cstring_span<> span = rhs;
    as_owned().append(span.data(), span.size());
    return *this;
}

inline bool String::empty() const
{
    return size() == 0;
}

inline size_t String::length() const
{
    gsl::cstring_span<> span = *this;
    return span.size();
}

inline size_t String::size() const
{
    return length();
}

inline String::operator gsl::cstring_span<>() const
{
    switch (tag)
    {
        case Owned:
            return data.owned;
        case Ref:
            return data.ref;
        default:
            unreachable();
    }
}

template<size_t len>
inline String::String(const char str[len])
    : tag(Ref), data(gsl::cstring_span<>{ str })
{
}

inline String operator+(String &&lhs, const String &rhs)
{
    String temp = std::move(lhs);
    gsl::cstring_span<> rspan = rhs;
    temp.as_owned().append(rspan.data(), rspan.length());
    return std::move(temp);
}

inline String operator+(const String &lhs, const String &rhs)
{
    return String(lhs) + rhs;
}

inline bool operator==(const String &lhs, const String &rhs)
{
    return static_cast<gsl::cstring_span<>>(lhs) == static_cast<gsl::cstring_span<>>(rhs);
}

inline bool operator!=(const String &lhs, const String &rhs)
{
    return static_cast<gsl::cstring_span<>>(lhs) != static_cast<gsl::cstring_span<>>(rhs);
}

inline bool operator<(const String &lhs, const String &rhs)
{
    return static_cast<gsl::cstring_span<>>(lhs) > static_cast<gsl::cstring_span<>>(rhs);
}

inline bool operator>(const String &lhs, const String &rhs)
{
    return static_cast<gsl::cstring_span<>>(lhs) < static_cast<gsl::cstring_span<>>(rhs);
}

inline bool operator<=(const String &lhs, const String &rhs)
{
    return static_cast<gsl::cstring_span<>>(lhs) <= static_cast<gsl::cstring_span<>>(rhs);
}

inline bool operator>=(const String &lhs, const String &rhs)
{
    return static_cast<gsl::cstring_span<>>(lhs) >= static_cast<gsl::cstring_span<>>(rhs);
}

inline fs::path operator/(fs::path &&lhs, const String &rhs)
{
    fs::path temp = std::move(lhs);
    gsl::cstring_span<> span = rhs;
    temp.append(span.data(), span.data() + span.size());
    return std::move(temp);
}

inline fs::path operator/(const fs::path &lhs, const String &rhs)
{
    return fs::path(lhs) / rhs;
}

template <typename H>
inline void hash_apply(const String &str, H &h)
{
    gsl::cstring_span<> span = str;
    hash_apply(span, h);
}
