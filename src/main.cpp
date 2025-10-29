#include <Arduino.h>
#include <U8g2lib.h>
// Legacy library for the temperature sensor
#include "driver/temp_sensor.h"

#include <Wire.h>

#define SDA_PIN 5
#define SCL_PIN 6

#define PIN_LED 8
#define PIN_BUTTON 9

U8G2_SSD1306_72X40_ER_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void drawText(const char *text)
{
  u8g2.drawStr(0, 0, text);
}

unsigned long lastFrameMillis;
unsigned long movingAverageFrameMillis;

void setup()
{
  // Serial.begin(115200);

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUTTON, INPUT);

  Wire.begin(SDA_PIN, SCL_PIN);
  u8g2.begin();

  u8g2.setDisplayRotation(U8G2_R0); // Rotate the display 180 degrees
  u8g2.setFont(u8g2_font_fub14_tf); // Use high pixel wide font
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();

  lastFrameMillis = millis();

  delay(100);
}

char buffer[16];

void loop()
{

  unsigned long currentFrameMillis = millis();
  unsigned long deltaFrameMillis = currentFrameMillis - lastFrameMillis;
  movingAverageFrameMillis -= movingAverageFrameMillis>>3;
  movingAverageFrameMillis += deltaFrameMillis;
  lastFrameMillis = currentFrameMillis;

  unsigned long fps = 1000/movingAverageFrameMillis;

  if(currentFrameMillis%1000 == 0)
  {
    snprintf(buffer, sizeof(buffer), "%d", fps);
    u8g2.clearBuffer();
    drawText(buffer);
    u8g2.sendBuffer();
  }

  digitalWrite(PIN_LED, digitalRead(PIN_BUTTON));
}