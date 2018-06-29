#pragma once

#include <memory>

namespace glcv {

namespace detail {

class GLCV;

} // namespace detail

using GLCV = std::shared_ptr<detail::GLCV>;

} // namespace glcv
