#ifdef ENABLE_ANALOG

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "an.h"
#include "config.h"
#include "utils.h"

#define ANALOG_KEY_0 "analog_0"

static uint32_t poll;
static uint32_t rolltime;
static uint32_t last_publish_time;
static char an_topic_0[128];

void an_init(Adafruit_MQTT_Client* mqtt, const char* serial, const uint32_t p) {

  poll = p;
  rolltime = 0;
  last_publish_time = 0;

  sprintf(an_topic_0, "%s/%s/%s", HOME_BASE, serial, ANALOG_KEY_0);
}

void an_exec(Adafruit_MQTT_Client* mqtt) {

  if ((int32_t)(millis() - rolltime) >= 0) {
    rolltime += poll;
    an_pub_0(mqtt);
  }
}

void an_refresh(Adafruit_MQTT_Client* mqtt) {
  an_pub_0(mqtt);
}

void an_pub_0(Adafruit_MQTT_Client* mqtt) {
  char fpayload[41];
  float f = -1;
  uint16_t total = 0;

  DEBUG_MSG("publish analog reading: "); DEBUG_MSG(an_topic_0); DEBUG_MSG(" ");

  for(int i = 0; i < 10; i++) {
    total += analogRead(A0);
  }
  f = (total / 10.0);

  if(isnan(f)) {
    DEBUG_MSGLN("(failed to read analog reading.");
  }
  else {
    dtostrf(f, 0, 2, fpayload);
    DEBUG_MSGLN(fpayload);
    mqtt->publish(an_topic_0, fpayload);
  }

}
#endif
