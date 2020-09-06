#ifndef APP_HPP
#define APP_HPP

#include "..\imgui\imgui.h"
#include "..\imgui\imgui_impl_glfw.h"
#include "..\imgui\imgui_impl_opengl3.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <algorithm> 
#include <stdlib.h>

#include "controls.hpp"
#include "imgui_win.hpp"

#include "torus.h"
#include "shader.h"

class AppOpenGL{
private:
    glm::mat4 projMat;
    glm::mat4 modelviewMat;

public: 
    AppOpenGL() {}
    void init();
    void setup();
    void display();
    void cleanAll();
    bool shouldClose();
    GLFWwindow *window;
    int width, heigth;
    glm::vec3 camera;
    Shader sp{"src/vertShader.glsl", "src/fragShader.glsl"};
    unsigned int renderingProgram;
    Torus torusShape{15, 20, 0.2, 0.05};
};

void AppOpenGL::init(){

    width = 800;
    heigth = 800;
    camera = glm::vec3(0,0,-0.6);

    // glfw init
    if (!glfwInit()){
        exit(1);
        std::cout<<"GLFW Error"<<std::endl;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_FALSE);
    

    window = glfwCreateWindow(width, heigth, "CG-Project-2020", NULL, NULL);
    glfwMakeContextCurrent(window);
    
    // glew init
    if (glewInit() != GLEW_OK)
        exit(1);
    
    std::cout << "GL_VERSION: " << glGetString(GL_VERSION) << std::endl;
    
    glfwSwapInterval(0);

    glfwSetCursorPosCallback(window, Controls::cursor_position_callback);
    glfwSetMouseButtonCallback(window, Controls::mouse_button_callback);
    glfwSetScrollCallback(window, Controls::scroll_callback);
    glfwSetKeyCallback(window, Controls::key_callback);

    // imgui init
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

}

void AppOpenGL::setup(){
    sp.readShaders();

    projMat = glm::perspective(1.0472f, (float)width / (float)heigth, 0.1f, 1000.0f);
    glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "proj_matrix"), 1, GL_FALSE, glm::value_ptr(projMat));
    
    sp.bind();
    sp.setUniformMat4fv("proj_matrix", projMat);
    sp.setUniform2f("u_resolution", (float)width, (float)heigth);
    torusShape.initData();
}

void AppOpenGL::display(){

    sp.bind();

    modelviewMat = glm::translate(glm::mat4(1.0f), glm::vec3(camera));
    modelviewMat *= Controls::getTransf();

    glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "mv_matrix"), 1, GL_FALSE, glm::value_ptr(modelviewMat));
    sp.setUniformMat4fv("mv_matrix", modelviewMat);
    
    glClearColor(ImGuiWin::fondo.x,
                 ImGuiWin::fondo.y, 
                 ImGuiWin::fondo.z, 
                 ImGuiWin::fondo.w);
    
    glClear(GL_COLOR_BUFFER_BIT);

    glm::vec3 color = glm::vec3(ImGuiWin::color_figura.x, 
                                ImGuiWin::color_figura.y, 
                                ImGuiWin::color_figura.z); 

    sp.setUniform3fv("u_color", color);
    torusShape.render();

    ImGuiWin::renderMainWindow();
    ImGuiWin::endMainWindow();

    glfwPollEvents();
    glfwSwapBuffers(window);

}

void AppOpenGL::cleanAll(){
    ImGuiWin::cleanAll();
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool AppOpenGL::shouldClose(){
    return glfwWindowShouldClose(window);
}

#endif
