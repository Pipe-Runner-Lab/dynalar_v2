#include "ambient_light.h"
#include "base_light.h"
#include "directional_light.h"
#include "point_light.h"
#include "spot_light.h"

struct LightsManager {
    unsigned int lightCounts[4] = {0, 0, 0, 0};
    std::vector<int> lightVsShadowMapIndices;
    std::vector<std::unique_ptr<BaseLight>> lightPtrs;
    int directionalShadowMapCount = 0;

    void GenerateShadowMaps(Renderer &renderer, WindowManager &window_manager,
                            std::vector<std::unique_ptr<Model>> &modelPtrs, Shader &shader);

    void ActivateShadowMaps(Shader &shader);

    void IncreaseLightCount(LightType type);

    void Bind(Shader &shader);

    void Unbind(Shader &shader);
};