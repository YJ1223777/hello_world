/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-11-13 13:55:39
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2024-01-05 17:00:03
 * @FilePath: \hello_world\components\DHT11\include\DHT11.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _DHT11_H_
#define _DHT11_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <driver/rmt.h>
// #include <driver/rmt_tx.h>
// #include <driver/rmt_rx.h>
#include <soc/rmt_reg.h>
#include "esp_log.h"
#include "driver/gpio.h" 
#include "esp_rom_sys.h"
#include <nvs_flash.h>

#define DHT11_GPIO 26

void DHT11_Init(uint8_t dht11_pin);
int Get_DHT11_Data(int *temperature, int *humidity);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif 

