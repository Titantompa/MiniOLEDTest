#include <Arduino.h>
#include <U8g2lib.h>
// Legacy library for the temperature sensor
#include "driver/temp_sensor.h"

#include "WiFi.h"

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
  // Serial.begin(115200);

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUTTON, INPUT);

  Wire.begin(SDA_PIN, SCL_PIN);
  u8g2.begin();

  u8g2.setDisplayRotation(U8G2_R0); // Rotate the display 180 degrees
  u8g2.setFont(u8g2_font_fub14_tf); // Use high pixel wide font
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();

  initTempSensor();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  delay(100);
}

char buffer[5];

void loop()
{

  if (((millis() / 1000) % 5) < 2)
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
  }
  else
  {
    // WiFi.scanNetworks will return the number of networks found.
    int n = WiFi.scanNetworks();

    snprintf(buffer, sizeof(buffer), "%d", n);
    
    u8g2.clearBuffer();
    drawText(buffer);
    u8g2.sendBuffer();

    digitalWrite(PIN_LED, digitalRead(PIN_BUTTON));

#if 0
    // Serial.println("Scan done");
    if (n == 0)
    {
      // Serial.println("no networks found");
    }
    else
    {
      Serial.print(n);
      Serial.println(" networks found");
      Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
      for (int i = 0; i < n; ++i)
      {
        // Print SSID and RSSI for each network found
        Serial.printf("%2d", i + 1);
        Serial.print(" | ");
        Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
        Serial.print(" | ");
        Serial.printf("%4d", WiFi.RSSI(i));
        Serial.print(" | ");
        Serial.printf("%2d", WiFi.channel(i));
        Serial.print(" | ");
        switch (WiFi.encryptionType(i))
        {
        case WIFI_AUTH_OPEN:
          Serial.print("open");
          break;
        case WIFI_AUTH_WEP:
          Serial.print("WEP");
          break;
        case WIFI_AUTH_WPA_PSK:
          Serial.print("WPA");
          break;
        case WIFI_AUTH_WPA2_PSK:
          Serial.print("WPA2");
          break;
        case WIFI_AUTH_WPA_WPA2_PSK:
          Serial.print("WPA+WPA2");
          break;
        case WIFI_AUTH_WPA2_ENTERPRISE:
          Serial.print("WPA2-EAP");
          break;
        case WIFI_AUTH_WPA3_PSK:
          Serial.print("WPA3");
          break;
        case WIFI_AUTH_WPA2_WPA3_PSK:
          Serial.print("WPA2+WPA3");
          break;
        case WIFI_AUTH_WAPI_PSK:
          Serial.print("WAPI");
          break;
        default:
          Serial.print("unknown");
        }
        Serial.println();
        delay(10);
      }
    }
    Serial.println("");
#endif

    // Delete the scan result to free memory for code below.
    WiFi.scanDelete();
  }

  delay(100);
}