/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-12-05 18:31:03
 * @LastEditors: YJ\YJ_1223 YJ2386708206@163.com
 * @LastEditTime: 2024-01-30 15:04:13
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
TaskHandle_t readDataHandle;
TaskHandle_t pubDataHandle;

static int AirTempTest, AirHumTest;
float luxTest=0.0;

TimerHandle_t lvglTimer;

/*用定时器给LVGL提供时钟*/
static void lv_tick_task(void *arg)
{
	(void)arg;
	lv_tick_inc(10);
}

void readDataTask(void *Param)
{
    while (1)
    {
        printf("readDataTask\r\n");
        Get_DHT11_Data(&AirTempTest, &AirHumTest);
        luxTest = BH1750_ReadLightIntensity();
        ESP_LOGI(TAG, "[%lld] temp->%i.%i C     hum->%i%%     Lux->%.02f", esp_timer_get_time(), AirTempTest / 10, AirTempTest % 10, AirHumTest, luxTest);
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

    // DHT11_Init(DHT11_GPIO);
    // I2C_Init();
    // BH1750_Init();

    lv_init();
    lv_port_disp_init();

    /*  esp_register_freertos_tick_hook(lv_tick_task);
    Create and start a periodic timer interrupt to call lv_tick_inc 
    */
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "periodic_gui"};
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 10 * 1000));

    lv_demo_music();

    // wifi_init_sta();
    // user_mqtt_app_start();
    // vTaskDelay(pdMS_TO_TICKS(1000));

    // xTaskCreate(readDataTask, "readData", itemStackSize * 8, NULL, 1, &readDataHandle);

    while (1)
    {
        /* code */
        // Get_DHT11_Data(&AirTempTest, &AirHumTest);
        // luxTest = BH1750_ReadLightIntensity();
        // ESP_LOGI(TAG, "[%lld] temp->%i.%i C     hum->%i%%     Lux->%.02f", esp_timer_get_time(), AirTempTest / 10, AirTempTest % 10, AirHumTest, luxTest);
        lv_task_handler();
        
        vTaskDelay(pdMS_TO_TICKS(10));
    }

}
