#define ESP8266_LED 5

void setup() {
  /* Configure debug */
  pinMode(ESP8266_LED, OUTPUT);

  /* Enable client mode */

  /* connect to router */
}

void loop() {
  /* Get requested time interval 'x' from server */

  /* Every 'x' seconds, post the number of pulses */
  /* Eventually, it should either be sending the flow rate, or telling the server who it is */
  digitalWrite(ESP8266_LED, HIGH);
  delay(150);
  digitalWrite(ESP8266_LED, LOW);
  delay(150);
}
