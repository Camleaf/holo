#include "SDL3/SDL_gamepad.h"
#include "SDL3/SDL_hints.h"
#include "SDL3/SDL_stdinc.h"
#include <SDL3/SDL.h>
#include <iostream>
#include <state.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

SDL_Gamepad* gamepad = nullptr;
mechmania::ControllerState* cState = new mechmania::ControllerState(); 

using bt = mechmania::buttons;

#define DEADZONE 8000
#define DEBUG false
#define PORT 8888

long map_val(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main(int argc, char* argv[]) {
    std::cout << "program start" << std::endl;
    SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");
    //setup sdl

SDL_SetHint(SDL_HINT_JOYSTICK_HIDAPI, "0");
    if (!SDL_Init(SDL_INIT_GAMEPAD)) {
        std::cerr << "Init failed:" << SDL_GetError() << std::endl;
        return 1;
    }
    std::cout << "gamepad init completed" << std::endl;
    

    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS,"1");
    bool running = true;
    
    // setup netsocket
    std::cout << "start init netsocket" << std::endl;
    // Create UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {

        std::cout << "socket creation failed" << std::endl;
        return 1;
    }

    std::cout << "create addr" << std::endl;
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));

    // Fill server address info
    servaddr.sin_family = AF_INET;              // IPv4
    servaddr.sin_port   = htons(PORT);          // Server port
    servaddr.sin_addr.s_addr = inet_addr("192.168.4.1"); // Server IP
    std::cout << "start main" << std::endl;

    // run sdl mainloop
    SDL_Event event;
    while (running) {
        // send over network
        sendto(sockfd,(const char*)cState,sizeof(*cState),0,
                  (const struct sockaddr *)&servaddr, sizeof(servaddr));

        // handle events
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
                    
                    // Empty buttonState on disconnect
                    for (int i = 0; i< std::size(cState->buttons);i++){
                        cState->buttons[i] = false;
                    }

                    std::cout << "gamepad disconnected" << std::endl;
                    break;

                case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
                    std::cout << "Button pressed: " << (int)event.gbutton.button << std::endl;
                    cState->buttons[event.gbutton.button] = true; 
                    break;
                
                case SDL_EVENT_GAMEPAD_BUTTON_UP:
                    
                    cState->buttons[event.gbutton.button] = false; 
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
