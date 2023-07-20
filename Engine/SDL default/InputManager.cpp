#include "InputManager.h"
#include "EventManager.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl.h>
#include <imgui_sdl.h>
#include <imgui_internal.h>


InputManager* InputManager::instance = NULL;


InputManager::InputManager()
{
    eventManager = EventManager::GetInstance();
}

void InputManager::Update()
{
    //Poll for keyboard and mouse events and queue the correct events into the input manager
    while (SDL_PollEvent(&m_event) != NULL) {
        if (m_event.type == SDL_QUIT)
            eventManager->QueueEvent(new QuitEvent());
        else if (m_event.type == SDL_TEXTINPUT)
            ImGui::GetIO().AddInputCharactersUTF8(m_event.text.text);
        else if (m_event.type == SDL_MOUSEBUTTONDOWN && m_event.button.button == SDL_BUTTON_LEFT) 
        {
            int x;
            int y;
            SDL_GetMouseState(&x, &y);
            eventManager->QueueEvent(new LMBInput(true, x, y));
        }
        else if (m_event.type == SDL_KEYDOWN) {
            SDL_Keycode keyPressed = m_event.key.keysym.sym;
            switch (keyPressed)
            {
            case SDLK_ESCAPE:
                eventManager->QueueEvent(new EscapeInput(true));
                break;
            case SDLK_LEFT:
            case SDLK_a:
                eventManager->QueueEvent(new LeftInput(true));
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                eventManager->QueueEvent(new RightInput(true));
                break;
            case SDLK_UP:
            case SDLK_w:
                eventManager->QueueEvent(new UpInput(true));
                break;
            case SDLK_DOWN:
            case SDLK_s:
                eventManager->QueueEvent(new DownInput(true));
                break;
            case SDLK_SPACE:
                eventManager->QueueEvent(new JumpInput(true));
                break;
            }
        }
        else if (m_event.type == SDL_KEYUP) {
            SDL_Keycode keyPressed = m_event.key.keysym.sym;

            switch (keyPressed)
            {
            case SDLK_LEFT:
            case SDLK_a:
                eventManager->QueueEvent(new LeftInput(false));
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                eventManager->QueueEvent(new RightInput(false));
                break;
            case SDLK_UP:
            case SDLK_w:
                eventManager->QueueEvent(new UpInput(false));
                break;
            case SDLK_DOWN:
            case SDLK_s:
                eventManager->QueueEvent(new DownInput(false));
                break;
            case SDLK_SPACE:
                eventManager->QueueEvent(new JumpInput(false));
                break;
            }
        }
    }
}

InputManager* InputManager::GetInstance()
{
    if (instance == NULL)
        instance = new InputManager();
    return instance;
}
