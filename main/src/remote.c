#include "remote.h"

void remote_fill_nibble(rmt_item32_t *pointer, uint8_t value) {
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

void remote_fill_byte(rmt_item32_t *pointer, uint8_t value) {
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

void remote_fill_leader(rmt_item32_t *pointer) {
    pointer->duration0 = 3500;
    pointer->level0 = 1;
    pointer->duration1 = 1700;
    pointer->level1 = 0;
}

void remote_fill_trailor(rmt_item32_t *pointer) {
    pointer->duration0 = 500;
    pointer->level0 = 1;
    pointer->duration1 = 10700;
    pointer->level1 = 0;
}

void remote_transmit(remote_state *state, remote_power prev_power) {
    rmt_item32_t signal_1[58];
    rmt_item32_t signal_2[58];
    rmt_item32_t signal_3[58];
    
    // 1st 7-byte
    remote_fill_leader(&(signal_1[0]));
    remote_fill_byte(&(signal_1[1]), 0x28);
    remote_fill_byte(&(signal_1[9]), 0x61);
    remote_fill_byte(&(signal_1[17]),0x3d);
    switch(state->wind) {
        case REMOTE_WIND_AUTO:
            remote_fill_byte(&(signal_1[25]), 0x10);
            remote_fill_byte(&(signal_1[33]),~0x10);
            break;
        case REMOTE_WIND_GENTLE:
            remote_fill_byte(&(signal_1[25]), 0x11);
            remote_fill_byte(&(signal_1[33]),~0x11);
            break;
        case REMOTE_WIND_WEAK:
            remote_fill_byte(&(signal_1[25]), 0x12);
            remote_fill_byte(&(signal_1[33]),~0x12);
            break;
        case REMOTE_WIND_STRONG:
            remote_fill_byte(&(signal_1[25]), 0x13);
            remote_fill_byte(&(signal_1[33]),~0x13);
            break;
    }
    remote_fill_nibble(&(signal_1[41]),  state->temp - 16);
    remote_fill_nibble(&(signal_1[49]),~(state->temp - 16));
    uint8_t buf = 0;
    switch(state->mode) {
        case REMOTE_MODE_WARM:  buf = 0x01; break;
        case REMOTE_MODE_DRY:   buf = 0x05; break;
        case REMOTE_MODE_COOL:  buf = 0x09; break;
        case REMOTE_MODE_FAN:   buf = 0x0d; break;
    }
    if(state->power == REMOTE_POWER_STOPPED) {
        buf += 1;
    } else {
        if(prev_power == REMOTE_POWER_STOPPED) buf += 2;
    }
    remote_fill_nibble(&(signal_1[45]), buf);
    remote_fill_nibble(&(signal_1[53]),~buf);
    remote_fill_trailor(&(signal_1[57]));

    // 2nd 7-byte
    remote_fill_leader(&(signal_2[0]));
    remote_fill_byte(&(signal_2[1]), 0x28);
    remote_fill_byte(&(signal_2[9]), 0x61);
    remote_fill_byte(&(signal_2[17]),0x6d);
    if((state->timer_on == REMOTE_TIMER_ON_TRUE) && (state->timer_mode == REMOTE_TIMER_MODE_ON)) {
        remote_fill_byte(&(signal_2[25]), ((state->timer_time / 60) * 8));
        remote_fill_byte(&(signal_2[33]),~((state->timer_time / 60) * 8));
        remote_fill_byte(&(signal_2[41]), ((state->timer_time / 60) * 7));
        remote_fill_byte(&(signal_2[49]),~((state->timer_time / 60) * 7));
    } else {
        remote_fill_byte(&(signal_2[25]), 0xff);
        remote_fill_byte(&(signal_2[33]), 0x00);
        remote_fill_byte(&(signal_2[41]), 0xff);
        remote_fill_byte(&(signal_2[49]), 0x00);
    }
    remote_fill_trailor(&(signal_2[57]));

    // 3nd 7-byte
    remote_fill_leader(&(signal_3[0]));
    remote_fill_byte(&(signal_3[1]), 0x28);
    remote_fill_byte(&(signal_3[9]), 0x61);
    remote_fill_byte(&(signal_3[17]),0xcd);
    if((state->timer_on == REMOTE_TIMER_ON_TRUE) && (state->timer_mode == REMOTE_TIMER_MODE_OFF)) {
        remote_fill_byte(&(signal_3[25]), ((state->timer_time / 60) * 8));
        remote_fill_byte(&(signal_3[33]),~((state->timer_time / 60) * 8));
        remote_fill_byte(&(signal_3[41]), ((state->timer_time / 60) * 7));
        remote_fill_byte(&(signal_3[49]),~((state->timer_time / 60) * 7));
    } else {
        remote_fill_byte(&(signal_3[25]), 0xff);
        remote_fill_byte(&(signal_3[33]), 0x00);
        remote_fill_byte(&(signal_3[41]), 0xff);
        remote_fill_byte(&(signal_3[49]), 0x00);
    }
    remote_fill_trailor(&(signal_3[57]));
    
    // Transmit
    rmt_write_items(REMOTE_CHANNEL, signal_1, 58, true);
    rmt_write_items(REMOTE_CHANNEL, signal_2, 58, true);
    rmt_write_items(REMOTE_CHANNEL, signal_3, 58, true);
}