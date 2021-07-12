/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/evanh/OneDrive/Desktop/CTD/InternetOfThings/Lab16/src/Lab16.ino"
void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);
void printToDisplay(String output);
void setup();
void loop();
#line 1 "c:/Users/evanh/OneDrive/Desktop/CTD/InternetOfThings/Lab16/src/Lab16.ino"
SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(MANUAL);

#include "oled-wing-adafruit.h"

const size_t UART_TX_BUF_SIZE = 20;

bool firstTime = true;

OledWingAdafruit display;

const BleUuid serviceUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid rxUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid txUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");

BleCharacteristic txCharacteristic("tx", BleCharacteristicProperty::NOTIFY, txUuid, serviceUuid);
BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::WRITE_WO_RSP, rxUuid, serviceUuid, onDataReceived, NULL);

LEDStatus statusY(RGB_COLOR_YELLOW, LED_PATTERN_SOLID, LED_SPEED_NORMAL, LED_PRIORITY_IMPORTANT);
LEDStatus statusB(RGB_COLOR_BLUE, LED_PATTERN_SOLID, LED_SPEED_NORMAL, LED_PRIORITY_IMPORTANT);

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) {
  if (data[0] == (0x30)){
    printToDisplay("0 inputted");
    BLE.disconnect();
  }
  else if (data[0] == (0x31)){
    printToDisplay("1 inputted");
  }
}

void printToDisplay(String output)
{
  //reset display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  //output to display
  display.println(output);
  display.display();
}
// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.

  pinMode(D7, OUTPUT);

  display.setup();
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.display();

  BLE.on();

  BLE.addCharacteristic(txCharacteristic);
  BLE.addCharacteristic(rxCharacteristic);
  
  BleAdvertisingData data;
  data.appendServiceUUID(serviceUuid);
  BLE.advertise(&data);
  Serial.begin();
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
  display.loop();

  if (BLE.connected()) {
    statusB.setActive(true);
    digitalWrite(D7, HIGH);

    uint8_t txBuf[UART_TX_BUF_SIZE];
    String message = "Status: Connected\n";
    message.toCharArray((char *)txBuf, message.length() + 1);
    txCharacteristic.setValue(txBuf, message.length() + 1); 
    delay(5000);
  }

  else{
    statusY.setActive(true);
    if (firstTime == false){
      firstTime = true;
    }
  }
}
