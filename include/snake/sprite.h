/* Interface for each sprite to implement */
#pragma once
#include <snake/types.h>

struct pos {
    f32 x;
    f32 y;
};

struct rect {
    f32 w;
    f32 h;
};

class Sprite
{
protected:
    Sprite(f32 width, f32 height, f32 x, f32 y, SDL_Color color) : 
        rect({width, height}),
        pos({x, y}),
        color(color) 
    {};

    f32 m_speed = 450; // pix/sec
    
public:
    SDL_Color   color;
    struct rect rect;
    struct pos  pos;
    struct pos  prev_pos;

    virtual void update(const b8* keyboardState, f64 delta_time) = 0;
    virtual ~Sprite() = default;
};