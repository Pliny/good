#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include "utils.h"
#include "DeviceModel.h"
#include "SampleDataModel.h"

static unsigned int lastEvent = 0;
static unsigned int pulseCount = 0;

DeviceModel myself;
SampleDataModel sampleData(myself);

void setup()
{
  initHardware();
  connectWiFi();

  if(!myself.populate()) {

    if(!myself.createAndPopulate()) {
      Utils::ASSERT(2000); /* Fail */
    }
  }
  Utils::netLog("Updating every " + String(myself.getTimeInterval()) + " mS.");
}

void loop()
{
  uint32_t currTime = millis();

  if(currTime >= myself.getTimeInterval() + lastEvent) {
    Utils::netLog("[" + String(currTime) + "]: Updating Server");

    if(!sampleData.send(pulseCount)) {
      Utils::netLog("ERROR: Failed to post '" + String(pulseCount) + "'. continuing");
    }
    lastEvent = currTime;
  }
  Utils::busyWait(1000);
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

