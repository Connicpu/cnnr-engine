#pragma once

#include <exception>
#include <comdef.h>

class DxException : public std::exception
{
public:
    DxException(HRESULT hr)
        : error(hr)
    {
    }

    virtual const char *what() const noexcept override
    {
        return error.ErrorMessage();
    }

private:
    _com_error error;
};

inline void CheckHR(HRESULT hr)
{
    if (FAILED(hr))
    {
        throw DxException{ hr };
    }
}
