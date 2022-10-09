#include <stdint.h>
#include "soc/rmt_struct.h"
#include "driver/rmt.h"
#include "my_rmt.h"

void my_rmt_fill_nibble(rmt_item32_t *pointer, uint8_t value) {
    for(int i = 0; i < 4; i++) {
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

void my_rmt_generate_command(my_rmt_command *command, my_rmt_mode mode, uint8_t temp, my_rmt_wind wind, my_rmt_power power, my_rmt_timer timer, uint8_t time) {
    // 1st 7-byte signal
    my_rmt_fill_leader(&(command->signal_1[0]));
    my_rmt_fill_byte(&(command->signal_1[1]), 0x28);    // byte[0]
    my_rmt_fill_byte(&(command->signal_1[9]), 0x61);    // byte[1]
    my_rmt_fill_byte(&(command->signal_1[17]), 0x3d);   // byte[2]
    switch(wind) {                                      // byte[3] and byte[4]
        case MY_RMT_WIND_AUTO:
            my_rmt_fill_byte(&(command->signal_1[25]), 0x10);
            my_rmt_fill_byte(&(command->signal_1[33]),~0x10);
            break;
        case MY_RMT_WIND_GENTLE:
            my_rmt_fill_byte(&(command->signal_1[25]), 0x11);
            my_rmt_fill_byte(&(command->signal_1[33]),~0x11);
            break;
        case MY_RMT_WIND_WEAK:
            my_rmt_fill_byte(&(command->signal_1[25]), 0x12);
            my_rmt_fill_byte(&(command->signal_1[33]),~0x12);
            break;
        case MY_RMT_WIND_STRONG:
            my_rmt_fill_byte(&(command->signal_1[25]), 0x13);
            my_rmt_fill_byte(&(command->signal_1[33]),~0x12);
            break;
    }
    my_rmt_fill_nibble(&(command->signal_1[41]),  temp - 16);  // lower 4 bits of byte[5]
    my_rmt_fill_nibble(&(command->signal_1[49]),~(temp - 16)); // lower 4 bits of byte[6]
    uint8_t tempo = 0;
    switch(mode) {
        case MY_RMT_MODE_WARM:  tempo = 0x01; break;
        case MY_RMT_MODE_DRY:   tempo = 0x05; break;
        case MY_RMT_MODE_COOL:  tempo = 0x09; break;
        case MY_RMT_MODE_FAN:   tempo = 0x0d; break;
    }
    switch(power) {
        case MY_RMT_POWER_NULL: tempo += 0; break;
        case MY_RMT_POWER_OFF:  tempo += 1; break;
        case MY_RMT_POWER_ON:   tempo += 2; break;
    }
    my_rmt_fill_nibble(&(command->signal_1[45]), tempo);    // higher 4 bits of byte[5]
    my_rmt_fill_nibble(&(command->signal_1[53]),~tempo);    // higher 4 bits of byte[6]
    my_rmt_fill_trailor(&(command->signal_1[57]));

    // 2nd 7-byte signal
    my_rmt_fill_leader(&(command->signal_2[0]));
    my_rmt_fill_byte(&(command->signal_2[1]), 0x28);                // byte[0]
    my_rmt_fill_byte(&(command->signal_2[9]), 0x61);                // byte[1]
    my_rmt_fill_byte(&(command->signal_2[17]), 0x6d);               // byte[2]
    if(timer == MY_RMT_TIMER_ON_AFTER) {
        my_rmt_fill_byte(&(command->signal_2[25]),  time * 8);      // byte[3]
        my_rmt_fill_byte(&(command->signal_2[33]),~(time * 8));     // byte[4]
        my_rmt_fill_byte(&(command->signal_2[41]),  time * 7);      // byte[5]
        my_rmt_fill_byte(&(command->signal_2[49]),~(time * 7));     // byte[6]
    } else {
        my_rmt_fill_byte(&(command->signal_2[25]), 0x00);           // byte[3]
        my_rmt_fill_byte(&(command->signal_2[33]),~0x00);           // byte[4]
        my_rmt_fill_byte(&(command->signal_2[41]), 0x00);           // byte[5]
        my_rmt_fill_byte(&(command->signal_2[49]),~0x00);           // byte[6]        
    }
    my_rmt_fill_trailor(&(command->signal_2[57]));

    // 3rd 7-byte signal
    my_rmt_fill_leader(&(command->signal_3[0]));
    my_rmt_fill_byte(&(command->signal_3[1]), 0x28);                // byte[0]
    my_rmt_fill_byte(&(command->signal_3[9]), 0x61);                // byte[1]
    my_rmt_fill_byte(&(command->signal_3[17]), 0xcd);               // byte[2]
    if(timer == MY_RMT_TIMER_OFF_AFTER) {
        my_rmt_fill_byte(&(command->signal_3[25]),  time * 8);      // byte[3]
        my_rmt_fill_byte(&(command->signal_3[33]),~(time * 8));     // byte[4]
        my_rmt_fill_byte(&(command->signal_3[41]),  time * 7);      // byte[5]
        my_rmt_fill_byte(&(command->signal_3[49]),~(time * 7));     // byte[6]
    } else {
        my_rmt_fill_byte(&(command->signal_3[25]), 0x00);           // byte[3]
        my_rmt_fill_byte(&(command->signal_3[33]),~0x00);           // byte[4]
        my_rmt_fill_byte(&(command->signal_3[41]), 0x00);           // byte[5]
        my_rmt_fill_byte(&(command->signal_3[49]),~0x00);           // byte[6]        
    }
    my_rmt_fill_trailor(&(command->signal_3[57]));
}

void my_rmt_transmit_command(rmt_channel_t channel, my_rmt_command *command) {
    rmt_write_items(channel, command->signal_1, 58, true);
    rmt_write_items(channel, command->signal_2, 58, true);
    rmt_write_items(channel, command->signal_3, 58, true);
}