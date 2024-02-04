idf_component_register(SRCS "WiFiConnect.c"
                    INCLUDE_DIRS "include"
                    REQUIRES    esp_wifi
                                nvs_flash
                                mqtt)
