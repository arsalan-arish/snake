#include <snake/sprites/snake.h>


Snake::Snake(u16 x, u16 y) : Sprite(200, 50, x, y, {255, 0, 0, 255})
{

}

Snake::~Snake()
{

}

void Snake::update(const b8* keyboardState, f64 delta_time) {
    auto& k = keyboardState;

    if (k[SDL_SCANCODE_W]) pos.y -= m_speed * delta_time;
    if (k[SDL_SCANCODE_S]) pos.y += m_speed * delta_time;
    if (k[SDL_SCANCODE_D]) pos.x += m_speed * delta_time;
    if (k[SDL_SCANCODE_A]) pos.x -= m_speed * delta_time;
}