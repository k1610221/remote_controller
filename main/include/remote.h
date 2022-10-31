#include <stdint.h>
#include "driver/rmt.h"

#define REMOTE_CHANNEL RMT_CHANNEL_0

typedef enum {
    REMOTE_MODE_WARM,
    REMOTE_MODE_COOL,
    REMOTE_MODE_DRY,
    REMOTE_MODE_FAN
} remote_mode;

typedef enum {
    REMOTE_WIND_AUTO,
    REMOTE_WIND_GENTLE,
    REMOTE_WIND_WEAK,
    REMOTE_WIND_STRONG
} remote_wind;

typedef enum {
    REMOTE_TIMER_ON_FALSE,
    REMOTE_TIMER_ON_TRUE
} remote_timer_on;

typedef enum {
    REMOTE_TIMER_MODE_OFF,
    REMOTE_TIMER_MODE_ON
} remote_timer_mode;

typedef enum {
    REMOTE_POWER_STOPPED,
    REMOTE_POWER_RUNNING,
    REMOTE_POWER_TIMER
} remote_power;

typedef struct {
    remote_mode         mode;
    remote_wind         wind;
    uint8_t             temp;
    remote_timer_on     timer_on;
    uint16_t             timer_time;
    remote_timer_mode   timer_mode;
    remote_power        power;
} remote_state;

void remote_fill_nibble(rmt_item32_t *pointer, uint8_t value);
void remote_fill_byte(rmt_item32_t *pointer, uint8_t value);
void remote_fill_leader(rmt_item32_t *pointer);
void remote_fill_trailor(rmt_item32_t *pointer);
void remote_transmit(remote_state *state, remote_power prev_power);