#ifdef ENABLE_RGB

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Adafruit_NeoPixel.h>

#include "rgb.h"
#include "config.h"
#include "utils.h"

#define RGB_KEY_0 "rgb_0"

static Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIN_RGB_TOTAL_LEDS, PIN_RGB, NEO_GRB + NEO_KHZ800);

static uint8_t cred = 0;
static uint8_t cgreen = 0;
static uint8_t cblue = 0;
static uint8_t red;
static uint8_t green;
static uint8_t blue;
static uint8_t fade;
static char rgb_sub_topic[128];
static char rgb_pub_topic[128];

Adafruit_MQTT_Subscribe *rgb_sub;

void set_led();

void rgb_init(Adafruit_MQTT_Client* mqtt, const char* serial, Adafruit_MQTT_Subscribe* sub) {

  sprintf(rgb_sub_topic, "%s/%s/%s", FEED_BASE, serial, RGB_KEY_0);
  sprintf(rgb_pub_topic, "%s/%s/%s", HOME_BASE, serial, RGB_KEY_0);
  rgb_sub = sub;

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

void rgb_process_cmds(Adafruit_MQTT_Client* mqtt, Adafruit_MQTT_Subscribe* subscription) {
  if ( subscription == rgb_sub ) {
    DEBUG_MSGLN("got rgb message");

    uint32_t data = atoi((char *)rgb_sub->lastread);
    fade = (data >> 24) & 0xFF;
    red = (data >> 16) & 0xFF;
    green = (data >> 8) & 0xFF;
    blue = (data) & 0xFF;

    if(fade > 100)
      fade = 100;

    set_led();
    rgb_pub_0(mqtt);
  }
}

void set_led() {
  bool continue_v = true;
  while(continue_v) {
    if((cred == red) && (cgreen == green) && (cblue == blue)){
      continue_v = false;
    }
    else {
      if(cred < red)
        cred++;
      else if (cred == red)
        cred = red;
      else
        cred--;

      if(cgreen < green)
        cgreen++;
      else if(cgreen == green)
        cgreen = green;
      else
        cgreen--;

      if(cblue < blue)
        cblue++;
      else if ( cblue == blue )
        cblue = blue;
      else
        cblue--;
    }

    for (int i = 0; i < PIN_RGB_TOTAL_LEDS; i++)
    {
      strip.setPixelColor(i, cred, cgreen, cblue);
    }

    delay(fade);
    strip.show();
  }
  fade = 20;
}
#endif
