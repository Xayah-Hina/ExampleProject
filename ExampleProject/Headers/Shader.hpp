#ifndef Shader_hpp
#define Shader_hpp

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


class Shader {
    
public:
    unsigned int ID;
    
    Shader() {};
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    
    Shader& use();
    
    // Utility functions
    void    SetBool     (const std::string &name, bool value, GLboolean useShader = false);
    void    SetFloat    (const std::string &name, GLfloat value, GLboolean useShader = false);
    void    SetInteger  (const std::string &name, GLint value, GLboolean useShader = false);
    void    SetVector2f (const std::string &name, GLfloat x, GLfloat y, GLboolean useShader = false);
    void    SetVector2f (const std::string &name, const glm::vec2 &value, GLboolean useShader = false);
    void    SetVector3f (const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
    void    SetVector3f (const std::string &name, const glm::vec3 &value, GLboolean useShader = false);
    void    SetVector4f (const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
    void    SetVector4f (const std::string &name, const glm::vec4 &value, GLboolean useShader = false);
    void    SetMatrix2  (const std::string &name, const glm::mat2 &matrix, GLboolean useShader = false);
    void    SetMatrix3  (const std::string &name, const glm::mat3 &matrix, GLboolean useShader = false);
    void    SetMatrix4  (const std::string &name, const glm::mat4 &matrix, GLboolean useShader = false);
private:
    void checkCompileErrors(GLuint shader, std::string type);
};

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {

    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if(geometryPath != nullptr)
        {
            gShaderFile.open(geometryPath);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // if geometry shader is given, compile geometry shader
    unsigned int geometry;
    if(geometryPath != nullptr)
    {
        const char * gShaderCode = geometryCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);
        checkCompileErrors(geometry, "GEOMETRY");
    }
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    if(geometryPath != nullptr)
        glAttachShader(ID, geometry);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if(geometryPath != nullptr)
        glDeleteShader(geometry);
}

Shader& Shader::use() {
    
    glUseProgram(this->ID);
    return *this;
}

void Shader::SetBool(const std::string &name, bool value, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}
void Shader::SetFloat(const std::string &name, GLfloat value, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}
void Shader::SetInteger(const std::string &name, GLint value, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}
void Shader::SetVector2f(const std::string &name, GLfloat x, GLfloat y, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniform2f(glGetUniformLocation(this->ID, name.c_str()), x, y);
}
void Shader::SetVector2f(const std::string &name, const glm::vec2 &value, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniform2f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y);
}
void Shader::SetVector3f(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniform3f(glGetUniformLocation(this->ID, name.c_str()), x, y, z);
}
void Shader::SetVector3f(const std::string &name, const glm::vec3 &value, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniform3f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z);
}
void Shader::SetVector4f(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniform4f(glGetUniformLocation(this->ID, name.c_str()), x, y, z, w);
}
void Shader::SetVector4f(const std::string &name, const glm::vec4 &value, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniform4f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z, value.w);
}
void Shader::SetMatrix2(const std::string &name, const glm::mat2 &matrix, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniformMatrix2fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}
void Shader::SetMatrix3(const std::string &name, const glm::mat3 &matrix, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniformMatrix3fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}
void Shader::SetMatrix4(const std::string &name, const glm::mat4 &matrix, GLboolean useShader)
{
    if (useShader)
        this->use();
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::checkCompileErrors(unsigned int shader, std::string type) {
    
    GLint success;
    GLchar infoLog[1024];
    if(type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

#endif /* Shader_hpp */
