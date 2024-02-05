/*
 * @Author: YJ\YJ_1223 YJ2386708206@163.com
 * @Date: 2024-02-05 23:01:31
 * @LastEditors: YJ\YJ_1223 YJ2386708206@163.com
 * @LastEditTime: 2024-02-05 23:06:41
 * @FilePath: \hello_world\components\MQ2\include\MQ2.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#ifndef _MQ2_H_
#define _MQ2_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_log.h"

#define DEFAULT_VREF    2100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NUM_OF_SAMPLES   64          //Multisampling

void MQ2_ADC2_CHANNEL_7_init(void);
float get_MQ2_Value(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif 
