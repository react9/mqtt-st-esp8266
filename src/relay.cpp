#ifdef ENABLE_RELAY

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#include "relay.h"
#include "config.h"
#include "utils.h"

#define RELAY_KEY_0 "relay_0"

static char relay_result[32];
static char relay_sub_topic[128];
static char relay_pub_topic[128];

Adafruit_MQTT_Subscribe relay_sub = Adafruit_MQTT_Subscribe(NULL, "");

void relay_init(Adafruit_MQTT_Client* mqtt, const char* serial) {

  sprintf(relay_result, "init");
  sprintf(relay_sub_topic, "%s/%s/%s", FEED_BASE, serial, RELAY_KEY_0);
  sprintf(relay_pub_topic, "%s/%s/%s", HOME_BASE, serial, RELAY_KEY_0);
  relay_sub.setMqtt(mqtt);
  relay_sub.setFeed(relay_sub_topic);
  mqtt->subscribe(&relay_sub);

  pinMode(PIN_RELAY_0, OUTPUT);

}

void relay_exec(Adafruit_MQTT_Client* mqtt) {
  /* nothing */
}

void relay_refresh(Adafruit_MQTT_Client* mqtt) {
  relay_pub_0(mqtt);
}

void relay_pub_0(Adafruit_MQTT_Client* mqtt) {

  DEBUG_MSG("publish relay 0 "); DEBUG_MSG(relay_pub_topic);  DEBUG_MSG(" ");
  DEBUG_MSGLN(relay_result);
  mqtt->publish(relay_pub_topic, relay_result);
}

void relay_process_cnds(Adafruit_MQTT_Client* mqtt, Adafruit_MQTT_Subscribe* subscription) {
  if ( subscription == &relay_sub ) {
    DEBUG_MSG("received relay sub ");

    if (strcmp((char *)relay_sub.lastread, "close") == 0) {
      DEBUG_MSGLN("close");
      digitalWrite(PIN_RELAY_0, HIGH);
      sprintf(relay_result, "close");
    }
    else if (strcmp((char *)relay_sub.lastread, "open") == 0) {
      DEBUG_MSGLN("open");
      digitalWrite(PIN_RELAY_0, LOW);
      sprintf(relay_result, "open");
    }
    else if (strcmp((char *)relay_sub.lastread, "toggle") == 0) {
      DEBUG_MSGLN("toggle");
      digitalWrite(PIN_RELAY_0, HIGH);
      delay(250);
      digitalWrite(PIN_RELAY_0, LOW);
      sprintf(relay_result, "toggle");
    }
    else {
      DEBUG_MSGLN("unknown message received.");
      sprintf(relay_result, "unknown");
    }

    relay_pub_0(mqtt);
  }
}
#endif
