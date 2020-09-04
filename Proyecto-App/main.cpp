/* CONTROLES
 * 		MOVERSE CON WASD O LAS FLECHITAS
 * 		AGRANDAR O ENCOGER CON EL SCROLL DEL MOUSE
 * 		ROTAR ARRASTRANDO CON CLICK DERECHO PRESIONADO
 * */

#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw.h"
#include "imgui\imgui_impl_opengl3.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "Utils.hpp"

#define numVAOs 1
#define numVBOs 1
#define numEBOs 2
#define PI 3.14
#define WIDTH 800
#define HEIGHT 800

#define PI 3.14

using namespace std;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLuint ebo[numEBOs];

double cameraX = 0.0, cameraY = 0.0, cameraZ = 0.6;    // POSICION DE LA CAMARA
float objLocX = 0.0f, objLocY = -0.4f, objLocZ = 0.0f; // POSICION DE SU OBJETO
glm::mat4 pMat, vMat, mMat, mvMat;                     // matrices de transformacion
glm::mat4 transf = glm::mat4(1.0);                     // matriz de transformacion generadas por los controles

/*------------------------------ [FUNCIONES] ---------------------------*/
void buildSphere(glm::vec3 center, double radius, int numPointX, int numPointY); // EJEMPLO DE FUNCION GENERADORA DE PUNTOS
void setupVertices(void);
void init(GLFWwindow *window);
void display(GLFWwindow *window, double currentTime);
/*----------------------------------------------------------------------*/

/*------------------------------ [CONTROLES] ---------------------------*/
// (No es muy importante conocer esta parte)
bool dragging = false;
double offsetX = 0.0;
double offsetY = 0.0;
double oldX = 0, oldY = 0;
double zoom = 1;
bool clickPuntoControl(glm::vec2 center, float x, float y);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
/*----------------------------------------------------------------------*/

/*------------------------------ [IMGUI] -------------------------------*/
ImVec4 fondo = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);                                     // COLOR INICIAL DEL FONDO (DEFAULT)
ImVec4 color_figura = ImVec4(217.0f / 255.0f, 199.0f / 255.0f, 10.0f / 255.0f, 1.00f); // COLOR INICIAL DE SU FIGURA (DEFAULT)
/*---------- ----------------------------------------------------------*/

vector<double> vertices; // VECTOR PARA ALMACENAR LOS VERTICES DE SU FIGURA

int main() {
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow *window = glfwCreateWindow(800, 800, "CG-T07-CornerTable", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
        exit(EXIT_FAILURE);
    cout << "GL_VERSION: " << glGetString(GL_VERSION) << endl;
    glfwSwapInterval(0);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    init(window);

    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
    }

    glDeleteBuffers(numVBOs, vbo);
    glDeleteBuffers(numEBOs, ebo);
    glDeleteShader(renderingProgram);
    glDeleteVertexArrays(numVAOs, vao);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
// EJEMPLO DE FUNCION GENERADORA DE PUNTOS
void buildSphere(glm::vec3 center, double radius, int numPointX, int numPointY) {
    double incX = 2 * PI / numPointX;
    double incY = PI / numPointY;
    for (double j = 0.0; j <= 2 * PI; j += incX) {
        for (double i = 0.0; i <= PI; i += incY) {
            glm::vec3 p;
            p.x = center.x + radius * sin(j) * cos(i);
            p.y = center.y + radius * sin(j) * sin(i);
            p.z = center.z + radius * cos(j);
            vertices.push_back(p.x);
            vertices.push_back(p.y);
            vertices.push_back(p.z);
        }
    }
}
// FUNCION: GENERA TODOS LOS VERTICES DE LA FIGURA (C++) Y LO MANDA AL VBO (GPU)
void setupVertices(void)  {
    buildSphere(glm::vec3(0, 0, 0), 0.5, 10, 10); // LLAMAN A LA FUNCION QUE GENERA SUS PUNTOS PARA QUE LLENE EL VECTOR "vertices"

    /*
    for(int i=0; i<vertices.size(); i+=3){ // [DEBUG] IMPRIMIMOS LOS VERTICES (OPCIONAL)
        cout<<"["<<vertices[i]<<","<<vertices[i+1]<<","<<vertices[i+2]<<"]"<<endl;
    }
    cout<<"Vertices:"<<vertices.size()/3<<endl; // [DEBUG] IMPRIMOS LA CANTIDAD DE VERTICES GENERADOS (OPCIONAL) 
    */

    glGenVertexArrays(numVAOs, vao); // CREAR VAO
    glBindVertexArray(vao[0]);       // SELECIONAR VAO (ANTES DE CREAR LOS VBO SINO DARIA ERROR)

    glGenBuffers(numVBOs, vbo);                                                                    // CREAR VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);                                                         // SELECCIONAR VBO
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(double), &vertices[0], GL_STATIC_DRAW); // PUSH DATA OF "vertices" TO VBO[0]
}

