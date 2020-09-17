#ifndef IMGUIWIN_HPP
#define IMGUIWIN_HPP

#include <Windows.h>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

namespace ImGuiWin
{
    // enum SHAPE {CUBE = 0, CYLINDER = 1, CONE = 2, SPHERE = 3, SPECIAL = 4};
    // void optFigure(std::string title, SHAPE shp);

    const int MAX_SEGMENTS = 60;
    const int MIN_SEGMENTS = 3;

    ImVec4 fondo = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 color_figura = ImVec4(0.2f, 0.85f, 0.2f, 1.00f);

    void optShaders();
    // boolean shader_opt_listener = false;
    int shader_opt = 0;
    // std::string vertexFile = "vertShader.glsl";
    // std::string fragmentFile = "fragShader.glsl";
    
    bool show_wire = false;
    bool show_smooth = false;
    bool show_fill = false;
    bool show_normals = false;
    
    bool draw_cube = false; 
    bool draw_cylinder = false; 
    bool draw_cone = false; 
    bool draw_sphere = false; 
    bool draw_special = false; 
    
    /* SEGMENTS */
    int segments_x = 10;
    int segments_y = 10;
    bool segments_event_listener = false;

    /* PPOPIEDADES CILINDRO */
    bool cyl_prop_listener = false;
    float cyl_base_rad = 0.1;
    float cyl_top_rad = 0.1;
    float cyl_heigth = 0.15;
    void editPropCylinder();

    /* PPOPIEDADES CONO */
    bool con_prop_listener = false;
    float con_top_rad = 0.1;
    float con_heigth = 0.15;
    void editPropCone();

    /* PPOPIEDADES SPHERE */
    bool sph_prop_listener = false;
    float sph_rad = 0.1;
    void editPropSphere();

    /* PPOPIEDADES TORUS */
    bool tor_prop_listener = false;
    float tor_R = 0.2;
    float tor_r = 0.05;
    void editPropTorus();

    void startup(LPCSTR lpApplicationName);

    float varRad[5];

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

            ImGui::TextColored(ImVec4(1,1,0,1), "Run extra programs!");
            ImGui::Indent();
            ImGui::Indent();
                if (ImGui::Button("MANDELBROT SET")){
                    startup("mandel.exe");
                }
                if (ImGui::Button("SHARINGAN")){
                    startup("sharingan.exe");
                }
                if (ImGui::Button("BART SIMPSON")){
                    startup("bart.exe");
                }
            ImGui::Unindent();
            ImGui::Unindent();
            
            /*INDICACIONES*/
            ImGui::TextColored(ImVec4(1,0.5,0,1), "INDICACIONES: ");
            ImGui::Indent();
                ImGui::TextColored(ImVec4(1,0.5,0,1), "_USE LAS TECLAS A-W-S-D: ");
                ImGui::TextColored(ImVec4(1,0.5,0,1), "PARA MOVERSE A LA IZQUIERDA,ARRIBA,");
                ImGui::TextColored(ImVec4(1,0.5,0,1), "ABAJO Y ARRIBA RESPECTIVAMENTE ");
                ImGui::TextColored(ImVec4(1,0.5,0,1), "_RUEDA DEL MOUSE: ");
                ImGui::TextColored(ImVec4(1,0.5,0,1), "ALEJARSE Y ACERCARSE(ZOOM) ");
                ImGui::TextColored(ImVec4(1,0.5,0,1), "_CLICK DERECHO: ");
                ImGui::TextColored(ImVec4(1,0.5,0,1), "PARA ROTAR ");
            ImGui::Unindent();

        ImGui::End();

        // if (draw_cube) optFigure("cube", SHAPE::CUBE);
        if (draw_cylinder) editPropCylinder();
        if (draw_cone) editPropCone();
        if (draw_sphere) editPropSphere();
        if (draw_special) editPropTorus();

        if (show_smooth) optShaders();

    }

    void optShaders(){
        ImGui::Begin("Shaders");
        ImGui::RadioButton("Ordinario", &shader_opt, 0);
        ImGui::RadioButton("BlinnPhongShaders", &shader_opt, 1);
        ImGui::RadioButton("GouraudShaders", &shader_opt, 2);
        ImGui::RadioButton("PhongShaders", &shader_opt, 3);
        ImGui::End();
    }

    // muestra opciones adicionales para el renderizado de la figura
    // void optFigure(std::string title, SHAPE shp){
    //     float *varRadio;
    //     varRadio = &varRad[shp];
    //     ImGui::Begin(title.c_str());
    //     if (ImGui::SliderFloat("Radio", varRadio, 0.01, 0.9)) { segments_event_listener = true; }
    //     ImGui::End();
    // }

    void editPropCylinder(){
        ImGui::Begin("Cylinder");
        if (ImGui::SliderFloat("base radio", &cyl_base_rad , 0.01, 0.9)) { cyl_prop_listener = true; } 
        if (ImGui::SliderFloat("top radio", &cyl_top_rad , 0.01, 0.9)) { cyl_prop_listener = true; } 
        if (ImGui::SliderFloat("heigth", &cyl_heigth , 0.01, 0.9)) { cyl_prop_listener = true; } 
        ImGui::End();
    }

    void editPropCone(){
        ImGui::Begin("Cone");
        if (ImGui::SliderFloat("top radio", &con_top_rad , 0.01, 0.9)) { con_prop_listener = true; } 
        if (ImGui::SliderFloat("heigth", &con_heigth , 0.01, 0.9)) { con_prop_listener = true; } 
        ImGui::End();
    }

    void editPropSphere(){
        ImGui::Begin("Sphere");
        if (ImGui::SliderFloat("radio", &sph_rad , 0.01, 0.9)) { sph_prop_listener = true; } 
        ImGui::End();
    }

    void editPropTorus(){
        ImGui::Begin("Torus");
        if (ImGui::SliderFloat("radio major", &tor_R , 0.01, 0.9)) { tor_prop_listener = true; } 
        if (ImGui::SliderFloat("radio minor", &tor_r , 0.01, 0.9)) { tor_prop_listener = true; } 
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

    void startup(LPCSTR lpApplicationName)
    {
    // additional information
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    // set the size of the structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // start the program up
    CreateProcessA
    (
        lpApplicationName,   // the path
        NULL,                // Command line
        NULL,                   // Process handle not inheritable
        NULL,                   // Thread handle not inheritable
        FALSE,                  // Set handle inheritance to FALSE
        CREATE_NEW_CONSOLE,     // Opens file in a separate console
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory
        &si,            // Pointer to STARTUPINFO structure
        &pi           // Pointer to PROCESS_INFORMATION structure
    );
        // Close process and thread handles.
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

}

#endif
