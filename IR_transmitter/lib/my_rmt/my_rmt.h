typedef enum {
    MY_RMT_ERROR_SUCCESS,
    MY_RMT_ERROR_TEMP,
    MY_RMT_ERROR_WIND,
    MY_RMT_ERROR_TIME
} my_rmt_error;

typedef enum {
    MY_RMT_WIND_GENTLE,
    MY_RMT_WIND_WEAK,
    MY_RMT_WIND_STRONG,
    MY_RMT_WIND_AUTO
} my_rmt_wind;

typedef enum {
    MY_RMT_MODE_COOL,
    MY_RMT_MODE_DRY,
    MY_RMT_MODE_FAN,
    MY_RMT_MODE_WARM 
} my_rmt_mode;

typedef enum {
    MY_RMT_POWER_ON,        // Available when the air conditioner is not running
    MY_RMT_POWER_OFF,       // Available when the air conditioner is running
    MY_RMT_POWER_NULL       // Available when the air conditioner is running
} my_rmt_power;

typedef enum {
    MY_RMT_TIMER_ON_AFTER,  // Turn on the air conditioner after n hours
    MY_RMT_TIMER_OFF_AFTER, // Turn off the air conditioner after n hours
    MY_RMT_TIMER_NULL       // Don't use timer
} my_rmt_timer;

/**
 * @brief Data struct of 58 * 3 items for 1 command.
*/
typedef struct {
    rmt_item32_t signal_1[58];
    rmt_item32_t signal_2[58];
    rmt_item32_t signal_3[58];
} my_rmt_command;

/**
 * @brief Fill 4 bits data from the array. Each bit takes 1 space of array.
 * @param pointer The pointer. This function fills 4 items from the array.
 * @param value The value you want to put it in the arrays. It's supposed to be 4 bits value.
*/
void my_rmt_fill_nibble(rmt_item32_t *pointer, uint8_t value);  // fill 4 items from pointer

/**
 * @brief Fill 1 byte data from the array. Each bit takes 1 space of array.
 * @param pointer The pointer. This function fills 8 items from the array.
 * @param value The value you want to put it in the arrays. It's supposed to be 8 bits value.
*/
void my_rmt_fill_byte(rmt_item32_t *pointer, uint8_t value);    // fill 8 items from pointer

/**
 * @brief Fill the leader signal in the array. This function fills 1 item in the array.
*/
void my_rmt_fill_leader(rmt_item32_t *pointer);

/**
 * @brief Fill the trailor signal in the array. This function fills 1 item in the array.
*/
void my_rmt_fill_trailor(rmt_item32_t *pointer);

/**
 * @brief Generate the 58 * 3 items to transmit in accordance with the parameters.
 * @param command The data struct of 58 * 3 items.
 * @param mode Indicates the mode of air conditioner.
 * @param temp 17 ~ 30 when the mode is MY_RMT_MODE_WARM, 20 ~ 30 in other modes.
 * @param wind MY_RMT_WIND_AUTO when the mode is dry.
 * @param power Indicates weather turn on or turn off.
 * @param timer Indicates the functionality of timer.
 * @param time 0 when the timer is MY_RMT_TIMER_NULL, 1 ~ 12 in other modes.
 * @return Error codes
*/
my_rmt_error my_rmt_generate_command(my_rmt_command *command, my_rmt_mode mode, uint8_t temp, my_rmt_wind wind, my_rmt_power power, my_rmt_timer timer, uint8_t time);

/**
 * @brief Transmit the 58 * 3 items in data structure.
 * @param channel RMT channel
 * @param command Data structure of 58 * 3 items.
*/
void my_rmt_transmit_command(rmt_channel_t channel, my_rmt_command *command);