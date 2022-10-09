typedef enum {
    MY_RMT_WIND_GENTLE, // 微風
    MY_RMT_WIND_WEAK,   // 弱風
    MY_RMT_WIND_STRONG, // 強風
    MY_RMT_WIND_AUTO    // 自動
} my_rmt_wind;

typedef enum {
    MY_RMT_MODE_COOL,   // 冷房
    MY_RMT_MODE_DRY,    // ドライ
    MY_RMT_MODE_FAN,    // 送風
    MY_RMT_MODE_WARM    // 暖房
} my_rmt_mode;

typedef enum {
    MY_RMT_POWER_ON,
    MY_RMT_POWER_OFF,
    MY_RMT_POWER_NULL
} my_rmt_power;

typedef enum {
    MY_RMT_TIMER_ON_AFTER,  // power ON after n hours
    MY_RMT_TIMER_OFF_AFTER, // power OFF after n hours
    MY_RMT_TIMER_NULL       // don't use timer
} my_rmt_timer;

typedef struct {
    rmt_item32_t signal_1[58];
    rmt_item32_t signal_2[58];
    rmt_item32_t signal_3[58];
} my_rmt_command;

void my_rmt_fill_nibble(rmt_item32_t *pointer, uint8_t value);  // fill 4 items from pointer
void my_rmt_fill_byte(rmt_item32_t *pointer, uint8_t value);    // fill 8 items from pointer
void my_rmt_fill_leader(rmt_item32_t *pointer);
void my_rmt_fill_trailor(rmt_item32_t *pointer);
void my_rmt_generate_command(my_rmt_command *command, my_rmt_mode mode, uint8_t temp, my_rmt_wind wind, my_rmt_power power, my_rmt_timer timer, uint8_t time);
void my_rmt_transmit_command(rmt_channel_t channel, my_rmt_command *command);