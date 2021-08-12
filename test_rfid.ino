#include <SPI.h>
#include <MFRC522.h>

String read_rfid;

void setup() {
    SPI.begin();                
    mfrc522.PCD_Init();

}

void loop() {
    if ( ! mfrc522.PICC_IsNewCardPresent())
    return;
    if ( ! mfrc522.PICC_ReadCardSerial())
    return;
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);

    Serial.println (read_rfid);

}

void dump_byte_array(byte *buffer, byte bufferSize) {
    read_rfid="";
    for (byte i = 0; i < bufferSize; i++) {
        read_rfid=read_rfid + String(buffer[i], HEX);
    }
}
