#include <Arduino.h>
#include <TM1637Display.h>

#define ADC_RESOLUTION 1023.0

#define PIN_OIL_PRESSURE_SENSOR 14 // A0

#define PIN_DIO 3 // D3
#define PIN_CLK 4 // D4

TM1637Display display(PIN_CLK, PIN_DIO);

int counter = 0;
float voltage = 1;
float oilPressureBar = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(A0, INPUT);
  
  display.setBrightness(1); 
}

void loop() {
  // put your main code here, to run repeatedly:
  voltage = analogRead(A0) * (5.0 / 1023.0);
  oilPressureBar = voltageLinearConversionToBar(voltage);
  Serial.print(voltage);
  Serial.print("V");
  Serial.print(" -> ");
  Serial.print(oilPressureBar);
  Serial.print("bar");
  Serial.println();

  showFloatOnSegmentedDisplay(oilPressureBar);
  // display.showNumberDec(1234);
  delay(250);
}

void showFloatOnSegmentedDisplay(float numberage) {
  int integerNum = (int)numberage;
  int fractal1Num = (int)round(numberage * 10.0) % 10;
  int fractal2Num = (int)round(numberage * 100.0) % 10;

  if (numberage > 10) {
    integerNum = 9;
    fractal1Num = 9;
    fractal2Num = 9;
  }

  Serial.print(integerNum);
  Serial.print("-");
  Serial.print(fractal1Num);
  Serial.print("-");
  Serial.print(fractal2Num);
  Serial.println();

  if (counter > 16) {
    uint8_t data[4];

    data[0] = 0x5E; //display.encodeDigit(0);
    data[1] = 0x06;
    data[2] = 0x58;
    // data[3] = 0x76;
    data[3] = 0x78;

    display.setSegments(data);

    if (counter > 24) {
      counter = 0;
    } else {
      counter++;
    }
  } else {
    uint8_t data[4];

    data[0] = 0x00; //display.encodeDigit(0);
    data[1] = display.encodeDigit(integerNum) | 0x80;
    data[2] = display.encodeDigit(fractal1Num);
    data[3] = display.encodeDigit(fractal2Num);

    display.setSegments(data);

    counter++;
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