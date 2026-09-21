/* ============ esp8266.c ============ */
#include "stm32f4xx.h"
#include "esp8266.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

void esp_delay(uint32_t ms) {
    for(uint32_t i = 0; i < ms * 8000; i++) {
        __NOP();
    }
}

void SendATCommand(char* cmd) {
    SendString_USART3(cmd);
    SendString_USART3("\r\n");
}

void ESP_Config(void) {
    SendATCommand("AT");
    esp_delay(500);
    SendATCommand("AT+CWMODE=1");
    esp_delay(500);
    SendATCommand("AT+CWQAP");
    esp_delay(500);
    SendString_USART3("AT+CWJAP=\"Salsabil\",\"77777777\"\r\n");
    esp_delay(7000);
    SendATCommand("AT+CIFSR");
    esp_delay(500);
}

void sendDataToCloud(char* http) {
    char AT_cmd[60];
    int len = strlen(http);

    SendATCommand("AT+CIPCLOSE");
    esp_delay(1000);

    SendATCommand("AT+CIPSTART=\"TCP\",\"184.106.153.149\",80");
    esp_delay(3000);

    sprintf(AT_cmd, "AT+CIPSEND=%d", len);
    SendString_USART3(AT_cmd);
    SendString_USART3("\r\n");
    esp_delay(1500);

    SendString_USART3(http);
    esp_delay(5000);

    SendATCommand("AT+CIPCLOSE");
    esp_delay(500);
}

void sendDataToNodeRED(char* json_payload) {
    char post_req[400];
    char at_cmd[60];
    int json_len = strlen(json_payload);

    SendATCommand("AT+CIPCLOSE");
    esp_delay(1000);

    SendATCommand("AT+CIPSTART=\"TCP\",\"10.30.157.46\",1880");
    esp_delay(3000);

    sprintf(post_req,
        "POST /stm32-data HTTP/1.1\r\n"
        "Host: 192.168.1.50:1880\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n\r\n"
        "%s", json_len, json_payload);

    int req_len = strlen(post_req);
    sprintf(at_cmd, "AT+CIPSEND=%d", req_len);
    SendString_USART3(at_cmd);
    SendString_USART3("\r\n");
    esp_delay(1500);

    SendString_USART3(post_req);
    esp_delay(3000);

    SendATCommand("AT+CIPCLOSE");
    esp_delay(500);
}