#pragma once

#include <snake/sprite.hpp>
#include <snake/types.h>
#include <glm/glm.hpp>

using glm::dvec2;

class Snake: public Sprite
{
public:
    Snake(f32 x, f32 y);
    ~Snake();

    void update(const b8* keyboardState, f64 dt) override;

private:
    const f64 m_mass = 1.0;
    dvec2 m_velocity {0.0};
    dvec2 m_acceleration {0.0};
    dvec2 m_momentum {m_velocity * m_mass};
};