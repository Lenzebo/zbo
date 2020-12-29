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

#include <cassert>
#include <span>

namespace zbo {

/**
 * @brief Helper class to iterate over a contiguous memory with an offset including wrapping to the beginning
 *
 * This allows iterating over all elements in an array/vector, but starting from any arbitrary element in the middle
 *
 * Example: given an array of ints containing data = [4, 5, 7, 8, 9],
 * then the sequence returned by
 *    CircularRange(data, 2) = [7, 8, 9, 4, 5]
 *    CircularRange(data, 4) = [9, 4, 5, 7, 8]
 *    CircularRange(data, 0) = [4, 5, 7, 8, 9]
 *
 * @tparam Type Underlying datatype
 */
template <typename Type>
class CircularRange
{
    struct Iterator : public std::iterator<std::random_access_iterator_tag, Type>
    {
        Iterator(std::span<Type> data, size_t offset) : data_(data), current_(offset){};

        [[nodiscard]] Type& operator*() const { return data_[current_ % data_.size()]; }
        Iterator& operator++()
        {
            ++current_;
            return *this;
        }
        Iterator operator++(int)
        {
            Iterator it = *this;
            ++(*this);
            return it;
        }

        [[nodiscard]] Type* operator->() const { return &this->operator*(); }

        ptrdiff_t operator-(const Iterator& other) const noexcept { return int(current_) - (other.current_); }

        [[nodiscard]] bool operator==(const Iterator& other) const noexcept { return current_ == other.current_; }
        [[nodiscard]] bool operator!=(const Iterator& other) const noexcept { return !((*this) == other); }

        // TODO not all functions necessary for random_access_iterator implemented. Extend when necessary
      private:
        std::span<Type> data_;
        size_t current_;
    };

  public:
    CircularRange(std::span<Type> data, size_t offset) : data_(data), startOffset_(offset){};

    void advance(size_t num) { startOffset_ = (startOffset_ + num) % data_.size(); }
    [[nodiscard]] Iterator begin() const { return {data_, startOffset_}; }
    [[nodiscard]] Iterator end() const { return {data_, (startOffset_ + data_.size())}; }
    [[nodiscard]] std::span<Type> data() const { return data_; }
    [[nodiscard]] Type& operator[](size_t idx) const
    {
        ZBO_PRECONDITION(idx < data_.size());
        return data_[(startOffset_ + idx) % data_.size()];
    }

  private:
    std::span<Type> data_;
    size_t startOffset_;
};
}  // namespace zbo