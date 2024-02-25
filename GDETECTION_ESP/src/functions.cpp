#include "functions.h"

// IMPORTANT! To implementation of each module:

void Functions::initPeripherals() {

    pinMode(LED_SWITCH, OUTPUT);
    pinMode(DATA_COLLECTION_READY, INPUT_PULLDOWN);
    pinMode(DATA_SAVE_PERMISSION, INPUT_PULLDOWN);

    ledcSetup(0, 400, 8);
    ledcAttachPin(LED_SWITCH, 0);
    ledcWrite(0, 0);

    ledcWrite(0, 255);
    vTaskDelay(200 / portTICK_PERIOD_MS);
    ledcWrite(0, 0);

}

/**********************************************************************************************/

// To implementation of each module:

void Functions::measure() {

    ledcWrite(0, 0);
    vTaskDelay(600 / portTICK_PERIOD_MS);

    measureForLevel(1);
    fcn.sdLogger.push();
    fcn.flashLogger.push();
    logADC();

    measureForLevel(32);
    fcn.sdLogger.push();
    fcn.flashLogger.push();
    logADC();

    measureForLevel(128);
    fcn.sdLogger.push();
    fcn.flashLogger.push();
    logADC();
    
    measureForLevel(255);
    fcn.sdLogger.push();
    fcn.flashLogger.push();
    logADC(); 

}

void Functions::measureForLevel(uint8_t PWMLevel){
    ledcWrite(0, PWMLevel);
    vTaskDelay(200 / portTICK_PERIOD_MS);

    moduleData.dataToSave.adcMeasurement.PWMLevel = PWMLevel;
    moduleData.dataToSave.adcMeasurement.gpio_34 = analogReadMultisample10(ADC_GPIO34);
    moduleData.dataToSave.adcMeasurement.gpio_35 = analogReadMultisample10(ADC_GPIO35);
    moduleData.dataToSave.adcMeasurement.gpio_32 = analogReadMultisample10(ADC_GPIO32);
    moduleData.dataToSave.adcMeasurement.gpio_33 = analogReadMultisample10(ADC_GPIO33);
    moduleData.dataToSave.adcMeasurement.gpio_25 = analogReadMultisample10(ADC_GPIO25);
    moduleData.dataToSave.adcMeasurement.gpio_26 = analogReadMultisample10(ADC_GPIO26);
    moduleData.dataToSave.adcMeasurement.gpio_27 = analogReadMultisample10(ADC_GPIO27);

}

uint16_t Functions::analogReadMultisample10(int channel){

    uint32_t sum = 0;

    uint16_t min = 4096;
    uint16_t max = 0;

    for (int i = 0; i<10; i++){
        uint16_t adcValue = analogRead(channel);
        sum += adcValue;
        if (adcValue > max){
            max = adcValue;
        }
        if (adcValue < min) {
            min = adcValue;
        }
    }

    return (uint16_t) (sum - min - max)/8;

}

void Functions::logADC(){
    char adcLog[220];
    sprintf(adcLog, "ADC: %u, %u, %u, %u, %u, %u, %u, %u",
        moduleData.dataToSave.adcMeasurement.PWMLevel,
        moduleData.dataToSave.adcMeasurement.gpio_34,
        moduleData.dataToSave.adcMeasurement.gpio_35,
        moduleData.dataToSave.adcMeasurement.gpio_32,
        moduleData.dataToSave.adcMeasurement.gpio_33,
        moduleData.dataToSave.adcMeasurement.gpio_25,
        moduleData.dataToSave.adcMeasurement.gpio_26,
        moduleData.dataToSave.adcMeasurement.gpio_27
    );
    
    lg.info(adcLog, "adc");
}

void Functions::blink() {

    ledcWrite(0, 255);
    vTaskDelay(5 / portTICK_PERIOD_MS);
    ledcWrite(0, 0);
    vTaskDelay(1200 / portTICK_PERIOD_MS);
}

uint32_t Functions::getPowerTime_ms() {

    timeval tv;
    timezone tz;

    gettimeofday(&tv, (void*) &tz);
    return tv.tv_sec * 1000 + tv.tv_usec/1000;
}