#include <MFRC522.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL29x_K3QaY"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "527fK5TJc4ShU644HYtIZQGIkS7fYxfJ"

#define SS_PIN 5       // MFRC522 slave select pin
#define RST_PIN 16     // MFRC522 reset pin
#define SERVO_PIN 13   // Servo data pin

#define TAG_1 "E3 6E 55 C5"  // Tag 1 UID
#define TAG_2 "03 CE 3D 11"  // Tag 2 UID

#define IR_SENSOR_1 36
#define IR_SENSOR_2 39
#define IR_SENSOR_3 34
#define IR_SENSOR_4 35

// Setting Wi-Fi credentials
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Declanrice";
char pass[] = "kevindurant";

int totalSlots = 4;  // Total number of parking slots
int remSlots;
int threshold = 1000;

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
Servo myServo;                      // Create ESP32Servo instance
LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize LCD with I2C address 0x27, 16 cols, 2 rows

WidgetLED led1(V4);
WidgetLED led2(V5);
WidgetLED led3(V6);
WidgetLED led4(V7);

String getTagUID() {
  String tagUID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    tagUID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    tagUID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  tagUID.toUpperCase();
  return tagUID.substring(1);
}

void setup() {
  Serial.begin(9600);
  SPI.begin();          // Initialize SPI bus
  mfrc522.PCD_Init();   // Initialize MFRC522

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Connect to Blynk
  Blynk.begin(auth, ssid, pass);

  myServo.attach(SERVO_PIN); // Attach servo to pin

  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);          // Initialize LCD
  lcd.setCursor(0, 0);
  lcd.print("Please Scan Card"); // Display initial text

  pinMode(IR_SENSOR_1, INPUT);
  pinMode(IR_SENSOR_2, INPUT);
  pinMode(IR_SENSOR_3, INPUT);
  pinMode(IR_SENSOR_4, INPUT);

  Serial.println("Ready to scan RFID tags...");
}

void loop() {
  // Check for new RFID tags
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String tagUID = getTagUID();
    Serial.println("RFID Tag UID: " + tagUID);

    // Check if the scanned tag is Tag 1 or Tag 2
    if (tagUID.equals(TAG_1) || tagUID.equals(TAG_2)) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ACCESS GRANTED");
      
      myServo.write(90);
      delay(3000);          // Wait 3 seconds
      myServo.write(0);           // Close gate

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Please Scan Card");
      
      mfrc522.PCD_Init();   // Re-initialize MFRC522
    } else {
      lcd.clear();
      lcd.print("ACCESS DENIED");
      delay(2000);          // Display for 2 seconds
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Please Scan Card");
    }
    // mfrc522.PICC_HaltA();
    // mfrc522.PCD_StopCrypto1();
  }

  // Read IR sensors
  int sensor1Value = analogRead(IR_SENSOR_1);
  int sensor2Value = analogRead(IR_SENSOR_2);
  int sensor3Value = analogRead(IR_SENSOR_3);
  int sensor4Value = analogRead(IR_SENSOR_4);

  Blynk.run();

  // Check Slot 1
  if (sensor1Value > threshold) {
    lcd.setCursor(0, 1);
    lcd.print("1");
    lcd.setCursor(0, 0);
    lcd.print("Please Scan Card");
    Serial.println("LED1 ON");
    led1.on();
  } else {
    lcd.setCursor(0, 1);
    lcd.print(" ");
    Serial.println("LED1 OFF");
    led1.off();
  }

  // Check Slot 2
  if (sensor2Value > threshold) {
    lcd.setCursor(1, 1);
    lcd.print("2");
    lcd.setCursor(0, 0);
    lcd.print("Please Scan Card");
    Serial.println("LED2 ON");
    led2.on();
  } else {
    lcd.setCursor(1, 1);
    lcd.print(" ");
    Serial.println("LED2 OFF");
    led2.off();
  }

  // Check Slot 3
  if (sensor3Value > threshold) {
    lcd.setCursor(2, 1);
    lcd.print("3");
    lcd.setCursor(0, 0);
    lcd.print("Please Scan Card");
    Serial.println("LED3 ON");
    led3.on();
  } else {
    lcd.setCursor(2, 1);
    lcd.print(" ");
    Serial.println("LED3 OFF");
    led3.off();
  }

  // Check Slot 4
  if (sensor4Value > threshold) {
    lcd.setCursor(3, 1);
    lcd.print("4");
    lcd.setCursor(0, 0);
    lcd.print("Please Scan Card");
    Serial.println("LED4 ON");
    led4.on();
  } else {
    lcd.setCursor(3, 1);
    lcd.print(" ");
    Serial.println("LED4 OFF");
    led4.off();
  }
}