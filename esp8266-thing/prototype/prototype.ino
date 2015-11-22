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

#ifdef DEBUG
  digitalWrite(Utils::TEST_PIN, HIGH);
  Utils::busyWait(100);
  digitalWrite(Utils::TEST_PIN, LOW);
  Utils::busyWait(100);
#else
  Utils::busyWait(1000);
#endif
}

void initHardware(void)
{
  pinMode(Utils::DIGITAL_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(Utils::DIGITAL_PIN), updateRate, RISING);

#ifdef DEBUG
  pinMode(Utils::TEST_PIN, OUTPUT);
  digitalWrite(Utils::LED_PIN, LOW);

  pinMode(Utils::LED_PIN, OUTPUT);
  digitalWrite(Utils::LED_PIN, LOW);
#endif
}

void updateRate(void)
{
  pulseCount++;
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

