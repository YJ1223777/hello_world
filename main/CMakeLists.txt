idf_component_register(SRCS "hello_world_main.c"
                    INCLUDE_DIRS "."
                    REQUIRES    DHT11
                                GY30
                                WiFiConnect
                                aliyunMQTT
                                lvgl
                                lvgl_esp32_drivers
                                lvgl_porting
                                CO2
                                MQ2
                                MQ7
                                )
