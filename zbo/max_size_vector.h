// MIT License
//
// Copyright (c) 2020 Lenzebo
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include "contracts.h"

#include <algorithm>
#include <array>

namespace zbo {

/**
 * @brief This is a simple implementation of a vector that lives on the stack and has a fixed maximum size at compile
 *        time. Exceeding its capacity will not allocate and will occur a contract violation (std::terminate)
 * @tparam T The value type within the container
 * @tparam maxSize The compile time maximum size
 */
template <typename T, size_t maxSize>
class MaxSizeVector
{
  public:
    using value_type = T;              // NOLINT (readability-identifier-naming)
    using reference = T&;              // NOLINT (readability-identifier-naming)
    using const_reference = const T&;  // NOLINT (readability-identifier-naming)
    using iterator = T*;               // NOLINT (readability-identifier-naming)

    static_assert(std::is_trivially_constructible_v<T>,
                  "T must be trivially constructable for now to simplify some assumptions and we can reuse "
                  "std::array as implementation detail");

    constexpr MaxSizeVector() = default;
    explicit constexpr MaxSizeVector(const std::initializer_list<T> init) { insert(begin(), init.begin(), init.end()); }
    template <class Container>
    explicit constexpr MaxSizeVector(const Container& other)
    {
        insert(begin(), std::begin(other), std::end(other));
    }

    template <class Container>
    constexpr MaxSizeVector& operator=(const Container& other)
    {
        clear();
        insert(begin(), std::begin(other), std::end(other));
        return *this;
    }

    [[nodiscard]] constexpr T* begin() noexcept { return data_.begin(); }
    [[nodiscard]] constexpr T* end() noexcept { return std::next(begin(), size()); }
    [[nodiscard]] constexpr const T* begin() const noexcept { return data_.begin(); }
    [[nodiscard]] constexpr const T* end() const noexcept { return std::next(begin(), size()); }

    constexpr void reserve([[maybe_unused]] size_t newSize) noexcept { ZBO_PRECONDITION(newSize < maxSize); }
    constexpr void clear() noexcept { count_ = 0; }

    template <class InputIterator>
    constexpr T* insert(T* position, InputIterator first, InputIterator last)
    {
        size_t dist = std::distance(first, last);
        ZBO_PRECONDITION(size() + dist <= capacity());

        // first move all the elements to the end...
        std::move(position, end(), position + dist);  // NOLINT

        // now copy the input range into the empty space
        std::copy(first, last, position);

        count_ += dist;
        return position;
    }

    [[nodiscard]] constexpr bool empty() const noexcept { return size() == 0; }
    [[nodiscard]] constexpr size_t size() const noexcept { return count_; }
    [[nodiscard]] constexpr size_t capacity() const noexcept { return maxSize; }
    [[nodiscard]] constexpr T* data() noexcept { return data_.data(); }
    [[nodiscard]] constexpr const T* data() const noexcept { return data_.data(); }

    constexpr void erase(T* first, T* last)
    {
        const size_t length = std::distance(first, last);
        ZBO_PRECONDITION(length <= size());
        std::move(last, end(), first);
        count_ -= length;
    }

    constexpr void erase(T* elem)
    {
        ZBO_PRECONDITION(elem != end());
        erase(elem, std::next(elem));
    }

    [[nodiscard]] constexpr T& at(size_t idx)
    {
        ZBO_PRECONDITION(idx < count_);
        return data_.at(idx);
    }
    [[nodiscard]] constexpr const T& at(size_t idx) const
    {
        ZBO_PRECONDITION(idx < count_);
        return data_.at(idx);
    }

    [[nodiscard]] constexpr T& operator[](size_t idx) { return at(idx); }
    [[nodiscard]] constexpr const T& operator[](size_t idx) const { return at(idx); }

    [[nodiscard]] constexpr const T& back() const { return at(size() - 1); }
    [[nodiscard]] constexpr T& back() { return at(size() - 1); }
    [[nodiscard]] constexpr const T& front() const { return at(0); }
    [[nodiscard]] constexpr T& front() { return at(0); }

    // NOLINTNEXTLINE (readability-identifier-naming)
    constexpr void pop_back() noexcept { count_--; }

    // NOLINTNEXTLINE (readability-identifier-naming)
    constexpr void push_back(T&& elem)
    {
        ZBO_PRECONDITION(count_ < maxSize);
        count_++;
        back() = elem;
    }

    // NOLINTNEXTLINE (readability-identifier-naming)
    constexpr void push_back(const T& elem)
    {
        ZBO_PRECONDITION(count_ < maxSize);
        count_++;
        back() = elem;
    }

  private:
    std::array<T, maxSize> data_{};
    size_t count_ = 0;
};
}  // namespace zbo