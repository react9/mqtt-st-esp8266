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

Adafruit_MQTT_Subscribe* relay_sub;

void relay_init(Adafruit_MQTT_Client* mqtt, const char* serial, Adafruit_MQTT_Subscribe* sub) {

  sprintf(relay_result, "init");
  sprintf(relay_sub_topic, "%s/%s/%s", FEED_BASE, serial, RELAY_KEY_0);
  sprintf(relay_pub_topic, "%s/%s/%s", HOME_BASE, serial, RELAY_KEY_0);
  relay_sub = sub;

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

void relay_process_cmds(Adafruit_MQTT_Client* mqtt, Adafruit_MQTT_Subscribe* subscription) {
  if ( subscription == relay_sub ) {
    DEBUG_MSG("received relay sub ");
    uint8_t handled = 0;

    if (strcmp((char *)relay_sub->lastread, "close") == 0) {
      DEBUG_MSGLN("close");
      digitalWrite(PIN_RELAY_0, HIGH);
      sprintf(relay_result, "close");
      handled = 1;
    }
    else if (strcmp((char *)relay_sub->lastread, "open") == 0) {
      DEBUG_MSGLN("open");
      digitalWrite(PIN_RELAY_0, LOW);
      sprintf(relay_result, "open");
      handled = 1;
    }
    else if (strcmp((char *)relay_sub->lastread, "toggle") == 0) {
      DEBUG_MSGLN("toggle");
      digitalWrite(PIN_RELAY_0, HIGH);
      delay(250);
      digitalWrite(PIN_RELAY_0, LOW);
      sprintf(relay_result, "toggle");
      handled = 1;
    }
    else {
      DEBUG_MSGLN("unknown message received.");
      /* do nothing? */
    }

    if(handled) {
      relay_pub_0(mqtt);
    }
  }
}
#endif
