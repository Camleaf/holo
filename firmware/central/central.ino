#include <Arduino.h>

//#include "ArduinoController.h"
#include "src/drivetrain.h"
#include "src/state.h"
#include "src/orientationprovider.h"
#include <cstdlib>
#include <WiFi.h>
#include <WiFiUdp.h>

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


// Wifi
const char* ssid = "camleaf_device";
const char* password = "";

WiFiUDP udp;
const int port = 8888;

using bt = mechmania::buttons;

mechmania::ControllerState* cState = new mechmania::ControllerState();

mechmania::OrientationProvider* orientStore = new mechmania::GyroMPU6050(); // reserve the sda and scl pins. 21 SDA, 22 SCL
// Only making one of these so should be fine to use new
mechmania::Drivetrain* drivetrain = new mechmania::HeadlessMecanum(kbr1,kbr2,kbl1,kbl2,kfr1,kfr2,kfl1,kfl2,orientStore);

int optionsTimeout = 0;
void process_commands(){
    drivetrain->updateMotor(
        cState->axii[bt::AXIS_X],
        cState->axii[bt::AXIS_RX],
        cState->axii[bt::AXIS_Y],
        cState->axii[bt::AXIS_RY]
    ); 
                    

    if (millis() - optionsTimeout > 1000 && cState->buttons[bt::SHARE]){ // Share button
        orientStore->setYaw(0);
        optionsTimeout = millis();
    }
}



bool check_for_packet(){
    if (!udp.available()){
        return false;
    }

    int packetSize = udp.parsePacket();
    if (packetSize != sizeof(*cState)) return false; // corrupted packet case
    

    udp.read((uint8_t*)&cState, sizeof(*cState));   
    Serial.println(cState->axii[bt::AXIS_X]);
    return true;
}



void setup(){

    Serial.begin(115200); 
    orientStore->begin(21,22);
    delay(100);
    orientStore->generate_tuned_values();
    
   
    // setup drivetrain
    optionsTimeout = millis(); 
    drivetrain->setMaxSpeed(maxSpeed);
    drivetrain->setTurnPower(turnPower); 

    drivetrain->invertMotor(0,true);
    drivetrain->invertMotor(3,true);
 
    //start wifi
    WiFi.softAP(ssid, password); 
    Serial.println("Wifi ON");
    Serial.print("ip: ");
    Serial.println(WiFi.softAPIP());
    
    udp.begin(port);

    
    delay(500);
}


void loop(){
    //msgCoproc();
    orientStore->fetch_data(esp_timer_get_time());
    if (check_for_packet()){
        process_commands();
    }
    delay(5);

}
