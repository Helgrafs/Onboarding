 #include <SPI.h>
 #include <Arduino.h>
 #include <Lora.h>
 
//LoRa definitions, shamelessly stolen from VIP code, after cross referencing to datasheet, I get it now.
#define SS 18 
#define RST 14
#define DI0 26
#define SCK 5
#define MISO 19
#define MOSI 27
Adafruit_BMP280 bmp;   

String outgoing = "string";
int syncWord = 0xF7;
byte _localAddress = 0xFF;
byte _destination = 0xFF;
boolean _canPrintHeaderMessage = false;
void setup() {
  // VERY IMPORTANT!!!! WHEN CONNECTING BMP280, SCL goes to pin 11 And SDA goes to pin 12!!!!!!!!!!!
  Serial.begin(115200);
  while (!Serial);
  unsigned status;
  status = bmp.begin();
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI0);
    if (!status) {
      Serial.println(F("BMP Initialisation failed"));
          while (1) delay(10);
    } else {
      Serial.println("BMP initialisation successful");
    }
  Serial.println("LoRa Transmitter");
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
   }else {
    Serial.println("LoRa initialisation successful");
 }
  LoRa.setSyncWord(0xF7);
  LoRa.setTxPower(20);
  LoRa.setSpreadingFactor(7);
  LoRa.setCodingRate4(8);
  LoRa.setSignalBandwidth(25E4);
}
void transmitter(String outgoing, int messageId) {
  LoRa.beginPacket();
  LoRa.write(_destination);
  LoRa.write(_localAddress);
  LoRa.write(messageId);
  LoRa.write(outgoing.length());
  LoRa.print(outgoing);
  LoRa.endPacket();
  Serial.println("Package sent");
}
void loop(){
    Serial.println("Sending barometer data");
    float Temperature = bmp.readTemperature();
    float pressure = bmp.readPressure();
    float altitude = bmp.readAltitude(1017);
    String Message = "Temperature: " + String(Temperature) + "*C; Pressure: " + String(pressure) + "Pa; Altitude: " + String(altitude); 
   transmitter(Message, 0);
   delay(3000);
}