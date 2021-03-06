// ////////////////////////////////////////////////////////////
// Graphics Library Classes Vulkan
// Copyright (c) 2018. All rights reserved.
// ////////////////////////////////////////////////////////////
#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

namespace glcv {
namespace util {

template <typename T>
void print_vector(const std::string &header, const std::vector<T> &vec)
{
    std::cout << header << '\n';

    if (vec.empty()) {
        std::cout << "\t<None>\n";
    }

    for (auto &val : vec) {
        std::cout << '\t' << val << '\n';
    }
    std::cout << std::endl;
}

template <typename T, typename Callback>
void print_vector(const std::string &header, const std::vector<T> &vec, const Callback &callback)
{
    std::cout << header << '\n';

    if (vec.empty()) {
        std::cout << "\t<None>\n";
    }

    for (auto &val : vec) {
        std::cout << '\t' << callback(val) << '\n';
    }
    std::cout << std::endl;
}

template <typename T>
bool has_value(const std::vector<T> &vec, const T &value)
{
    return std::find(vec.begin(), vec.end(), value) != vec.end();
}

} // namespace util
} // namespace glcv
