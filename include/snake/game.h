#pragma once
#include <vector>

class Game
{
public:
    Game();
    ~Game();

    void loop();

private:
    bool m_running = true;
    std::vector<int> m_event_queue = {};

    void event();
    void iterate();
};