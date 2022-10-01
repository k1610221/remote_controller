// IR_receiver ver.1.0.2
#include "driver/rmt.h"
#include <string.h>

RingbufHandle_t buffer = NULL;

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

    while(1) {
        size_t item_size;
        rmt_item32_t *item = (rmt_item32_t *)xRingbufferReceive(buffer, &item_size, pdMS_TO_TICKS(1000));
        if(item) {
            #if 1
            char data[64]; // data after processing item(raw data)
            memset(data, 0, sizeof(data));
            uint8_t j = 0; // index for data[64]
            for(int i = 1; i < item_size; i++) {
                if(item[i].duration0 == 0 || item[i].duration1 == 0) break; // trailer
                if(item[i].duration0 > 1300) {
                    data[j++] = '0';
                }
                if(item[i].duration1 > 500) data[j++] = '1';
                else data[j++] = '0';
            }
            for(int i = 0; i < j; i++) {
                printf("%c", data[i]);
                if((i + 1) % 8 == 0) printf(" ");
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
        }
    }
}