// IR_transmitter ver.2.2.1
#include "driver/rmt.h"
#include "my_rmt.h"

void app_main(void) {
    rmt_config_t Config;
    Config.channel = RMT_CHANNEL_0;
    Config.rmt_mode = RMT_MODE_TX;
    Config.gpio_num = GPIO_NUM_25;
    Config.mem_block_num = 1;
    Config.clk_div = 80;
    Config.flags = 0; // if not 0, clk_div would be corrupted in some situation
    Config.tx_config.loop_en = false;
    Config.tx_config.carrier_en = true; // true when practice;
    Config.tx_config.carrier_freq_hz = 38000;
    Config.tx_config.carrier_duty_percent = 33;
    Config.tx_config.carrier_level = RMT_CARRIER_LEVEL_HIGH;
    Config.tx_config.idle_output_en = false;
    rmt_config(&Config);
    rmt_driver_install(Config.channel, 0, 0);

    my_rmt_command data;
    my_rmt_generate_command(&data, MY_RMT_MODE_COOL, 20, MY_RMT_WIND_WEAK, MY_RMT_POWER_ON, MY_RMT_TIMER_NULL, 0);
    while(1) {
        my_rmt_transmit_command(Config.channel, &data);
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}