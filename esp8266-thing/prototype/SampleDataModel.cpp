#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "utils.h"
#include "DeviceModel.h"
#include "SampleDataModel.h"

SampleDataModel::SampleDataModel(DeviceModel &dev): device(dev), first_post(true)
{
}

int SampleDataModel::send(uint32_t count)
{
  String result = "";
  int httpStatusCode;

  String params = "{\"value\": "        + String(count)  + "," +
                   "\"device_id\":"     + device.getId() + "," +
                   "\"session_start\":" + first_post     + "}";

  CREATE(result, "api/v1/sampledata", params.c_str());

  httpStatusCode = getStatusCode(result);

  if(httpStatusCode == 201) {
    processHttpResult(result);
    first_post = false;
  }

  return httpStatusCode == 201;
}

int SampleDataModel::processJson(String &json)
{
  StaticJsonBuffer<256> jsonBuffer;

  JsonObject& root = jsonBuffer.parseObject(json.c_str());

  if(!root.success()) {
    Utils::netLog(String(__FUNCTION__) + String(": Bad json."));
    return 0;
  }

  if(root["device_id"] != device.getId()) {
    Utils::netLog("ERROR: Returned device id doesn't match my id... continuing.");
    return 0;
  }

  return 1;
}

