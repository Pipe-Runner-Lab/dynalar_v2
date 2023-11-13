#include "gui.h"

GUI::GUI(WindowManager &windowManager) {
    /* Initialize ImGui */
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(windowManager.GetWindowPtr(), true);
    ImGui_ImplOpenGL3_Init("#version 440");

    /* Set GUI scale -
     * https://github.com/ocornut/imgui/issues/2442#issuecomment-487364993 */
    ImGui::GetStyle().ScaleAllSizes(m_guiScale);
    ImGui::GetIO().FontGlobalScale = m_guiScale;

    /* Set GUI font */
    io.Fonts->AddFontDefault();
    float baseFontSize = 13.0f;
    float iconFontSize = baseFontSize * 2.0f / 3.0f;
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.GlyphMinAdvanceX = iconFontSize;
    io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, iconFontSize,
                                 &icons_config, icons_ranges);
}

GUI::~GUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::CreateFrame() {
    /* Create IMGUI frame */
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GUI::RenderFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
