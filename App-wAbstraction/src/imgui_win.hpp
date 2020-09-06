#ifndef IMGUIWIN_HPP
#define IMGUIWIN_HPP

#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

namespace ImGuiWin
{

    ImVec4 fondo = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 color_figura = ImVec4(217.0f / 255.0f, 199.0f / 255.0f, 10.0f / 255.0f, 1.00f);

    void renderMainWindow()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Computer Graphics Project - UNMSM - 2020");        // CREAMOS LA VENTANA ESTABLECIENDO UN TITULO
        ImGui::ColorEdit3("Color de fondo", (float *)&fondo);         // COLOR EDIT PARA CAMBIAR EL COLOR DE FONDO
        ImGui::ColorEdit3("Color de figura", (float *)&color_figura); // COLOR EDIT PARA CAMBIAR EL COLOR DE SU FIGURA
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