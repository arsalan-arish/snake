#include <SDL3/SDL.h>
#include <snake/game.h>
#include <snake/types.h>


Game::Game()
{
    
    m_prev_time = SDL_NS_TO_SECONDS(SDL_GetTicksNS());
}

Game::~Game()
{

}


void Game::run()
{
    // Game Loop
    while (true)
    {
        // Handle time
        m_current_time = SDL_NS_TO_SECONDS(SDL_GetTicksNS());
        m_delta_time   = m_current_time - m_prev_time;
        m_accumulator += m_delta_time;
        m_prev_time    = m_current_time;

        // Empty the event queue; Process all
        while (!m_event_queue.empty()) {
            if (!m_running) break;
            this->event();
        }

        // Compute & Update the game physics state
        if (!m_running) break;
        while (m_accumulator >= m_update_spf) {
            this->update();
            m_accumulator -= m_update_spf;
        }

        // Render frame graphics from the game state
        if (!m_running) break;
        m_alpha = m_accumulator / m_update_spf;
        this->render();
    }
}


void Game::event()
{

}

void Game::update()
{
    
}

void Game::render()
{

}