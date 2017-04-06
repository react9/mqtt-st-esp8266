#ifdef ENABLE_MCP9808
#include <Wire.h>
void mcp_init();
void pub_mcp_temperature(Adafruit_MQTT_Client* mqtt, char* feed_key);
#endif
