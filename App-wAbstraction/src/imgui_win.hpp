#ifndef IMGUIWIN_HPP
#define IMGUIWIN_HPP

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace ImGuiWin
{
    const int MAX_SEGMENTS = 60;
    const int MIN_SEGMENTS = 3;

    ImVec4 fondo = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 color_figura = ImVec4(0.2f, 0.85f, 0.2f, 1.00f);

    bool show_wire = false;
    bool show_smooth = false;
    bool show_fill = false;
    bool show_normals = false;

    bool draw_cube = false;
    bool draw_cylinder = false;
    bool draw_cone = false;
    bool draw_sphere = false;
    bool draw_special = false;

    int segments_x = 10;
    int segments_y = 10;
    bool segments_event_listener = false;

    void renderMainWindow()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Computer Graphics Project - UNMSM - 2020");
                /* COLOR EDIT */
            ImGui::TextColored(ImVec4(1,1,0,1), "Color");
            ImGui::Indent();
                ImGui::ColorEdit3("Fondo", (float *)&fondo);
                ImGui::ColorEdit3("Shape", (float *)&color_figura);
            ImGui::Unindent();

                /* SHAPES */
            ImGui::TextColored(ImVec4(1,1,0,1), "Shapes");
            ImGui::Indent();
                ImGui::Checkbox("Cube", &draw_cube);
                ImGui::Checkbox("Cylinder", &draw_cylinder);
                ImGui::Checkbox("Cone", &draw_cone);
                ImGui::Checkbox("Sphere", &draw_sphere);
                ImGui::Checkbox("Special", &draw_special);
            ImGui::Unindent();

                /* MODOS */
            ImGui::TextColored(ImVec4(1,1,0,1), "Render");
            ImGui::Indent();
                ImGui::Checkbox("Wire", &show_wire);
                ImGui::Checkbox("Smooth", &show_smooth);
                ImGui::Checkbox("Fill", &show_fill);
                ImGui::Checkbox("Normals", &show_normals);
            ImGui::Unindent();

                /* SEGMENTS */
            ImGui::TextColored(ImVec4(1,1,0,1), "Segments");
            ImGui::Indent();
                if (ImGui::SliderInt("X", &segments_x, MIN_SEGMENTS, MAX_SEGMENTS)) { segments_event_listener = true; }
                if (ImGui::SliderInt("Y", &segments_y, MIN_SEGMENTS, MAX_SEGMENTS)) { segments_event_listener = true; }
            ImGui::Unindent();

        ImGui::End();
    }

    void endMainWindow()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void cleanAll()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}

#endif
