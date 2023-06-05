#include "functions.h"

// IMPORTANT! To implementation of each module:

void Functions::initPeripherals() {

    pinMode(DETECTION_MOSFET_PIN, OUTPUT);
    digitalWrite(DETECTION_MOSFET_PIN, MOSFET_OFF);

    pinMode(LED_GREEN, OUTPUT);
    digitalWrite(LED_GREEN, 0);

    pinMode(LED_YELLOW, OUTPUT);
    digitalWrite(LED_YELLOW, 0);

}

/**********************************************************************************************/

// To implementation of each module:

void Functions::measure() {
    moduleData.dataToObc.wakenUp = Functions::stateInWakenUp() || moduleData.payloadState == WRITE || moduleData.payloadState == READY;
    lg.info("Payload awake: "+ String(moduleData.dataToObc.wakenUp), "power");

    float vbat = getVBatt();
    moduleData.dataToObc.batteryVoltage = vbat;

    if(vbat < 6.5) {
        lg.error("Battery voltage: "+ String(moduleData.dataToObc.batteryVoltage) + " V", "power");
    } else if (vbat < 7.0){
        lg.warn("Battery voltage: "+ String(moduleData.dataToObc.batteryVoltage) + " V", "power");
    } else {
        lg.info("Battery voltage: "+ String(moduleData.dataToObc.batteryVoltage) + " V", "power");
    }
}

void Functions::internalStateManagement(){
    detectionPowerMgmt();

    if(moduleData.obcState == INIT) {
        changePayloadStateToAwait();
    }

    if(moduleData.obcState == IDLE) {
        changePayloadStateToAwait();
    }

    if(moduleData.obcState == RECOVERY_ARM){
        changePayloadStateToAwait();
    }

    if(moduleData.obcState == FUELING){
        changePayloadStateToAwait();
    }

    if(moduleData.obcState == ARMED_TO_LAUNCH){
        changePayloadStateToAwait();
    }

    if(moduleData.obcState == RDY_TO_LAUNCH){
        changePayloadStateToReady();
    }

    if(moduleData.obcState == COUNTDOWN){
        changePayloadStateToWrite();
    }

    if(moduleData.obcState == FLIGHT){
        changePayloadStateToWrite();
    }

    if(moduleData.obcState == FIRST_STAGE_RECOVERY){
        changePayloadStateToWrite();
    }

    if(moduleData.obcState == SECOND_STAGE_RECOVERY){
        changePayloadStateToWrite();
    }

    if(moduleData.obcState == ON_GROUND){
        changePayloadStateToAwaitOnGround();
    }

    if(moduleData.obcState == HOLD){
        changePayloadStateToAwait();
    }

    if(moduleData.obcState == ABORT){
        changePayloadStateToAwait();
    }

    sendStatus();
    lg.info("Payload state is '" + String(payloadInternalStateNames[moduleData.payloadState]) + "' ("+moduleData.payloadState+")", "state");
}

void Functions::changePayloadStateToAwaitOnGround(){
    if(moduleData.payloadState == AWAIT){
        return;
    }

    lg.info("changePayloadStateToAwaitOnGround", "state");
    if(moduleData.payloadState == WRITE){
        if(!moduleData.writeStopInProgress){
            moduleData.writeStopInProgress = true;
            moduleData.millisFromChangeWriteState = millis();
            lg.info("In LONG time payload state will change to READY", "state");
            return;
        } else if(millis() - moduleData.millisFromChangeWriteState < GROUND_GRACE_PERIOD_WRITE){
            return;
        }
        moduleData.writeStopInProgress = false;
        moduleData.payloadState = READY;
    }

    if(moduleData.payloadState == READY){
        if(!moduleData.readyStopInProgress){
            moduleData.readyStopInProgress = true;
            moduleData.millisFromChangeReadyState = millis();
            lg.info("In short time payload state will change to AWAIT", "state");
            return;
        } else if(millis() - moduleData.millisFromChangeWriteState < GROUND_GRACE_PERIOD_READY){
            return;
        }
        moduleData.readyStopInProgress = false;
        moduleData.payloadState = AWAIT;
    }
}

