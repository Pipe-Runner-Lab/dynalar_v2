#pragma once

#include <GL/glew.h>
#include <fmt/core.h>
#include <stb_image.h>

#include <string>

#include "../utils/error.h"

enum class TextureType { DIFFUSE, SPECULAR, NORMAL, HEIGHT };

/**
 * Note: Copying a texture is not allowed. This is because texture automatically
 * uploads data to the GPU
 */
class Texture {
private:
    GLuint m_textureID;
    TextureType m_type = TextureType::DIFFUSE;
    std::string m_filePath;
    unsigned char *m_localBuffer;
    int m_width, m_height, m_bpp;  // BPP => Bits Per Pixel

public:
    Texture(const std::string &filePath, bool hasAlpha = true);
    Texture(const std::string &filePath, TextureType type,
            bool hasAlpha = true);

    Texture(const Texture &other) = delete;

    ~Texture();

    void Bind(GLuint slot = 0) const;
    void Unbind() const;

    inline int GetHeight() const {
        return m_height;
    }
    inline int GetWidth() const {
        return m_width;
    }

    void LoadDefaultTexture();
};