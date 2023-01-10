#pragma once
// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include "Iterator.h"
#include <initializer_list>
#include <memory>
#include <new>
#include <vector>
namespace std
{
template<typename T> struct ReadOnlyVector : public ForwardIterator<T>
{
  private:
    T *data;
    const size_t size_;

  public:
    constexpr ReadOnlyVector(T *data, size_t size) noexcept
        : data(data)
        , size_(size)
    {
    }
    constexpr ReadOnlyVector(initializer_list<T> entries) noexcept
        : size_(entries.size())
    {
        data = new (std::nothrow) T[entries.size()];
        size_t index = 0;
        for (auto &entry : entries)
            data[index++] = entry;
    }

    constexpr const T &operator[](size_t i)
    {
        return data[i];
    }

    virtual size_t i_size() const noexcept override final
    {
        return size_;
    }

    constexpr size_t size() const noexcept
    {
        return size_;
    }

    virtual constexpr T *begin() noexcept override final
    {
        return data;
    }

    virtual constexpr const T *begin() const noexcept override final
    {
        return data;
    }

    virtual constexpr T *cbegin() const noexcept override final
    {
        return data;
    }

    static ReadOnlyVector<T> lock(vector<T> data)
    {
        return *new ReadOnlyVector(data.data(), data.size());
    }
};
} // namespace internal_std