#ifndef CONTROLS_HPP
#define CONTROLS_HPP

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

namespace Controls
{

    float zoom = 1.0;
    glm::mat4 transf = glm::mat4(1.0f);
    bool dragging = false;
    double offsetX = 0.0;
    double offsetY = 0.0;
    double oldX = 0;
    double oldY = 0;

    float aspect;
    glm::mat4 pMat = glm::mat4(1.0f);

    glm::mat4 getTransf();

    void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
    void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
    void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
    void window_size_callback(GLFWwindow* win, int newWidth, int newHeight);

    void resetTrans() { transf = glm::mat4(1.0f); } ;

}; // namespace Controls

glm::mat4 Controls::getTransf()
{
    return transf;
}

void Controls::window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
        aspect = (float)newWidth / (float)newHeight;
        glViewport(0, 0, newWidth, newHeight);
        pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

void Controls::cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    float PI = 3.14f;
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    if (dragging)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        offsetX = (xpos - oldX) / zoom;
        offsetY = (ypos - oldY) / zoom;

        oldX = xpos;
        oldY = ypos;

        float angleRotX = (PI / w) * offsetY;
        float angleRotY = (PI / h) * offsetX;

        transf = glm::rotate(transf, angleRotX, glm::vec3(1.0, 0.0, 0.0));
        transf = glm::rotate(transf, angleRotY, glm::vec3(0.0, 1.0, 0.0));
    }
}

void Controls::mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &oldX, &oldY);
        dragging = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        dragging = false;
    }
}

void Controls::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    double vel = 0.15 / zoom;
    if ((key == GLFW_KEY_LEFT || key == GLFW_KEY_A) && action == GLFW_PRESS)
        transf = glm::translate(transf, glm::vec3(-vel, 0.0, 0.0));

    if ((key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) && action == GLFW_PRESS)
        transf = glm::translate(transf, glm::vec3(+vel, 0.0, 0.0));

    if ((key == GLFW_KEY_UP || key == GLFW_KEY_W) && action == GLFW_PRESS)
        transf = glm::translate(transf, glm::vec3(0.0, vel, 0.0));

    if ((key == GLFW_KEY_DOWN || key == GLFW_KEY_S) && action == GLFW_PRESS)
        transf = glm::translate(transf, glm::vec3(0.0, -vel, 0.0));

    if (key == GLFW_KEY_R && action == GLFW_PRESS)
        transf = glm::mat4(1.0f);
}

void Controls::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    double sensibilidad = 1.09;
    if (yoffset != 0)
    {
        zoom = yoffset > 0 ? sensibilidad : 1 / sensibilidad;
        transf = glm::scale(transf, glm::vec3(zoom, zoom, zoom));
    }
}

#endif
