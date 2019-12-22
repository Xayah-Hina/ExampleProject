#ifndef GameObject_hpp
#define GameObject_hpp

#include "Texture2D.hpp"
#include "Sprite.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class GameObject {
    
public:
    // Object state
    glm::vec2   Position, Size, Velocity;
    glm::vec3   Color;
    GLfloat     Rotation;
    GLboolean   IsSolid;
    GLboolean   Destroyed;
    // Render state
    Texture2D   Sprite2D;
    // Constructor(s)
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
    // Draw sprite
    virtual void Draw(Sprite &renderer);
};

GameObject::GameObject()
    : Position(0, 0), Size(1, 1), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite2D(), IsSolid(false), Destroyed(false) { }

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity)
    : Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Sprite2D(sprite), IsSolid(false), Destroyed(false) { }

void GameObject::Draw(Sprite &renderer)
{
    renderer.DrawSprite(this->Sprite2D, this->Position, this->Size, this->Rotation, this->Color);
}

#endif /* GameObject_hpp */
