idf_component_register(SRCS "aliyunMQTT.c"
                    INCLUDE_DIRS "include"
                    REQUIRES    mqtt
                                GY30
                                DHT11)
