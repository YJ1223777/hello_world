/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-11-13 13:55:39
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2024-01-05 16:59:54
 * @FilePath: \hello_world\components\DHT11\DHT11.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include "DHT11.h"

#define TAG "dht11"

int temp_x10 = 123;
int humidity = 60;
const int channel = 1;

uint8_t DHT11_PIN = -1;
/*RMT分频后可以获得一个方波信号，这里将这个方波信号的周期称为tick，
之后由RMT生成的信号都是基于tick的
比如说tick为1us，那么生成信号低电平或高电平时间必然是n*tick，n=0，1，2，...*/
void DHT11_Init(uint8_t dht11_pin)
{
    DHT11_PIN = dht11_pin;
	const int RMT_CLK_DIV = 80;                                   // RMT计数器时钟分频器
	const int RMT_TICK_10_US = (80000000 / RMT_CLK_DIV / 100000); // RMT计数器10us.(时钟源是APB时钟)
	const int rmt_item32_tIMEOUT_US = 1000;                       // RMT接收超时us
	rmt_config_t rmt_rx;
	rmt_rx.gpio_num = dht11_pin;
	rmt_rx.channel = channel;
	rmt_rx.clk_div = RMT_CLK_DIV;
	rmt_rx.mem_block_num = 1;
	rmt_rx.rmt_mode = RMT_MODE_RX;
	rmt_rx.rx_config.filter_en = false;
	rmt_rx.rx_config.filter_ticks_thresh = 100;
	rmt_rx.rx_config.idle_threshold = rmt_item32_tIMEOUT_US / 10 * (RMT_TICK_10_US);
	rmt_config(&rmt_rx);
	rmt_driver_install(rmt_rx.channel, 1000, 0);
}


// 将RMT读取到的脉冲数据处理为温度和湿度
static int parse_items(rmt_item32_t *item, int item_num, int *humidity, int *temp_x10)
{
	int i = 0;
	unsigned rh = 0, temp = 0, checksum = 0;
	if (item_num < 41){					// 检查是否有足够的脉冲数
		return 0;
	}
	item++;								// 跳过开始信号脉冲
	for (i = 0; i < 16; i++, item++){	// 提取湿度数据
		rh = (rh << 1) + (item->duration1 < 35 ? 0 : 1);
	}
	for (i = 0; i < 16; i++, item++){	// 提取温度数据
		temp = (temp << 1) + (item->duration1 < 35 ? 0 : 1);
	}
	for (i = 0; i < 8; i++, item++){	// 提取校验数据
		checksum = (checksum << 1) + (item->duration1 < 35 ? 0 : 1);
	}

	// 检查校验
	if ((((temp >> 8) + temp + (rh >> 8) + rh) & 0xFF) != checksum){
		printf("Checksum failure %4X %4X %2X\n", temp, rh, checksum);
		return 0;
	}

	// 返回数据
	// *humidity = rh >> 8;
	*humidity = (rh >> 8);
	*temp_x10 = (temp >> 8) * 10 + (temp & 0xFF);
	return 1;

}

// 使用RMT接收DHT11数据
int Get_DHT11_Data(int *temp_x10, int *humidity)
{
	RingbufHandle_t rb = NULL;
	size_t rx_size = 0;
	rmt_item32_t *item;
	int rtn = 0;
	//获得RMT RX环形缓冲区句柄，并处理RX数据
	rmt_get_ringbuf_handle(channel, &rb);
	if (!rb){
		return 0;
	}
	//发送20ms脉冲启动DHT11单总线
	gpio_set_level(DHT11_PIN, 1);
	gpio_set_direction(DHT11_PIN, GPIO_MODE_OUTPUT);
	esp_rom_delay_us(1000);
	gpio_set_level(DHT11_PIN, 0);
	esp_rom_delay_us(20000);
 
	//将rmt_rx_start和rmt_rx_stop放入缓存
	rmt_rx_start(channel, 1);
	rmt_rx_stop(channel);
 
	//信号线设置为输入准备接收数据
	gpio_set_level(DHT11_PIN, 1);
	gpio_set_direction(DHT11_PIN, GPIO_MODE_INPUT);
 
	//这次启动RMT接收器以获取数据
	rmt_rx_start(channel, 1);
 
	//从环形缓冲区中取出数据
	item = (rmt_item32_t *)xRingbufferReceive(rb, &rx_size, 2);
	if (item){
		int n;
		n = rx_size / 4 - 0;
		printf("number:%d\r\n", n);
		// 解析来自ringbuffer的数据值.
		rtn = parse_items(item, n, humidity, temp_x10);
		// 解析数据后，将空格返回到ringbuffer.
		vRingbufferReturnItem(rb, (void *)item);
	}
	//停止RMT接收
	rmt_rx_stop(channel);
	return rtn;
	
}



