#include <Arduino.h>
#include <cstdlib>
#include <esp_now.h>
#include <WiFi.h>
#include "./state.h"



const uint8_t destination_mac_address[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // replace with real mac address 


void setup(){

    Serial.begin(115200);
     
    delay(500);


    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
      Serial.println("Error initializing ESP-NOW");
      return;
    }
    
    //recieve callback
    esp_now_register_recv_cb(recieve_data);
}


void loop(){
    delay(5);

}
