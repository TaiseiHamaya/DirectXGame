#pragma once

#include <concepts>

template<typename T>
concept StructuredBufferType =
std::is_same_v<T, std::remove_cvref_t<T>> && // CV修飾、Reference修飾されていない
// クラス、算術型
(std::is_class_v<T> || std::is_arithmetic_v<T>);