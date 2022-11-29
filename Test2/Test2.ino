#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
SoftwareSerial esp(2, 3);
LiquidCrystal_I2C lcd(0x27, 16, 2);
int sensor = A0;
int pot = A2;
int relay = 8;
//int pot = A2;
void lcdOutput(int sensorPercent, int potPercent) {
  Serial.println(sensorPercent);
  Serial.println(potPercent);
  lcd.setCursor(0, 0); //Đặt vị trí con trỏ ở cột 1 hàng 1
  lcd.print("Do am: "); //In lên màn hình lcd "Do am: "
  lcd.setCursor(7, 0); //Đặt vị trí con trỏ ở cột 8 hàng 1
  lcd.print(sensorPercent); //In lên màn hình lcd giá trị cảm biến
  lcd.print("%");
  //Hiển thị giá trị biến trở lên màn hình lcd
  lcd.setCursor(0, 1); //Đặt vị trí con trỏ ở cột 1 hàng 2
  lcd.print("Chon do am: "); //In lên màn hình lcd "Chon do am: "
  lcd.setCursor(12, 1); //Đặt vị trí con trỏ ở cột 13 hàng 2
  lcd.print(potPercent); //In lên màn hình lcd giá trị biến trở
  lcd.print("%");
}
void automatic(int sensor, int pot) {
  if (sensor < pot) {
    digitalWrite(relay, HIGH);
    digitalWrite(LED_BUILTIN,HIGH);
  } else {
    digitalWrite(relay, LOW);
    digitalWrite(LED_BUILTIN,LOW);
  }
}
void handle(int state) {
  digitalWrite(relay, state);
  if(state==1){
  digitalWrite(LED_BUILTIN,HIGH);}
  else {
    digitalWrite(LED_BUILTIN,LOW);
  }
}
void setup() {
  pinMode(relay, OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  lcd.init();
  lcd.backlight();
  Serial.begin(9600); /* begin serial for debug */
  esp.begin(9600); /* join i2c bus with SDA=D1 and SCL=D2 of NodeMCU */
}

void loop() {
  lcd.clear();
  //Đọc giá trị và chuyển đổi sang phần trăm
  int sensorValue = analogRead(sensor);
  int sensorPercent = map(sensorValue, 0, 1023, 100, 0);
  int potValue = analogRead(pot);
  int potPercent = map(potValue, 0, 1023, 0, 100);
  String dataRaw = String(sensorPercent) + "," + String(potPercent);
  char c[7];
  String data = "";
  dataRaw.toCharArray(c, sizeof(c));
  esp.write(c, sizeof(c));
  if (esp.available()) {
    while (esp.available() > 0) {
      char c;
      c = esp.read();
      data += c;
      esp.flush();
      delay(5);
    }
  }
  if (data.charAt(0) == '0') {
    automatic(sensorPercent, potPercent);
  }
  else if (data.charAt(0) == '1') {
    int state;
    if (data.charAt(2) == '1') {
      state = 1;
    }
    else {
      state = 0;
    }
    handle(state);
  }
  lcdOutput(sensorPercent, potPercent);
  delay(100);
}
