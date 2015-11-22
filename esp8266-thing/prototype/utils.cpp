#include "utils.h"

namespace Utils
{
  WiFiClient logClient;

  const uint8_t LED_PIN     =  0;
  const uint8_t DIGITAL_PIN = 12;
  const uint8_t TEST_PIN = 13;

  const char* WiFiSSID = "Hi_There";
  const char* WiFiPSK  = "Let me in jambony1";

  const uint16_t API_PORT = 3000;
  const char* SERVER_DOMAIN = "192.168.1.14";
  const char* API_SERVER = "192.168.1.14" ":3000";

  const char* LOGGING_DOMAIN  = "192.168.1.14";
  const uint16_t LOGGING_PORT =   9393;

  void busyWait(unsigned int timeMs)
  {
#ifdef DEBUG
    static uint8_t ledStatus = LOW;
    digitalWrite(Utils::LED_PIN, ledStatus);
    ledStatus = (ledStatus == HIGH) ? LOW : HIGH;
#endif
    delay(timeMs);
  }

  void netLog(String str)
  {
#ifdef DEBUG
    if(logClient.connect(Utils::LOGGING_DOMAIN, Utils::LOGGING_PORT)) {
      logClient.println(str);
      logClient.stop();
    }
#endif
  }

  void ASSERT(uint16_t _timeMs_)
  {
    while(true) busyWait(_timeMs_);
  }
}


