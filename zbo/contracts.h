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

#if __has_cpp_attribute(unlikely)
#define ZBO_UNLIKELY [[unlikely]]
#else
#define ZBO_UNLIKELY
#endif

// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define ZBO_CONTRACT_CHECK(type, condition) \
    if (!(condition)) ZBO_UNLIKELY          \
        {                                   \
            std::terminate();               \
        }

#define ZBO_PRECONDITION(condition) ZBO_CONTRACT_CHECK("Pre", condition)    // NOLINT (cppcoreguidelines-macro-usage)
#define ZBO_POSTCONDITION(condition) ZBO_CONTRACT_CHECK("Post", condition)  // NOLINT (cppcoreguidelines-macro-usage)
#define ZBO_EXPECT(condition) ZBO_CONTRACT_CHECK("Check", condition)        // NOLINT (cppcoreguidelines-macro-usage)
#define ZBO_UNREACHABLE() ZBO_CONTRACT_CHECK("Unreachable", false)          // NOLINT (cppcoreguidelines-macro-usage)

// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define ZBO_ASSERT_EXEC(condition, command)          \
    if (!(condition)) ZBO_UNLIKELY                   \
        {                                            \
            command;                                 \
            ZBO_CONTRACT_CHECK("Assert", condition); \
        }
