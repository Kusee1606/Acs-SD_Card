#include <stdio.h>
#include <string.h>

#include "driver/ledc.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "../include/sd_card.h"
#include "../include/flight_data.h"

void rocket_state_increment(rocket_state_t *rocket_state);

void app_main(void)
{
    sd_card_t card;
    rocket_state_t rocket_state;
    rocket_state.position[0] = 1.0;
    rocket_state.position[1] = 2.0;
    rocket_state.position[2] = 3.0;
    rocket_state.velocity[0] = 4.0;
    rocket_state.velocity[1] = 5.0;
    rocket_state.velocity[2] = 6.0;
    rocket_state.orientation[0] = 7.0;
    rocket_state.orientation[1] = 8.0;
    rocket_state.orientation[2] = 9.0;
    rocket_state.angular_vel[0] = 10.0;
    rocket_state.angular_vel[1] = 11.0;
    rocket_state.angular_vel[2] = 12.0;
    rocket_state.alpha = 13.0;
    rocket_state.beta = 14.0;

    init_sd_card(&card);
    while (1)
    {
        update_sd_card_data(&card, &rocket_state);
        rocket_state_increment(&rocket_state);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void rocket_state_increment(rocket_state_t *rocket_state)
{
    rocket_state->position[0] += 1.0;
    rocket_state->position[1] += 1.0;
    rocket_state->position[2] += 1.0;
    rocket_state->velocity[0] += 1.0;
    rocket_state->velocity[1] += 1.0;
    rocket_state->velocity[2] += 1.0;
    rocket_state->orientation[0] += 1.0;
    rocket_state->orientation[1] += 1.0;
    rocket_state->orientation[2] += 1.0;
    rocket_state->angular_vel[0] += 1.0;
    rocket_state->angular_vel[1] += 1.0;
    rocket_state->angular_vel[2] += 1.0;
    rocket_state->alpha += 1.0;
    rocket_state->beta += 1.0;
}