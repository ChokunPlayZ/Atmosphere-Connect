#ifndef SRC_HAINTEGRATION
#define SRC_HAINTEGRATION

#include <ArduinoHA.h>

class HAIntegration {
    public:
    void loop();
    void configure();
    static void stateHandler(bool state, HAFan* sender);
    static void speedHandler(uint16_t speed, HAFan* sender);
};

#endif // SRC_HAINTEGRATION
