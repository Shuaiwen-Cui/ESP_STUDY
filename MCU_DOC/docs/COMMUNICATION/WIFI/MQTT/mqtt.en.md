# MQTT PROTOCOL

## 1. Introduction

> Like HTTP, MQTT runs on top of the Transmission Control Protocol/Internet Protocol (TCP/IP) stack.

### 1.1 Introduction to MQTT

> [MQTT Official Website](https://mqtt.org/)  
> [Getting Started with MQTT Protocol](https://www.emqx.com/zh/blog/the-easiest-guide-to-getting-started-with-mqtt)

MQTT (Message Queuing Telemetry Transport) is a lightweight communication protocol based on the publish-subscribe model, primarily used for communication between devices in the Internet of Things (IoT).

![](20240327113720.png)

The MQTT protocol uses a simple binary encoding format, making it suitable for environments with limited bandwidth, high latency, or unstable networks. Additionally, MQTT provides a Quality of Service (QoS) mechanism with three different QoS levels:

- **QoS 0:** At most once delivery; no reliability guarantee.
- **QoS 1:** At least once delivery; ensures the message reaches the receiver, but duplicates may occur.
- **QoS 2:** Exactly once delivery; ensures the message reaches the receiver only once.

Key features of MQTT include excellent scalability, low overhead, ease of implementation, open-source nature, and support for multiple programming languages. In MQTT, there are two main participants: the publisher and the subscriber. The publisher sends messages to one or more topics, and subscribers can subscribe to topics of interest to receive those messages. Topics serve as a classification standard for specific types of messages.

MQTT is used in various scenarios including, but not limited to, smart homes, smart lighting, smart security, agricultural IoT, and industrial IoT.

##### Advantages

1. **Lightweight and Efficient:** MQTT clients are very small and require minimal resources, making them suitable for use on small microcontrollers. The MQTT message header is also very small, which helps optimize network bandwidth.
2. **Bidirectional Communication:** MQTT allows messages to be transmitted from device to cloud and vice versa, making it easy to broadcast messages to groups of devices.
3. **Scalable to Millions of Devices:** MQTT can scale to connect millions of IoT devices.
4. **Reliable Message Delivery:** Reliable message delivery is critical for many IoT use cases. That is why MQTT defines three Quality of Service (QoS) levels: 0 - at most once, 1 - at least once, and 2 - exactly once.
5. **Supports Unreliable Networks:** Many IoT devices connect over unreliable cellular networks. MQTT's support for persistent sessions reduces the time required to reconnect a client to the broker.
6. **Built-in Security:** MQTT makes it easy to encrypt messages using TLS and to authenticate clients using modern authentication protocols, such as OAuth.

### 1.2 MQTT Packet Structure

- **Fixed Header**: Present in all MQTT packets, indicating the packet type and associated flags.
- **Variable Header**: Present in some MQTT packets; the packet type determines whether a variable header exists and what its specific contents are.
- **Payload**: Present in some MQTT packets, representing the actual content delivered to the client.

![](20240327114242.png)

### 1.3 Other MQTT Concepts

**1. Subscription**

A subscription consists of a Topic Filter and a maximum Quality of Service (QoS). Subscriptions are associated with a session, and a session can contain multiple subscriptions. Each subscription within a session has a distinct Topic Filter.

**2. Session**

Once a client connects to the server, a session is established, allowing stateful interactions between the client and the server. A session exists within one network and may span multiple consecutive network connections between the client and the server.

**3. Topic Name**

A label attached to an application message that matches the subscriptions on the server. The server will send the message to each client whose subscription matches the topic label.

**4. Topic Filter**

A wildcard filter for topic names, used in subscription expressions, representing multiple topics that match the subscription criteria.

**5. Payload**

The actual content received by the message subscriber.

For more details, refer to the MQTT protocol documentation: [MQTT Version 5.0](extension://bfdogplmndidlpjfhoijckpakkdjkkil/pdf/viewer.html?file=https%3A%2F%2Fdocs.oasis-open.org%2Fmqtt%2Fmqtt%2Fv5.0%2Fmqtt-v5.0.pdf)

### 1.4 MQTT Support on ESP32

> https://docs.espressif.com/projects/esp-idf/zh_CN/v5.1.3/esp32/api-reference/protocols/mqtt.html

- Supports MQTT over TCP, MQTT over SSL based on Mbed TLS, MQTT over WebSocket, and MQTT over Secure WebSocket.
- Simplifies configuration through the use of URIs.
- Supports multiple instances (i.e., multiple clients within a single application).
- Provides support for subscription, publishing, authentication, last will messages, keep-alive heartbeats, and three Quality of Service (QoS) levels, making it a fully-featured client.

## 2. Usage

> Example code reference: https://github.com/espressif/esp-idf/tree/master/examples/protocols/mqtt/tcp

#### 2.1 Creating a Simple MQTT Client

```c
// MQTT Client Application Start Function
static void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = CONFIG_BROKER_URL,
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument can be used to pass data to the event handler, in this example mqtt_event_handler */
    // Register the event handler
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    // Start the MQTT client
    esp_mqtt_client_start(client);
}
```

The code above is very straightforward, so we will not elaborate on it further.

#### 2.2 Event Handling

Event handling is the key part of the MQTT client implementation. Below is the code:

```c
// MQTT Client Event Handler
/*
 * @brief Event handler registered to receive MQTT events.
 *
 * This function is called by the MQTT client event loop.
 *
 * @param handler_args User data registered to the event.
 * @param base Event base for the handler (always MQTT Base in this example).
 * @param event_id The ID for the received event.
 * @param event_data The data for the event, of type esp_mqtt_event_handle_t.
 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32 "", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id)
    {
        // MQTT connected successfully
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            // Publish a message
            msg_id = esp_mqtt_client_publish(client, "/topic/qos1", "data_3", 0, 1, 0);
            ESP_LOGI(TAG, "Sent publish successfully, msg_id=%d", msg_id);
            // Subscribe to topics
            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
            ESP_LOGI(TAG, "Sent subscribe successfully, msg_id=%d", msg_id);
            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
            ESP_LOGI(TAG, "Sent subscribe successfully, msg_id=%d", msg_id);
            // Unsubscribe from a topic
            msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
            ESP_LOGI(TAG, "Sent unsubscribe successfully, msg_id=%d", msg_id);
            break;
        // MQTT disconnected
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;
        // MQTT subscription successful
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            // Publish a message
            msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
            ESP_LOGI(TAG, "Sent publish successfully, msg_id=%d", msg_id);
            break;
        // MQTT unsubscription successful
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        // MQTT published successfully
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        // MQTT data received
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            break;
        // MQTT error occurred
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT)
            {
                log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
                log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
                log_error_if_nonzero("captured as transport's socket errno", event->error_handle->esp_transport_sock_errno);
                ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
            }
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
}
```
- `MQTT_EVENT_CONNECTED`: Handles the event when the MQTT client successfully connects to the broker. In this case, it demonstrates how to publish, subscribe to, and unsubscribe from MQTT topics.
- `MQTT_EVENT_DISCONNECTED`: Handles the event when the MQTT client disconnects from the broker.
- `MQTT_EVENT_SUBSCRIBED`: Handles the event when the MQTT client successfully subscribes to a topic.
- `MQTT_EVENT_UNSUBSCRIBED`: Handles the event when the MQTT client successfully unsubscribes from a topic.
- `MQTT_EVENT_PUBLISHED`: Handles the event when the MQTT client successfully publishes a message.
- `MQTT_EVENT_DATA`: Handles the event when data is received from the broker. In this case, it prints the topic and data.
- `MQTT_EVENT_ERROR`: Handles the event when the MQTT client encounters an error.


## 3. Instance

```c
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_mac.h"
#include "esp_netif.h"
#include <sys/socket.h>
#include "esp_eth.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "mqtt_client.h"

// WiFi network to connect to
#define ESP_WIFI_STA_SSID "duruofu_win10"
#define ESP_WIFI_STA_PASSWD "1234567890"

static const char *TAG = "main";

// WiFi event callback
void WIFI_CallBack(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    static uint8_t connect_count = 0;
    // WiFi started successfully
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        ESP_LOGI("WIFI_EVENT", "WIFI_EVENT_STA_START");
        ESP_ERROR_CHECK(esp_wifi_connect());
    }
    // WiFi disconnected (connection failed)
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        ESP_LOGI("WIFI_EVENT", "WIFI_EVENT_STA_DISCONNECTED");
        connect_count++;
        if (connect_count < 6)
        {
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            ESP_ERROR_CHECK(esp_wifi_connect());
        }
        else
        {
            ESP_LOGI("WIFI_EVENT", "WIFI_EVENT_STA_DISCONNECTED 10 times");
        }
    }
    // WiFi connected and IP obtained
    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ESP_LOGI("WIFI_EVENT", "WIFI_EVENT_STA_GOT_IP");
        ip_event_got_ip_t *info = (ip_event_got_ip_t *)event_data;
        ESP_LOGI("WIFI_EVENT", "got ip:" IPSTR "", IP2STR(&info->ip_info.ip));
    }
}

// WiFi initialization in station mode
static void wifi_sta_init(void)
{
    ESP_ERROR_CHECK(esp_netif_init());

    // Register event: WiFi started
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_STA_START, WIFI_CallBack, NULL, NULL));
    // Register event: WiFi disconnected
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, WIFI_CallBack, NULL, NULL));
    // Register event: IP obtained
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, WIFI_CallBack, NULL, NULL));

    // Create default WiFi station interface
    esp_netif_create_default_wifi_sta();

    /* Initialize WiFi */
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    // WIFI_INIT_CONFIG_DEFAULT is a macro for default configuration

    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    //---------------- Configuration Phase -------------------
    // Set WiFi mode to station
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    // Station configuration
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

    //---------------- Configure Power Saving Mode -------------------
    // No power saving (for faster data transmission)
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
}

static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0)
    {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}

// MQTT client event handler
/*
 * @brief Event handler registered to receive MQTT events.
 *
 * This function is called by the MQTT client event loop.
 *
 * @param handler_args User data registered to the event.
 * @param base Event base for the handler (always MQTT Base in this example).
 * @param event_id The ID for the received event.
 * @param event_data The data for the event, of type esp_mqtt_event_handle_t.
 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32 "", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id)
    {
        // MQTT connected successfully
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            // Publish a message
            msg_id = esp_mqtt_client_publish(client, "/topic/qos1", "data_3", 0, 1, 0);
            ESP_LOGI(TAG, "Sent publish successfully, msg_id=%d", msg_id);
            // Subscribe to topics
            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
            ESP_LOGI(TAG, "Sent subscribe successfully, msg_id=%d", msg_id);
            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
            ESP_LOGI(TAG, "Sent subscribe successfully, msg_id=%d", msg_id);
            // Unsubscribe from a topic
            msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
            ESP_LOGI(TAG, "Sent unsubscribe successfully, msg_id=%d", msg_id);
            break;
        // MQTT disconnected
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;
        // MQTT subscribed successfully
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            // Publish a message
            msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
            ESP_LOGI(TAG, "Sent publish successfully, msg_id=%d", msg_id);
            break;
        // MQTT unsubscribed successfully
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        // MQTT published successfully
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        // MQTT data received
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            break;
        // MQTT error occurred
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT)
            {
                log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
                log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
                log_error_if_nonzero("captured as transport's socket errno", event->error_handle->esp_transport_sock_errno);
                ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
            }
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
}

// MQTT client start function
static void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = "mqtt://www.duruofu.top:1883",
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument can be used to pass data to the event handler, in this example mqtt_event_handler */
    // Register the event handler
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    // Start the MQTT client
    esp_mqtt_client_start(client);
}

void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Create the default event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Initialize and start WiFi
    wifi_sta_init();

    // Wait for WiFi connection (temporary solution)
    vTaskDelay(5000 / portTICK_PERIOD_MS);

    // Start the MQTT client
    mqtt_app_start();
}
```