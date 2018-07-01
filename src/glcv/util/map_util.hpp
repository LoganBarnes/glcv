// ////////////////////////////////////////////////////////////
// Created on 7/1/18.
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#pragma once

#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>

namespace util {

template <typename MapOrSet, typename Func>
void erase_if(MapOrSet &mos, const Func erase_func)
{
    for (auto iter = mos.cbegin(); iter != mos.cend(); /* no increment */) {
        if (erase_func(*iter)) {
            iter = mos.erase(iter);
        } else {
            ++iter;
        }
    }
}

template <typename MapOrSet, typename T>
bool has_key(const MapOrSet &mos, const T &key)
{
    return mos.find(key) != mos.end();
}

} // namespace util
