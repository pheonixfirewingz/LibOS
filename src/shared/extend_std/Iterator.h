#pragma once
// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include <stddef.h>
namespace std
{
struct Iterator
{
  protected:
    virtual constexpr size_t i_size() const noexcept = 0;
  public:
    virtual ~Iterator() noexcept = default;
};
template<typename T> struct ForwardIterator : public Iterator
{
    virtual constexpr T *begin() noexcept = 0;
    virtual constexpr const T *begin() const noexcept = 0;
    virtual constexpr const T *cbegin() const noexcept = 0;
    constexpr T* end() noexcept
    {
        return begin() + i_size();
    }
    constexpr const T *end() const noexcept
    {
        return begin() + i_size();
    }

    constexpr const T *cend() const noexcept
    {
        return cbegin() + i_size();
    }

    virtual ~ForwardIterator() noexcept override = default;
};

template<typename T> struct ReverseIterator : public Iterator
{
    virtual constexpr T *rbegin() noexcept = 0;
    virtual constexpr const T *rbegin() const noexcept = 0;
    virtual constexpr const T *crbegin() const noexcept = 0;
    constexpr T *rend() noexcept
    {
        return rbegin() - i_size();
    }
    constexpr const T *rend() const noexcept
    {
        return rbegin() - i_size();
    }

    constexpr const T *crend() const noexcept
    {
        return crbegin() - i_size();
    }
    virtual ~ReverseIterator() noexcept override = default;
};

} // namespace internal_std