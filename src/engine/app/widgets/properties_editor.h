#pragma once

#include <imgui.h>

#include <functional>

class BasePropertiesEditor {
protected:
    std::function<void()> m_renderCb;

public:
    BasePropertiesEditor(std::function<void()> renderCb)
        : m_renderCb(renderCb) {
    }
    virtual void Render() = 0;
};

class CameraPropertiesEditor : public BasePropertiesEditor {
public:
    CameraPropertiesEditor(std::function<void()> renderCb);
    void Render();
};

class LightPropertiesEditor : public BasePropertiesEditor {
public:
    LightPropertiesEditor(std::function<void()> renderCb);
    void Render();
};

class ObjectPropertiesEditor : public BasePropertiesEditor {
private:
    // TODO: Having issues with reference here
    std::function<void()> m_additionalRenderCb = []() {};

public:
    ObjectPropertiesEditor(std::function<void()> renderCb);
    void SetAdditionalRenderCallback(std::function<void()> renderCb);
    void Render();
};

class InputPropertiesEditor : public BasePropertiesEditor {
public:
    InputPropertiesEditor(std::function<void()> renderCb);
    void Render();
};