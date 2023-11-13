#include <vector>

#include "../../engine/app/base_scene.h"
#include "../../engine/app/pre_made_objects/plane.h"
#include "../../engine/app/widgets/properties_editor.h"

class Hello3DWorldScene : public BaseScene {
private:
    Shader m_shader;
    std::unique_ptr<Model> m_modelPtr;

    std::shared_ptr<ObjectPropertiesEditor> m_objectPropertiesEditorPtr;

public:
    Hello3DWorldScene(RenderContext &renderContext);
    void OnRender() override;
    void OnGUIRender() override;
};