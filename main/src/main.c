#include "nvs_flash.h"
#include "wifi.h"
#include "esp_http_server.h"

esp_err_t home_handler(httpd_req_t *req) {
    const char resp[] = "<h1>Hello, world!</h1>";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

httpd_uri_t uri_home = {
    .uri        = "/",
    .method     = HTTP_GET,
    .handler    = home_handler,
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

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    if(httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &uri_home);
    }
}