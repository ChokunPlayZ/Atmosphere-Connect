#include "HAIntegration.h"
#include "Credentials.h"

#include <ArduinoHA.h>
#include <WiFi.h>

//Adapted via:
//  https://github.com/dawidchyrzynski/arduino-home-assistant/blob/main/examples/nano33iot/nano33iot.ino

WiFiClient client;
HADevice device;
HAMqtt mqtt(client, device);
HAFan fan("fan", HAFan::SpeedsFeature);

int lastspeed = 0;
bool gstate = false;

void HAIntegration::configure() {

    Serial.println("Setting Up Home Assistant, Hang tight!");

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(0, OUTPUT);
    
    //Set device ID as MAC address

    byte mac[WL_MAC_ADDR_LENGTH];
    WiFi.macAddress(mac);
    device.setUniqueId(mac, sizeof(mac));

    //Device metadata:

    device.setName("Atmosphere Connect");
    device.setSoftwareVersion("2.0");

    // handle switch state
    fan.setName("Speed");
    fan.setSpeedRangeMin(1);
    fan.setSpeedRangeMax(50);

    fan.onStateCommand(stateHandler);
    fan.onSpeedCommand(speedHandler);

    Serial.print("Connecting to MQTT\n");
    
    if (mqtt.begin(MQTT_BROKER, MQTT_LOGIN, MQTT_PASSWORD) == true) {
        Serial.print("Connected to MQTT broker");
    } else {
        Serial.print("Could not connect to MQTT broker");
    }
}

void HAIntegration::stateHandler(bool state, HAFan* sender) {
    digitalWrite(LED_BUILTIN, HIGH);
    if (!state)
    {
        analogWrite(0, 0);
        gstate = false;
    } else {
        analogWrite(0, map(lastspeed, 0, 100, 0, 255));
        gstate = true;
    }
    sender->setState(state);  // report state back to Home Assistant
    digitalWrite(LED_BUILTIN, LOW);
}

void HAIntegration::speedHandler(uint16_t speed, HAFan* sender) {
    digitalWrite(LED_BUILTIN, HIGH);
    if (gstate)
    {
        analogWrite(0, map(speed, 0, 100, 0, 255));
    }
    sender->setSpeed(speed);  // report state back to Home Assistant
    lastspeed = speed;
    digitalWrite(LED_BUILTIN, LOW);
}

void HAIntegration::loop() {
    mqtt.loop();
}
