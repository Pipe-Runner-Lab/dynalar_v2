#pragma once

#include <GL/glew.h>
#include <fmt/color.h>
#include <fmt/core.h>

#ifdef _DEBUG
#define GL_CALL(statement) \
  ClearAllGLErrors();      \
  statement;               \
  CheckOpenGLError(#statement, __FILE__, __LINE__);
#else
#define GL_CALL(statement) statement
#endif

/// @brief Clears all OpenGL errors before calling any OpenGL function.
void ClearAllGLErrors();

/// @brief
/// @param statement
/// @param file
/// @param line
/// @return
bool CheckOpenGLError(const char *statement, const char *file, int line);