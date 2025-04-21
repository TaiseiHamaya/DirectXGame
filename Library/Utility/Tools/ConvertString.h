#pragma once

#include <string>

std::wstring ConvertString(const std::string_view& str);

std::string ConvertString(const std::wstring_view& str);
