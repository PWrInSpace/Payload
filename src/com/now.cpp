#include "../../include/com/now.h"
#include "../../include/structs/PayloadControl.h"
#include "../../include/hardware/RPiControl.h"

extern PayloadControl payload;

bool adressCompare(const uint8_t *addr1, const uint8_t *addr2);

/**********************************************************************************************/

bool nowInit()
{

    WiFi.mode(WIFI_STA);
    if (esp_now_init())
        return false;
    // esp_wifi_set_mac(WIFI_IF_STA, addressPayload);
    Serial.println(WiFi.macAddress());
    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);
    return true;
}

/**********************************************************************************************/

bool nowAddPeer(const uint8_t *address, uint8_t channel)
{

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, address, 6);
    peerInfo.channel = channel;

    if (esp_now_add_peer(&peerInfo))
        return false;
    return true;
}

/**********************************************************************************************/

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{

    if (status && payload.nextSendTime > MIN_SLEEP_TIME_MS)
        goToSleep();
}

/**********************************************************************************************/

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{

    if (adressCompare(mac, addressObc))
    {

        // if sleepTime:
        if (len == sizeof(payload.nextSendTime))
        {

            memcpy((void *)&payload.nextSendTime, (uint16_t *)incomingData, sizeof(sleepTime));

            // if long time - go to sleep:
            if (payload.nextSendTime > MIN_SLEEP_TIME_MS)
                goToSleep();
            else
            {
                dataToObc.wakenUp = true;
                RPiControl::raspberryPower();
            }
        }

        // if different command:
        else
            rxNowHandler(incomingData, len);
    }
}

/**********************************************************************************************/

bool adressCompare(const uint8_t *addr1, const uint8_t *addr2)
{

    for (int8_t i = 0; i < 6; i++)
    {

        if (addr1[i] != addr2[i])
            return false;
    }

    return true;
}