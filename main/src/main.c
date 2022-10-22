#include "nvs_flash.h"
#include "wifi.h"
#include "esp_http_server.h"
#include "esp_spiffs.h"
#include "stdio.h"

esp_err_t home_handler(httpd_req_t *req) {
    char line[100];
    FILE *fs = fopen("/spiffs/index.html", "r");
    while(fgets(line, sizeof(line), fs)) {
        httpd_resp_send_chunk(req, line, HTTPD_RESP_USE_STRLEN);
    }
    httpd_resp_send_chunk(req, line, 0);
    fclose(fs);
    return ESP_OK;
}

esp_err_t script_handler(httpd_req_t *req) {
    char line[100];
    FILE *fs = fopen("/spiffs/script.js", "r");
    while(fgets(line, sizeof(line), fs)) {
        httpd_resp_send_chunk(req, line, HTTPD_RESP_USE_STRLEN);
    }
    httpd_resp_send_chunk(req, line, 0);
    fclose(fs);
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

void app_main(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    wifi_init_sta();

    esp_vfs_spiffs_conf_t conf = {
        .base_path              = "/spiffs",
        .partition_label        = NULL,
        .max_files              = 5,
        .format_if_mount_failed = true
    };
    ESP_ERROR_CHECK(esp_vfs_spiffs_register(&conf));

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    if(httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &uri_home);
        httpd_register_uri_handler(server, &uri_script);
    }
}