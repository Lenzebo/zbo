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

#include <array>
#include <optional>
#include <string_view>

namespace zbo {

/**
 * @brief Struct describing one enum value of a given EnumType
 * @tparam EnumType
 */
template <typename EnumType>
struct MetaEnumMember
{
    EnumType value = {};
    std::string_view name;
    std::string_view string;
    size_t index = {};
};

/**
 * @brief Struct describing an complete enum type
 * @tparam EnumType
 * @tparam UnderlyingTypeIn
 * @tparam SIZE
 */
template <typename EnumType, typename UnderlyingTypeIn, size_t numEnums>
struct MetaEnum
{
    using UnderlyingType = UnderlyingTypeIn;
    [[nodiscard]] constexpr size_t size() const { return numEnums; }
    std::string_view string;
    std::array<MetaEnumMember<EnumType>, numEnums> members = {};
};

#include "detail/meta_enum_detail.h"

// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define ZBO_ENUM_CLASS(Type, UnderlyingType, ...) ZBO_ENUM_IMPL(Type, class, , UnderlyingType, __VA_ARGS__)
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define ZBO_NESTED_ENUM_CLASS(Type, UnderlyingType, ...) ZBO_ENUM_IMPL(Type, class, friend, UnderlyingType, __VA_ARGS__)
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define ZBO_ENUM(Type, UnderlyingType, ...) ZBO_ENUM_IMPL(Type, , , UnderlyingType, __VA_ARGS__)
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define ZBO_DECLARE_NESTED_ENUM(Type, UnderlyingType, ...) ZBO_ENUM_IMPL(Type, , friend, UnderlyingType, __VA_ARGS__)

/**
 * @brief This function returns the MetaEnum definition for a given enum type @see MetaEnum
 * @tparam E enum type
 */
template <class E>
const auto& metaEnum()
{
    return metaEnum(::zbo::meta_enum_internal::Tag<E>());
}

/**
 * @brief Function to easily transform an enum to a string that has been registered with ZBO_ENUM
 * @tparam E enum type (will be automatically deduced normally)
 * @return string representation of the passed enum value
 */
template <class E>
std::string_view enumToString(E e)
{
    return enumToString<E>(e, meta_enum_internal::Tag<E>());
}

/**
 * @brief Returns the index (in MetaEnum.members) of the given enum value
 * @tparam E enum type
 * @param e value of that enum
 */
template <class E>
size_t enumToIndex(E e)
{
    return enumToIndex<E>(e, meta_enum_internal::Tag<E>());
}

/**
 * @brief Function to easily transform a string to an enum that has been registered with ZBO_ENUM
 * @tparam E enum type to generate
 * @return enum value that corresponds to str, or empty if parsing fails
 */
template <class E>
std::optional<E> stringToEnum(std::string_view str)
{
    return stringToEnum<E>(str, meta_enum_internal::Tag<E>());
}

/**
 * @brief Helper class to convert a string to an enum or return the default value if it cannot be parsed corretly
 * @tparam E enum type to parse
 * @return enum value that corresponds to str, or the default_value if parsing fails
 */
template <class E>
E stringToEnum(std::string_view str, E defaultValue)
{
    return stringToEnum<E>(str).value_or(defaultValue);
}

}  // namespace zbo