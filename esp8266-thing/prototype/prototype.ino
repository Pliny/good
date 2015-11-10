#include <ESP8266WiFi.h>

#define LED_PIN      5
#define DIGITAL_PIN 12

#define WiFiSSID  "Hi_There"
#define WiFiPSK   "Let me in jambony1"

#define API_PORT 3000
#define SERVER_DOMAIN "192.168.1.14"
#define API_SERVER SERVER_DOMAIN ":3000"

#define LOGGING_DOMAIN "192.168.1.14"
#define LOGGING_PORT   9393

#define ASSERT(_timeMs_) while(true) busyWait(_timeMs_);

static unsigned int timeIntervalInSec = 2;
static unsigned int totalSec = 0;

WiFiClient client;
WiFiClient logClient;

void setup()
{
  initHardware();
  connectWiFi();
  timeIntervalInSec = getTimeInterval();
}

void loop()
{
  if(totalSec % timeIntervalInSec == 0) {
    /* Post the number of pulses (proportional to flow rate) */
  }
  busyWait(1000);
  ++totalSec;
}

void netLog(String str)
{
  if(logClient.connect(LOGGING_DOMAIN, LOGGING_PORT)) {
    logClient.println(str);
    logClient.stop();
  }
}

void initHardware(void)
{
  /* Serial.begin(9600); */
  pinMode(DIGITAL_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void connectWiFi(void)
{
  uint8_t ledStatus = LOW;

  WiFi.mode(WIFI_STA);

  WiFi.begin(WiFiSSID, WiFiPSK);

  while (WiFi.status() != WL_CONNECTED) {
    busyWait(100);
  }
  digitalWrite(LED_PIN, LOW);
  netLog("------ RESET ------");
}

unsigned int getTimeInterval(void)
{
  uint8_t ledStatus = HIGH;
  String macID = getMacAddr();

  String result = "π";

  fetchObject(&result, "api/v1/devices/" + macID, "");

  netLog(result);
  /* Parse JSON here */

  return result.toInt();
}

void fetchObject(String *line, String path, String params)
{
  if (!client.connect(SERVER_DOMAIN, API_PORT)) {
    ASSERT(1000);
  }

  String request = "GET /" + path;
  if(params.length() != 0) {
    request += "/?" + params;
  }
  request += " HTTP/1.1";

  client.println(request);
  client.println("Host: " API_SERVER);
  client.println("User-Agent: good/0.0");
  client.println("Accept: */*");
  client.println();

  /* client.setTimeout(10000); */
  do {
    (*line) += client.readString();
    /* client.setTimeout(10); */
    busyWait(10);
  } while(client.available());

  digitalWrite(LED_PIN, LOW);
}

void busyWait(unsigned int timeMs)
{
  static uint8_t ledStatus = LOW;
  digitalWrite(LED_PIN, ledStatus);
  ledStatus = (ledStatus == HIGH) ? LOW : HIGH;
  delay(timeMs);
}

String getMacAddr(void)
{
  int i;
  uint8_t buf[WL_MAC_ADDR_LENGTH];

  WiFi.macAddress(buf);

  String macID = "";
  for(i = 0; i != WL_MAC_ADDR_LENGTH; ++i) {
    macID += String(buf[i], HEX);
  }
  macID.toUpperCase();

  return macID;
}

