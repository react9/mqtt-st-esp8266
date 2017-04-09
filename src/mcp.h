#ifdef ENABLE_MCP9808
#include <Wire.h>
void mcp_init(Adafruit_MQTT_Client* mqtt, const char* serial, const uint32_t p);
void mcp_exec(Adafruit_MQTT_Client* mqtt);
void mcp_refresh(Adafruit_MQTT_Client* mqtt);
void mcp_pub_temperature(Adafruit_MQTT_Client* mqtt);
#endif
