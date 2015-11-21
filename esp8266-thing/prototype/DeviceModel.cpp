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

  String path = "api/v1/devices";
  String params = "{ \"device_id\" : \"" + getMacAddr() + "\" }";

  CREATE(result, path.c_str(), params.c_str());

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

  JsonObject& root = jsonBuffer.parseObject(json.c_str());

  if(!root.success()) {
    Utils::netLog(String(__FUNCTION__) + String(": Bad json."));
    return 0;
  }

  /* Server has the time units in seconds, we have it in milli-seconds */
  timeInterval = root["sample_interval"].as<int>() * 1000;
  id = root["id"];

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

int DeviceModel::getId(void)
{
  return id;
}

