#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include <Adafruit_MLX90614.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
 
#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

String read_rfid;

int var = 0;

int state = 0;
int state2 = 0;

// pin driver motor
int in1 = 2;
int in2 = 3;

// ir sensor for stop motor
int irsensor = 4;
int irsensorvalue = 0;

// ultrasonic sensor for detact human
#define echoPin 2 
#define trigPin 3

long duration; 
int distance;

float data[5];
float dataTemp;

int changedata = 0;

String nama1 = "ABU";
String nama2 = "ALI";
 
void setup() 
{
  lcd.begin();
  lcd.backlight();
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  mlx.begin();
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(irsensor, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.println("CLEARDATA");
  Serial.println("LABEL,Timer,Name,Temperature");

}
void loop() 
{
  if (state2 == 0){
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("WELCOME");
    lcd.setCursor(0, 1);
    lcd.print("SCAN CARD....");
    state2 = 1;
    }
  

  switch (var) {
  case 0:
    if ( ! mfrc522.PICC_IsNewCardPresent())
    return;
    if ( ! mfrc522.PICC_ReadCardSerial())
    return;
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);

  if (read_rfid == "3aede6be")
  {
    
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("CARD ACCESS...");
    changedata = 1;
    var = 1;
  }

  if (read_rfid == "9c613731")
  {
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("CARD ACCESS...");
    changedata = 2;
    var = 1;
  }
 
 
    break;
  case 1:
    
    // running motor
    if (state == 0){
       digitalWrite(in1, HIGH);
       digitalWrite(in2, LOW);
       lcd.clear();
       lcd.setCursor(3, 0);
       lcd.print("TEMPERATURE");
       lcd.setCursor(0, 1);
       lcd.print("= WAIT..");
       state = 1;
      }
  
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;

    if (distance <= 10){
      var = 2;
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      }
    break;
  case 2:
     digitalWrite(trigPin, LOW);
     delayMicroseconds(2);
     digitalWrite(trigPin, HIGH);
     delayMicroseconds(10);
     digitalWrite(trigPin, LOW);
     duration = pulseIn(echoPin, HIGH);
     distance = duration * 0.034 / 2;

     if ( distance <= 10){
      for(int i = 0; i < 5; i++){
        data[i] = mlx.readObjectTempC();
        }

      dataTemp = (data[0] + data[1] + data[2] + data[3] + data[4])/ 5;

      lcd.setCursor(2, 1);
      lcd.print("             ");
      lcd.setCursor(2, 1);
      lcd.print(dataTemp);
      var = 3;
      state = 0;
      }

    else {
      lcd.setCursor(2, 1);
      lcd.print("            ");
      lcd.setCursor(2, 1);
      lcd.print("SCAN TEMP...");
      }
     
    break;
  case 3:

    if (changedata == 1){
      changedata = 0;
      Serial.print("DATA,TIME,");
      Serial.print(nama1);
      Serial.print(",");
      Serial.println(dataTemp);
      var = 4;
      } 

    if (changedata == 2){
      changedata = 0;
      Serial.print("DATA,TIME,");
      Serial.print(nama2);
      Serial.print(",");
      Serial.println(dataTemp);
      var = 4;
      } 
     
    break;

  case 4:
     if (state == 0){
       digitalWrite(in1, LOW);
       digitalWrite(in2, HIGH);
       state = 1;
      }
    irsensorvalue = digitalRead (irsensor);

    if (irsensorvalue == LOW){
       digitalWrite(in1, LOW);
       digitalWrite(in2, LOW);
       var = 0;
       dataTemp = 0;
       state2 = 0;
      }
    break;
}
  // Look for new cards
  
} 


void dump_byte_array(byte *buffer, byte bufferSize) {
    read_rfid="";
    for (byte i = 0; i < bufferSize; i++) {
        read_rfid=read_rfid + String(buffer[i], HEX);
    }
}
