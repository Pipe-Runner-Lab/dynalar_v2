#pragma once

#include <fmt/core.h>

#include <glm/glm.hpp>
#include <string>

namespace logger {
void printMatrix(glm::mat4 matrix, std::string name = "");
};