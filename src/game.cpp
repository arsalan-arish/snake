#include <SDL3/SDL.h>
#include <format>
#include <string>
#include <memory>

#include <snake/game.h>
#include <snake/types.h>
#include <snake/sprites/snake.h>


using std::string;
using std::format;
using std::runtime_error;
using std::make_unique;


Game::Game()
{
    if (!SDL_Init(
        SDL_INIT_VIDEO |
        SDL_INIT_EVENTS |
        SDL_INIT_AUDIO
    )) {
        throw runtime_error(format("SDL Failed to Initialize: {}", SDL_GetError()));
    }

    if (!(m_window = SDL_CreateWindow("Snake", m_width, m_height,
        SDL_WINDOW_MAXIMIZED | 
        SDL_WINDOW_RESIZABLE | 
        SDL_WINDOW_INPUT_FOCUS | 
        SDL_WINDOW_MOUSE_FOCUS
    ))) {
        throw runtime_error(format("Window could not be created {}", SDL_GetError()));
    }

    if (!(m_renderer = SDL_CreateRenderer(m_window, nullptr))) {
        throw runtime_error(format("Renderer creation failed: {}", SDL_GetError()));
    };

    if (!SDL_SetRenderVSync(m_renderer, 1)) {
        SDL_Log("Could not set up VSync: %s", SDL_GetError());
    }

    m_sprites.emplace_back(
        make_unique<Snake>(m_width / 2, m_height / 2)
    );

    m_prev_time = SDL_GetTicksNS() / 1e9;
}

Game::~Game()
{
    if (m_window) SDL_DestroyWindow(m_window);
    if (m_renderer) SDL_DestroyRenderer(m_renderer);
    if (SDL_WasInit(0)) SDL_Quit();
}


void Game::run()
{
    // Game Loop
    while (true)
    {
        // Handle time
        m_current_time = SDL_GetTicksNS() / 1e9;
        m_delta_time   = m_current_time - m_prev_time;
        m_accumulator += m_delta_time;
        m_prev_time    = m_current_time;

        // Empty the event queue; Process all
        while (SDL_PollEvent(&m_event)) {
            if (!m_running) break;
            this->event();
        }

        // Compute & Update the game physics state
        if (!m_running) break;
        while (m_accumulator >= m_update_spf) {
            this->update();
            m_accumulator -= m_update_spf;
        }
        m_alpha = m_accumulator / m_update_spf;

        // Render frame graphics from the game state
        if (!m_running) break;
        this->render();
    }
}


void Game::event()
{
    switch (m_event.type) {
        case SDL_EVENT_QUIT:
            m_running = false;
            break;
        default:
            break;
    }
}

void Game::update()
{
    const b8* keyboardState = SDL_GetKeyboardState(nullptr);
    // Async/Parallelize this loop because most spent time is waiting
    for (auto& sprite : m_sprites) {
        sprite->prev_rect = sprite->rect;
        sprite->update(keyboardState, m_update_spf);
    }
}

void Game::render()
{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    for (auto& sprite : m_sprites) {
        SDL_SetRenderDrawColor(m_renderer, sprite->color.r, sprite->color.g, sprite->color.b, sprite->color.a);
        SDL_FRect rect = {
            sprite->rect.x + (sprite->rect.x - sprite->prev_rect.x) * m_alpha,
            sprite->rect.y + (sprite->rect.y - sprite->prev_rect.y) * m_alpha,
            sprite->rect.w,
            sprite->rect.h
        };
        SDL_RenderFillRect(m_renderer, &rect);
    }
    SDL_RenderPresent(m_renderer);
}
