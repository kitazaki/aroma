#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <Milkcocoa.h>

#define WLAN_SSID       ""  // Wi-Fi SSID
#define WLAN_PASS       ""  // Wi-Fi Password

#define PIN 5  // ESP-WROOM-02 Dev Board IO5 => GPIO5
#define DEVICEID 1  // if selector is DEVICEID, PIN is switched on during timer. if NOT, MOTOR doesn't work.
#define LEDPIN 4  // ESP-WROOM-02 Dev Board IO4 => GPIO4
#define NUMPIXELS 16   // Number of LEDs
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

WiFiClient client;

#define MILKCOCOA_APP_ID      ""  // Milkcocoa App ID
#define MILKCOCOA_DATASTORE   ""  // Milkcocoa DataStore
#define MILKCOCOA_SERVERPORT  1883

const char MQTT_SERVER[] PROGMEM    = MILKCOCOA_APP_ID ".mlkcca.com";
const char MQTT_CLIENTID[] PROGMEM  = __TIME__ MILKCOCOA_APP_ID;

Milkcocoa milkcocoa = Milkcocoa(&client, MQTT_SERVER, MILKCOCOA_SERVERPORT, MILKCOCOA_APP_ID, MQTT_CLIENTID);

int red, green, blue, strength, timer;

void onpush(DataElement *elem) {
  Serial.println("onpush");
  Serial.print("r:");
  Serial.print(elem->getInt("r"));
  Serial.print(",g:");
  Serial.print(elem->getInt("g"));
  Serial.print(",b:");
  Serial.print(elem->getInt("b"));
  Serial.print(",s:");
  Serial.print(elem->getInt("s"));
  Serial.print(",t:");
  Serial.print(elem->getInt("t"));
  Serial.println();

  red = elem->getInt("r");
  green = elem->getInt("g");
  blue = elem->getInt("b");
  strength = elem->getInt("s");
  timer = elem->getInt("t");

  if (selector) {
    if (selector == DEVICEID) {
      digitalWrite(PIN, HIGH); 
      delay(timer * 100);
      digitalWrite(PIN, LOW); 
      led(red,green,blue);
    }
  }
}

void led(int v_r, int v_g, int v_b) {
  strip.setBrightness(50);
  for(int i=0;i<NUMPIXELS;i++){
    strip.setPixelColor(i, strip.Color(v_r, v_g, v_b));
  }
  strip.show();
}

void setupWiFi() {
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  setupWiFi();
  Serial.println( milkcocoa.on(MILKCOCOA_DATASTORE, "push", onpush) );
  pinMode(PIN, OUTPUT); 
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  milkcocoa.loop();
}

