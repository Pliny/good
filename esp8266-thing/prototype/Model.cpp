#include "utils.h"
#include "Model.h"

Model::Model(void)
{
}

void Model::GET(String &line, const char *path, const char *params)
{
  fetchObject("GET", line, path, params);
}

void Model::CREATE(String &line, const char *path, const char *params)
{
  fetchObject("POST", line, path, params);
}

void Model::fetchObject(const char *fetchType, String &line, const char *path, const char *params)
{

  String ftype = String(fetchType);

  if (!client.connect(Utils::SERVER_DOMAIN, Utils::API_PORT)) {
    Utils::ASSERT(1000);
  }

  String request = ftype + " /" + String(path);
  if(ftype == "GET" && String(params).length() != 0) {
    request += "/?" + String(params);
  }
  request += " HTTP/1.1";

  client.println(request);
  client.println(String("Host: ") + String(Utils::API_SERVER));
  client.println("User-Agent: good/0.0");
  client.println("Accept: */*");

  if(ftype == "POST" || ftype == "PUT") {
    client.println(String("Content-Length: ") + String(String(params).length()));
    client.println("Content-Type: application/json");
  }

  client.println();

  if(ftype == "POST" || ftype == "PUT") {
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
  int subBeg = retData.indexOf(' ', 1)+1;
  int subEnd = subBeg + 3;

  return retData.substring(subBeg, subEnd).toInt();
}

int Model::processHttpResult(String &result)
{
  while(!result.startsWith("\r\n")) {
    result.remove(0, result.indexOf('\n')+1);
  }
  result.remove(0, result.indexOf('\n')+1);

  return processJson(result);
}

