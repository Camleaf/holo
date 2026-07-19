#include <Arduino.h>

//#include "ArduinoController.h"
#include "src/drivetrain.h"
#include "src/state.h"
#include "src/orientationprovider.h"
#include <HardwareSerial.h>
#include <cstdlib>
#include <esp_now.h>
#include <WiFi.h>


//// drivetrain
// Back right
#define kbr1 12
#define kbr2 13
// Back left
#define kbl1 26
#define kbl2 27
// Front right
#define kfr1 18
#define kfr2 19
// Front left
#define kfl1 15
#define kfl2 5

// Misc vars
#define maxSpeed 140
#define turnPower 140


const uint8_t controller_mac_address[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // replace with real mac address 

mechmania::RobotState* rState = new mechmania::RobotState();

mechmania::OrientationProvider* orientStore = new mechmania::GyroMPU6050(); // reserve the sda and scl pins. 21 SDA, 22 SCL
// Only making one of these so should be fine to use new
mechmania::Drivetrain* drivetrain = new mechmania::HeadlessMecanum(kbr1,kbr2,kbl1,kbl2,kfr1,kfr2,kfl1,kfl2,orientStore);

int optionsTimeout = 0;
void process_commands(){
    drivetrain->updateMotor(
        rState->axii[MM_AXIS_X],
        rState->axii[MM_AXIS_RX],
        rState->axii[MM_AXIS_Y],
        rState->axii[MM_AXIS_RY]
    ); 
                    

    if (millis() - optionsTimeout > 1000 && rState->misc_buttons & 0x04){ // Share button
        orientStore->setYaw(0);
        optionsTimeout = millis();
    }
}


bool update = false;
void recieve_data(const esp_now_recv_info* mac, const unsigned char* inc, int len){
    
    // check equality
    if (memcmp(mac->src_addr,controller_mac_address,6)){
      return;
    }

    memcpy(&rState, inc, sizeof(*rState));
}

void setup(){

    Serial.begin(115200);
     
    orientStore->begin(21,22);
    delay(100);
    orientStore->generate_tuned_values();


    optionsTimeout = millis(); 
    drivetrain->setMaxSpeed(maxSpeed);
    drivetrain->setTurnPower(turnPower); 

    drivetrain->invertMotor(0,true);
    drivetrain->invertMotor(3,true);
 
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
    //msgCoproc();
    orientStore->fetch_data(esp_timer_get_time());
    if (update){
        process_commands();
        update = false;
    }

    delay(5);

}
