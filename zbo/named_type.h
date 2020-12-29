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
#include <iosfwd>
#include <utility>

namespace zbo {

/**
 * @brief This is a template for creating a strong typedef, similar to https://github.com/foonathan/type_safe
 *
 * you can use this class and inherit from it or use in a typedef.
 * You can add additional operators by also inheriting from any of the following behaviors:
 *  - EqualityComparable<>: defines operator== and operator!=
 *  - Comparable<>: EqualityComparable + operator<,>,<=,>=
 *  - Addition<> defines +=, +
 *  - Subtraction<> defines -=, -
 *  - Multiplication<> defines *, *-
 *  - Division<> defines /, /-
 *  - Arithmetic: Addition+Subtraction+Multiplication+Division
 *
 * Usage:
 *  struct MyType : public NamedType<int, MyType>, Comparable<MyType> {
 *      using NamedType::NamedType;
 *  };
 *
 *  or if you only want to have a plain wrapper without additional functionality use
 *   using MyType = NamedType<int, struct MyTypeTag>;
 * @tparam T The underlying type
 * @tparam Tag A Tag type to differentiate different types
 */
template <typename T, typename Tag>
class NamedType
{
  public:
    NamedType() = default;
    explicit constexpr NamedType(T value) : value_(value){};
    [[nodiscard]] explicit constexpr operator const T&() const { return value_; }
    [[nodiscard]] explicit constexpr operator T&() { return value_; }

    [[nodiscard]] constexpr const T& get() const { return value_; }
    [[nodiscard]] constexpr T& get() { return value_; }

  private:
    T value_;
};

namespace detail {
template <typename T, typename Tag>
T getUnderlyingType(NamedType<T, Tag>);
}

/// Trait to get the underlying type of a NamedType
template <typename NamedTypeT>
using UnderlyingType = decltype(detail::getUnderlyingType(std::declval<NamedTypeT>()));

/// Adds addition to a NamedType (or anything that has a .get() function)
template <typename NamedTypeT>
struct Addition
{
    friend constexpr NamedTypeT& operator+=(NamedTypeT& lhs, const NamedTypeT& rhs)
    {
        lhs.get() += rhs.get();
        return lhs;
    }
    [[nodiscard]] friend constexpr NamedTypeT operator+(const NamedTypeT& lhs, const NamedTypeT& rhs)
    {
        return NamedTypeT(lhs.get() + rhs.get());
    }
};

/// Adds subtraction to a NamedType (or anything that has a .get() function)
template <typename NamedTypeT>
struct Subtraction
{
    friend constexpr NamedTypeT& operator-=(NamedTypeT& lhs, const NamedTypeT& rhs)
    {
        lhs.get() -= rhs.get();
        return lhs;
    }
    [[nodiscard]] friend constexpr NamedTypeT operator-(const NamedTypeT& lhs, const NamedTypeT& rhs)
    {
        return NamedTypeT(lhs.get() - rhs.get());
    }
};

/// Adds multiplication to a NamedType (or anything that has a .get() function)
template <typename NamedTypeT>
struct Multiplication
{
    template <typename T>
    friend constexpr NamedTypeT& operator*=(NamedTypeT& lhs, const T& rhs)
    {
        lhs.get() *= rhs;
        return lhs;
    }

    template <typename T>
    [[nodiscard]] friend constexpr NamedTypeT operator*(const NamedTypeT& lhs, const T& rhs)
    {
        return NamedTypeT(lhs.get() * rhs);
    }

    template <typename T>
    [[nodiscard]] friend constexpr NamedTypeT operator*(const T& lhs, const NamedTypeT& rhs)
    {
        return rhs * lhs;
    }
};

/// Adds division to a NamedType (or anything that has a .get() function)
template <typename NamedTypeT>
struct Division
{
    template <typename T>
    friend constexpr NamedTypeT& operator/=(NamedTypeT& lhs, const T& rhs)
    {
        lhs.get() /= rhs;
        return lhs;
    }
    template <typename T>
    [[nodiscard]] friend constexpr NamedTypeT operator/(const NamedTypeT& lhs, const T& rhs)
    {
        return NamedTypeT(lhs.get() / rhs);
    }
    [[nodiscard]] friend constexpr auto operator/(const NamedTypeT& lhs, const NamedTypeT& rhs)
    {
        return lhs.get() / rhs.get();
    }
};

/// Adds all arithmetic functions to a NamedType (or anything that has a .get() function)
template <typename NamedTypeT>
struct Arithmetic : public Addition<NamedTypeT>,
                    Subtraction<NamedTypeT>,
                    Division<NamedTypeT>,
                    Multiplication<NamedTypeT>
{
};

/// Adds equality and inequality operator to a NamedType (or anything that has a .get() function)
template <typename NamedTypeT>
struct EqualityComparable
{
    [[nodiscard]] friend constexpr bool operator==(const NamedTypeT& lhs, const NamedTypeT& rhs)
    {
        return lhs.get() == rhs.get();
    }
    [[nodiscard]] friend constexpr bool operator!=(const NamedTypeT& lhs, const NamedTypeT& rhs)
    {
        return !(lhs == rhs);
    }
};

/// Adds all relational operators to a NamedType (or anything that has a .get() function)
template <typename NamedTypeT>
struct Comparable : public EqualityComparable<NamedTypeT>
{
    [[nodiscard]] friend constexpr bool operator>(const NamedTypeT& lhs, const NamedTypeT& rhs)
    {
        return lhs.get() > rhs.get();
    }
    [[nodiscard]] friend constexpr bool operator>=(const NamedTypeT& lhs, const NamedTypeT& rhs)
    {
        return lhs.get() >= rhs.get();
    }
    [[nodiscard]] friend constexpr bool operator<(const NamedTypeT& lhs, const NamedTypeT& rhs)
    {
        return lhs.get() < rhs.get();
    }
    [[nodiscard]] friend constexpr bool operator<=(const NamedTypeT& lhs, const NamedTypeT& rhs)
    {
        return lhs.get() <= rhs.get();
    }
};

/// Allows the NamedType to be streamed to std::ostream
template <typename NamedTypeT>
struct Streamable
{
    friend std::ostream& operator<<(std::ostream& stream, const NamedTypeT& type) { return stream << type.get(); }
};

}  // namespace zbo