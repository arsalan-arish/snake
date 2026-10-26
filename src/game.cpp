#include <snake/game.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>

Game::Game()
{
    
}

Game::~Game()
{

}

void Game::loop()
{
    while (m_running)
    {
        while (!m_event_queue.empty()) {
            this->event();
        }
        this->iterate();
    }
}

void Game::event()
{

}

void Game::iterate()
{

}