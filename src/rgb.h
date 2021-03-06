#ifdef ENABLE_RGB
#include <Adafruit_NeoPixel.h>
void rgb_init(Adafruit_MQTT_Client* mqtt, const char* serial, Adafruit_MQTT_Subscribe* sub);
void rgb_exec(Adafruit_MQTT_Client* mqtt);
void rgb_refresh(Adafruit_MQTT_Client* mqtt);
void rgb_pub_0(Adafruit_MQTT_Client* mqtt);
void rgb_process_cmds(Adafruit_MQTT_Client* mqtt, Adafruit_MQTT_Subscribe* subscription);
#endif
