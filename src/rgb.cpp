#ifdef ENABLE_RGB

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Adafruit_NeoPixel.h>

#include "rgb.h"
#include "config.h"
#include "utils.h"

#define RGB_KEY_0 "rgb_0"

static Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIN_RGB_TOTAL_LEDS, PIN_RGB, NEO_GRB + NEO_KHZ800);
static uint8_t red;
static uint8_t green;
static uint8_t blue;
static uint8_t fade;
static char rgb_sub_topic[128];
static char rgb_pub_topic[128];

Adafruit_MQTT_Subscribe rgb_sub = Adafruit_MQTT_Subscribe(NULL, "");

void rgb_init(Adafruit_MQTT_Client* mqtt, const char* serial) {

  sprintf(rgb_sub_topic, "%s/%s/%s", FEED_BASE, serial, RGB_KEY_0);
  sprintf(rgb_pub_topic, "%s/%s/%s", HOME_BASE, serial, RGB_KEY_0);
  rgb_sub.setMqtt(mqtt);
  rgb_sub.setFeed(rgb_sub_topic);
  mqtt->subscribe(&rgb_sub);

  red = 0;
  green = 0;
  blue = 0;
  fade = 20;

  strip.begin();
  strip.show();
}

void rgb_exec(Adafruit_MQTT_Client* mqtt) {
  /* nothing */
}

void rgb_refresh(Adafruit_MQTT_Client* mqtt) {
  rgb_pub_0(mqtt);
}

void rgb_pub_0(Adafruit_MQTT_Client* mqtt) {
  char fpayload[41];
  uint32_t v = 0;
  v = (fade << 24) | (red << 16) | (green << 8) | blue;

  DEBUG_MSG("publish rgb light: "); DEBUG_MSG(rgb_pub_topic);  DEBUG_MSG(" ");
  itoa(v, fpayload, 10);
  DEBUG_MSGLN(fpayload);
  mqtt->publish(rgb_pub_topic, fpayload);
}

void rgb_process_cnds(Adafruit_MQTT_Client* mqtt, Adafruit_MQTT_Subscribe* subscription) {
  if ( subscription == &rgb_sub ) {
    DEBUG_MSGLN("got rgb message");
    rgb_pub_0(mqtt);
  }
}
#endif
