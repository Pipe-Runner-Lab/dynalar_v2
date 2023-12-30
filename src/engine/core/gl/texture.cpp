#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION

Texture::Texture(const std::string &filePath, bool hasAlpha)
    : Texture(filePath, TextureType::DIFFUSE, hasAlpha) {
}

// TODO: Remove hasAlpha parameter and refer the following code
// https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/model.h
Texture::Texture(const std::string &filePath, TextureType type, bool hasAlpha)
    : type(type) {
    GL_CALL(glGenTextures(1, &m_textureID));
    // Bind(0, false);
    GL_CALL(glBindTexture(GL_TEXTURE_2D, m_textureID))

    GL_CALL(glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_LINEAR));  // setting minification filter to linear (image larger)
    GL_CALL(glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
        GL_LINEAR));  // setting magnification filter to linear (image smaller)
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                            GL_CLAMP_TO_EDGE));  // s -> horizontal
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                            GL_CLAMP_TO_EDGE));  // t -> vertical

    // read data and store it in local buffer
    stbi_set_flip_vertically_on_load(true);
    m_localBuffer = stbi_load(filePath.c_str(), &m_width, &m_height, &m_bpp,
                              4);  // desired channel 4 since RGBA
    if (m_localBuffer) {
        if (hasAlpha) {
            // sending texture data to GPU
            GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height,
                                 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
        } else {
            // sending texture data to GPU
            GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0,
                                 GL_RGB, GL_UNSIGNED_BYTE, m_localBuffer));
        }

        // generating mipmap
        GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
    } else {
        fmt::print(stderr, "Failed to load texture: {}\n", filePath);
        LoadDefaultTexture();
    }

    Unbind();

    // if localBuffer holds data, we free it here
    if (m_localBuffer) {
        stbi_image_free(m_localBuffer);
    }
}

Texture::~Texture() {
    GL_CALL(glDeleteTextures(1, &m_textureID));
}

void Texture::Bind(GLuint slot) const {
    GL_CALL(glActiveTexture(
        GL_TEXTURE0 +
        slot));  // since GL_TEXTURE<number> are consecutive we can add directly
    GL_CALL(glBindTexture(GL_TEXTURE_2D,
                          m_textureID));  // other types of textures also exist
}

void Texture::Unbind() const {
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::LoadDefaultTexture() {
    m_localBuffer =
        stbi_load("assets/textures/default.png", &m_width, &m_height, &m_bpp,
                  4);  // desired channel 4 since RGBA
    if (m_localBuffer) {
        // sending texture data to GPU
        GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0,
                             GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));

        // generating mipmap
        GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
    } else {
        throw "Failed to load texture default";
    }
}
