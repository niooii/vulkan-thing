#pragma once

// STD INCLUDES

#include <map>
#include <vector>

//MY INCLUDES

#include <SDL.h>

class Window
{
public:

    void create();
    void create(int w, int h, const char* n);
    void close();
    bool shouldRun();

    void clear();
    void clear(float r, float g, float b, float a);

    //Change in time from previous frame to current in seconds.
    float deltaTime();
    //Elapsed time since SDL_Library initialization in seconds.
    float elapsedTime();

    void pollEvents();

    SDL_Window* raw();

    //set useful callback functions

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

    uint16_t width, height;
    const char* name;
    SDL_Window* window;
    bool running{true};
    SDL_Event e;

    //CALLBACK DECLARATIONS
    void (*OnMouseMotion)(int x, int y, int xrel, int yrel){nullptr};
    void (*OnMouseButtonDown)(int x, int y, Uint8 button){nullptr};
    void (*OnMouseButtonUp)(int x, int y, Uint8 button){nullptr};
    void (*OnKeyDown)(SDL_Keycode key, Uint16 modifier){ nullptr };
    void (*OnKeyUp)(SDL_Keycode key, Uint16 modifier){ nullptr };

    //temp stuff for testing

    //for deltaTime
    Uint64 NOW, LAST;
    float dt;
};