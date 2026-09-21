/* ============ esp8266.h ============ */
#ifndef __ESP8266_H
#define __ESP8266_H

#include <stdint.h>

void SendATCommand(char* cmd);
void ESP_Config(void);
void sendDataToCloud(char* http);
void sendDataToNodeRED(char* json_payload);
void esp_delay(uint32_t ms);

#endif