#include "HAIntegration.h"
#include "Credentials.h"

#include <ArduinoHA.h>
#include <WiFi.h>

// Adapted via:
//   https://github.com/dawidchyrzynski/arduino-home-assistant/blob/main/examples/nano33iot/nano33iot.ino

WiFiClient client;
HADevice device;
HAMqtt mqtt(client, device);
HAFan fan("fanspeed", HAFan::SpeedsFeature);
HASensorNumber dustSensor("dustsensor");

unsigned long lastUpdateAt = 0;

int lastspeed = 0;
bool gstate = false;

const int dustLed = 2;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

void HAIntegration::configure()
{

    Serial.println("Setting Up Home Assistant, Hang tight!");

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(0, OUTPUT);
    pinMode(dustLed, OUTPUT);

    // Set device ID as MAC address

    byte mac[WL_MAC_ADDR_LENGTH];
    WiFi.macAddress(mac);
    device.setUniqueId(mac, sizeof(mac));

    // Device metadata:

    device.setName("Atmosphere Connect");
    device.setSoftwareVersion("2.0");

    // handle switch state
    fan.setName("Speed");
    fan.setSpeedRangeMin(1);
    fan.setSpeedRangeMax(50);

    fan.onStateCommand(stateHandler);
    fan.onSpeedCommand(speedHandler);

    dustSensor.setName("Dust Density");
    dustSensor.setIcon("mdi:air-filter");
    dustSensor.setUnitOfMeasurement("g");

    Serial.print("Connecting to MQTT\n");

    if (mqtt.begin(MQTT_BROKER, MQTT_LOGIN, MQTT_PASSWORD) == true)
    {
        Serial.print("Connected to MQTT broker");
    }
    else
    {
        Serial.print("Could not connect to MQTT broker");
    }
}

void HAIntegration::stateHandler(bool state, HAFan *sender)
{
    digitalWrite(LED_BUILTIN, HIGH);
    if (!state)
    {
        analogWrite(0, 0);
        gstate = false;
    }
    else
    {
        analogWrite(0, map(lastspeed, 0, 100, 0, 255));
        gstate = true;
    }
    sender->setState(state); // report state back to Home Assistant
    digitalWrite(LED_BUILTIN, LOW);
}

void HAIntegration::speedHandler(uint16_t speed, HAFan *sender)
{
    digitalWrite(LED_BUILTIN, HIGH);
    if (gstate)
    {
        analogWrite(0, map(speed, 0, 100, 0, 255));
    }
    sender->setSpeed(speed); // report state back to Home Assistant
    lastspeed = speed;
    digitalWrite(LED_BUILTIN, LOW);
}

void HAIntegration::loop()
{
    mqtt.loop();

    if ((millis() - lastUpdateAt) > 2000)
    {
        digitalWrite(dustLed, LOW);
        delayMicroseconds(280);

        voMeasured = analogRead(A0);

        delayMicroseconds(40);

        digitalWrite(dustLed, HIGH);
        delayMicroseconds(9680);

        calcVoltage = voMeasured * (3.3 / 1024);
        dustDensity = 0.17 * calcVoltage - 0.1;

        if (dustDensity < 0)
        {
            dustDensity = 0.00;
        }
        dustSensor.setValue(dustDensity);

        Serial.println("Sent Update "+String(dustDensity));

        lastUpdateAt = millis();
    }
}
