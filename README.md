# mqtt-st-esp8266
A simple esp8266 firmware to publish data via MQTT.  At the moment, this supports DHT, MCP9808, CONTACT(digitalRead) and RELAY(digitalWrite).

Set your platform.ini accordingly.

Set your config.h accordingly.

### Libraries
- Modified Adafruit MQTT Library (https://github.com/react9/Adafruit_MQTT_Library) (install via cmdline: pio lib install https://github.com/react9/Adafruit_MQTT_Library.git **not sure if this works**)
- Adafruit MCP9808 Library (https://github.com/adafruit/Adafruit_MCP9808_Library) (install via cmdline: pio lib install 820)


