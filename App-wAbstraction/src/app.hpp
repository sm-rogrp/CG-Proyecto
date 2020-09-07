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
#include "utils.h"

class AppOpenGL{
private:
    glm::mat4 projMat;
    glm::mat4 modelviewMat;
    glm::mat4 viewMat;

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
	Shader sp { "src/vertShader.glsl", "src/fragShader.glsl" };
    Torus torusShape{10, 10, 0.2, 0.05};
private:
        /* Ligthing */
    void installLights(glm::mat4 vMatrix);
    GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc, mAmbLoc, mDiffLoc, mSpecLoc, mShiLoc;
    glm::mat4 invTrMat, rMat;
    glm::vec3 currentLightPos, lightPosV; // light position as Vector3f, in both model and view space
    float lightPos[3]; // light position as float array
    // initial light location
    glm::vec3 initialLightLoc = glm::vec3(5.0f, 2.0f, 2.0f);
    // white light properties
    float globalAmbient[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
    float lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    // gold material properties
    float* matAmb = Utils::goldAmbient();
    float* matDif = Utils::goldDiffuse();
    float* matSpe = Utils::goldSpecular();
    float matShi = Utils::goldShininess();
    float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }
    float amt = 0.0f;
};

void AppOpenGL::init() {

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
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_FALSE);


    window = glfwCreateWindow(width, heigth, "CG-Project-2020", NULL, NULL);
    glfwMakeContextCurrent(window);

    // glew init
    if (glewInit() != GLEW_OK){
        exit(1);
        std::cout<<"GLEW Error"<<std::endl;
    }

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

void AppOpenGL::setup() {
    sp.compile();
    sp.bind();

    projMat = glm::perspective(1.0472f, (float)width / (float)heigth, 0.1f, 1000.0f);

    sp.setUniformMat4fv("proj_matrix", projMat);
    sp.setUniform2f("u_resolution", (float)width, (float)heigth);

    torusShape.initData();
    viewMat = glm::translate(glm::mat4(1.0), camera);
}

void AppOpenGL::installLights(glm::mat4 vMatrix)
{
    // convert light’s position to view space, and save it in a float array
    lightPosV = glm::vec3(vMatrix * glm::vec4(currentLightPos, 1.0));
    lightPos[0] = lightPosV.x;
    lightPos[1] = lightPosV.y;
    lightPos[2] = lightPosV.z;
    // get the locations of the light and material fields in the shader
    globalAmbLoc = glGetUniformLocation(sp.getID(), "globalAmbient");
    ambLoc = glGetUniformLocation(sp.getID(), "light.ambient");
    diffLoc = glGetUniformLocation(sp.getID(), "light.diffuse");
    specLoc = glGetUniformLocation(sp.getID(), "light.specular");
    posLoc = glGetUniformLocation(sp.getID(), "light.position");
    mAmbLoc = glGetUniformLocation(sp.getID(), "material.ambient");
    mDiffLoc = glGetUniformLocation(sp.getID(), "material.diffuse");
    mSpecLoc = glGetUniformLocation(sp.getID(), "material.specular");
    mShiLoc = glGetUniformLocation(sp.getID(), "material.shininess");
    // set the uniform light and material values in the shader
    glProgramUniform4fv(sp.getID(), globalAmbLoc, 1, globalAmbient);
    glProgramUniform4fv(sp.getID(), ambLoc, 1, lightAmbient);
    glProgramUniform4fv(sp.getID(), diffLoc, 1, lightDiffuse);
    glProgramUniform4fv(sp.getID(), specLoc, 1, lightSpecular);
    glProgramUniform3fv(sp.getID(), posLoc, 1, lightPos);
    glProgramUniform4fv(sp.getID(), mAmbLoc, 1, matAmb);
    glProgramUniform4fv(sp.getID(), mDiffLoc, 1, matDif);
    glProgramUniform4fv(sp.getID(), mSpecLoc, 1, matSpe);
    glProgramUniform1f(sp.getID(), mShiLoc, matShi);
}

void AppOpenGL::display(){

    sp.bind();
    modelviewMat = viewMat;
    modelviewMat *= Controls::getTransf();

    sp.setUniformMat4fv("mv_matrix", modelviewMat);

    // -- ligthing --
    currentLightPos = glm::vec3(initialLightLoc.x, initialLightLoc.y, initialLightLoc.z);
    amt += 0.05f; // movimiento de la luz
	rMat = glm::rotate(glm::mat4(1.0f), toRadians(amt), glm::vec3(0.0f, 0.0f, 1.0f));
    currentLightPos =  glm::vec3(rMat * glm::vec4(currentLightPos, 1.0f));
    invTrMat = glm::transpose(glm::inverse(modelviewMat));
    sp.setUniformMat4fv("norm_matrix", invTrMat);
    installLights(viewMat);
    // -- ligthing --

    glClearColor(ImGuiWin::fondo.x,
                 ImGuiWin::fondo.y,
                 ImGuiWin::fondo.z,
                 ImGuiWin::fondo.w);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(2.5); // "ancho" de lineas

    glm::vec3 color;

    color = glm::vec3(ImGuiWin::color_figura.x,
                      ImGuiWin::color_figura.y,
                      ImGuiWin::color_figura.z);

    sp.setUniform3fv("u_color", color);

    if (ImGuiWin::show_smooth){
        sp.setUniform1i("show_smooth", 1);
    } else {
        sp.setUniform1i("show_smooth", 0);
    }

    if (ImGuiWin::show_fill){
        torusShape.renderFill();
    }

    if (ImGuiWin::show_normals){

        sp.setUniform1i("show_smooth", 0);
        sp.setUniform3fv("u_color", glm::vec3(1,0,0)); // color rojo
        torusShape.renderNormals();
        //glDisable(GL_LINE_SMOOTH);
    }

    if (ImGuiWin::show_wire){
        sp.setUniform1i("show_smooth", 0);
        sp.setUniform3fv("u_color", glm::vec3(1,1,0)); // color amarillo
        torusShape.renderWire();
    }

    if (ImGuiWin::segments_event_listener){
        torusShape.num_x = ImGuiWin::segments_x;
        torusShape.num_y = ImGuiWin::segments_y;
        torusShape.initData();
        ImGuiWin::segments_event_listener = false;
    }

    ImGuiWin::renderMainWindow();
    ImGuiWin::endMainWindow();

    glfwPollEvents();
    glfwSwapBuffers(window);

}

void AppOpenGL::cleanAll() {
	ImGuiWin::cleanAll();
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool AppOpenGL::shouldClose() {
	return glfwWindowShouldClose(window);
}

#endif
