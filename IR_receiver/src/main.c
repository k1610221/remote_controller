// IR_receiver ver.1.2.1
#include "driver/rmt.h"
#include <string.h>

RingbufHandle_t buffer = NULL;

typedef union { // (MSB) upper_nibble | lower_nibble (LSB)
    struct {
        uint8_t lower_nibble : 4;
        uint8_t upper_nibble : 4;
    };
    uint8_t val;
} byte;

void app_main(void) {
    rmt_config_t Config;
    Config.channel = RMT_CHANNEL_0;
    Config.rmt_mode = RMT_MODE_RX;
    Config.gpio_num = GPIO_NUM_25;
    Config.mem_block_num = 1; // 64 items
    Config.clk_div = 80; // 1 tick = 1 us
    Config.rx_config.filter_ticks_thresh = 255;
    Config.rx_config.idle_threshold = 10000;
    Config.rx_config.filter_en = true;
    rmt_config(&Config);
    rmt_driver_install(Config.channel, 768, 0);
    rmt_get_ringbuf_handle(Config.channel, &buffer);
    rmt_rx_start(Config.channel, true);

    uint8_t line_counter = 0; // for adding an empty line every 3 lines

    while(1) {
        size_t item_size;
        rmt_item32_t *item = (rmt_item32_t *)xRingbufferReceive(buffer, &item_size, pdMS_TO_TICKS(1000)); // raw data
        if(item) {
            #if 1
            byte data[8];
            memset(data, 0, sizeof(data));
            uint8_t j = 0; // counter for data (bit)
            for(int i = 1; i < item_size; i++) {
                if(item[i].duration0 == 0 || item[i].duration1 == 0) break; // trailer
                if(item[i].duration1 > 1300) j++;
                if(item[i].duration1 > 500) {
                    data[j / 8].val |= 1 << (j % 8); // store data from LSB to MSB
                    j++;
                } else j++;
            }

            bool error_flag = 0; // 0 -> no error, 1 -> error
            if((data[0].upper_nibble ^ data[0].lower_nibble ^ data[1].upper_nibble ^ data[1].lower_nibble) != data[2].lower_nibble) error_flag = 1;
            if((data[3].val & data[4].val) != 0) error_flag = 1;
            if((data[5].val & data[6].val) != 0) error_flag = 1;

            for(int i = 0; i < 7; i++) printf("%02x ", data[i].val); // display hexadecimal
            if(error_flag) printf("// error detected");
            printf("\n");
            
            #else // Display details
            for(int i = 0; i < item_size; i++) {
                if(item[i].duration0 == 0 || item[i].duration1 == 0) break;
                printf("item[%d] ",i);
                item[i].level0 == 0 ? printf("LOW: %d", item[i].duration0) : printf("HIGH: %d", item[i].duration0);
                item[i].level1 == 0 ? printf("  LOW: %d", item[i].duration1) : printf("  HIGH: %d", item[i].duration1);
                printf("\n");
            }
            #endif
            vRingbufferReturnItem(buffer, (void *)item);
            if(++line_counter == 3 ) {printf("\n"); line_counter = 0;}
        }
    }
}