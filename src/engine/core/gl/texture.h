#include <stb_image.h>
#include <string>
#include <GL/glew.h>
#include <fmt/core.h>
#include "../utils/error.h"

class Texture
{
private:
  GLuint m_textureID;
  std::string m_filePath;
  unsigned char *m_localBuffer;
  int m_width, m_height, m_bpp; // BPP => Bits Per Pixel

public:
  Texture(const std::string &filePath, bool hasAlpha = true);
  ~Texture();

  void Bind(GLuint slot = 0) const;
  void Unbind() const;

  inline int GetHeight() const { return m_height; }
  inline int GetWidth() const { return m_width; }

  void LoadDefaultTexture();
};