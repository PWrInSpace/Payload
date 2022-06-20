#ifndef NOW_H
#define NOW_H

#include "esp_now.h"
#include "WiFi.h"
#include "dataStructs.h"
#include "functions.h"

// Adresy:
const uint8_t addressObc[] = {0x04, 0x20, 0x04, 0x20, 0x04, 0x20};
const uint8_t addressPayload[] = {0x34, 0x94, 0x54, 0xD9, 0x5A, 0xE4};

// Init:
bool nowInit();

// Dodanie peera:
bool nowAddPeer(const uint8_t* address, uint8_t channel);

// Przerwania:
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);

#endif