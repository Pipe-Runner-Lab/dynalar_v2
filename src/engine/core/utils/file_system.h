#pragma once

#include <sys/stat.h>

#include <filesystem>
#include <stdexcept>
#include <string>

namespace file_system {
/// @brief Checks if a folder exists, else creates it
/// @param path File path to check
void create_directory(const std::string& path);
}  // namespace file_system