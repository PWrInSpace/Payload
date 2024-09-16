#include "functions.h"

Globals glob;

bool isWritingMode() { return 1/*glob.rotcketState >= 6*/; };

/*************************************************************************************************/

void writeData() {

    Serial.println("SER1");
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_12);
    adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_12);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_12);

    if (/*glob.rotcketState ==*/ 6) {

        // Remove old data:
        Serial.println("SER2");
        File file2;
        file2 = SPIFFS.open("/data.bin", "w", true);
        Serial.println("SER3");
        file2.close();
        Serial.println("SER4");
    }

    Frame frame;
    uint32_t timer2;
    Serial.println("SER5");

    while(1) {

        // Measure data:
        for (uint16_t i = 0; i < DATA_IN_FRAME; i++) {

            timer2 = micros();

            frame.adcX[i] = adc1_get_raw(ADC1_CHANNEL_0) >> 4;
            frame.adcY[i] = adc1_get_raw(ADC1_CHANNEL_3) >> 4;
            frame.adcZ[i] = adc1_get_raw(ADC1_CHANNEL_6) >> 4;

            while (micros() - timer2 < 100);
        }
        // Push to save que:
        Serial.println("SER6");
        xQueueSend(glob.dataFramesFifo, &frame, portMAX_DELAY);
        Serial.println(uxQueueMessagesWaiting(glob.dataFramesFifo));

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

    while (glob.rotcketState < 6) {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    while (1) {

        if (uxQueueMessagesWaiting(glob.dataFramesFifo) > 2) {

            file = SPIFFS.open("/data.bin", "a", true);

            while (uxQueueMessagesWaiting(glob.dataFramesFifo)) {

                Frame tempData;
                xQueueReceive(glob.dataFramesFifo, &tempData, portMAX_DELAY);
                file.write((uint8_t*) &tempData, sizeof(tempData));
            }

            file.close();
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
