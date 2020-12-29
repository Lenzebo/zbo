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

namespace meta_enum_internal {

template <class T>
struct Tag
{
};

constexpr bool isNested(size_t brackets, bool quote)
{
    return brackets != 0 || quote;
}

constexpr size_t nextEnumCommaOrEnd(size_t start, std::string_view enumString)
{
    size_t brackets = 0;  //()[]{}
    bool quote = false;   //""
    char lastChar = '\0';
    char nextChar = '\0';

    auto feedCounters = [&brackets, &quote, &lastChar, &nextChar](char c) {
        if (quote)
        {
            if (lastChar != '\\' && c == '"')  // ignore " if they are backslashed
                quote = false;
            return;
        }

        switch (c)
        {
            case '"':
                if (lastChar != '\\')  // ignore " if they are backslashed
                    quote = true;
                break;
            case '(':
            case '<':
                if (lastChar == '<' || nextChar == '<') break;
                [[fallthrough]];
            case '{':
                ++brackets;
                break;
            case ')':
            case '>':
                if (lastChar == '>' || nextChar == '>') break;
                [[fallthrough]];
            case '}':
                --brackets;
                break;
            default:
                break;
        }
    };

    size_t current = start;
    for (; current < enumString.size() && (isNested(brackets, quote) || (enumString[current] != ',')); ++current)
    {
        feedCounters(enumString[current]);
        lastChar = enumString[current];
        nextChar = current + 2 < enumString.size() ? enumString[current + 2] : '\0';
    }

    return current;
}

constexpr bool isAllowedIdentifierChar(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
}

constexpr std::string_view parseEnumMemberName(std::string_view memberString)
{
    size_t nameStart = 0;
    while (!isAllowedIdentifierChar(memberString[nameStart]))
    {
        ++nameStart;
    }

    size_t nameSize = 0;

    while ((nameStart + nameSize) < memberString.size() && isAllowedIdentifierChar(memberString[nameStart + nameSize]))
    {
        ++nameSize;
    }

    return std::string_view(memberString.data() + nameStart, nameSize);
}

template <typename EnumType, typename UnderlyingType, size_t size>
constexpr MetaEnum<EnumType, UnderlyingType, size> parseMetaEnum(std::string_view in,
                                                                 const std::array<EnumType, size>& values)
{
    MetaEnum<EnumType, UnderlyingType, size> result;
    result.string = in;

    std::array<std::string_view, size> memberStrings;
    size_t amountFilled = 0;

    size_t currentStringStart = 0;

    while (amountFilled < size)
    {
        size_t currentStringEnd = nextEnumCommaOrEnd(currentStringStart + 1, in);
        size_t currentStringSize = currentStringEnd - currentStringStart;

        if (currentStringStart != 0)
        {
            ++currentStringStart;
            --currentStringSize;
        }

        memberStrings.at(amountFilled) = std::string_view(in.data() + currentStringStart, currentStringSize);
        ++amountFilled;
        currentStringStart = currentStringEnd;
    }

    for (size_t i = 0; i < memberStrings.size(); ++i)
    {
        auto& member = result.members.at(i);
        member.name = parseEnumMemberName(memberStrings.at(i));
        member.string = memberStrings.at(i);
        member.value = values.at(i);
        member.index = i;
    }

    return result;
}

template <typename EnumUnderlyingType>
struct IntWrapper
{
    constexpr IntWrapper() : value(0) {}
    constexpr IntWrapper(EnumUnderlyingType in) : value(in), empty(false) {}
    constexpr IntWrapper& operator=(EnumUnderlyingType in)
    {
        value = in;
        empty = false;
        return *this;
    }
    EnumUnderlyingType value;
    bool empty{true};
};

template <typename EnumType, typename EnumUnderlyingType, size_t size>
constexpr std::array<EnumType, size> resolveEnumValuesArray(
    const std::initializer_list<IntWrapper<EnumUnderlyingType>>& in)
{
    std::array<EnumType, size> result{};

    EnumUnderlyingType nextValue = 0;
    for (size_t i = 0; i < size; ++i)
    {
        auto wrapper = *std::next(in.begin(), i);
        EnumUnderlyingType newValue = wrapper.empty ? nextValue : wrapper.value;
        nextValue = newValue + 1;
        result.at(i) = static_cast<EnumType>(newValue);
    }

    return result;
}

}  // namespace meta_enum_internal

// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define ZBO_ENUM_IMPL(Type, Class, Friend, UnderlyingType, ...)                                                  \
    enum Class Type : UnderlyingType                                                                             \
    {                                                                                                            \
        __VA_ARGS__                                                                                              \
    };                                                                                                           \
    constexpr static auto Type##_internal_size = []() constexpr                                                  \
    {                                                                                                            \
        using IntWrapperType = ::zbo::meta_enum_internal::IntWrapper<UnderlyingType>;                            \
        IntWrapperType __VA_ARGS__;                                                                              \
        return std::initializer_list<IntWrapperType>{__VA_ARGS__}.size();                                        \
    };                                                                                                           \
    inline Friend const auto& metaEnum(::zbo::meta_enum_internal::Tag<Type>)                                     \
    {                                                                                                            \
        static auto m = ::zbo::meta_enum_internal::parseMetaEnum<Type, UnderlyingType, Type##_internal_size()>(  \
            #__VA_ARGS__, []() {                                                                                 \
                using IntWrapperType = ::zbo::meta_enum_internal::IntWrapper<UnderlyingType>;                    \
                IntWrapperType __VA_ARGS__;                                                                      \
                return ::zbo::meta_enum_internal::resolveEnumValuesArray<Type, UnderlyingType,                   \
                                                                         Type##_internal_size()>({__VA_ARGS__}); \
            }());                                                                                                \
        return m;                                                                                                \
    }                                                                                                            \
    template <class E>                                                                                           \
    Friend std::string_view enumToString(E e, ::zbo::meta_enum_internal::Tag<Type> tag)                          \
    {                                                                                                            \
        const auto& m = metaEnum(tag);                                                                           \
        for (const auto& member : m.members)                                                                     \
        {                                                                                                        \
            if (member.value == e)                                                                               \
            {                                                                                                    \
                return member.name;                                                                              \
            }                                                                                                    \
        }                                                                                                        \
        return std::string_view("__INVALID_ENUM_VAL__");                                                         \
    }                                                                                                            \
    template <class E>                                                                                           \
    Friend size_t enumToIndex(E e, ::zbo::meta_enum_internal::Tag<Type> tag)                                     \
    {                                                                                                            \
        const auto& m = metaEnum(tag);                                                                           \
        for (size_t i = 0; i < m.members.size(); ++i)                                                            \
        {                                                                                                        \
            if (m.members.at(i).value == e)                                                                      \
            {                                                                                                    \
                return i;                                                                                        \
            }                                                                                                    \
        }                                                                                                        \
        return m.members.size();                                                                                 \
    }                                                                                                            \
    template <class E>                                                                                           \
    Friend std::optional<Type> stringToEnum(std::string_view str, ::zbo::meta_enum_internal::Tag<Type> tag)      \
    {                                                                                                            \
        const auto& m = metaEnum(tag);                                                                           \
        for (const auto& member : m.members)                                                                     \
        {                                                                                                        \
            if (member.name == str)                                                                              \
            {                                                                                                    \
                return member.value;                                                                             \
            }                                                                                                    \
        }                                                                                                        \
        return std::nullopt;                                                                                     \
    }
