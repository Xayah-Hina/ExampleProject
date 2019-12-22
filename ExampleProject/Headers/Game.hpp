#ifndef Game_hpp
#define Game_hpp

#include "BallObject.hpp"
#include "GameLevel.hpp"
#include "Sprite.hpp"
#include "Manager.hpp"
#include "ParticleSystem.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

// Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(100, 20);
// Initial velocity of the player paddle
const GLfloat PLAYER_VELOCITY(500.0f);
// Initial velocity of the Ball
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// Radius of the ball object
const GLfloat BALL_RADIUS = 12.5f;

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // Game state
    GameState              State;
    GLboolean              Keys[1024];
    GLuint                 Width, Height;
    
    std::vector<GameLevel> Levels;
    unsigned int Level;
    
    Game(GLuint width, GLuint height);
    ~Game();
    
    void Init();
    
    void ProcessInput(GLfloat dt);
    void Update(GLfloat dt);
    void Render();
    
    void DoCollisions();
    
    void ResetLevel();
    void ResetPlayer();
};

// Game-related State data
Sprite  *Renderer;
GameObject *Player;
BallObject *Ball;
ParticleSystem *Particles;

Game::Game(GLuint width, GLuint height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
    delete Renderer;
//    delete Player;
//    delete Ball;
//    delete Particles;
}

void Game::Init()
{
    Manager::LoadShader("/Users/polaris/Projects/Graphics/2Dgame-Breakout/Glitter/Shaders/sprite.vs", "/Users/polaris/Projects/Graphics/2Dgame-Breakout/Glitter/Shaders/sprite.fs", nullptr, "sprite");
    Manager::LoadShader("/Users/polaris/Projects/Graphics/2Dgame-Breakout/Glitter/Shaders/particle.vs", "/Users/polaris/Projects/Graphics/2Dgame-Breakout/Glitter/Shaders/particle.fs", nullptr, "particle");
    Manager::LoadTexture("/Users/polaris/Projects/Graphics/2Dgame-Breakout/Glitter/Asserts/awesomeface.png", false, "face");
    Manager::LoadTexture("/Users/polaris/Projects/Graphics/2Dgame-Breakout/Glitter/Asserts/background.jpg", false, "background");
    Manager::LoadTexture("/Users/polaris/Projects/Graphics/2Dgame-Breakout/Glitter/Asserts/block.png", false, "block");
    Manager::LoadTexture("/Users/polaris/Projects/Graphics/2Dgame-Breakout/Glitter/Asserts/block_solid.png", false, "block_solid");
    Manager::LoadTexture("/Users/polaris/Projects/Graphics/2Dgame-Breakout/Glitter/Asserts/paddle.png", true, "paddle");
    Manager::LoadTexture("/Users/polaris/Projects/Graphics/2Dgame-Breakout/Glitter/Asserts/particle.png", true, "particle");

    
    // Sprite Shader Part
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    Manager::GetShader("sprite").use().SetInteger("image", 0);
    Manager::GetShader("sprite").SetMatrix4("projection", projection);
    Renderer = new Sprite(Manager::GetShader("sprite"));
    
    // Particle System Part
    Manager::GetShader("particle").use().SetInteger("sprite", 0);
    Manager::GetShader("particle").SetMatrix4("projection", projection);
    Particles = new ParticleSystem(Manager::GetShader("particle"), Manager::GetTexture("particle"), 500);
    
    // Game Level Part
    GameLevel one; one.Load("/Users/polaris/Projects/Graphics/2Dgame-Breakout/Glitter/Asserts/bricks_pos.txt", this->Width, this->Height * 0.5);
    this->Levels.push_back(one);
    this->Level = 0;
    glm::vec2 playerPos = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, Manager::GetTexture("paddle"));
    
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, - BALL_RADIUS * 2);
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, Manager::GetTexture("face"));
}

void Game::Update(GLfloat dt)
{
    Ball->Move(dt, this->Width);
    Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2));
    this->DoCollisions();
    if (Ball->Position.y >= this->Height) 
    {
        this->ResetLevel();
        this->ResetPlayer();
    }
}

