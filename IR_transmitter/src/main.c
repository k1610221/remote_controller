// IR_transmitter ver.2.1.0
#include "driver/rmt.h"
#include "my_rmt.h"

void app_main(void) {
    rmt_config_t Config;
    Config.channel = RMT_CHANNEL_0;
    Config.rmt_mode = RMT_MODE_TX;
    Config.gpio_num = GPIO_NUM_25;
    Config.mem_block_num = 1;
    Config.clk_div = 80;
    Config.flags = 0; // if not 0, clk_div would be corrupted
    Config.tx_config.loop_en = false;
    Config.tx_config.carrier_en = false; // true;
    Config.tx_config.carrier_freq_hz = 38000;
    Config.tx_config.carrier_duty_percent = 33;
    Config.tx_config.carrier_level = RMT_CARRIER_LEVEL_HIGH;
    Config.tx_config.idle_output_en = false;
    rmt_config(&Config);
    rmt_driver_install(Config.channel, 0, 0);

    // Sending 0x28 0x61 0x3d 0x11 0xee 0x94 0x6b
    rmt_item32_t data[58];
    my_rmt_fill_leader(&data[0]);
    my_rmt_fill_byte(&data[1], 0x28);
    my_rmt_fill_byte(&data[9], 0x61);
    my_rmt_fill_byte(&data[17], 0x3d);
    my_rmt_fill_byte(&data[25], 0x11);
    my_rmt_fill_byte(&data[33], 0xee);
    my_rmt_fill_byte(&data[41], 0x94);
    my_rmt_fill_byte(&data[49], 0x6b);
    my_rmt_fill_trailor(&data[57]);

    while(1) {
        rmt_write_items(Config.channel, &data[0], 58, true);
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}