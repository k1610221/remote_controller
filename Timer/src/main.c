#include <stdio.h>
#include "unistd.h"
#include "driver/timer.h"

uint16_t var = 60;

bool IRAM_ATTR handler(void *arg) { // No pritnf() in ISR
    if(var > 0) var--;
    else timer_pause(TIMER_GROUP_0, TIMER_0);
    return true;
}

void app_main() {
    timer_config_t timer_config = {
        .divider        = 80,
        .counter_dir    = TIMER_COUNT_DOWN,
        .counter_en     = TIMER_PAUSE,
        .alarm_en       = TIMER_ALARM_EN,
        .auto_reload    = TIMER_AUTORELOAD_EN
    };
    timer_init(TIMER_GROUP_0, TIMER_0, &timer_config);
    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 1000000);
    timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, 0);
    timer_enable_intr(TIMER_GROUP_0, TIMER_0);
    timer_isr_callback_add(TIMER_GROUP_0, TIMER_0, handler, NULL, 1);
    timer_start(TIMER_GROUP_0, TIMER_0);

    while(1){
        printf("%02d\n", var);
        usleep(1000000);
    }
}