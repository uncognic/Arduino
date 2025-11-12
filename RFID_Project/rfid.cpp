// Include 
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
// Declare pins
#define SS_PIN 10
#define RST_PIN 9
#define BUZZER 3
const int position1 = 0;
const int position2 = 90;  
// Using libraries, create objects for device
MFRC522 mfrc522(SS_PIN, RST_PIN);   
LiquidCrystal_I2C lcd(0x27,16,2);
Servo myservo;
// Clear LCD function
void clearlcd()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan or swipe");
  lcd.setCursor(0, 1);
  lcd.print("card");
}
void setup() 
{
  // Start serial stream
  Serial.begin(9600);   
    Serial.print("sus");
// Initialize devices
  SPI.begin();      
  mfrc522.PCD_Init();  
// Debug purposes
  Serial.println("Touch your card to the reader...");
  Serial.println();
// Intialize LCD
  lcd.init();
  lcd.clear();
  lcd.backlight();
  clearlcd();
// Initialize buzzer pin
  pinMode(BUZZER, OUTPUT);
// Initialize servo
  myservo.attach(5);
  myservo.write(position1);
}

void loop() 
{
// If not reading then return (???)
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  Serial.print("sus");
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  // For loop to convert character by character from array uidByte into hex, then print character
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
// Print debug
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
// Authorized card flow
  if (content.substring(1) == "19 A9 46 03") 
  {

    Serial.println("Authorized access");
    Serial.println();
    tone(BUZZER, 800); 
    delay(400);
    noTone(BUZZER);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Access Granted");
    myservo.write(position2);              
    delay(5000);    
    clearlcd(); 
    myservo.write(position1);
  // Refused flow
  } else   {
    Serial.println(" Access denied");
    tone(BUZZER, 400); 
    delay(400);
    noTone(BUZZER);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Access Denied");
    delay(3000);

    clearlcd();
  }
// Debug
    Serial.print("sus");

} 
