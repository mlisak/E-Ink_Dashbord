#include <stdio.h>

#include "esp8266/gpio_struct.h"
#include "esp8266/spi_struct.h"

#include "esp_log.h"
#include "esp_attr.h"

#include "driver/gpio.h"
#include "driver/spi.h"

#define EINK_EN_PIN 15
#define OE_PIN 4
#define CL_PIN 5
#define STR_PIN 12

static const char* TAG = "e-ink dashboard";

static void IRAM_ATTR spi_event_callback(int event, void* arg) {}

static esp_err_t init_gpio()
{
    ESP_LOGI(TAG, "initializing gpio...");

    gpio_config_t io_config;

    io_config.pin_bit_mask =
        (1 << EINK_EN_PIN) |
        (1 << OE_PIN) |
        (1 << CL_PIN) |
        (1 << STR_PIN);
    io_config.intr_type = GPIO_INTR_DISABLE;
    io_config.mode = GPIO_MODE_OUTPUT;
    io_config.pull_down_en = 0;
    io_config.pull_up_en = 0;

    gpio_config(&io_config);

    ESP_LOGI(TAG, "gpio initialized!");

    return ESP_OK;
}

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
    init_gpio();
}
