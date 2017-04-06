#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "config.h"
#include "main.h"
#include "utils.h"

char pub_feeds[8][256];
char sub_feeds[8][256];

uint8_t pub_feed_ix = 0;
uint8_t sub_feed_ix = 0;

void add_pub_feed_key(const char* base, const char* name, uint8_t ix) {
  char serial[16];

  String(ESP.getChipId(), HEX).toCharArray(serial, 16);
  sprintf(pub_feeds[ix], "%s/%s/%s", base, serial, name);
  Serial.println(pub_feeds[ix]);
}

char* get_pub_feed_key(uint8_t ix) {
  return pub_feeds[ix];
}

void set_pub_feed_key(Adafruit_MQTT_Publish* pub, const char* base, const char* name) {
  char serial[16];

  String(ESP.getChipId(), HEX).toCharArray(serial, 16);
  sprintf(pub_feeds[pub_feed_ix], "%s/%s/%s", base, serial, name);
  Serial.println(pub_feeds[pub_feed_ix]);
  pub->setFeed(pub_feeds[pub_feed_ix]);
  pub_feed_ix++;

}

void set_sub_feed_key(Adafruit_MQTT_Subscribe* sub, const char* base, const char* name) {
  char serial[16];

  String(ESP.getChipId(), HEX).toCharArray(serial, 16);
  sprintf(sub_feeds[sub_feed_ix], "%s/%s/%s", base, serial, name);
  Serial.println(sub_feeds[sub_feed_ix]);
  sub->setFeed(sub_feeds[sub_feed_ix]);
  sub_feed_ix++;

}