void Functions::changePayloadStateToReady(){
    if(moduleData.payloadState == READY){
        return;
    }

    lg.info("changePayloadStateToReady", "functions");
    if(moduleData.payloadState == WRITE){
        if(!moduleData.writeStopInProgress){
            moduleData.writeStopInProgress = true;
            moduleData.millisFromChangeWriteState = millis();
            lg.info("In short time payload state will change to READY", "state");
            return;
        } else if(millis() - moduleData.millisFromChangeWriteState < PLD_STATE_CH_DELAY){
            return;
        }
        moduleData.writeStopInProgress = false;
        moduleData.payloadState = READY;
    } else {
        moduleData.payloadState = READY;
    }
}

void Functions::changePayloadStateToAwait(){
    if(moduleData.payloadState == AWAIT){
        return;
    }

    lg.info("changePayloadStateToAwait", "functions");
    if(moduleData.payloadState == WRITE){
        if(!moduleData.writeStopInProgress){
            moduleData.writeStopInProgress = true;
            moduleData.millisFromChangeWriteState = millis();
            lg.info("In short time payload state will change to READY", "state");
            return;
        } else if (millis() - moduleData.millisFromChangeWriteState < PLD_STATE_CH_DELAY){
            return;
        }
        moduleData.writeStopInProgress = false;
        moduleData.payloadState = READY;
    } else if(moduleData.payloadState == READY){
        if(!moduleData.readyStopInProgress){
            moduleData.readyStopInProgress = true;
            moduleData.millisFromChangeReadyState = millis();
            lg.info("In short time payload state will change to AWAIT", "state");
            return;
        } else if (millis() - moduleData.millisFromChangeWriteState < PLD_STATE_CH_DELAY){
            return;
        }
        moduleData.readyStopInProgress = false;
        moduleData.payloadState = AWAIT;
    }
}

void Functions::changePayloadStateToWrite(){
    if(moduleData.payloadState == WRITE){
        return;
    }

    lg.info("changePayloadStateToWrite", "functions");
    moduleData.writeStopInProgress = false;
    moduleData.readyStopInProgress = false;
    moduleData.payloadState = WRITE;
}

float Functions::getVBatt(){

    uint16_t adcAvg = analogRead(VBAT_PIN);

    for(uint8_t i = 0; i<9; i++){
        adcAvg = (adcAvg + analogRead(VBAT_PIN))/2;
    }

    return ((adcAvg*3.3)/4095) * 3.539823;
}

void Functions::detectionPowerMgmt(){
    if(stateInWakenUp() || sleepBlockedByPayloadState()){
        lg.info("Detection power ON", "power");
        digitalWrite(DETECTION_MOSFET_PIN, MOSFET_ON);
    } else {
        lg.info("Detection power OFF", "power");
        digitalWrite(DETECTION_MOSFET_PIN, MOSFET_OFF);
    }
}

/**********************************************************************************************/

// To implementation of each module:

void Functions::rxNowHandler(const uint8_t *incomingData, int len) {

    DataFromObc rxData;
    if (len == sizeof(rxData)) {

        memcpy((uint8_t*) &rxData, incomingData, len);

        // No need for power module
    }
}

// IMPORTANT END!

/**********************************************************************************************/
/**********************************************************************************************/
/**********************************************************************************************/

uint32_t Functions::getPowerTime_ms() {

    timeval tv;
    timezone tz;

    gettimeofday(&tv, (void*) &tz);
    return tv.tv_sec * 1000 + tv.tv_usec/1000;
}

/**********************************************************************************************/

void Functions::goToSleep() {

    lg.warn("Going to sleep!", "power");
    esp_sleep_enable_timer_wakeup((SLEEP_TIME_MS) * 10e2);
    esp_deep_sleep_start();
}

/**********************************************************************************************/

bool Functions::stateInWakenUp() {

    return moduleData.stateTimes[moduleData.obcState] != SLEEP_TIME_MS;
}

bool Functions::sleepBlockedByPayloadState() {
    return moduleData.payloadState == WRITE || moduleData.payloadState == READY;
}

/**********************************************************************************************/

void Functions::checkSleepThings() {

    if (!stateInWakenUp() && !sleepBlockedByPayloadState()) {
        lg.warn("Waiting for OBC state...", "state");
        vTaskDelay(ANSWER_WAIT_MS / portTICK_PERIOD_MS);

        if (!stateInWakenUp()) {
            esp_now_send(adressObc, (uint8_t *) &moduleData.dataToObc, sizeof(moduleData.dataToObc));
            goToSleep();
        }
    }
}