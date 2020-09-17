#include "shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string &filepath_v, const std::string &filepath_f)
{
    this->filepathvertex = filepath_v;
    this->filepathfragment = filepath_f;
}

void Shader::setFiles(const std::string &filepath_v, const std::string &filepath_f) {
    this->filepathvertex = filepath_v;
    this->filepathfragment = filepath_f;
}

void Shader::compile(){
    shader_id = createShaderProgram( filepathvertex.c_str(), filepathfragment.c_str());
    glUseProgram(shader_id);
}

void Shader::bind() const
{
    glUseProgram(shader_id);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

int Shader::getUniformLocation(const std::string &name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    int location = glGetUniformLocation(shader_id, name.c_str());
    if (location == -1)
        std::cout << "No active uniform variable with name " << name << " found" << std::endl;

    m_UniformLocationCache[name] = location;

    return location;
}

void Shader::setUniform1f(const std::string &name, float value)
{
    bind();
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setUniform2f(const std::string &name, float f1, float f2)
{
    bind();
    glUniform2f(getUniformLocation(name), f1, f2);
}

void Shader::setUniform4f(const std::string &name, float f0, float f1, float f2, float f3)
{
    bind();
    glUniform4f(getUniformLocation(name), f0, f1, f2, f3);
}

void Shader::setUniformMat4fv(const std::string &name, glm::mat4 mat)
{
    bind();
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setUniform3fv(const std::string &name, glm::vec3 vec)
{
    bind();
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(vec));
}

void Shader::setUniform1i(const std::string &name, int val){
    bind();
    glUniform1i(getUniformLocation(name), val);
}

std::string Shader::readShader(const char *filePath)
{
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);
    std::string line = "";

    while (getline(fileStream, line))
    {
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

unsigned int Shader::createShaderProgram(const char *file_vs, const char *file_fs)
{
    std::string str_src_vs = readShader(file_vs);
    std::string str_src_fs = readShader(file_fs);

    const char *src_vs = str_src_vs.c_str();
    const char *src_fs = str_src_fs.c_str();

    unsigned int vs, fs;
    vs = glCreateShader(GL_VERTEX_SHADER);
    fs = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vs, 1, &(src_vs), NULL);
    glShaderSource(fs, 1, &(src_fs), NULL);

    glCompileShader(vs);
    glCompileShader(fs);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    return program;
}