#pragma once

#include <snake/sprite.h>

class Snake: public Sprite
{
public:
    Snake(f32 x, f32 y);
    ~Snake();

    void update(const b8* keyboardState, f64 delta_time) override;

private:
    const f64 m_mass = 1.0;
    f64 m_velocity = 0.0;
    f64 m_momentum = m_mass * m_velocity;
    f64 m_acceleration = 0.0;
};