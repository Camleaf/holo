#include "SDL3/SDL_gamepad.h"
#include "SDL3/SDL_hints.h"
#include <SDL3/SDL.h>
#include <iostream>
#include <state.h>

SDL_Gamepad* gamepad = nullptr;
mechmania::ControllerState* cState = new mechmania::ControllerState(); 

#define DEADZONE 8000
#define DEBUG false

long map_val(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_GAMEPAD)) {
        std::cerr << "Init failed:" << SDL_GetError() << std::endl;
        return 1;
    }
   
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS,"1");
    bool running = true;


    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;

                case SDL_EVENT_GAMEPAD_ADDED:
                    if (gamepad == nullptr) {
    
                        gamepad = SDL_OpenGamepad(event.gdevice.which);
                        if (gamepad) {
                            std::cout << "connected: " << SDL_GetGamepadName(gamepad) << std::endl;
                        }
                    }
                    break;
                
                case SDL_EVENT_GAMEPAD_REMOVED:
                    SDL_CloseGamepad(gamepad);
                    gamepad = nullptr;
                    std::cout << "gamepad disconnected" << std::endl;


                case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
                    std::cout << "Button pressed: " << (int)event.gbutton.button << std::endl;
                     
                    break;
                
                case SDL_EVENT_GAMEPAD_BUTTON_UP:
                    
                    break;

                case SDL_EVENT_GAMEPAD_AXIS_MOTION:
                        
                    int mapped_move_value = 0;

                    if (SDL_abs(event.gaxis.value) > DEADZONE) {
                        
                        
                        //map from signed 16 bit to signed 10 bit.
                        mapped_move_value = map_val(event.gaxis.value,-1UL<<15,(1UL>>15)-1,-1UL<<9,(1UL>>9)-1);
                        
#if DEBUG
                        std::cout << "Axis " << (int)event.gaxis.axis;
                        std::cout << "moved: " << mapped_move_value << std::endl;
#endif               
                    }
                    cState->axii[event.gaxis.axis] = mapped_move_value;

                    break;
            }
        }
        SDL_Delay(20); //50hz polling loop so my pc dont explode
    }

    if (gamepad) SDL_CloseGamepad(gamepad);
    SDL_Quit();
    return 0;
}
