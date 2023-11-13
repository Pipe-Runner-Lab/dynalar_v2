#pragma once

#include <imgui.h>

#include <functional>
#include <string>
#include <vector>

#include "base_scene.h"

class SceneManager {
private:
    RenderContext &m_renderContext;
    std::vector<std::pair<std::string, std::function<BaseScene *()>>> m_scenes;
    BaseScene *m_activeScenePtr;

public:
    SceneManager(RenderContext &renderContext);

    template <typename T>
    void RegisterScene(const std::string &sceneName) {
        fmt::print("Registering scene: {}\n", sceneName);
        m_scenes.push_back(std::make_pair(
            sceneName, [&]() { return new T(m_renderContext); }));
    }

    void RenderSceneList();

    BaseScene *GetActiveScenePtr() const {
        return m_activeScenePtr;
    }

    void DeleteActiveScenePtr() {
        delete m_activeScenePtr;
        m_activeScenePtr = nullptr;
    }
};