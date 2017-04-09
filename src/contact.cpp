#ifdef ENABLE_CONTACT

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "contact.h"
#include "config.h"
#include "utils.h"

#define CONTACT_KEY_0 "contact_0"

static char contact_topic_0[128];

void contact_init(Adafruit_MQTT_Client* mqtt, const char* serial) {

  sprintf(contact_topic_0, "%s/%s/%s", HOME_BASE, serial, CONTACT_KEY_0);
  pinMode(PIN_CONTACT_0, INPUT);

}

void contact_exec(Adafruit_MQTT_Client* mqtt) {
  contact_pub_0(mqtt, 0);
}

void contact_refresh(Adafruit_MQTT_Client* mqtt) {
  contact_pub_0(mqtt, 1);
}

void contact_pub_0(Adafruit_MQTT_Client* mqtt, uint8_t force = 0) {
    static uint8_t first_read = 0;
    static uint8_t last_contact = 0;

    if(force == 1)
      first_read = 0;

    if(last_contact != digitalRead(PIN_CONTACT_0) || first_read == 0) {
      first_read = 1;
      last_contact = digitalRead(PIN_CONTACT_0);
      if(last_contact == 0) {
        DEBUG_MSG("publish contact 0: "); DEBUG_MSGLN("0");
        mqtt->publish(contact_topic_0, "0");
      }
      else {
        DEBUG_MSG("publish contact 0: "); DEBUG_MSGLN("1");
        mqtt->publish(contact_topic_0, "1");
      }
    }
}

#endif
