#ifndef __FLIGHTDATA__
#define __FLIGHTDATA__

#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

// STATE ENUM
typedef enum
{
    RAIL,
    ENGINE_FLIGHT,
    CONTROLLED_FLIGHT,
    FALL,
    GROUND
} State;

typedef struct
{
    State state;
    SemaphoreHandle_t mutex;
} state_t;

typedef struct
{
    uint32_t timestamp;
    float accelX;
    float accelY;
    float accelZ;
    float gyroX;
    float gyroY;
    float gyroZ;
    SemaphoreHandle_t mutex;
} imu_data_t;

typedef struct
{
    uint32_t timestamp;
    float magX;
    float magY;
    float magZ;
    SemaphoreHandle_t mutex;
} mag_data_t;

typedef struct
{
    uint32_t timestamp;
    float pressure;
    float temperature;
    SemaphoreHandle_t mutex;
} baro_data_t;

typedef struct
{
    uint32_t timestamp;

    // ONLY JACEK KNOWS WHAT SHOULD BE HERE
    // Same state as the control algo and kalman.

    // Absolutes
    float position[3];
    float velocity[3];
    float orientation[3];
    float angular_vel[3];

    float alpha;
    float beta;

    SemaphoreHandle_t mutex;
} rocket_state_t;

// State in earth body frame
typedef struct
{
    uint32_t timestamp;

    float position[3];
    float velocity[3];
    float orientation[3]; 
    float angular_vel[3];
    SemaphoreHandle_t mutex;
} absolute_state_t;

typedef struct
{
    float canard_angle1;
    float canard_angle2;
    float canard_angle3;
    float canard_angle4;
} canards_t;

// Main flight data container
typedef struct
{
    imu_data_t imu_latest;
    mag_data_t mag_latest;
    baro_data_t baro_latest;

    rocket_state_t rocket_state;
    absolute_state_t abs_state;
    canards_t controls;
} container_t;

// Definitions of functions
void update_imu_data(container_t *data, const imu_data_t *new_data);
void update_mag_data(container_t *data, const mag_data_t *new_data);
void update_baro_data(container_t *data, const baro_data_t *new_data);
void update_rocket_state(container_t *data, const rocket_state_t *new_state);
void update_canards(container_t *data, const canards_t *new_canards);

bool get_rocket_state(container_t *data, rocket_state_t *rocket_state);
bool get_latest_sensor_data(container_t *data,
                            imu_data_t *imu,
                            mag_data_t *mag,
                            baro_data_t *baro);

#endif // __FLIGHTDATA__
