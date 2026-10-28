// #include <numbers>
#include <cstdlib>
#include <snake/sprites/snake.h>
#include <snake/types.h>
#include <glm/glm.hpp>

using std::abs;
// using std::cos;
// using std::sin;
// using std::tan;
// using std::acos;
// using std::asin;
// using std::atan;
// using std::numbers::pi;
// using glm::radians;
using glm::dvec2;

Snake::Snake(f32 x, f32 y) : Sprite(
    {x, y, 100.0, 100.0}, 
    {255, 0, 0, 255}
)
{

}

Snake::~Snake()
{

}

void Snake::update(const b8* keyboardState, f64 dt) {

    b8 change = false;
    dvec2 force;

    auto& k = keyboardState;
    if      (k[SDL_SCANCODE_W]) { force = {0.0, -1000.0}; change = true; }
    else if (k[SDL_SCANCODE_S]) { force = {0.0, 1000.0};  change = true; }
    else if (k[SDL_SCANCODE_D]) { force = {1000.0, 0.0};  change = true; }
    else if (k[SDL_SCANCODE_A]) { force = {-1000.0, 0.0}; change = true; }
    
    if (change) {
        m_acceleration  = force / m_mass;
        m_velocity     += m_acceleration * dt;
        m_momentum     += force * dt;
    } else { 
        m_acceleration = {0.0, 0.0};
    }

    if (abs(m_velocity.x) < 0.0001  && !change) m_velocity.x = 0.0;
    if (abs(m_velocity.y) < 0.0001  && !change) m_velocity.y = 0.0;

    this->rect.x += m_velocity.x * dt;
    this->rect.y += m_velocity.y * dt;
}