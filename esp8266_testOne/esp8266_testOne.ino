#include <ESP8266WiFi.h>

void setup() {
  // put your setup code here, to run once:
  WiFi.mode(WIFI_AP);
  WiFi.softAP("DIMA_SSID", "PASSWORD");
}

void loop() {
  // put your main code here, to run repeatedly:

}
