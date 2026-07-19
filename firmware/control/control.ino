/*
 * Requires bluepad32 installed
 */
#include "ArduinoController.h"
#include <Arduino.h>
#include <cstdlib>
#include <esp_now.h>
#include <WiFi.h>
#include "./state.h"
#include <Bluepad32.h>

const uint8_t destination_mac_address[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // replace with real mac address 

mechmania::RobotState* rState = new mechmania::RobotState();

ControllerPtr gamepads[BP32_MAX_GAMEPADS];

// --- Bluepad32 Callbacks ---
void onConnectedController(ControllerPtr cptr) {
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (gamepads[i] == nullptr) {
            gamepads[i] = cptr;
            break;
        }
    }
}

void onDisconnectedController(ControllerPtr cptr) {
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (gamepads[i] == cptr) {
            gamepads[i] = nullptr;
            break;
        }
    }
}



void setup(){

    Serial.begin(115200);
     


    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    if (esp_now_init() != ESP_OK) {
      Serial.println("Error initializing ESP-NOW");
      return;
    }
    
    //recieve callback
    //esp_now_register_send_cb(send_data);

    /*
    // If I want to directly connect to peer as opposed to packet blasting
    esp_now_peer_info_t peer_info = {};
    memcpy(peer_info.peer_addr, destination_mac_address, 6);
    peer_info.channel = 0;  
    peer_info.encrypt = false;
    esp_now_add_peer(&peer_info);
    */

    BP32.setup(onConnectedController, onDisconnectedController); 
    BP32.forgetBluetoothKeys();
    BP32.enableNewBluetoothConnections(false); // we want only wired connections
}


int last_send;
void loop(){
    BP32.update();
    
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        ControllerPtr cptr = gamepads[i];
        
        if (cptr && cptr->isConnected()) {
            
            rState->buttons = cptr->buttons();
            rState->misc_buttons = cptr->miscButtons();
            rState->dpad = cptr->dpad();
            rState->axii[MM_AXIS_X] = cptr->axisX();
            rState->axii[MM_AXIS_Y] = cptr->axisY();
            rState->axii[MM_AXIS_RX] = cptr->axisRX();
            rState->axii[MM_AXIS_RY] = cptr->axisRY();

            if (millis() - last_send >= 20) { //50hz transmission limit
                last_send = millis();
              
                esp_now_send(destination_mac_address, (uint8_t *) rState, sizeof(*rState));
            }
            break;
        }
    }
}
