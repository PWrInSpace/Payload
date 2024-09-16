#include "functions.h"

Globals glob;

bool isWritingMode() { return 1/*glob.rotcketState >= 6*/; };

/*************************************************************************************************/

void writeData() {

    blinkNTimes(3);

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC_X_CHANNEL, ADC_ATTEN_DB_12);
    adc1_config_channel_atten(ADC_Y_CHANNEL, ADC_ATTEN_DB_12);
    adc1_config_channel_atten(ADC_Z_CHANNEL, ADC_ATTEN_DB_12);

    File file;
    if (/*glob.rotcketState ==*/ 6) {

        // Remove old data:
        SPIFFS.remove("/data.bin");
        file = SPIFFS.open("/data.bin", "w", true);
        file.close();
    }

    Frame frame;
    uint32_t timer2;

    while(1) {

        // Measure data:
        for (uint16_t i = 0; i < DATA_IN_FRAME; i++) {

            timer2 = micros();

            frame.adcX[i] = adc1_get_raw(ADC_X_CHANNEL) >> 4;
            frame.adcY[i] = adc1_get_raw(ADC_Y_CHANNEL) >> 4;
            frame.adcZ[i] = adc1_get_raw(ADC_Z_CHANNEL) >> 4;

            while (micros() - timer2 < 100);
        }
        // Push to save que:
        //xQueueSend(glob.dataFramesFifo, &frame, portMAX_DELAY);
        //Serial.println(uxQueueMessagesWaiting(glob.dataFramesFifo));

        digitalWrite(LED_PIN, 0);
        file = SPIFFS.open("/data.bin", "a", true);

        file.write((uint8_t*) &frame, sizeof(frame));

        file.close();
        digitalWrite(LED_PIN, 1);

        vTaskDelay(1);
    }
}

/*************************************************************************************************/

String generateHtml() {

    return String("<a href=\"/data.bin\">data.bin</a>");
}

void readData() {

    String ssid = String("PWrinSpace_payload");
    const char* password = "12345678";
    AsyncWebServer server = AsyncWebServer(80);

    WiFi.softAP(ssid.c_str(), password);
    MDNS.begin("payload");

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {

        request->send(200, "text/html", generateHtml());
    });

    server.on("/data.bin", HTTP_GET, [](AsyncWebServerRequest *request) {

        request->send(SPIFFS, "/data.bin", String());
    });

    server.begin();
    Serial.println("server on");

    blinkNTimes(2);

    while(1) {
        vTaskDelay(10);
    }
}

/*************************************************************************************************/

void uartCommTask() {

    Serial1.begin(115200, SERIAL_8N1, RX_COMM_PIN, TX_COMM_PIN);
    Serial1.setTimeout(10);

    while(1) {

        if (Serial1.available()) {
            String rxData = Serial1.readString();
            glob.rotcketState = rxData.toInt();
        }
        vTaskDelay(10);
    }
}

/*************************************************************************************************/

void flashTask() {

    File file;

    /*while (glob.rotcketState < 6) {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }*/

    while (1) {

        if (uxQueueMessagesWaiting(glob.dataFramesFifo) > 2) {

            digitalWrite(LED_PIN, 0);
            file = SPIFFS.open("/data.bin", "a", true);

            while (uxQueueMessagesWaiting(glob.dataFramesFifo)) {

                Frame tempData;
                xQueueReceive(glob.dataFramesFifo, &tempData, portMAX_DELAY);
                file.write((uint8_t*) &tempData, sizeof(tempData));
            }

            file.close();
            digitalWrite(LED_PIN, 1);
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

/*************************************************************************************************/

void blinkNTimes(uint8_t n) {

    for (uint8_t i = 0; i < n; i++) {

        digitalWrite(LED_PIN, 0);
        vTaskDelay(100);
        digitalWrite(LED_PIN, 1);
        vTaskDelay(100);
    }
}