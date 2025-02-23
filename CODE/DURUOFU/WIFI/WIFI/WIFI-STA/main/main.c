#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_mac.h"

#define ESP_WIFI_STA_SSID "CSW@CEE"
#define ESP_WIFI_STA_PASSWD "88888888"

void WIFI_CallBack(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    static uint8_t connect_count = 0;
    // WIFI startup successful
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        ESP_LOGI("WIFI_EVENT", "WIFI_EVENT_STA_START");
        ESP_ERROR_CHECK(esp_wifi_connect());
    }
    // WIFI connection failed
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        ESP_LOGI("WIFI_EVENT", "WIFI_EVENT_STA_DISCONNECTED");
        connect_count++;
        if (connect_count < 6)
        {
            ESP_ERROR_CHECK(esp_wifi_connect());
        }
        else {
            ESP_LOGI("WIFI_EVENT", "WIFI_EVENT_STA_DISCONNECTED 10 times");
        }
    }
    // WIFI connection successful (IP acquired)
    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ESP_LOGI("WIFI_EVENT", "WIFI_EVENT_STA_GOT_IP");
        ip_event_got_ip_t *info = (ip_event_got_ip_t *)event_data;
        ESP_LOGI("WIFI_EVENT", "got ip:" IPSTR "", IP2STR(&info->ip_info.ip));
    }
}

void app_main(void)
{
    //---------------- Preparation Phase -------------------
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    //---------------- Initialization Phase -------------------
    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    // Register event (WIFI startup successful)
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_STA_START, WIFI_CallBack, NULL, NULL));
    // Register event (WIFI connection failed)
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, WIFI_CallBack, NULL, NULL));
    // Register event (WIFI connection successful - IP acquired)
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, WIFI_CallBack, NULL, NULL));

    // Initialize STA device
    esp_netif_create_default_wifi_sta();

    /* Initialize WiFi */
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    // WIFI_INIT_CONFIG_DEFAULT is a macro for the default configuration

    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    //---------------- Configuration Phase -------------------
    // Set to STA mode
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    // Detailed STA configuration
    wifi_config_t sta_config = {
        .sta = {
            .ssid = ESP_WIFI_STA_SSID,
            .password = ESP_WIFI_STA_PASSWD,
            .bssid_set = false,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config));

    //---------------- Startup Phase -------------------
    ESP_ERROR_CHECK(esp_wifi_start());
}
