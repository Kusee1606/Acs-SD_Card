#ifndef __SDCARD__
#define __SDCARD__

#include <stdint.h>
#include <stdbool.h>

#include "../include/flight_data.h"

typedef struct
{
    esp_vfs_fat_sdmmc_mount_config_t mount_config;
    sdspi_device_config_t slot_config;
    spi_bus_config_t bus_cfg;
    sdmmc_card_t *card;
    sdmmc_host_t host;
    char path[256];
    FILE *log_file;
} sd_card_t;

bool init_sd_card(sd_card_t *sd_card_data);
bool mount_sd_card(sd_card_t *sd_card_data);
bool unmount_sd_card(sd_card_t *sd_card_data);
bool find_free_path(sd_card_t *sd_card_data);
void update_sd_card_data(sd_card_t *sd_card_data);

#endif //__SDCARD__