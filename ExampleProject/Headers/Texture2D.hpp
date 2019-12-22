#ifndef Texture2D_hpp
#define Texture2D_hpp

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glad/glad.h>


class Texture2D {
    
public:
    unsigned int ID;
    int Width, Height;
    unsigned int Internal_Format;
    unsigned int Image_Format;
    unsigned int Wrap_S;
    unsigned int Wrap_T;
    unsigned int Filter_Min;
    unsigned int Filter_Max;
    
    Texture2D(){}
    Texture2D(const char* path, bool alpha = false);

    void bind() const;
};

Texture2D::Texture2D(const char* path, bool alpha) : Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR) {
    
    if (alpha) {
        Internal_Format = GL_RGBA;
        Image_Format = GL_RGBA;
    }
    
    glGenTextures(1, &this->ID);
    int channel;
    unsigned char* image = stbi_load(path, &this->Width, &this->Height, &channel, 0);
    // Set the Correct Channel Format
    
    
    switch (channel)
    {
        case 1: this->Internal_Format = GL_ALPHA; this->Image_Format = GL_ALPHA;     break;
        case 2: this->Internal_Format = GL_LUMINANCE; this->Image_Format = GL_LUMINANCE; break;
        case 3: this->Internal_Format = GL_RGB; this->Image_Format = GL_RGB;       break;
        case 4: this->Internal_Format = GL_RGBA; this->Image_Format = GL_RGBA;     break;
    }
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, this->ID);
    glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, this->Width, this->Height, 0, this->Image_Format, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);
}

void Texture2D::bind() const {
    
    glBindTexture(GL_TEXTURE_2D, this->ID);
}

#endif /* Texture2D_hpp */
