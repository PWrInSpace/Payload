#include "main.h"

void setup() {

    pinMode(DETECTION_MOSFET_PIN, OUTPUT);
    digitalWrite(DETECTION_MOSFET_PIN, MOSFET_OFF);

    delay(100);
    lg.setLogLevel(INFO);

    fcn.initPeripherals();

    nowInit();
    nowAddPeer(adressObc, 0);

    loopTimer = SLEEP_TIME_MS;

    lg.info("Main setup finished", "main");
}

void loop() {

    if ( 
        (millis() - loopTimer >= moduleData.stateTimes[moduleData.obcState]) || (millis() - loopTimer >= 1000)
    ) {

        loopTimer = millis();
        fcn.measure();
        fcn.internalStateManagement();
        esp_now_send(adressObc, (uint8_t *) &moduleData.dataToObc, sizeof(moduleData.dataToObc));
        
    }

    vTaskDelay(1 / portTICK_PERIOD_MS);
    // Check things related to sleep mode and eventually go to sleep:
    fcn.checkSleepThings();

}
