#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include "utils.h"
#include "DeviceModel.h"

static unsigned int totalSec = 0;
static unsigned int pulseCount = 0;
  
DeviceModel myself;

void setup()
{
  initHardware();
  connectWiFi();

  if(!myself.populate()) {

    if(!myself.createAndPopulate()) {
      Utils::ASSERT(2000); /* Fail */
    }
  }
}

void loop()
{
  if(totalSec % myself.getTimeInterval() == 0) {
    /* Post the number of pulses (proportional to flow rate) */
    /* if(!sampleData->send(pulseCount)) { */
    /*   Utils::netLog("ERROR: Failed to post '" + pulseCount + "'. continuing"); */
    /* } */
  }
  Utils::busyWait(1000);
  ++totalSec;
}

void initHardware(void)
{
  pinMode(Utils::DIGITAL_PIN, INPUT_PULLUP);
  pinMode(Utils::LED_PIN, OUTPUT);
  digitalWrite(Utils::LED_PIN, LOW);
}

void connectWiFi(void)
{
  uint8_t ledStatus = LOW;

  WiFi.mode(WIFI_STA);

  WiFi.begin(Utils::WiFiSSID, Utils::WiFiPSK);

  while (WiFi.status() != WL_CONNECTED) {
    Utils::busyWait(100);
  }
  digitalWrite(Utils::LED_PIN, LOW);
  Utils::netLog("------ RESET ------");
}

