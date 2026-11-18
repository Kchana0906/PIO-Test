#include <Arduino.h>
#include <Wire.h>
#include <ArtronShop_RX8130CE.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>


const int LCD_PLS = 7;
const int LCD_MNS = 8;

int CLICK_CNT = 0;

int PLAST_STATE = HIGH;
int MLAST_STATE = HIGH;

ArtronShop_RX8130CE rtc(&Wire);
LiquidCrystal_I2C lcd(0x27, 16, 2);


void setup() {

  Serial.begin(9600);
  Wire.begin();

  pinMode(LCD_PLS, INPUT_PULLUP);
  pinMode(LCD_MNS, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  lcd.print(CLICK_CNT);

  if (!rtc.begin()) {
    Serial.println("RTC initialization error!");

    lcd.setCursor(0, 0);
    lcd.print("RTC Error!");

    while (1);
  }

  struct tm t = {};

  t.tm_year = 2026 - 1900;
  t.tm_mon = 10 - 1;
  t.tm_mday = 20;

  t.tm_hour = 13;
  t.tm_min = 0;
  t.tm_sec = 0;

  // RTCに時刻を設定
  if (rtc.setTime(t)) {
    Serial.println("Time setting successful!");
  } else {
    Serial.println("Time setting failed!");
  }
}

void loop() {
  struct tm t;

  // RTCから時刻を取得
  if (rtc.getTime(&t)) {

    // シリアルモニターに表示
    Serial.print(t.tm_year + 1900);
    Serial.print("/");
    Serial.print(t.tm_mon + 1);
    Serial.print("/");
    Serial.print(t.tm_mday);

    Serial.print(" ");

    Serial.print(t.tm_hour);
    Serial.print(":");
    Serial.print(t.tm_min);
    Serial.print(":");
    Serial.println(t.tm_sec);

    lcd.setCursor(0, 0);

    lcd.print("Time: ");

    if (t.tm_hour < 10) {
      lcd.print("0");
    }
    lcd.print(t.tm_hour);

    lcd.print(":");

    if (t.tm_min < 10) {
      lcd.print("0");
    }
    lcd.print(t.tm_min);

    lcd.print(":");

    if (t.tm_sec < 10) {
      lcd.print("0");
    }
    lcd.print(t.tm_sec);

  } else {
    Serial.println("Failed to get time!");

    lcd.setCursor(0, 0);
    lcd.print("RTC Read Error  ");
  }

  delay(1000);

  int PNOW_STATE = digitalRead(LCD_PLS);
  int MNOW_STATE = digitalRead(LCD_MNS);

  if(PNOW_STATE == LOW && PLAST_STATE == HIGH){
    CLICK_CNT++;

    lcd.clear();
    lcd.print(CLICK_CNT);
  }

  if(MNOW_STATE == LOW && MLAST_STATE == HIGH){
    CLICK_CNT--;

    lcd.clear();
    lcd.print(CLICK_CNT);
  }

  PLAST_STATE = PNOW_STATE;
  MLAST_STATE = MNOW_STATE;
}
