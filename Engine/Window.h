    #pragma once

// STD INCLUDES

#include <map>
#include <vector>

//MY INCLUDES

#include <SDL.h>

namespace Engine {
    class Window {
    public:
        Window(const char *name, int x, int y, int w, int h);
        ~Window();

        bool requested_quit();

        void Clear();
        void Clear(float r, float g, float b, float a);

        // Change in time from previous frame to current in seconds.
        float DeltaTime();
        // Elapsed time since SDL_Library initialization in seconds.
        float ElapsedTime();

        void PollEvents();

        SDL_Window* sdl_window();

        //TODO! replace with event architecture thing.

        /* PARAMS:
         * int x: abs
         * int y: abs
         * int xrel: relative
         * int yrel: relative */
        void SetMouseMotionCallback(void (*callback)(int x, int y, int xrel, int yrel));
        /* PARAMS:
         * int x: abs
         * int y: abs
         * Uint8 button (could either be SDL_BUTTON_RIGHT or SDL_BUTTON_LEFT*/
        void SetMouseButtonDownCallback(void (*callback)(int x, int y, Uint8 button));
        /* PARAMS:
         * int x: abs
         * int y: abs
         * Uint8 button (could either be SDL_BUTTON_RIGHT or SDL_BUTTON_LEFT*/
        void SetMouseButtonUpCallback(void (*callback)(int x, int y, Uint8 button));
        /* PARAMS:
         * SDL_Keycode key: the SDL_Keycode representing the pressed key
         * Uint16 modifier: the modifier flags (e.g., ctrl is down, win is down...)
         */
        void SetKeyDownCallback(void (*callback)(SDL_Keycode key, Uint16 modifier));

        /* PARAMS:
         * SDL_Keycode key: the SDL_Keycode representing the released key
         * Uint16 modifier: the modifier flags (e.g., ctrl is down, win is down...)
         */
        void SetKeyUpCallback(void (*callback)(SDL_Keycode key, Uint16 modifier));

    private:
        //WINDOW PROPERTIES & INTERNALS
        uint16_t width_;
        uint16_t height_;
        const char* window_name_;
        SDL_Window* sdl_window_;
        SDL_Event event_;

        //OTHER ATTRIBUTES
        bool requested_quit_{false};
        Uint64 now_, last_;
        float deltatime_;

        //CALLBACK DECLARATIONS
        void (*on_mouse_motion_)(int x, int y, int xrel, int yrel){nullptr};
        void (*on_mousebutton_down_)(int x, int y, Uint8 button){nullptr};
        void (*on_mousebutton_up)(int x, int y, Uint8 button){nullptr};
        void (*on_keydown_)(SDL_Keycode key, Uint16 modifier){nullptr };
        void (*on_keyup_)(SDL_Keycode key, Uint16 modifier){nullptr };

    };
}