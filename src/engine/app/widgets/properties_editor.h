#pragma once

#include <functional>
#include <imgui.h>

class BasePropertiesEditor
{
  virtual void Render(){};
};

class CameraPropertiesEditor : public BasePropertiesEditor
{
  CameraPropertiesEditor();
};

class LightPropertiesEditor : public BasePropertiesEditor
{
};

class ObjectPropertiesEditor : public BasePropertiesEditor
{
private:
  // TODO: Having issues with reference here
  std::function<void()> m_renderCb;

public:
  ObjectPropertiesEditor(std::function<void()> renderCb);
  void Render();
};