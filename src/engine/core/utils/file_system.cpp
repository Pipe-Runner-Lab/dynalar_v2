#include "file_system.h"

namespace fs = std::filesystem;

void file_system::create_directory(const std::string& path) {
    if (fs::is_directory(fs::file_status(fs::status(path)))) {
        return;
    }

    // https://en.cppreference.com/w/cpp/filesystem/create_directory
    if (!fs::create_directory(path)) {
        throw std::runtime_error("Could not create directory: " + path);
    }
}