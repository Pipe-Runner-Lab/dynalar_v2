#include <GL/glew.h>
#include <fmt/core.h>

#include "../../window/window_manager.h"
#include "../utils/error.h"
#include "renderer.h"

class ShadowMap {
private:
    unsigned int m_depthMapFBO;
    unsigned int m_depthMap;

    int m_width;
    int m_height;

public:
    ShadowMap(int width = 1024, int height = 1024);
    ~ShadowMap();

    void Bind() const;
    void Unbind() const;

    void Draw(Renderer& renderer, WindowManager& window_manager) const;
};