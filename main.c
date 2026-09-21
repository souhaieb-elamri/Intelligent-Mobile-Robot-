#include "stm32f4xx.h"
#include "gpio.h"
#include "motor.h"
#include "adc.h"
#include "usart.h"
#include "exti.h"
#include "esp8266.h"
#include "i2c.h"
#include "config.h"
#include <stdio.h>

int main(void) {
    Config_GPIO();
    Config_TIM3();
    TIM3->CR1 |= 1;        // START TIM3 PWM
    Config_TIM2();
    Config_USART2();
    Config_USART1(9600);   // Bluetooth
    USART3_Config(115200); // ESP8266
    Config_DMA2_ADC1();
    config_adc1();
    config_EXTI();
    Config_I2C1();
    I2C1_Init_DS1621();

    TIM2->CR1 |= 1;
    ESP_Config();

    SendString_USART2("\r\n--- SYSTEM READY ---\r\n");
    SendString_USART1("BT OK\r\n");

    while (1) {
        if (adc_ready) {
            adc_ready = 0;

            int8_t temperature = I2C1_Read_Temp();

            char debug[80];
            sprintf(debug, "ADC=%d,%d,%d  Temp=%dC\r\n",
                    adc_val[0], adc_val[1], adc_val[2], temperature);
            SendString_USART2(debug);

            char http[250];
            sprintf(http,
                "GET /update?api_key=" THINGSPEAK_API_KEY
                "&field1=%d&field2=%d&field3=%d&field4=%d"
                " HTTP/1.0\r\nHost: " THINGSPEAK_HOST "\r\n\r\n",
                adc_val[0], adc_val[1], adc_val[2], temperature);
            sendDataToCloud(http);

            char json_payload[120];
            sprintf(json_payload,
                "{\"adc1\":%d,\"adc2\":%d,\"adc3\":%d,\"temp\":%d}",
                adc_val[0], adc_val[1], adc_val[2], temperature);
            sendDataToNodeRED(json_payload);
        }
    }
}