#ifdef ENABLE_MCP9808

#include "Adafruit_MCP9808.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "mcp.h"
#include "config.h"
#include "utils.h"

static Adafruit_MCP9808 sensor = Adafruit_MCP9808();

void mcp_init() {
  if (!sensor.begin()) {
    DEBUG_MSGLN("could not find mcp9808 sensor! halt.");
    while (1);
  }
}

void pub_mcp_temperature(Adafruit_MQTT_Client* mqtt, char* feed_key) {
  char fpayload[41];

  DEBUG_MSG("publish mcp temperature: "); DEBUG_MSG(feed_key); DEBUG_MSG(" ");
  sensor.shutdown_wake(0);
  float f = sensor.readTempC();
  if(isnan(f)) {
    DEBUG_MSGLN("(failed to read MCP temperature)");
  }
  else {
    dtostrf(f, 0, 2, fpayload);
    DEBUG_MSGLN(fpayload);
    mqtt->publish(feed_key, fpayload);
  }

  sensor.shutdown_wake(1);
}
#endif
