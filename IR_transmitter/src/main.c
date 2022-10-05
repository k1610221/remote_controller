// IR_transmitter ver.2.0.0
#include "driver/rmt.h"

void app_main(void) {
    rmt_config_t Config;
    Config.channel = RMT_CHANNEL_0;
    Config.rmt_mode = RMT_MODE_TX;
    Config.gpio_num = GPIO_NUM_25;
    Config.mem_block_num = 1;
    Config.clk_div = 80;
    Config.tx_config.loop_en = false;
    Config.tx_config.carrier_en = true;
    Config.tx_config.carrier_freq_hz = 38000;
    Config.tx_config.carrier_duty_percent = 33;
    Config.tx_config.carrier_level = RMT_CARRIER_LEVEL_HIGH;
    Config.tx_config.idle_output_en = false;
    rmt_config(&Config);
    rmt_driver_install(Config.channel, 0, 0);

    rmt_item32_t data[2];
    data[0].duration0 = 500; // data[0] = logical 0
    data[0].level0 = 1;
    data[0].duration1 = 360;
    data[0].level1 = 0;
    data[1].duration0 = 500; // data[1] = logical 1
    data[1].level0 = 1;
    data[1].duration1 = 1200;
    data[1].level1 = 0;

    while(1) {
        rmt_write_items(Config.channel, data, 2, true); // emits "010101..."
    }
}