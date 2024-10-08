#include "functions.h"

#define TAG "FNC"

// IMPORTANT! To implementation of each module:

void initPeripherals() {

    // Enable UART1:
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, MEASUREMENT_TX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_1, 256, 0, 0, NULL, 0);

    // Enable ADC1:
    adc1_config_width(ADC_WIDTH_BIT_12); // Ustawienie rozdzielczości na 12 bitów
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_0);

    // Configure mosfet pin:
    gpio_set_direction(MOSFET_PIN, GPIO_MODE_OUTPUT);
}

/**********************************************************************************************/

// To implementation of each module:

void measure() {

    // Measure battery voltage with ADC:
    moduleData.dataToObc.power_voltage = (float)adc1_get_raw((adc1_channel_t)ADC1_CHANNEL_6) * 3.3 / 4095.0 * 2.435;
    ESP_LOGI(TAG, "Batt: %f\n", moduleData.dataToObc.power_voltage);

    if (moduleData.obcState >= COUNTDOWN && moduleData.obcState <= SECOND_STAGE_RECOVERY) {

        // Enable mosfet:
        gpio_set_level(MOSFET_PIN, 1);

        // Send state number by UART1 to measurement boards:
        uart_write_bytes(UART_NUM_1, (void*) &moduleData.obcState, sizeof(moduleData.obcState));
    }
    else {
        // Disable mosfet:
        gpio_set_level(MOSFET_PIN, 0);
    }
}

/**********************************************************************************************/

// To implementation of each module:

void rxNowHandler(const uint8_t *incomingData, int len) {

    DataFromObc rxData;
    if (len == sizeof(rxData)) {

        memcpy((uint8_t*) &rxData, incomingData, len);
        // From this moment you can use rxData.commandNum and rxData.commandArg.

        // Nothing to do here in Payload :)
    }
}


#ifdef TEMPLATE_FLASH

bool can_write_to_flash(void) {
    #warning "Function is incomplete"
    return true;
}

#endif

#ifdef TEMPLATE_SD

bool can_write_to_sd(void) {
    #warning "Function is incomplete"
    return true;
}

bool create_sd_string(DataToSave *data, char *buffer, size_t len) {
    //snprintf(buffer, len, "%0.2f\n", data->dupa);
    #warning "Function is incomplete"
    return true;
}

#endif
// IMPORTANT END!

/**********************************************************************************************/
/**********************************************************************************************/
/**********************************************************************************************/
/**********************************************************************************************/
void initDataStructsAndUart(void) {
    // STRUCTS:
    moduleData.obcState = 0;
    moduleData.inServiceMode = false;
    uint16_t stateTimes[STATE_NUM] = {
        INIT_PERIOD, IDLE_PERIOD, RECOVERY_ARM_PERIOD,
        FUELING_PERIOD, ARMED_TO_LAUNCH_PERIOD, RDY_TO_LAUNCH_PERIOD,
        COUNTDOWN_PERIOD, FLIGHT_PERIOD, FIRST_STAGE_REC_PERIOD,
        SECOND_STAGE_REC_PERIOD, ON_GROUND_PERIOD,
        HOLD_PERIOD, ABORT_PERIOD};

    memcpy((uint8_t*) moduleData.stateTimes, (uint8_t*) stateTimes, sizeof(stateTimes));
}

uint32_t getPowerTime_ms() {

    struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv, (void*) &tz);
    return tv.tv_sec * 1000 + tv.tv_usec/1000;
    return 0;
}

/**********************************************************************************************/

void goToSleep() {
    if (moduleData.inServiceMode == true) {
        return;
    }
    ESP_LOGI(TAG, "Going to sleep!");
    esp_sleep_enable_timer_wakeup((SLEEP_TIME_MS) * 10e2);
    esp_deep_sleep_start();
}

/**********************************************************************************************/

bool stateInWakenUp() {

    return moduleData.stateTimes[moduleData.obcState] != SLEEP_TIME_MS;
}

/**********************************************************************************************/

void checkSleepThings() {

    if (!stateInWakenUp()) {

        ESP_LOGI(TAG, "Waiting for OBC state");
        vTaskDelay(ANSWER_WAIT_MS / portTICK_PERIOD_MS);

        if (!stateInWakenUp()) {
            esp_now_send(adressObc, (uint8_t *) &moduleData.dataToObc, sizeof(moduleData.dataToObc));
            goToSleep();
        }
    }
}

/**********************************************************************************************/

uint32_t getTimeMS(void) {
    return esp_timer_get_time() / 1000ULL;
}

/**********************************************************************************************/

bool initializeSPI(void) {
    esp_err_t ret;
    spi_bus_config_t bus = {
      .miso_io_num = MISO_PIN,
      .mosi_io_num = MOSI_PIN,
      .sclk_io_num = SCLK_PIN,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .max_transfer_sz = 4000,
    };

   ret = spi_bus_initialize(VSPI_HOST, &bus, SDSPI_DEFAULT_HOST);
   return ret == ESP_OK;
}

void memory_check_start_condition(void) {
    while(!stateInWakenUp()) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void memory_check_end_condition(void) {
    if (moduleData.obcState == ON_GROUND) {
            vTaskDelete(NULL);
    }
}

inline void terminate_task_on_error(bool fail_cond, char *mess) {
    if (fail_cond == true) {
        ESP_LOGE(TAG, "ERROR %s", mess);
        vTaskDelete(NULL);
    }
}
