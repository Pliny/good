#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "utils.h"
#include "DeviceModel.h"

DeviceModel::DeviceModel(void)
{
	timeInterval = -1;
}

int DeviceModel::getTimeInterval(void)
{
  return timeInterval;
}

int DeviceModel::populate(void)
{
  String result = "";
  int httpStatusCode;

  String line = "api/v1/devices/" + getMacAddr();
  GET(result, line.c_str(), "");

  httpStatusCode = getStatusCode(result);
  
  if(httpStatusCode == 200) {
    processHttpResult(result);
  }

  return httpStatusCode == 200;
}

int DeviceModel::createAndPopulate(void)
{
  String result = "";
  int httpStatusCode;

  String params = "api/v1/devices";
  String macId  = getMacAddr();

  CREATE(result, params.c_str(), macId.c_str());

  httpStatusCode = getStatusCode(result);

  if(httpStatusCode == 201) {
    processHttpResult(result);
  }

  return httpStatusCode == 201;
}

/*
 *
 * {
 *   device_id: "123456789",
 *   name: "blabla",
 *   sample_interval: 2
 * }
 *
 */
int DeviceModel::processJson(String &json)
{
  StaticJsonBuffer<256> jsonBuffer;

  JsonObject& root = jsonBuffer.parseObject(json);

  if(!root.success()) {
    Utils::netLog(String("processJson") + String(": Bad json."));
    return 0;
  }

  timeInterval = root["sample_interval"];

  Utils::netLog(String(timeInterval));

  return 1;
}

String DeviceModel::getMacAddr(void)
{
  String macId = (String)0;
  int i;
  uint8_t buf[WL_MAC_ADDR_LENGTH];

  WiFi.macAddress(buf);

  macId = "";
  for(i = 0; i != WL_MAC_ADDR_LENGTH; ++i) {
	  macId += String(buf[i], HEX);
  }
  macId.toUpperCase();

  return macId;
}


