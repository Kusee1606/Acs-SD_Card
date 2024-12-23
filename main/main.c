#include <stdio.h>
#include <string.h>

#include "driver/ledc.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "../include/sd_card.h"
#include "../include/flight_data.h"

void app_main(void)
{
    sd_card_t card;
    init_sd_card(&card);
    while (1)
    {
        vTaskDelay(1000);
    }
}