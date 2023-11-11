#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <fmt/core.h>
#include "../core/gl/model.h"
#include "../core/gl/mesh.h"
#include "../core/gl/shader.h"
#include "../core/gl/camera.h"
#include "../core/gl/vertex_array.h"
#include "../core/gl/vertex_buffer.h"
#include "../core/gl/vertex_buffer_layout.h"
#include "../core/gl/texture.h"
#include "../window/window_manager.h"
#include "../app/widgets/properties_editor.h"

struct RenderContext
{
  std::shared_ptr<WindowManager> windowManagerPtr;
  std::shared_ptr<Renderer> rendererPtr;
};

class BaseScene
{
protected:
  RenderContext &m_renderContext;

private:
  std::vector<Camera> m_cameras;
  int m_activeCameraIndex = 0;
  float m_xSensitivity = 0.1f;
  float m_ySensitivity = 0.1f;

public:
  BaseScene(RenderContext &renderContext) : m_renderContext(renderContext){};

  virtual void OnUpdate(){};
  virtual void OnRender(){};
  virtual void OnGUIRender(){};

protected:
  void OnGUIRender(
      std::shared_ptr<ObjectPropertiesEditor> objectPropertiesEditorPtr)
  {
    if (ImGui::BeginTabBar("Editor"))
    {
      objectPropertiesEditorPtr->Render();

      if (m_cameras.size() > 0 && ImGui::BeginTabItem("Camera"))
      {
        if (ImGui::BeginCombo("Active Camera", fmt::format("Camera {}", m_activeCameraIndex).c_str()))
        {
          for (int itemIdx = 0; itemIdx < m_cameras.size(); itemIdx++)
          {
            const bool is_selected = (m_activeCameraIndex == itemIdx);
            if (ImGui::Selectable(fmt::format("Camera {}", itemIdx).c_str(), is_selected))
              m_activeCameraIndex = itemIdx;

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
              ImGui::SetItemDefaultFocus();
          }
          ImGui::EndCombo();
        }

        ImGui::SeparatorText("Camera Properties");
        ImGui::EndTabItem();
      }

      if (ImGui::BeginTabItem("Input"))
      {
        ImGui::SeparatorText("Mouse");
        ImGui::SliderFloat("X Sensitivity", &m_xSensitivity, 0.01f, 10.0f);
        ImGui::SliderFloat("Y Sensitivity", &m_ySensitivity, 0.01f, 10.0f);
        ImGui::EndTabItem();
      }

      ImGui::EndTabBar();
    }
  };

protected:
  void AddCamera(Camera camera)
  {
    m_cameras.push_back(std::move(camera));
  }

  Camera &GetActiveCamera()
  {
    return m_cameras[m_activeCameraIndex];
  }

  void SetActiveCameraIndex(int index)
  {
    if (index < m_cameras.size())
    {
      m_activeCameraIndex = index;
    }
  }
};
