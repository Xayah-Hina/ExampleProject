#ifndef Manager_hpp
#define Manager_hpp

#include "Texture2D.hpp"
#include "Shader.hpp"

#include <glad/glad.h>

#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>


class Manager {

public:
    static std::map<std::string, Shader>    Shaders;
    static std::map<std::string, Texture2D> Textures;

    static void   LoadShader(const char* VERTEX_PATH, const char* FRAGMENT_PATH, const char* GEOMITRY_PATH, std::string name);

    static void LoadTexture(const char* file, bool alpha, std::string name);

    static Shader&   GetShader(std::string name);

    static Texture2D& GetTexture(std::string name);

    static void Clear();
private:
    // Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    Manager() { }
};

// Instantiate static variables
std::map<std::string, Shader>       Manager::Shaders;
std::map<std::string, Texture2D>    Manager::Textures;


void Manager::LoadShader(const char *VERTEX_PATH, const char *FRAGMENT_PATH, const char *GEOMITRY_PATH, std::string name) {

    Shader shader(VERTEX_PATH, FRAGMENT_PATH, GEOMITRY_PATH!=nullptr?GEOMITRY_PATH:nullptr);
    Shaders[name] = shader;
}

void Manager::LoadTexture(const char *TEXTURE_PATH, bool alpha, std::string name) {

    Texture2D texture2D(TEXTURE_PATH, alpha);
    Textures[name] = texture2D;
}

Shader& Manager::GetShader(std::string name) {

    return Shaders[name];
}

Texture2D& Manager::GetTexture(std::string name) {

    return Textures[name];
}

void Manager::Clear() {

    for (auto iter : Shaders)
        glDeleteProgram(iter.second.ID);

    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
}


#endif /* Manager_hpp */
