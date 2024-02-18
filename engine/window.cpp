#include "window.h"
#include <SDL2/SDL_video.h>

namespace Engine {
    Window::Window(const char *name, int x, int y, int w, int h) {
        width_ = w;
        height_ = h;
        window_name_ = name;

        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

        // make it so the screen doesn't flicker on KDE
        SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");

        uint32_t WindowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN | SDL_WINDOW_ALLOW_HIGHDPI;
        sdl_window_ = SDL_CreateWindow(name, x, y, w, h, WindowFlags);

        spdlog::debug("Window initialized.");
    }

    Window::~Window() {
        SDL_DestroyWindow(sdl_window_);
        SDL_Quit();
    }

    bool Window::requested_quit() { return requested_quit_; }

    void Window::Clear() { Clear(0.0f, 0.0f, 0.0f, 1.0f); }

    void Window::Clear(float r, float g, float b, float a) {}

    float Window::DeltaTime() { return deltatime_; }

    float Window::ElapsedTime() { return (SDL_GetTicks64()) / 1000.0; }

    void Window::PollEvents() {
        while (SDL_PollEvent(&event_)) {
            switch (event_.type) {
                case SDL_QUIT:
                    requested_quit_ = true;
                    break;

                case SDL_MOUSEMOTION:
                    if (on_mouse_motion_ == nullptr)
                        continue;
                    on_mouse_motion_(event_.motion.x, event_.motion.y, event_.motion.xrel, event_.motion.yrel);
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (on_mousebutton_down_ == nullptr)
                        continue;
                    on_mousebutton_down_(event_.button.x, event_.button.y, event_.button.button);
                    break;

                case SDL_MOUSEBUTTONUP:
                    if (on_mousebutton_up == nullptr)
                        continue;
                    on_mousebutton_up(event_.button.x, event_.button.y, event_.button.button);
                    break;

                case SDL_KEYDOWN:
                    if (on_keydown_ == nullptr)
                        continue;
                    on_keydown_(event_.key.keysym.sym, event_.key.keysym.mod);
                    break;

                case SDL_KEYUP:
                    if (on_keyup_ == nullptr)
                        continue;
                    on_keyup_(event_.key.keysym.sym, event_.key.keysym.mod);
            }
        }
    }

    SDL_Window* Window::sdl_window() {
        return sdl_window_;
    }

    uint16_t Window::width() {
        return width_;
    }

    uint16_t Window::height() {
        return height_;
    }

    void Window::SetMouseMotionCallback(void (*callback)(int, int, int, int)) {
        on_mouse_motion_ = callback;
    }

    void Window::SetMouseButtonDownCallback(void (*callback)(int, int, Uint8)) {
        on_mousebutton_down_ = callback;
    }

    void Window::SetMouseButtonUpCallback(void (*callback)(int, int, Uint8)) {
        on_mousebutton_up = callback;
    }

    void Window::SetKeyDownCallback(void (*callback)(SDL_Keycode key,
                                                     Uint16 modifier)) {
        on_keydown_ = callback;
    }

    void Window::SetKeyUpCallback(void (*callback)(SDL_Keycode key,
                                                   Uint16 modifier)) {
        on_keyup_ = callback;
    }
}
