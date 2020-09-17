#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
    private:
        unsigned int shader_id = 0;
        std::string filepathfragment;
        std::string filepathvertex;
        std::unordered_map<std::string, int> m_UniformLocationCache;

    public:
        Shader() {}
        ~Shader() {}
        Shader(const std::string& filepath_vertex, const std::string& filepath_frament);
        
        void setFiles(const std::string& filepath_vertex, const std::string& filepath_frament); 
        void bind() const;
        void unbind() const;
        void compile();
        inline unsigned int getID() const { return shader_id; }; 

        // Set uniforms
        void setUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
        void setUniform1f(const std::string& name, float value);
        void setUniform2f(const std::string& name, float f1, float f2);
        void setUniformMat4fv(const std::string& name, glm::mat4 mat);
        void setUniform3fv(const std::string& name, glm::vec3 vec);
        void setUniform1i(const std::string& name, int val);
    private:
        int getUniformLocation(const std::string& name);
        std::string readShader(const char *filePath);
        unsigned int createShaderProgram(const char *file_vs, const char *file_fs);
};

#endif