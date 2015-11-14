#include "utils.h"
#include "Model.h"

Model::Model(void)
{
}

void Model::GET(String &line, const char *path, const char *params)
{
  fetchObject("GET", line, path, params);
}

void Model::CREATE(String &line, const char *path, const char *macId)
{
  String params = "{ \"device_id\" : " + String(macId) + "}";
  fetchObject("POST", line, path, params.c_str());
}

void Model::fetchObject(const char *fetchType, String &line, const char *path, const char *params)
{
  if (!client.connect(Utils::SERVER_DOMAIN, Utils::API_PORT)) {
    Utils::ASSERT(1000);
  }

  String request = String(fetchType) + " /" + String(path);
  if(fetchType == "GET" && String(params).length() != 0) {
    request += "/?" + String(params);
  }
  request += " HTTP/1.1";

  client.println(request);
  client.println(String("Host: ") + String(Utils::API_SERVER));
  client.println("User-Agent: good/0.0");
  client.println("Accept: */*");
  client.println();

  if(fetchType == "POST" || fetchType == "PUT") {
    client.println(params);
  }

  do {
    line += client.readString();
    Utils::busyWait(10);
  } while(client.available());

  digitalWrite(Utils::LED_PIN, LOW);
}

int Model::getStatusCode(String &retData)
{
  return retData.substring(retData.indexOf(' ', 1)+1, 3).toInt();
}

int Model::processHttpResult(String &result)
{
  while(!result.startsWith("\r\n")) {
    result.remove(0, (result[result.indexOf('\n')]));
  }
  return processJson(result);
}

