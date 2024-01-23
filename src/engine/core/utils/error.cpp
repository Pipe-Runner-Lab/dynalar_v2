#include "error.h"

void ClearAllGLErrors() {
    // keep looping error buffer until it's empty
    while (glGetError() != GL_NO_ERROR)
        ;
}

bool CheckOpenGLError(const char *statement, const char *file, int line) {
    while (GLenum error = glGetError()) {
        fmt::print(stderr, "[OpenGL Error] {}\n{}\n{}\n{}\n", error, statement, file, line);
        throw std::runtime_error("OpenGL Error");
        return false;
    }
    return true;
}
