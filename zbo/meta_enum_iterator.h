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
#include "meta_enum.h"

namespace zbo {

template <class EnumType>
class MetaEnumIterator
{
    static_assert(std::is_enum<EnumType>::value, "Underlying type must be an enum");

  private:
    size_t enumIdx_{};

  public:
    using SizeType = std::size_t;
    using DifferenceType = std::ptrdiff_t;
    using value_type = EnumType;  // NOLINT(readability-identifier-naming)
    using reference = EnumType;   // NOLINT(readability-identifier-naming)
    using pointer = EnumType*;    // NOLINT(readability-identifier-naming)
    using IteratorCategory = std::random_access_iterator_tag;

    constexpr MetaEnumIterator() = default;
    constexpr MetaEnumIterator(const MetaEnumIterator& rhs) noexcept(true) : enumIdx_(rhs.enumIdx_) {}
    constexpr explicit MetaEnumIterator(EnumType value) noexcept(true) : enumIdx_(enumToIndex(value)) {}
    constexpr explicit MetaEnumIterator(size_t value) noexcept(true) : enumIdx_(value) {}
    ~MetaEnumIterator() noexcept(true) = default;
    MetaEnumIterator& operator=(const MetaEnumIterator& rhs) noexcept(true)
    {
        enumIdx_ = rhs.enumIdx_;
        return *this;
    }
    MetaEnumIterator& operator++() noexcept(true)
    {
        enumIdx_++;
        return *this;
    }
    MetaEnumIterator operator++(int) noexcept(true)
    {
        MetaEnumIterator r(*this);
        ++*this;
        return r;
    }
    MetaEnumIterator& operator+=(SizeType o) noexcept(true)
    {
        enumIdx_ += o;
        return *this;
    }
    friend constexpr MetaEnumIterator operator+(const MetaEnumIterator& it, SizeType o) noexcept(true)
    {
        return MetaEnumIterator(it.enumIdx_ + o);
    }
    friend constexpr MetaEnumIterator operator+(SizeType o, const MetaEnumIterator& it) noexcept(true)
    {
        return MetaEnumIterator(it.enumIdx_ + o);
    }
    MetaEnumIterator& operator--() noexcept(true)
    {
        enumIdx_--;
        return *this;
    }
    MetaEnumIterator operator--(int) noexcept(true)
    {
        MetaEnumIterator r(*this);
        --*this;
        return r;
    }
    MetaEnumIterator& operator-=(SizeType o) noexcept(true)
    {
        enumIdx_ -= o;
        return *this;
    }
    friend constexpr MetaEnumIterator operator-(const MetaEnumIterator& it, SizeType o) noexcept(true)
    {
        return MetaEnumIterator(it.enumIdx_ - o);
    }
    friend constexpr DifferenceType operator-(MetaEnumIterator lhs, MetaEnumIterator rhs) noexcept(true)
    {
        return DifferenceType(lhs.enumIdx_) - DifferenceType(rhs.enumIdx_);
    }
    constexpr reference operator*() const noexcept(true) { return metaEnum<EnumType>().members.at(enumIdx_).value; }
    constexpr reference operator[](SizeType o) const noexcept(true)
    {
        return metaEnum<EnumType>().members.at(enumIdx_ + o).value;
    }
    constexpr const EnumType* operator->() const noexcept(true)
    {
        return &metaEnum<EnumType>().members.at(enumIdx_).value;
    }
    constexpr friend bool operator==(const MetaEnumIterator& lhs, const MetaEnumIterator& rhs) noexcept(true)
    {
        return lhs.enumIdx_ == rhs.enumIdx_;
    }
    constexpr friend bool operator!=(const MetaEnumIterator& lhs, const MetaEnumIterator& rhs) noexcept(true)
    {
        return lhs.enumIdx_ != rhs.enumIdx_;
    }
    constexpr friend bool operator<(const MetaEnumIterator& lhs, const MetaEnumIterator& rhs) noexcept(true)
    {
        return lhs.enumIdx_ < rhs.enumIdx_;
    }
    constexpr friend bool operator>(const MetaEnumIterator& lhs, const MetaEnumIterator& rhs) noexcept(true)
    {
        return lhs.enumIdx_ > rhs.enumIdx_;
    }
    constexpr friend bool operator<=(const MetaEnumIterator& lhs, const MetaEnumIterator& rhs) noexcept(true)
    {
        return lhs.enumIdx_ <= rhs.enumIdx_;
    }
    constexpr friend bool operator>=(const MetaEnumIterator& lhs, const MetaEnumIterator& rhs) noexcept(true)
    {
        return lhs.enumIdx_ >= rhs.enumIdx_;
    }
    friend void swap(MetaEnumIterator& lhs, MetaEnumIterator& rhs) noexcept(true)
    {
        std::swap(lhs.enumIdx_, rhs.enumIdx_);
    }
};

template <typename T>
using ReverseMetaEnumIterator = std::reverse_iterator<MetaEnumIterator<T>>;

template <typename T>
class MetaEnumRange
{
    using iterator = MetaEnumIterator<T>;  // NOLINT(readability-identifier-naming)

  public:
    iterator begin() { return iterator(0); };
    iterator end() { return iterator(metaEnum<T>().size()); };
};

}  // namespace zbo