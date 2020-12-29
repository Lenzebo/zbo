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

#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>

namespace zbo {

/**
 * @brief Template class to create a factory for a given interface
 * @tparam Interface The base class for all objects that shall be creatable with this factory
 * @tparam KeyT The type of key to be used to create types, defaults to a typedef within the Interface class
 */
template <typename Interface, typename KeyT = typename Interface::Key>
class Factory
{
  public:
    using Key = KeyT;

    static Factory& get()
    {
        static Factory instance;
        return instance;
    }

    using Allocator = std::function<std::unique_ptr<Interface>()>;

    /// register a type with a custom function to create a new object
    static void registerType(const Key& key, std::function<std::unique_ptr<Interface>()> allocator)
    {
        get().allocators_.insert({key, allocator});
    }

    /// register a type with a default constructor to create it
    template <typename Type>
    static void registerType(const Key& key)
    {
        Factory::registerType(key, []() { return std::make_unique<Type>(); });
    }

    /**
     * @brief Creates an instance of an object that is stored under the Key id
     * @param id Key to lookup
     * @return a valid object created by the allocator function
     * @throws std::invalid_argument if key was not registered
     */
    static std::unique_ptr<Interface> make(const Key& id)
    {
        auto alloc = get().allocators_.find(id);
        if (alloc == get().allocators_.end())
        {
            throw std::invalid_argument("invalid key");
        }
        return alloc->second();
    }

    /// Returns a vector of all stored keys in the factory
    static std::vector<Key> getAvailableKeys()
    {
        std::vector<Key> retval;
        retval.resize(get().allocators_.size());
        std::transform(get().allocators_.begin(), get().allocators_.end(), retval.begin(),
                       [](const auto& keyValue) { return keyValue.first; });
        return retval;
    }

  private:
    std::unordered_map<Key, Allocator> allocators_;
};

/**
 * @brief Helper struct to automatically register a type within the facotory
 * @note WARNING the registration might not work if its within a statically linked library,
 *       @see https://dzone.com/articles/factory-with-self-registering-types
 *       @see https://www.bfilipek.com/2018/02/static-vars-static-lib.html
 * @tparam Instance
 * @tparam Interface
 */
template <typename Instance, typename Interface>
struct RegisterInFactory
{
    template <typename T>
    RegisterInFactory(T key)
    {
        Factory<Interface>::get().template registerType<Instance>(key);
    }
};

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define ZBO_CONCATENATE_DETAIL(x, y) x##y
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define ZBO_CONCATENATE(x, y) ZBO_CONCATENATE_DETAIL(x, y)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define ZBO_REGISTER_IN_FACTORY(className, interfaceName, key) \
    ::zbo::RegisterInFactory<className, interfaceName> ZBO_CONCATENATE(instance, __LINE__){key};

}  // namespace zbo