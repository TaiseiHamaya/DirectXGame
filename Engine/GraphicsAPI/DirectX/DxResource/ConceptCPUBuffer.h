#pragma once

#include <type_traits>

template<typename T>
concept ConceptCPUBufferACE =
std::is_same_v<T, std::remove_cvref_t<T>> && // CV修飾、Reference修飾されていない
(std::is_arithmetic_v<T> || std::is_class_v<T> || std::is_enum_v<T>); // 算術型、クラス型、列挙型のいずれか

template<typename T>
concept ConceptCPUBufferAC =
std::is_same_v<T, std::remove_cvref_t<T>> && // CV修飾、Reference修飾されていない
(std::is_arithmetic_v<T> || std::is_class_v<T>); // 算術型、クラス型のいずれか
