/*
 * @Author: YJ\YJ_1223 YJ2386708206@163.com
 * @Date: 2024-02-05 23:01:15
 * @LastEditors: YJ\YJ_1223 YJ2386708206@163.com
 * @LastEditTime: 2024-02-05 23:22:47
 * @FilePath: \hello_world\components\MQ7\MQ7.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include "MQ7.h"

#define MQ7_ADC_CHANNEL ADC2_CHANNEL_0

#define CAL_PPM  10  // 校准环境中PPM值
#define RL	10  // RL阻值
#define R0	16  // R0阻值

// adc斜率曲线
static esp_adc_cal_characteristics_t *adc_chars;

void MQ7_ADC2_CHANNEL_0_init(void)
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(MQ7_ADC_CHANNEL,ADC_ATTEN_DB_0);
    //Characterize ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_2, ADC_ATTEN_DB_0, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);

    if (val_type == ESP_ADC_CAL_VAL_DEFAULT_VREF){
        ESP_LOGI("USER MQ2", "使用默认参考电压");
    }else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF){
        ESP_LOGI("USER MQ2", "使用 eFuse 中的电压");
    }
}

// static float R0;
 
// // 传感器校准函数
// void MQ7_PPM_Calibration(float RS)
// {
//     R0 = RS / pow(CAL_PPM / 98.322, 1 / -1.458f);
// }

/**
 * 计算MQ7测量的CO含量 单位：ppm
*/
float get_MQ7_Value(void)
{
    uint32_t adc_reading = 0;
    //Multisampling
    for (int i = 0; i < NUM_OF_SAMPLES; i++) {
        int raw;
        raw = adc1_get_raw(ADC1_CHANNEL_6);
        adc_reading += raw;
    }
    adc_reading /= NUM_OF_SAMPLES;
    // printf("adc_reading:%d\r\n",adc_reading);
    //Convert adc_reading to voltage in mV
    uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
    float coVol = voltage/1000.0f;
    float RS = (3.3f - coVol) / coVol * RL;
    float MQ7_ppm = 98.322f * pow(RS/R0, -1.458f);
    ESP_LOGI("MQ7 Test","MQ7_ppm:%.2f",MQ7_ppm);
    return MQ7_ppm;
}

