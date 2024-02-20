#include <stdio.h>
#include "MQ2.h"

#define MQ2_ADC_CHANNEL ADC2_CHANNEL_7
#define CAL_PPM  10  // 校准环境中PPM值
#define RL	     10  // RL阻值
#define R0	     26  // R0阻值

// adc斜率曲线
static esp_adc_cal_characteristics_t *adc_chars;

void MQ2_ADC2_CHANNEL_7_init(void)
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(MQ2_ADC_CHANNEL,ADC_ATTEN_DB_0);
    //Characterize ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_2, ADC_ATTEN_DB_0, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);

    if (val_type == ESP_ADC_CAL_VAL_DEFAULT_VREF){
        ESP_LOGI("USER MQ2", "使用默认参考电压");
    }else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF){
        ESP_LOGI("USER MQ2", "使用 eFuse 中的电压");
    }
}

/**
 * 计算MQ2测量的烟雾含量 单位：ppm
*/
float get_MQ2_Value(void)
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
    float smogVol = voltage/1000.0f;
    float RS = (3.3f - smogVol) / smogVol * RL;
	float MQ2_ppm = 98.322f * pow(RS/R0, -1.458f);
    // ESP_LOGI("MQ2 Test","MQ2_ppm:%.2f",MQ2_ppm);
    return MQ2_ppm;
}
