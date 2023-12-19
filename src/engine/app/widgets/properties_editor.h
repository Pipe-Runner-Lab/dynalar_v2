#pragma once

#include <imgui.h>

#include <functional>

class BasePropertiesEditor {
    virtual void Render(){};
};

class CameraPropertiesEditor : public BasePropertiesEditor {
private:
    std::function<void()> m_renderCb;

public:
    CameraPropertiesEditor(std::function<void()> renderCb);
    void Render();
};

class LightPropertiesEditor : public BasePropertiesEditor {};

class ObjectPropertiesEditor : public BasePropertiesEditor {
private:
    // TODO: Having issues with reference here
    std::function<void()> m_renderCb;
    std::function<void()> m_additionalRenderCb = []() {};

public:
    ObjectPropertiesEditor(std::function<void()> renderCb);
    void SetAdditionalRenderCallback(std::function<void()> renderCb);
    void Render();
};

class InputPropertiesEditor : public BasePropertiesEditor {
private:
    std::function<void()> m_renderCb;

public:
    InputPropertiesEditor(std::function<void()> renderCb);
    void Render();
};