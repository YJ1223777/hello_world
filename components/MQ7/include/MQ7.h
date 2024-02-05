
#ifndef _MQ7_H_
#define _MQ7_H_

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

void MQ7_ADC2_CHANNEL_0_init(void);
float get_MQ7_Value(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif 
