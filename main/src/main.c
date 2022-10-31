#include "nvs_flash.h"
#include "wifi.h"
#include "esp_http_server.h"
#include "esp_spiffs.h"
#include "remote.h"
#include <stdio.h>

remote_state state = {
    .mode       = REMOTE_MODE_WARM,
    .wind       = REMOTE_WIND_GENTLE,
    .temp       = 25,
    .timer_on   = REMOTE_TIMER_ON_FALSE,
    .timer_time = 0, // (minutes)
    .timer_mode = REMOTE_TIMER_MODE_OFF,
    .power      = REMOTE_POWER_STOPPED
};

esp_err_t home_handler(httpd_req_t *req) {
    FILE *fs = fopen("/spiffs/index.html", "r");
    char buf[100];
    while(fgets(buf, sizeof(buf), fs)) {
        httpd_resp_send_chunk(req, buf, HTTPD_RESP_USE_STRLEN);
    }
    httpd_resp_send_chunk(req, buf, 0);
    fclose(fs);
    return ESP_OK;
}
esp_err_t script_handler(httpd_req_t *req) {
    FILE *fs = fopen("/spiffs/script.js", "r");
    char buf[100];
    while(fgets(buf, sizeof(buf), fs)) {httpd_resp_send_chunk(req, buf, HTTPD_RESP_USE_STRLEN);}
    httpd_resp_send_chunk(req, buf, 0);
    fclose(fs);
    return ESP_OK;
}
esp_err_t state_handler(httpd_req_t *req) {
    char buf[5]; char msg[20];
    itoa(state.mode, buf, 10); strcat(msg, buf); strcat(msg, ",");
    itoa(state.wind, buf, 10); strcat(msg, buf); strcat(msg, ",");
    itoa(state.temp, buf, 10); strcat(msg, buf); strcat(msg, ",");
    itoa(state.timer_on, buf, 10); strcat(msg, buf); strcat(msg, ",");
    itoa(state.timer_time, buf, 10); strcat(msg, buf); strcat(msg, ",");
    itoa(state.timer_mode, buf, 10); strcat(msg, buf); strcat(msg, ",");
    itoa(state.power, buf, 10); strcat(msg, buf);
    httpd_resp_send(req, msg, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t run_handler(httpd_req_t *req) {
    char rx[100];
    int ret = httpd_req_recv(req, rx, req->content_len);
    if(ret <= 0) {
        if(ret == HTTPD_SOCK_ERR_TIMEOUT) {
            httpd_resp_send_408(req);
        }
        return ESP_FAIL;
    }
    char buf[5];
    uint8_t prev_power = state.power;
    httpd_query_key_value(rx, "mode", buf, sizeof(buf)); state.mode = atoi(buf);
    httpd_query_key_value(rx, "wind", buf, sizeof(buf)); state.wind = atoi(buf);
    httpd_query_key_value(rx, "temp", buf, sizeof(buf)); state.temp = atoi(buf);
    httpd_query_key_value(rx, "timer_on", buf, sizeof(buf)); state.timer_on = atoi(buf);
    httpd_query_key_value(rx, "timer_time", buf, sizeof(buf)); state.timer_time = atoi(buf);
    httpd_query_key_value(rx, "timer_mode", buf, sizeof(buf)); state.timer_mode = atoi(buf);
    if(state.timer_on == REMOTE_TIMER_ON_FALSE) state.power = REMOTE_POWER_RUNNING;
    else state.power = REMOTE_POWER_TIMER;
    httpd_resp_send(req, "Done", HTTPD_RESP_USE_STRLEN);
    remote_transmit(&state, prev_power);
    return ESP_OK;
}

esp_err_t stop_handler(httpd_req_t *req) {
    uint8_t prev_power = state.power;
    state.power = REMOTE_POWER_STOPPED;
    httpd_resp_send(req, "Done", HTTPD_RESP_USE_STRLEN);
    remote_transmit(&state, prev_power);
    return ESP_OK;
}

httpd_uri_t uri_home = {
    .uri        = "/",
    .method     = HTTP_GET,
    .handler    = home_handler,
    .user_ctx   = NULL
};
httpd_uri_t uri_script = {
    .uri        = "/script.js",
    .method     = HTTP_GET,
    .handler    = script_handler,
    .user_ctx   = NULL
};
httpd_uri_t uri_state = {
    .uri        = "/state.csv",
    .method     = HTTP_GET,
    .handler    = state_handler,
    .user_ctx   = NULL
};
httpd_uri_t uri_run = {
    .uri        = "/run",
    .method     = HTTP_POST,
    .handler    = run_handler,
    .user_ctx   = NULL
};
httpd_uri_t uri_stop = {
    .uri        = "/stop",
    .method     = HTTP_POST,
    .handler    = stop_handler,
    .user_ctx   = NULL
};

void app_main(void) {
    // IR transmitter
    rmt_config_t remote_config = {
        .channel       = REMOTE_CHANNEL,
        .rmt_mode      = RMT_MODE_TX,
        .gpio_num      = GPIO_NUM_25,
        .mem_block_num = 1,
        .clk_div       = 80,
        .flags         = 0,
        .tx_config.loop_en             = false,
        .tx_config.carrier_en          = true,
        .tx_config.carrier_freq_hz     = 38000,
        .tx_config.carrier_duty_percent= 33,
        .tx_config.carrier_level       = RMT_CARRIER_LEVEL_HIGH,
        .tx_config.idle_output_en      = false
    };
    rmt_config(&remote_config);
    rmt_driver_install(remote_config.channel, 0, 0);

    // Timer


    // Wifi
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    wifi_init_sta();

    // // Flash memory
    esp_vfs_spiffs_conf_t flash_config = {
        .base_path              = "/spiffs",
        .partition_label        = NULL,
        .max_files              = 5,
        .format_if_mount_failed = true
    };
    ESP_ERROR_CHECK(esp_vfs_spiffs_register(&flash_config));

    // // HTTP
    httpd_config_t http_config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    if(httpd_start(&server, &http_config) == ESP_OK) {
        httpd_register_uri_handler(server, &uri_home);
        httpd_register_uri_handler(server, &uri_script);
        httpd_register_uri_handler(server, &uri_state);
        httpd_register_uri_handler(server, &uri_run);
        httpd_register_uri_handler(server, &uri_stop);
    }
}