#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int LCD_PLS = 7;
const int LCD_MNS = 8;

int CLICK_CNT = 0;

int PLAST_STATE = HIGH;
int MLAST_STATE = HIGH;

LiquidCrystal_I2C lcd(0x27, 16, 2);


void setup() {
  pinMode(LCD_PLS, INPUT_PULLUP);
  pinMode(LCD_MNS, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  lcd.print(CLICK_CNT);

}

void loop() {
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
