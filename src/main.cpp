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

// Initialize the temperature sensor
void initTempSensor()
{
  temp_sensor_config_t temp_sensor = TSENS_CONFIG_DEFAULT();
  temp_sensor.dac_offset = TSENS_DAC_L2;
  temp_sensor_set_config(temp_sensor);
  temp_sensor_start();
}

void setup()
{
  Serial.begin(115200);

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUTTON, INPUT);

  Wire.begin(SDA_PIN, SCL_PIN);
  u8g2.begin();

  u8g2.setDisplayRotation(U8G2_R0); // Rotate the display 180 degrees
  u8g2.setFont(u8g2_font_fub14_tf); // Use high pixel wide font
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();

  initTempSensor();

  delay(100);
}

char buffer[5];

void loop()
{
  // Read temperature and convert it to const char*
  float temp_celsius = 0;
  temp_sensor_read_celsius(&temp_celsius);
  snprintf(buffer, sizeof(buffer), "%.0f", temp_celsius);
  const char *buffer_c = buffer;

  u8g2.clearBuffer();
  drawText(buffer_c);
  u8g2.sendBuffer();

  digitalWrite(PIN_LED, digitalRead(PIN_BUTTON));

  if ((millis() / 100) % 10 == 0)
  {
    Serial.println(buffer);
  }

  delay(100);
}