#include <stdint.h>
#include "soc/rmt_struct.h"

void my_rmt_fill_byte(rmt_item32_t *pointer, uint8_t value) {
    for(int i = 0; i < 8; i++) {
        if((value & 0x01) == 0x01) {
            (pointer + i)->duration0 = 500;
            (pointer + i)->level0 = 1;
            (pointer + i)->duration1 = 1300;
            (pointer + i)->level1 = 0;
        } else {
            (pointer + i)->duration0 = 500;
            (pointer + i)->level0 = 1;
            (pointer + i)->duration1 = 360;
            (pointer + i)->level1 = 0;
        }
        value >>= 1;
    }
}

void my_rmt_fill_leader(rmt_item32_t *pointer) {
    pointer->duration0 = 3500;
    pointer->level0 = 1;
    pointer->duration1 = 1700;
    pointer->level1 = 0;
}

void my_rmt_fill_trailor(rmt_item32_t *pointer) {
    pointer->duration0 = 500;
    pointer->level0 = 1;
    pointer->duration1 = 10700;
    pointer->level1 = 0;
}