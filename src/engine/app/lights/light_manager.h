#include "ambient_light.h"
#include "base_light.h"
#include "directional_light.h"
#include "point_light.h"
#include "spot_light.h"

struct LightsManager {
    std::vector<std::unique_ptr<BaseLight>> lightPtrs;
    int reservedTextureSlotCount = 0;

private:
    std::vector<int> lightVsShadowMapIndices;
    unsigned int lightCounts[4] = {0, 0, 0, 0};
    int directionalShadowMapCount = 0;
    int OmniDirectionalShadowMapCount = 0;

public:
    void GenerateShadowMaps(Renderer &renderer, WindowManager &window_manager,
                            std::vector<std::unique_ptr<Model>> &modelPtrs,
                            Shader &directionalShadowShader, Shader &omniDirectionalShadowShader);

    void ActivateShadowMaps(Shader &shader);

    void IncreaseLightCount(LightType type);

    void Bind(Shader &shader);

    void Unbind(Shader &shader);
};