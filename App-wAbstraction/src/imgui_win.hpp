#ifndef IMGUIWIN_HPP
#define IMGUIWIN_HPP

#include "..\imgui\imgui.h"
#include "..\imgui\imgui_impl_glfw.h"
#include "..\imgui\imgui_impl_opengl3.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>


namespace ImGuiWin
{

    ImVec4 fondo = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 color_figura = ImVec4(217.0f / 255.0f, 199.0f / 255.0f, 10.0f / 255.0f, 1.00f);
    bool show_wire = false;
    bool show_smooth = false;
    bool show_fill = false;
    bool show_normals = false;
    int segments_x = 10;
    int segments_y = 10;
    bool segments_event_listener = false;

    void renderMainWindow()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Computer Graphics Project - UNMSM - 2020");        // CREAMOS LA VENTANA ESTABLECIENDO UN TITULO
        ImGui::ColorEdit3("Color de fondo", (float *)&fondo);         // COLOR EDIT PARA CAMBIAR EL COLOR DE FONDO
        ImGui::ColorEdit3("Color de figura", (float *)&color_figura); // COLOR EDIT PARA CAMBIAR EL COLOR DE SU FIGURA
            /* MODOS */
        ImGui::Checkbox("Wire", &show_wire);
        ImGui::Checkbox("Smooth", &show_smooth);
        ImGui::Checkbox("Fill", &show_fill);
        ImGui::Checkbox("Normals", &show_normals);
            /* SEGMENTS */
        if (ImGui::SliderInt("Segments X", &segments_x, 3, 30)) { segments_event_listener = true; }
        if (ImGui::SliderInt("Segments Y", &segments_y, 3, 30)) { segments_event_listener = true; }

        ImGui::End();                                                 // FINALIZA LA VENTANA
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
