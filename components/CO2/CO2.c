#include <stdio.h>
#include "CO2.h"

#define CO2_ADC_CHANNEL ADC1_CHANNEL_6
#define offsetVoltage 267


// adc斜率曲线
static esp_adc_cal_characteristics_t *adc_chars;


void CO2_ADC1_CHANNEL_6_init(void)
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(CO2_ADC_CHANNEL,ADC_ATTEN_DB_0);
    //Characterize ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_0, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);

    if (val_type == ESP_ADC_CAL_VAL_DEFAULT_VREF){
        ESP_LOGI("USER CO2", "使用默认参考电压");
    }else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF){
        ESP_LOGI("USER CO2", "使用 eFuse 中的电压");
    }
}

/**
 * 计算CO2含量 单位：ppm
*/
float get_CO2_Value(void)
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
    uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars)-offsetVoltage;
    float CO2_ppm = 100.0-(((float)voltage-330.0)/0.06778);
    // printf("Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);
    // printf("CO2_ppm: %.2f\r\n", CO2_ppm);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    return CO2_ppm;
}
