#ifdef ENABLE_ANALOG

#include "Adafruit_MCP9808.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "config.h"
#include "utils.h"


void an_init() {
  /* nothing */
}

void pub_analog_reading(Adafruit_MQTT_Client* mqtt, char* feed_key) {
  char fpayload[41];
  float f = -1;
  uint16_t total = 0;

  DEBUG_MSG("publish analog reading: "); DEBUG_MSG(feed_key); DEBUG_MSG(" ");

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
    mqtt->publish(feed_key, fpayload);
  }

}
#endif
