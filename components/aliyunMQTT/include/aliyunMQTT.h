/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-12-07 19:01:51
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2024-01-05 16:05:21
 * @FilePath: \hello_world\components\MQTT\include\MQTT.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%A
 */
#ifndef _ALIYUNMQTT_H_
#define _ALIYUNMQTT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "mqtt_client.h"

/*Broker Address：${YourProductKey}.iot-as-mqtt.${YourRegionId}.aliyuncs.com*/
#define   Aliyun_host       "iot-06z00h7m3i2gldy.mqtt.iothub.aliyuncs.com"
#define   Aliyun_port       1883
/*Client ID：     ${ClientID}|securemode=${Mode},signmethod=${SignMethod}|*/
#define   Aliyun_client_id  "idsyUiYx0qO.ESP32_farm|securemode=2,signmethod=hmacsha256,timestamp=1701067694329|"
/*User Name：     ${DeviceName}&${ProductKey}*/
#define   Aliyun_username   "ESP32_farm&idsyUiYx0qO"
/*使用官网 MQTT_Password 工具生成*/
#define   Aliyun_password   "e92ae7913e4ad6cb40871c61695131486f52e4eb24552d72c162d77eab2fdbb4"
/*a1Pvdruq4mz/${deviceName}/user/update*/   
#define   AliyunPublishTopic_user_update    "/idsyUiYx0qO/ESP32_farm/user/update"
#define   AliyunSubscribeTopic_user_get     "/idsyUiYx0qO/ESP32_farm/user/get"
#define   AliyunPublishTopic                "/sys/idsyUiYx0qO/ESP32_farm/thing/event/property/post" 

esp_mqtt_client_handle_t client;

void user_mqtt_app_start(void);
void publishData(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif 
