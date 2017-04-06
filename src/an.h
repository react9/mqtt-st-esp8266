#ifdef ENABLE_ANALOG
void an_init();
void pub_analog_reading(Adafruit_MQTT_Client* mqtt, char* feed_key);
#endif
