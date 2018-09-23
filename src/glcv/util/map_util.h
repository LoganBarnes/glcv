// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#pragma once

#include <unordered_set>
#include <unordered_map>

namespace util {

template <typename Map, typename T>
bool has_key(const Map &map, const T &value)
{
    return map.find(value) != map.end();
}

} // namespace util
