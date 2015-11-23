#ifndef _UTILS_H
#define _UTILS_H

#define DEBUG

#include <Arduino.h>
#include <ESP8266WiFi.h>

namespace Utils
{
  extern WiFiClient logClient;

  extern const uint8_t LED_PIN;
  extern const uint8_t DIGITAL_PIN;
  extern const uint8_t TEST_PIN;

  extern const char* WiFiSSID;
  extern const char* WiFiPSK;

  extern const uint16_t API_PORT;
  extern const char* SERVER_DOMAIN;
  extern const char* API_SERVER;

  extern const char* LOGGING_DOMAIN;
  extern const uint16_t LOGGING_PORT;

  extern void busyWait(unsigned int timeMs);

  extern void netLog(String str);

  extern void ASSERT(uint16_t _timeMs_);
}


#endif

