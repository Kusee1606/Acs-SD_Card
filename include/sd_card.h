#ifndef __SDCARD__
#define __SDCARD__

#include <stdint.h>
#include <stdbool.h>

#include "flight_data.h"

bool init_sd_card();
void update_sd_card_data();

#endif //__SDCARD__