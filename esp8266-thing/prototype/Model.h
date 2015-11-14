#ifndef _MODEL_H
#define _MODEL_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

class Model {

public:
  Model(void);

  virtual int populate(void) = 0;

  virtual int createAndPopulate(void) = 0;

protected:
  WiFiClient client;

  void GET(String &line, const char *path, const char *params);

  void CREATE(String &line, const char *path, const char *macId);

  virtual int processJson(String &json) = 0;

  int getStatusCode(String &retData);

  int processHttpResult(String &result);

private:
  void fetchObject(const char *fetchType, String &line, const char *path, const char *params);

};

#endif


