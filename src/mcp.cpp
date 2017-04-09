#ifdef ENABLE_MCP9808

#include "Adafruit_MCP9808.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "mcp.h"
#include "config.h"
#include "utils.h"

#define MCP_KEY_TEMPERATURE "temperature_mcp"

static Adafruit_MCP9808 sensor = Adafruit_MCP9808();

static uint8_t cont;
static uint32_t poll;
static uint32_t rolltime;
static uint32_t last_publish_time;
static char mcp_topic_temperature[128];

void mcp_init(Adafruit_MQTT_Client* mqtt, const char* serial, const uint32_t p) {

  poll = p;
  rolltime = 0;
  last_publish_time = 0;
  cont = 1;

  sprintf(mcp_topic_temperature, "%s/%s/%s", HOME_BASE, serial, MCP_KEY_TEMPERATURE);

  if (!sensor.begin()) {
    DEBUG_MSGLN("could not find mcp9808 sensor! halt.");
    cont = 0;
  }
}

void mcp_exec(Adafruit_MQTT_Client* mqtt) {

  if ((int32_t)(millis() - rolltime) >= 0) {
    rolltime += poll;
    mcp_pub_temperature(mqtt);
  }
}

void mcp_refresh(Adafruit_MQTT_Client* mqtt) {
  mcp_pub_temperature(mqtt);
}

void mcp_pub_temperature(Adafruit_MQTT_Client* mqtt) {

  if(!cont) {
    return;
  }

  char fpayload[41];

  DEBUG_MSG("publish mcp temperature: "); DEBUG_MSG(mcp_topic_temperature); DEBUG_MSG(" ");
  sensor.shutdown_wake(0);
  float f = sensor.readTempC();
  if(isnan(f)) {
    DEBUG_MSGLN("(failed to read MCP temperature)");
  }
  else {
    dtostrf(f, 0, 2, fpayload);
    DEBUG_MSGLN(fpayload);
    mqtt->publish(mcp_topic_temperature, fpayload);
  }

  sensor.shutdown_wake(1);
}
#endif
