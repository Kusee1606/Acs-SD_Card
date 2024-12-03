#include <stdio.h>
#include <string.h>

#include "driver/ledc.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "../include/sd_card.h"
#include "../include/flight_data.h"


void app_main(void)
{
    init_sd_card();

    vTaskDelay(pdMS_TO_TICKS(1000));
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}