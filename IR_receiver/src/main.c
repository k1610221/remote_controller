// IR_receiver ver.1.0.4
#include "driver/rmt.h"
#include <string.h>

RingbufHandle_t buffer = NULL;

typedef union {
    struct {
        uint8_t b0 : 1;
        uint8_t b1 : 1;
        uint8_t b2 : 1;
        uint8_t b3 : 1;
        uint8_t b4 : 1;
        uint8_t b5 : 1;
        uint8_t b6 : 1;
        uint8_t b7 : 1;
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

    uint8_t counter = 0; // for adding an empty line every 3 lines

    while(1) {
        size_t item_size;
        rmt_item32_t *item = (rmt_item32_t *)xRingbufferReceive(buffer, &item_size, pdMS_TO_TICKS(1000));
        if(item) {
            #if 1
            uint8_t data[64]; // data after processing item(raw data)
            memset(data, 0, sizeof(data));
            uint8_t j = 0; // index for data[64]
            for(int i = 1; i < item_size; i++) {
                if(item[i].duration0 == 0 || item[i].duration1 == 0) break; // trailer
                if(item[i].duration0 > 1300) {
                    data[j++] = 0;
                }
                if(item[i].duration1 > 500) data[j++] = 1;
                else data[j++] = 0;
            }

            for(int i = 0; i < 56;) { // display hexadecimal in little endian
                byte temp;
                temp.b7 = data[i++];
                temp.b6 = data[i++];
                temp.b5 = data[i++];
                temp.b4 = data[i++];
                temp.b3 = data[i++];
                temp.b2 = data[i++];
                temp.b1 = data[i++];
                temp.b0 = data[i++];
                printf("%02x ", temp.val);
            }
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
            if(++counter == 3 ) {printf("\n"); counter = 0;}
        }
    }
}