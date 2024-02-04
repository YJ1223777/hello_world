#include <stdio.h>
#include "aliyunMQTT.h"
#include "GY30.h"
#include "DHT11.h"

/*
 * @brief Event handler registered to receive MQTT events
 *
 *  This function is called by the MQTT client event loop.
 *
 * @param handler_args user data registered to the event.
 * @param base Event base for the handler(always MQTT Base in this example).
 * @param event_id The id for the received event.
 * @param event_data The data for the event, esp_mqtt_event_handle_t.
 */

static const char *TAG_MQTT = "MQTT Test";

// esp_mqtt_client_handle_t client;

float lux=0.0;
static int AirTemp=0, AirHum=0;


static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG_MQTT, "Last error %s: 0x%x", message, error_code);
    }
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG_MQTT, "Event dispatched from event loop base=%s, event_id=%" PRIi32 "", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_CONNECTED");
        msg_id = esp_mqtt_client_publish(client, "/topic/qos1", "data_3", 0, 1, 0);
        ESP_LOGI(TAG_MQTT, "sent publish successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
        ESP_LOGI(TAG_MQTT, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
        ESP_LOGI(TAG_MQTT, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
        ESP_LOGI(TAG_MQTT, "sent unsubscribe successful, msg_id=%d", msg_id);

        //订阅主题
        msg_id = esp_mqtt_client_subscribe(client, AliyunPublishTopic, 0);
        ESP_LOGI(TAG_MQTT, "sent subscribe successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
        ESP_LOGI(TAG_MQTT, "sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG_MQTT, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
            ESP_LOGI(TAG_MQTT, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));

        }
        break;
    default:
        ESP_LOGI(TAG_MQTT, "Other event id:%d", event->event_id);
        break;
    }
}

// static void mqtt_publish_data_task(void *Param)
// {
//     char payload[1024];

//     // strcpy(payload,"{\"id\":1701069511681,\"params\":{\"airTemp\":27.3,\"airHumi\":44,\"LightLux\":876.67},\"version\":\"1.0\",\"method\":\"thing.event.property.post\"}");

//     while(1)
//     {
//         lux = BH1750_ReadLightIntensity();
//         Get_DHT11_Data(&AirTemp, &AirHum);
//         float AirTemp1 = AirTemp/10 + AirTemp%10;
//         sprintf(payload, "{\"id\":1701069511681,\"params\":{\"airTemp\":%.1f,\"airHumi\":%d,\"LightLux\":%.2f},\"version\":\"1.0\",\"method\":\"thing.event.property.post\"}", AirTemp1, AirHum, lux);
//         esp_mqtt_client_publish(client, AliyunPublishTopic, payload, strlen(payload), 1, 0);
//         vTaskDelay(pdMS_TO_TICKS(1000));
// 	}
//     // vTaskDelete(NULL);
// }


void user_mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .host = Aliyun_host,
        .port = Aliyun_port,
        .transport = MQTT_TRANSPORT_OVER_TCP,
        .client_id = Aliyun_client_id,
        .username = Aliyun_username,
        .password = Aliyun_password,
        .protocol_ver = MQTT_PROTOCOL_V_3_1_1,
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);

    vTaskDelay(pdMS_TO_TICKS(1000));

	// xTaskCreate(&mqtt_publish_data_task, "pub data", 4096, NULL, 1, NULL);

    // 以下为测试代码
    // char payload[1024];
    // strcpy(payload,"{\"id\":1701069511681,\"params\":{\"airTemp\":27.3,\"airHumi\":44,\"LightLux\":876.67},\"version\":\"1.0\",\"method\":\"thing.event.property.post\"}");
    // esp_mqtt_client_publish(client, AliyunPublishTopic,payload, strlen(payload), 1, 0);

}

void publishData(void)
{
    char payload[1024];
    // esp_mqtt_client_config_t mqtt_cfg = {
    //     .host = Aliyun_host,
    //     .port = Aliyun_port,
    //     .transport = MQTT_TRANSPORT_OVER_TCP,
    //     .client_id = Aliyun_client_id,
    //     .username = Aliyun_username,
    //     .password = Aliyun_password
    // };
    // esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    
    strcpy(payload,"{\"id\":1701069511681,\"params\":{\"airTemp\":27.3,\"airHumi\":44,\"LightLux\":876.67},\"version\":\"1.0\",\"method\":\"thing.event.property.post\"}");
    // printf("%s\r\n", payload);

    // esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    // esp_mqtt_client_start(client);
    esp_mqtt_client_publish(client, AliyunPublishTopic,payload, strlen(payload), 1, 0);

    ESP_LOGI(TAG_MQTT, "data publish");
}