#include <snake/sprites/snake.h>


Snake::Snake(f32 x, f32 y) : Sprite(
    {x, y, 200, 50}, 
    {255, 0, 0, 255}
)
{

}

Snake::~Snake()
{

}

void Snake::update(const b8* keyboardState, f64 delta_time) {
    auto& k = keyboardState;

    if (k[SDL_SCANCODE_W]) rect.y -= m_speed * delta_time;
    if (k[SDL_SCANCODE_S]) rect.y += m_speed * delta_time;
    if (k[SDL_SCANCODE_D]) rect.x += m_speed * delta_time;
    if (k[SDL_SCANCODE_A]) rect.x -= m_speed * delta_time;
}