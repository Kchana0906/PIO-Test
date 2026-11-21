#include <Arduino.h>
#include <Wire.h>
#include <ArtronShop_RX8130CE.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>


const int LCD_PLS = 7;
const int LCD_MNS = 8;
const int ENTER_BTN = 9;
const int BACK_BTN = 10;


int CLICK_CNT = 0;

int PLAST_STATE = HIGH;
int MLAST_STATE = HIGH;
int ELAST_STATE = HIGH;
int BLAST_STATE = HIGH;

int settingMode = 0;

struct tm settingTime = {};

ArtronShop_RX8130CE rtc(&Wire);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void showSetting() {

  lcd.clear();

  switch (settingMode) {

    case 0:
      lcd.print("SET YEAR");
      lcd.setCursor(0, 1);
      lcd.print(settingTime.tm_year + 1900);
      break;

    case 1:
      lcd.print("SET MONTH");
      lcd.setCursor(0, 1);
      lcd.print(settingTime.tm_mon + 1);
      break;

    case 2:
      lcd.print("SET DAY");
      lcd.setCursor(0, 1);
      lcd.print(settingTime.tm_mday);
      break;

    case 3:
      lcd.print("SET HOUR");
      lcd.setCursor(0, 1);
      lcd.print(settingTime.tm_hour);
      break;

    case 4:
      lcd.print("SET MINUTE");
      lcd.setCursor(0, 1);
      lcd.print(settingTime.tm_min);
      break;

    case 5:
      lcd.print("SET SECOND");
      lcd.setCursor(0, 1);
      lcd.print(settingTime.tm_sec);
      break;

    case 6:
      lcd.print("CONFIRM TIME");
      lcd.setCursor(0, 1);
      lcd.print("ENTER: SAVE");
      break;
  }
}

void changeValue(int amount) {

  switch (settingMode) {

    case 0:
      settingTime.tm_year += amount;

      if (settingTime.tm_year + 1900 > 2099)
        settingTime.tm_year = 126;

      if (settingTime.tm_year + 1900 < 2026)
        settingTime.tm_year = 199;
      break;

    case 1:
      settingTime.tm_mon += amount;

      if (settingTime.tm_mon > 11)
        settingTime.tm_mon = 0;

      if (settingTime.tm_mon < 0)
        settingTime.tm_mon = 11;
      break;

    case 2:
      settingTime.tm_mday += amount;

      if (settingTime.tm_mday > 31)
        settingTime.tm_mday = 1;

      if (settingTime.tm_mday < 1)
        settingTime.tm_mday = 31;
      break;

    case 3:
      settingTime.tm_hour += amount;

      if (settingTime.tm_hour > 23)
        settingTime.tm_hour = 0;

      if (settingTime.tm_hour < 0)
        settingTime.tm_hour = 23;
      break;

    case 4:
      settingTime.tm_min += amount;

      if (settingTime.tm_min > 59)
        settingTime.tm_min = 0;

      if (settingTime.tm_min < 0)
        settingTime.tm_min = 59;
      break;

    case 5:
      settingTime.tm_sec += amount;

      if (settingTime.tm_sec > 59)
        settingTime.tm_sec = 0;

      if (settingTime.tm_sec < 0)
        settingTime.tm_sec = 59;
      break;
  }

  showSetting();
}

void setup() {

  Serial.begin(9600);
  Wire.begin();

  pinMode(LCD_PLS, INPUT_PULLUP);
  pinMode(LCD_MNS, INPUT_PULLUP);
  pinMode(ENTER_BTN, INPUT_PULLUP);
  pinMode(BACK_BTN, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  if (!rtc.begin()) {
    lcd.print("RTC Error!");
    while (1);
  }

  // 初期設定値
  settingTime.tm_year = 2026 - 1900;
  settingTime.tm_mon = 11 - 1;
  settingTime.tm_mday = 21;
  settingTime.tm_hour = 0;
  settingTime.tm_min = 0;
  settingTime.tm_sec = 0;

  settingMode = 0;
  showSetting();
}

void loop() {

  int PNOW_STATE = digitalRead(LCD_PLS);
  int MNOW_STATE = digitalRead(LCD_MNS);
  int ENOW_STATE = digitalRead(ENTER_BTN);
  int BNOW_STATE = digitalRead(BACK_BTN);

  if (PNOW_STATE == LOW && PLAST_STATE == HIGH) {
    if (settingMode <= 5) {
      changeValue(1);
    }
  }


  // DOWN
  if (MNOW_STATE == LOW && MLAST_STATE == HIGH) {
    if (settingMode <= 5) {
      changeValue(-1);
    }
  }


  // ENTER
  if (ENOW_STATE == LOW && ELAST_STATE == HIGH) {

    if (settingMode < 6) {
      settingMode++;
      showSetting();
    }

    else if (settingMode == 6) {

      if (rtc.setTime(settingTime)) {
        lcd.clear();
        lcd.print("Time Saved!");
        settingMode = 7;
      }
    }
  }


  // BACK
  if (BNOW_STATE == LOW && BLAST_STATE == HIGH) {

    if (settingMode > 0 && settingMode <= 6) {
      settingMode--;
      showSetting();
    }
  }


  // ボタン状態更新
  PLAST_STATE = PNOW_STATE;
  MLAST_STATE = MNOW_STATE;
  ELAST_STATE = ENOW_STATE;
  BLAST_STATE = BNOW_STATE;


  delay(50);
}
