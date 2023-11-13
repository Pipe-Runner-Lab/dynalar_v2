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
