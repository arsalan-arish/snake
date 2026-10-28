#include <snake/sprites/snake.h>


Snake::Snake(f32 x, f32 y) : Sprite(
    {x, y, 100, 100}, 
    {255, 0, 0, 255}
)
{

}

Snake::~Snake()
{

}

void Snake::update(const b8* keyboardState, f64 delta_time) {

    f64 force = 1.0; // Standardized arbitrary change that a button press applies per time
    
    auto& k = keyboardState;
    if (k[SDL_SCANCODE_W] || k[SDL_SCANCODE_S] || k[SDL_SCANCODE_D] || k[SDL_SCANCODE_A]) {
        m_acceleration += force / m_mass;
        m_velocity     += m_acceleration * delta_time;
        m_momentum     += force * delta_time; // OR m_mass * m_velocity
    }

    this->rect.x += m_velocity * delta_time; // Ensure x component of m_velocity
    this->rect.y += m_velocity * delta_time; // Ensure y component of m_velocity
}