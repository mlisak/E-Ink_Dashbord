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

    gpio_set_level(EINK_EN_PIN, 1);

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
    spi_config.clk_div = SPI_2MHz_DIV;
    spi_config.event_cb = NULL;

    spi_config.intr_enable.val = SPI_MASTER_DEFAULT_INTR_ENABLE;

    spi_init(HSPI_HOST, &spi_config);

    ESP_LOGI(TAG, "spi initialized!");

    return ESP_OK;
}

static esp_err_t spi_write_byte(uint8_t data)
{
    uint32_t buf = data << 24;
    spi_trans_t trans = {0};
    trans.mosi = &buf;
    trans.bits.mosi = 8;
    spi_trans(HSPI_HOST, trans);

    return ESP_OK;
}


static inline esp_err_t write_eink_data(uint8_t data)
{
    spi_write_byte(data);
    gpio_set_level(CL_PIN, 1);
    gpio_set_level(CL_PIN, 0);

    return ESP_OK;
}

void app_main(void)
{
    init_spi();
    init_gpio();
}
