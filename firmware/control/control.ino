#include <Arduino.h>
#include <cstdlib>
#include <esp_now.h>
#include <WiFi.h>
#include "./state.h"
#include <EspUsbHost.h>


mechmania::RobotState* rState = new mechmania::RobotState();

const uint8_t destination_mac_address[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // replace with real mac address 


void send_data(const wifi_tx_info_t* tx_info, esp_now_send_status_t status){

}


void setup(){

    Serial.begin(115200);
     


    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
      Serial.println("Error initializing ESP-NOW");
      return;
    }
    
    //recieve callback
    esp_now_register_send_cb(send_data);
}


void loop(){

}
