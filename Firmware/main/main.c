#include <stdio.h>

#include "esp8266/spi_struct.h"

#include "esp_log.h"
#include "esp_attr.h"

#include "driver/spi.h"

static const char* TAG = "e-ink dashboard";

static void IRAM_ATTR spi_event_callback(int event, void* arg) {}

static esp_err_t init_spi()
{
    ESP_LOGI(TAG, "initializing spi...");

    spi_config_t spi_config;

    spi_config.interface.val = SPI_DEFAULT_INTERFACE;
    spi_config.interface.cs_en = 0;
    spi_config.interface.miso_en = 0;

    spi_config.mode = SPI_MASTER_MODE;
    spi_config.clk_div = SPI_4MHz_DIV;
    spi_config.event_cb = spi_event_callback;

    spi_config.intr_enable.val = SPI_MASTER_DEFAULT_INTR_ENABLE;

    spi_init(HSPI_HOST, &spi_config);

    ESP_LOGI(TAG, "spi initialized!");

    return ESP_OK;
}

void app_main(void)
{
    init_spi();
}
