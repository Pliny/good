#include <ESP8266WiFi.h>

#define LED_PIN      5
#define DIGITAL_PIN 12

#define WiFiSSID  "Hi_There"
#define WiFiPSK   "Let me in jambony1"

#define HTTP_PORT 80
#define SERVER_DOMAIN "www.davesdesrochers.com"

#define LOGGING_DOMAIN "192.168.1.250"
#define LOGGING_PORT   9393

#define ASSERT(_timeMs_) while(true) busyWait(_timeMs_);

static unsigned int timeIntervalInSec = 2;
static unsigned int totalSec = 0;

void setup() {
  initHardware();
  connectWiFi();
  timeIntervalInSec = getTimeInterval();
}

void loop() {
  if(totalSec % timeIntervalInSec == 0) {
    /* Post the number of pulses (proportional to flow rate) */
  }
  busyWait(1000);
  netLog("Testing.");
  ++totalSec;
}

WiFiClient logClient;
void netLog(String str)
{
  if(!logClient.connected()) {
    logClient.connect(LOGGING_DOMAIN, LOGGING_PORT);
  }

  if(logClient.connected()) {
    logClient.println(str);
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
}

unsigned int getTimeInterval(void)
{
  uint8_t ledStatus = HIGH;
  String macID = getMacAddr();

  String result = fetchObject("", "?m=" + macID);

  /* Parse JSON here */

  return result.toInt();
}

String fetchObject(String path, String params)
{
  String line = "";
  WiFiClient client;

  if (!client.connect(SERVER_DOMAIN, HTTP_PORT)) {
    ASSERT(1000);
  }

  String request = "GET /" + path + "?" + params + " HTTP/1.1";
  client.println(request);
  client.println("User-Agent: good/0.0");
  client.println("Host: " SERVER_DOMAIN);
  client.println("Accept: */*");
  client.println();

  while(client.available()){
    line += client.readStringUntil('\r');
    busyWait(10);
    /* Serial.print(line); */
  }
  digitalWrite(LED_PIN, LOW);

  return line;
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

