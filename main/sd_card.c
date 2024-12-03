#include "../include/sd_card.h"
#include "../include/flight_data.h"

#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_vfs_fat.h"
#include "esp_err.h"

#define TAG "SD_CARD"
#define MOUNT_POINT "/sdcard"

bool init_sd_card()
{
    bool error_occurred = false;

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 2,
        .allocation_unit_size = 16 * 1024};
    sdmmc_card_t *card;
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };
    error_occurred |= (ESP_ERROR_CHECK_WITHOUT_ABORT(spi_bus_initialize()) != ESP_OK);
    esp_vfs_fat_sdspi_mount();
    return !(error_occurred);
}

void update_sd_card_data();
{
}
