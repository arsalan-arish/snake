#pragma once

#include <snake/types.h>
#include <snake/screen.hpp>
#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;


class Game
{
public:
    Game();
    ~Game();

    void run();

private:
    vector<int> m_Windows;
    vector<Screen> m_Screens;
    vector<Screen> m_SpecialScreens;
    vector<int> m_HierarchyTree;
    Screen         m_ActiveScreen;
    Screen         m_LoadingScreen;

    /* Game loop state */
    int              m_event;
    b8               m_running     = true;
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