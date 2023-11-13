#include "logger.h"

void logger::printMatrix(glm::mat4 matrix, std::string name) {
    fmt::print(name == "" ? "Matrix:\n" : "{}:\n", name);
    for (int i = 0; i < 4; i++) {
        fmt::print("[");
        for (int j = 0; j < 4; j++) {
            fmt::print("{:6.2f} ", matrix[i][j]);
        }
        fmt::print("]\n");
    }
}
