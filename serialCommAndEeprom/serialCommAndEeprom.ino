#include <SoftwareSerial.h>
#include <EEPROM.h>

#define relayOutput 11

SoftwareSerial serial1(12, 13); // RX, TX

void setup() {

  pinMode(relayOutput, OUTPUT);
  digitalWrite(relayOutput, LOW);
  serial1.begin(9600);
  Serial.begin(9600);

}

void loop() {

  if (serial1.available())
    mainTask();
}

void mainTask() {

  String option = serial1.readString();

  if (option.indexOf("0x01") >= 0) {
    serial1.println("Ligando...");
    writeByte(0, 1);
    digitalWrite(relayOutput, HIGH);
  } else if (option.indexOf("0x02") >= 0) {
    serial1.println("Desligando...");
    writeByte(0, 0);
    digitalWrite(relayOutput, LOW);
  } else if (option.indexOf("0x03") >= 0) {
    if (readByte(0) == 1)
        serial1.println("Ligado!");
    else
        serial1.println("Desligado!");
  } else {
    serial1.println("unknown command");
    }
  option = "";
}

void writeByte (int address, byte value){
  byte currentValue = readByte(address);
  if (currentValue == value){
    return;
  }
  else {
    EEPROM.write(address, value);
  }
}

byte readByte (int address){
  return EEPROM.read(address);
}

void writeInt(int address1, int address2, int value){
  int currentValue = readInt(address1, address2);
  if (currentValue == value){
    return;
  } else{
      byte firstByte = value&0xff;
      byte secondByte = (value >> 8) &0xff;
      EEPROM.write(address1, firstByte);
      EEPROM.write(address2, secondByte);
  }
}


int readInt(int address1, int address2){
  int value = 0;
  byte fistByte = EEPROM.read(address1);
  byte secondByte = EEPROM.read(address2);
  value = (secondByte << 8) + fistByte;
  return value;
}

void writeString(int address, String nessage){
  if (nessage.length()>EEPROM.length() || (address+nessage.length()) >EEPROM.length() ){
    Serial.println ("A sua String não cabe na EEPROM");
  } else{
    for (int i = 0; i<nessage.length(); i++){
       EEPROM.write(address,nessage[i]);
       address++;
    }
    EEPROM.write(address,'\0');
  }
}

String readString(int address){
  String message="";
  if (address>EEPROM.length()){
    return message;
  } else {
    char pos;
    do{
      pos = EEPROM.read(address);
      address++;
      message = message + pos;
    }
    while (pos != '\0');
  }
  return message;
}