void init(GLFWwindow *window) {
    /* --------------- [INICIALIZACION DE IMGUI] --------------- */
    // (No es muy importante conocer esta parte)
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    /* ---------------------------------------------------------- */

    renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl"); // CREAMOS EL PROGRAMA SHADER
    glUseProgram(renderingProgram);                                                      // SELECCIONAMOS EL PROGRAMA CREADO

    setupVertices(); // GENERA VERTICES DE LA FIGURA Y LOS MANDA AL VBO (GPU)

    /* CREA MATRICES DE PROYECCION Y DE VISTA INICIALES */
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect = (float)width / (float)height;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);                         // MATRIZ DE PROYECCION
    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ)); // MATRIZ VISTA - esta matriz aun no se envia porque falta concatenarse con la matriz model
    /* ENVIA LOS UNIFORMES CREADOS A LOS SHADER */
    glUniform2f(glGetUniformLocation(renderingProgram, "u_resolution"), (float)WIDTH, (float)HEIGHT);             // ENVIA UNIFORM RESOLUCION - wiki: para saber que resolucion tiene el programa y manejar desde los shader (cuando lo necesitemos)
    glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "proj_matrix"), 1, GL_FALSE, glm::value_ptr(pMat)); // ENVIA LA MATRIZ DE PROYECCION AL SHADER - wiki: la matriz de proyeccion sirve para el efecto de 'profundidad' cuando el objeto es alejado de la camara
}

