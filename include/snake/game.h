#pragma once
#include <vector>
#include <snake/types.h>

class Game
{
public:
    Game();
    ~Game();

    void run();

private:
    /* Game loop state */
    b8               m_running     = true;
    std::vector<int> m_event_queue = {};
    f64              m_prev_time;
    f64              m_current_time;
    f64              m_delta_time  = 0;
    f64              m_accumulator = 0.0;
    f32              m_alpha;
    const f64        m_update_spf  = 1.0 / 60.0; // seconds per frame, or fixed time-step delta time, == 60 update_fps
    

    /* Game loop callbacks */
    void event();   // Must be lightweight 
    void update();  // Does the heavylifting compute
    void render();  // Graphics Pipeline
};