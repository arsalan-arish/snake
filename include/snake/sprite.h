/* Interface for each sprite to implement */
#pragma once
#include <snake/types.h>


class Sprite
{
protected:
    Sprite(SDL_FRect rect, SDL_Color color) : 
        rect(rect),
        color(color) 
    {};

    f32 m_speed = 450; // pix/sec
    
public:
    SDL_FRect   rect;  // A sprite implementation must not initialize x & y manually, only recieve it in its constructor and set.
    SDL_FRect   prev_rect;
    SDL_Color   color;

    virtual void update(const b8* keyboardState, f64 delta_time) = 0;
    virtual ~Sprite() = default;
};