#include "../include/sd_card.h"
#include "../include/flight_data.h"

#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_vfs_fat.h"
#include "esp_err.h"
#include "esp_log.h"
#include "sdmmc_cmd.h"
#include "string.h"
#include "sys/stat.h"

#define DATAHEADER ""
#define DATAFORMAT ""

#define TAG "SD_CARD"
#define MOUNT_POINT "/sdcard"
#define FILENAME "LOT_"
#define PIN_NUM_MISO 19
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK 18
#define PIN_NUM_CS 5
#define SPI_FREQUENCY 5000

bool init_sd_card(sd_card_t *sd_card_data)
{
    esp_err_t ret;

    ESP_LOGI(TAG, "Initializing SD card");

    // Pin configuration
    gpio_set_pull_mode(PIN_NUM_MISO, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(PIN_NUM_MOSI, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(PIN_NUM_CLK, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(PIN_NUM_CS, GPIO_PULLUP_ONLY);

    esp_vfs_fat_sdmmc_mount_config_t mount_config_init = {
        .format_if_mount_failed = false,
        .max_files = 2,
        .allocation_unit_size = 16 * 1024};

    sdmmc_host_t host_init = SDSPI_HOST_DEFAULT();
    host_init.max_freq_khz = SPI_FREQUENCY;
    host_init.slot = VSPI_HOST;

    spi_bus_config_t bus_cfg_init = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 0};

    sdspi_device_config_t slot_config_init = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config_init.gpio_cs = PIN_NUM_CS;
    slot_config_init.host_id = sd_card_data->host.slot;

    sd_card_data->mount_config = mount_config_init;
    sd_card_data->slot_config = slot_config_init;
    sd_card_data->host = host_init;
    sd_card_data->bus_cfg = bus_cfg_init;

    ESP_LOGI(TAG, "Initializing SPI bus");
    ret = ESP_ERROR_CHECK_WITHOUT_ABORT(spi_bus_initialize(sd_card_data->host.slot, &sd_card_data->bus_cfg, SDSPI_DEFAULT_DMA));
    if (ret != ESP_OK)
        return false;

    ESP_LOGI(TAG, "Mounting SD card to %s ", MOUNT_POINT);
    if (!mount_sd_card(sd_card_data))
        return false;
    sdmmc_card_print_info(stdout, sd_card_data->card);

    if (!find_free_path(sd_card_data))
        return false;
    ESP_LOGI(TAG, "Creating a file %s", sd_card_data->path);
    sd_card_data->log_file = fopen(sd_card_data->path, "a+");
    if (sd_card_data->log_file == NULL)
    {
        ESP_LOGE(TAG, "Falied to create file");
        return false;
    }
    else
    {
        ESP_LOGI(TAG, "File ");
    }
    fclose(sd_card_data->log_file);
    return true;
}

bool mount_sd_card(sd_card_t *sd_card_data)
{
    esp_err_t ret;
    ret = ESP_ERROR_CHECK_WITHOUT_ABORT(esp_vfs_fat_sdspi_mount(MOUNT_POINT, &sd_card_data->host, &sd_card_data->slot_config, &sd_card_data->mount_config, &sd_card_data->card));
    if (ret != ESP_OK)
        return false;
    return true;
}

bool unmount_sd_card(sd_card_t *sd_card_data)
{
    esp_err_t ret;
    fclose(sd_card_data->log_file);
    ret = ESP_ERROR_CHECK_WITHOUT_ABORT(esp_vfs_fat_sdcard_unmount(MOUNT_POINT, sd_card_data->card));
    if (ret != ESP_OK)
        return false;
    return true;
}

bool find_free_path(sd_card_t *sd_card_data)
{
    char path_init[256];
    struct stat fdata;
    for (int i = 0; i < 1000; i++)
    {
        snprintf(path_init, 256, "%s/%s%d.csv", MOUNT_POINT, FILENAME, i);
        if (stat(path_init, &fdata) != 0)
        {
            memcpy(sd_card_data->path, path_init, 256);
            return true;
        }
    }
    ESP_LOGE(TAG, "Unable to find a path");
    return false;
}

void update_sd_card_data(sd_card_t *sd_card_data)
{
}
