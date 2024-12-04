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


#define TAG "SD_CARD"
#define MOUNT_POINT "/sdcard"
#define FILENAME "/LOT_%d.csv"
#define PIN_NUM_MISO 19
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK 18
#define PIN_NUM_CS 5

FILE *file;

bool init_sd_card()
{
    gpio_set_pull_mode(PIN_NUM_MISO, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(PIN_NUM_MOSI, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(PIN_NUM_CLK, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(PIN_NUM_CS, GPIO_PULLUP_ONLY);
    bool error_occurred = false;
    ESP_LOGI(TAG, "Initializing SD card");
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 2,
        .allocation_unit_size = 16 * 1024};
    sdmmc_card_t *card;
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.max_freq_khz = 5000;
    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };
    ESP_LOGI(TAG, "Initializing SPI device");
    error_occurred |= (ESP_ERROR_CHECK_WITHOUT_ABORT(spi_bus_initialize(host.slot, &bus_cfg, SDSPI_DEFAULT_DMA)) != ESP_OK);
    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = PIN_NUM_CS;
    slot_config.host_id = host.slot;
    ESP_LOGI(TAG, "Mounting SD card to %s ", MOUNT_POINT);
    error_occurred |= (ESP_ERROR_CHECK_WITHOUT_ABORT(esp_vfs_fat_sdspi_mount(MOUNT_POINT, &host, &slot_config, &mount_config, &card)) != ESP_OK);
    sdmmc_card_print_info(stdout, card);

    char *path = malloc(strlen(MOUNT_POINT) + strlen(FILENAME) + 1);
    int path_size = strlen(path) + strlen("1000");
    char *formated_path = malloc(path_size);

    strcpy(path, MOUNT_POINT);
    strcat(path, FILENAME);
    sprintf(formated_path, path, 1);
    struct stat fdata;
    int i = 1;
    while (stat(formated_path, &fdata) == 0)
    {
        i++;
        if (i > 100)
        {
            ESP_LOGE(TAG, "Clean SD Card");
            error_occurred |= 1;
            return !(error_occurred);
        }
        sprintf(formated_path, path, i);
    }
    ESP_LOGI(TAG, "Trying to create file %s", formated_path);
    file = fopen(formated_path, "a+");
    if (file == NULL)
    {
        ESP_LOGE(TAG, "Falied to create file");
        error_occurred |= (file != NULL);
        return !(error_occurred);
    }
    else
        ESP_LOGI(TAG, "File created");

    fclose(file);
    return !(error_occurred);
}

void update_sd_card_data()
{
}