void Game::ProcessInput(GLfloat dt)
{
    if (this->State == GAME_ACTIVE)
    {
        GLfloat velocity = PLAYER_VELOCITY * dt;
        // Move playerboard
        if (this->Keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0) {
                Player->Position.x -= velocity;
                if (Ball->Stuck)
                   Ball->Position.x -= velocity;
            }
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= this->Width - Player->Size.x) {
                Player->Position.x += velocity;
                if (Ball->Stuck)
                Ball->Position.x += velocity;
            }
        }
        if (this->Keys[GLFW_KEY_SPACE]) {
            Ball->Stuck = false;
        }
    }
}

void Game::Render()
{
    if(this->State == GAME_ACTIVE) {
        // Draw background
        Renderer->DrawSprite(Manager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
        // Draw level
        this->Levels[this->Level].Draw(*Renderer);
        Player->Draw(*Renderer);
        Particles->Draw();
        Ball->Draw(*Renderer);
    }
}

Direction VectorDirection(glm::vec2 target){
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, -1.0f),
        glm::vec2(-1.0f, 0.0f)
    };
    GLfloat max = 0.0f;
    GLuint best_match = -1;
    for (GLuint i = 0; i < 4; i++)
    {
        GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}

GLboolean CheckCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
    // Collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
    // Collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
    // Collision only if on both axes
    return collisionX && collisionY;
}

Collision CheckCollision(BallObject &one, GameObject &two) // AABB - AABB collision
{
    glm::vec2 center(one.Position + one.Radius);
    glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
    glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);
    
    glm::vec2 difference = center - aabb_center;
    
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    
    glm::vec2 closest = aabb_center + clamped;
    
    difference = closest - center;
    
    if (glm::length(difference) <= one.Radius)
        return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
    else
        return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));
}

void Game::DoCollisions() {
    
    for (GameObject &box : this->Levels[this->Level].Bricks) {
        
        if (!box.Destroyed) {
            Collision collision = CheckCollision(*Ball, box);
            if (std::get<0>(collision)) {
                if (!box.IsSolid) {
                    box.Destroyed = GL_TRUE;
                }
                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if (dir == LEFT || dir == RIGHT) {
                    Ball->Velocity.x = -Ball->Velocity.x;
                    GLfloat penetration = Ball->Radius - std::abs(diff_vector.x);
                    if (dir == LEFT)
                        Ball->Position.x += penetration;
                    else
                        Ball->Position.x -= penetration;
                }
                else {
                    Ball->Velocity.y = -Ball->Velocity.y;
                    GLfloat penetration = Ball->Radius - std::abs(diff_vector.y);
                    if (dir == UP)
                        Ball->Position.y += penetration;
                    else
                        Ball->Position.y -= penetration;
                }
            }
        }
    }
    
    Collision result = CheckCollision(*Ball, *Player);
    if (!Ball->Stuck && std::get<0>(result))
    {
        // Check where it hit the board, and change velocity based on where it hit the board
        GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
        GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
        GLfloat percentage = distance / (Player->Size.x / 2);
        // Then move accordingly
        GLfloat strength = 2.0f;
        glm::vec2 oldVelocity = Ball->Velocity;
        Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
//        Ball->Velocity.y = -Ball->Velocity.y;
        Ball->Velocity.y = -1 * abs(Ball->Velocity.y);
        Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);// TODO
    }
}

void Game::ResetLevel()
{
    if (this->Level == 0)
        this->Levels[0].Load("/Users/polaris/Projects/Graphics/2Dgame-Breakout/Glitter/Asserts/bricks_pos.txt", this->Width, this->Height * 0.5f);
//    else if (this->Level == 1)
//        this->Levels[1].Load("levels/two.lvl", this->Width, this->Height * 0.5f);
//    else if (this->Level == 2)
//        this->Levels[2].Load("levels/three.lvl", this->Width, this->Height * 0.5f);
//    else if (this->Level == 3)
//        this->Levels[3].Load("levels/four.lvl", this->Width, this->Height * 0.5f);
}

void Game::ResetPlayer()
{
    // Reset player/ball states
    Player->Size = PLAYER_SIZE;
    Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
    Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
}


#endif /* Game_hpp */
