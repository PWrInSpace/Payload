#ifndef NOW_H
#define NOW_H

#include "esp_now.h"
#include "WiFi.h"
#include "dataStructs.h"
#include "functions.h"

// Adresy:
const uint8_t adressObc[] = {0xB4, 0xE6, 0x2D, 0x85, 0x49, 0x4D};

// Init:
bool nowInit();

// Dodanie peera:
bool nowAddPeer(const uint8_t* address, uint8_t channel);

// Przerwania:
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);

#endif