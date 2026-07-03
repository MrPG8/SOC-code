#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define LED 2

#define ADC_1_RESOLUTION 4095.0
#define OIL_PRESSURE_SENSOR 34 // ADC1_6
#define R1             30000.0 // resistor values in voltage sensor (in ohms)
#define R2             7500.0  // resistor values in voltage sensor (in ohms)

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

unsigned long lastTime = 0;
unsigned long timerInterval = 500; // timer for loop function

float voltage = 1;
bool ledOn = false;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  analogReadResolution(12);
  analogSetPinAttenuation(OIL_PRESSURE_SENSOR, ADC_11db)

  pinMode(LED, OUTPUT);

  pinMode(OIL_PRESSURE_SENSOR, INPUT);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  delay(2000);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5, 25);
  // Display static text
  display.println("dickbutt!NOTLOOP");
  display.println("0V");
  // display.println("can it fit tho?");
  display.display(); 
}

void loop() {
  // display.startscrollleft(0x00, 0x0F);

  if ((millis() - lastTime) > timerInterval) {
    lastTime = millis();
    voltage = analogReadMilliVolts(OIL_PRESSURE_SENSOR) / 1000.0f;

    float voltage_in = voltage * (R1 + R2) / R2;
    // float voltage_adc = ((float)voltage * 3.3f) / ADC_1_RESOLUTION;
    // voltage = sensorValue * (3.3 / 4095.0);
    // Serial.println("Value:" + oilPressureAnalogValue);

    float voltage_adc = (analogReadMilliVolts(OIL_PRESSURE_SENSOR) / 1000.0f * 4.118f);

    // Clear previous screen contents
    display.clearDisplay();

    display.setCursor(0, 0);
    // display.println("dickbutt!");
    display.print(voltage);
    display.print("V orig");
    display.println();

    display.print(voltage_in);
    display.print("V c1");
    display.println();

    display.print(voltage_adc);
    display.print("V c2");
    display.println();

    display.print(voltageLinearConversionToBar(voltage_adc));
    display.print("bar");

    display.display();

    // delay(1000);

    // if (ledOn == true) {
    //   digitalWrite(LED, LOW);
    //   Serial.println("LED is off");
    //   // delay(1000);
    //   ledOn = false;
    // }else {
    //   digitalWrite(LED, HIGH);
    //   Serial.println("LED is on");
    //   // delay(1000);
    //   ledOn = true;    
    // }
  }
}

float voltageLinearConversionToBar(float voltage) {
  return mapFloat(voltage, 0.5f, 4.5f, 0.5f, 10.5f);
}

float mapFloat(float x, float inMin, float inMax, float outMin, float outMax)
{
  return (x - inMin) *
         (outMax - outMin) /
         (inMax - inMin) +
         outMin;
  }
