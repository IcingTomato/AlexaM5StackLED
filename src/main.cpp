#include <Arduino.h>
// #include <M5StickC.h>
#include <WiFi.h>
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char THING_ID[]           = ""; //Enter THING ID
const char DEVICE_LOGIN_NAME[]  = ""; //Enter DEVICE ID

const char SSID[]               = "";    //Enter WiFi SSID (name)
const char PASS[]               = "";    //Enter WiFi password
const char DEVICE_KEY[]         = "";    //Enter Secret device password (Secret Key)

#define LED_PIN 21 //Positive pin of LED in 3v3, negative pin of LED in 21

#define wifiLEDPin 2 //Red LED on when WiFi connected

void onSwitch1Change();

int toggleState_1 = 0;

CloudSwitch switch1;

void initProperties(){

  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.setThingId(THING_ID);
  ArduinoCloud.addProperty(switch1, READWRITE, ON_CHANGE, onSwitch1Change);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);

void relayOnOff(int relay) {

  switch (relay) {
    case 1:
      if (toggleState_1 == 0) {
        digitalWrite(LED_PIN, LOW); // turn on relay 1
        toggleState_1 = 1;
        Serial.println("Device1 ON");
      }
      else {
        digitalWrite(LED_PIN, HIGH); // turn off relay 1
        toggleState_1 = 0;
        Serial.println("Device1 OFF");
      }
      delay(100);
      break;
  }
}

void doThisOnConnect(){
  /* add your custom code here */
  Serial.println("Board successfully connected to Arduino IoT Cloud");
  digitalWrite(wifiLEDPin, HIGH); //Turn off WiFi LED
}
void doThisOnSync(){
  /* add your custom code here */
  Serial.println("Thing Properties synchronised");
}

void doThisOnDisconnect(){
  /* add your custom code here */
  Serial.println("Board disconnected from Arduino IoT Cloud");
  digitalWrite(wifiLEDPin, LOW); //Turn off WiFi LED
}

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(115200);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);

  // Defined in thingProperties.h
  initProperties();
  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::CONNECT, doThisOnConnect);
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::SYNC, doThisOnSync);
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::DISCONNECT, doThisOnDisconnect);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  pinMode(LED_PIN, OUTPUT);

  pinMode(wifiLEDPin, OUTPUT);

  //During Starting all Relays should TURN OFF
  digitalWrite(LED_PIN, HIGH);

}

void loop() {

  ArduinoCloud.update();
}

void onSwitch1Change() {
  //Control the device
  if (switch1 == 1)
  {
    digitalWrite(LED_PIN, LOW);
    Serial.println("Device1 ON");
    toggleState_1 = 1;
  }
  else
  {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Device1 OFF");
    toggleState_1 = 0;
  }
}