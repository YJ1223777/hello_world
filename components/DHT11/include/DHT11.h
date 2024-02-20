/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-11-13 13:55:39
 * @LastEditors: YJ\YJ_1223 YJ2386708206@163.com
 * @LastEditTime: 2024-02-19 10:32:40
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
#define uchar unsigned char
#define uint8 unsigned char
#define uint16 unsigned short

// void DHT11_Init(uint8_t dht11_pin);
// int Get_DHT11_Data(int *temperature, int *humidity);

static void InputInitial(void);
static void OutputHigh(void);
static void OutputLow(void);
static uint8 getData(void);
static void COM(void);
float* readDHT11Data(void);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif 

