#pragma once

#include <snake/sprite.h>

class Snake: public Sprite
{
public:
    Snake(u16 x, u16 y);
    ~Snake();

    void update(const b8* keyboardState, f64 delta_time) override;
};