/*
 * @Author: YJ\YJ_1223 YJ2386708206@163.com
 * @Date: 2023-12-05 18:31:03
 * @LastEditors: YJ\YJ_1223 YJ2386708206@163.com
 * @LastEditTime: 2024-02-20 21:24:27
 * @FilePath: \hello_world\main\hello_world_main.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

// 系统
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "esp_task_wdt.h"
#include "freertos/semphr.h"
#include "esp_timer.h"

// 传感器
#include "DHT11.h"
#include "GY30.h"
#include "CO2.h"
#include "MQ2.h"
#include "MQ7.h"

// WiFi 上云
#include "WiFiConnect.h"
#include "aliyunMQTT.h"

// 其余外设 LCD
#include "lvgl.h"
#include "lv_demos.h"
#include "lv_port_disp.h"

#define itemStackSize 1024
const static char *TAG = "smart farm";
int WiFiState=0;
TaskHandle_t dealDataTaskHandle;
// TaskHandle_t pubDataHandle;

float* DHT11Res;
float luxTest=0.0;
float co2Val,smogVal,coVal;

TimerHandle_t lvglTimer;

/*用定时器给LVGL提供时钟*/
// static void lv_tick_task(void *arg)
// {
// 	(void)arg;
// 	lv_tick_inc(10);
// }

void dealDataTask(void *Param)
{
    while (1)
    {
        char payload[1024];
        DHT11Res = readDHT11Data();
        luxTest = BH1750_ReadLightIntensity();
        co2Val = get_CO2_Value();
        smogVal = get_MQ2_Value();
        coVal = get_MQ7_Value();
        // ESP_LOGI(TAG, "temp->%.01fC   hum->%d  Lux->%.02f", DHT11Res[0], (int)DHT11Res[1], luxTest);
        sprintf(payload,
        "{\"id\":1701069511681,\"params\":{\"airTemp\":%.1f,\"airHumi\":%d,\"lightLux\":%.2f,\"co\":%.2f,\"co2\":%.2f,\"smog\":%.2f,\"soilHumidity\":9.99,\"waterLevel\":9.99},\"version\":\"1.0\",\"method\":\"thing.event.property.post\"}"
        ,DHT11Res[0],(int)DHT11Res[1],luxTest,coVal,smogVal,co2Val);
        esp_mqtt_client_publish(client, AliyunPublishTopic,payload, strlen(payload), 1, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }   
}

void app_main(void)
{
    ESP_LOGI(TAG, "APP Start......");

    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    vTaskDelay(pdMS_TO_TICKS(1000));

    I2C_Init();
    BH1750_Init();
    CO2_ADC1_CHANNEL_6_init();
    MQ2_ADC2_CHANNEL_7_init();
    MQ7_ADC2_CHANNEL_0_init();

    // lv_init();
    // lv_port_disp_init();

    /*  esp_register_freertos_tick_hook(lv_tick_task);
    Create and start a periodic timer interrupt to call lv_tick_inc 
    */
    // const esp_timer_create_args_t periodic_timer_args = {
    //     .callback = &lv_tick_task,
    //     .name = "periodic_gui"};
    // esp_timer_handle_t periodic_timer;
    // ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    // ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 10 * 1000));

    // lv_demo_music();

    wifi_init_sta();
    user_mqtt_app_start();
    vTaskDelay(pdMS_TO_TICKS(2000));

    xTaskCreate(dealDataTask, "readData", itemStackSize * 4, NULL, 5, &dealDataTaskHandle);

    while (1)
    {
        /* code */
        // Get_DHT11_Data(&AirTempTest, &AirHumTest);
        // luxTest = BH1750_ReadLightIntensity();
        // ESP_LOGI(TAG, "[%lld] temp->%i.%i C     hum->%i%%     Lux->%.02f", esp_timer_get_time(), AirTempTest / 10, AirTempTest % 10, AirHumTest, luxTest);
        // lv_task_handler();
        // printf("temp:%.2f,humi:%.2f\r\n",DHT11Res[0],DHT11Res[1]);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}
 