#include <ESP8266WiFi.h>
#include <Milkcocoa.h>

#define WLAN_SSID       ""  // Wi-Fi SSID
#define WLAN_PASS       ""  // Wi-Fi Password

#define PIN 4  // ESP-WROOM-02 Dev Board IO4 => GPIO4
#define DEVICEID 1  // if selector is DEVICEID, PIN is switched on during timer. if NOT, MOTOR doesn't work.

WiFiClient client;

#define MILKCOCOA_APP_ID      ""  // Milkcocoa App ID
#define MILKCOCOA_DATASTORE   ""  // Milkcocoa DataStore
#define MILKCOCOA_SERVERPORT  1883

const char MQTT_SERVER[] PROGMEM    = MILKCOCOA_APP_ID ".mlkcca.com";
const char MQTT_CLIENTID[] PROGMEM  = __TIME__ MILKCOCOA_APP_ID;

Milkcocoa milkcocoa = Milkcocoa(&client, MQTT_SERVER, MILKCOCOA_SERVERPORT, MILKCOCOA_APP_ID, MQTT_CLIENTID);

int red, green, blue, selector, timer;

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
  selector = elem->getInt("s");
  timer = elem->getInt("t");

  if (selector) {
    if (selector == DEVICEID) {
      digitalWrite(PIN, HIGH); 
      delay(timer * 100);
      digitalWrite(PIN, LOW); 
    }
  }
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
}

void loop() {
  milkcocoa.loop();
}

