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

#include <chrono>
#include <functional>

namespace zbo {

/**
 * @brief Class to measure time
 * @tparam Clock The clock implementation to use
 */
template <typename Clock>
class StopWatchT
{
  public:
    /// start (or reset the timer)
    void start() noexcept
    {
        running_ = true;
        startTime_ = Clock::now();
    }

    /// stops the timer and returns the elapsed time so far
    std::chrono::duration<double> stop() noexcept
    {
        endTime_ = Clock::now();
        running_ = false;
        return elapsed();
    }

    /// indicates whether the stopwatch is still running
    [[nodiscard]] bool isRunning() const noexcept { return running_; }

    /// returns the elapsed time since start up to now() or the last time stop() was called
    [[nodiscard]] std::chrono::duration<double> elapsed() const noexcept
    {
        return (running_ ? Clock::now() : endTime_) - startTime_;
    }

  private:
    bool running_ = false;
    typename Clock::time_point startTime_;
    typename Clock::time_point endTime_;
};

/// The default stopwatch using the std::chrono::steady_clock
using StopWatch = StopWatchT<std::chrono::steady_clock>;

/// Helper function to run a function and time its execution
template <typename Clock = std::chrono::steady_clock, typename Callable, typename... T>
std::chrono::duration<double> timeFunction(Callable&& c, T&&... args)
{
    StopWatchT<Clock> watch;
    watch.start();
    std::invoke(c, std::forward<T>(args)...);
    return watch.stop();
}

}  // namespace zbo