void display(GLFWwindow *window, double currentTime) {
    /* --------------------- INICIA VENTANA IMGUI ---------------------*/
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Camino mas corto en CornerTable - GUI");        // CREAMOS LA VENTANA ESTABLECIENDO UN TITULO
    ImGui::ColorEdit3("Color de fondo", (float *)&fondo);         // COLOR EDIT PARA CAMBIAR EL COLOR DE FONDO
    ImGui::ColorEdit3("Color de figura", (float *)&color_figura); // COLOR EDIT PARA CAMBIAR EL COLOR DE SU FIGURA
    ImGui::End();                                                 // FINALIZA LA VENTANA

    glClearColor(fondo.x, fondo.y, fondo.z, fondo.w); // PASAMOS EL COLOR OBTENIDO CON EL COLOREDIT DE IMGUI Y LO ESTABLECEMOS COMO COLOR DE FONDO
    glClear(GL_COLOR_BUFFER_BIT);                     // PINTAMOS EL FONDO

    glEnable(GL_LINE_SMOOTH); // PARA CAMBIAR EL GROSOR DE LA LINEA
    glLineWidth(2);           // ESTABLESCA UN "GROSOR" PARA LAS LINEAS SI ES QUE DIBUJA ALGUNA (GL_LINES, GL_LINES_STRIP, GL_LINE_LOOP, ETC)

    /* COMENZAMOS A DIBUJAR NUESTRA FIGURA */
    glUseProgram(renderingProgram); // SELECCIONAMOS EL SHADER

    /* MATRICES DE TRANSFORMACION (CONCATENACION Y ENVIOS AL SHADER)*/
    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(objLocX, objLocY, objLocZ));                                // CONSTRUIMOS LA MATRIZ MODELO - wiki: esta matriz posiciona el objeto en la posicion que se definio arriba
    mMat = glm::scale(mMat, glm::vec3(1.0, 1.0, 1.0));                                                           // EN CASO SU FIFURA SALGA MUY PEQUEÑA PUEDE CAMBIAR EL 1.0 POR POR UN VALOR MAYOR PARA AGRANDAR, O EN CASO SALGA MUY GRANDE POR UNO MENOR
    mMat *= transf;                                                                                              // APLICA LAS TRANSFORMACIONES GENERADAS POR LOS CONTROLES
    mvMat = vMat * mMat;                                                                                         // CREA LA MATRIZ MODEL-VIEW - esta tiene todas las transformaciones anteriores concatenadas
    glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "mv_matrix"), 1, GL_FALSE, glm::value_ptr(mvMat)); // ENVIO LA MODEL-VIEW MATRIZ AL SHADER

    /* DRAW ! */
    glm::vec3 color = glm::vec3(color_figura.x, color_figura.y, color_figura.z);               // CREAMOS UNA VARIABLE PARA RECUPERAR LOS COLORES DE NUESTRA FIGURA OBTENIDOS POR EL COLOREDIT DE IMGUI
    glUniform3fv(glGetUniformLocation(renderingProgram, "u_color"), 1, glm::value_ptr(color)); // MANDAMOS LOS COLORES DE LA FIGURA COMO UNIFORME AL SHADER PARA QUE EL FRAGMENT LO PUEDA USAR
    glBindVertexArray(vao[0]);                                                                 // SELECCION EL VAO QUE CONTIENE EL VBO DE NUESTRA FIGURA
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);                                                     // SELECCIONAMOS EL VBO QUE TIENE LOS VERTICES DE NUESTRA FIGURA
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, 0);                                    // INDICAMOS LA ESTRUCTURA DE LOS VERTICES ALMACENADOS EN EL VBO - basicamente significa: "el atrib. numero 0 que es la posicion (ver vertexshader.glsl) coge vertices del vbo de 3 en 3 (x,y,z) que son double desde la pos 0 (pos inicial)"
    glEnableVertexAttribArray(0);                                                              // HABILITAMOS EL ATRIB NUMERO 0
    glPointSize(3.0);                                                                          // ESTRABLECEMOS UN GROSOR PARA LOS PUNTOS SI EN CASO USAMOS ALGUN GL_POINTS
    glDrawArrays(GL_POINTS, 0, vertices.size() / 3);                                           // DIBUJA
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() / 3);                                   // DIBUJA

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/* -------------------------------- CONTROLES -------------------------------------- */

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods){
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        glfwGetCursorPos(window, &oldX, &oldY);
        dragging = true;
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        dragging = false;
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
    if (dragging) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        offsetX = (xpos - oldX) / zoom;
        offsetY = (ypos - oldY) / zoom;

        oldX = xpos;
        oldY = ypos;

        float angleRotX = (PI / 800) * offsetY;
        float angleRotY = (PI / 800) * offsetX;

        transf = glm::rotate(transf, angleRotX, glm::vec3(1.0, 0.0, 0.0));
        transf = glm::rotate(transf, angleRotY, glm::vec3(0.0, 1.0, 0.0));
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset){
    double sensibilidad = 1.09;
    if (yoffset != 0) {
        zoom = yoffset > 0 ? sensibilidad : 1 / sensibilidad;
        transf = glm::scale(transf, glm::vec3(zoom, zoom, zoom));
    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
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
/* -------------------------------------------------------------------------------- */
