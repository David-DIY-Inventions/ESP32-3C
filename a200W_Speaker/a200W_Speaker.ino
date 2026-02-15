#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#define frames
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
#define SCREEN_I2C_ADDR 0x3D // or 0x3C
#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels
#define OLED_RST_PIN -1      // Reset pin (-1 if not available)
#define BATTERY_PIN
#define ADC_MAX    4.095
#define ADC_REF    3.3


#include "U8g2lib.h" // include u8g2 library for drawing on the OLED display

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0); // ["F" = full framebuffer, size = 1024 bytes, hardware I2C connection]

byte audio_bar_height[7]; // sizes for the individual bars
byte audio_bar_peak[7]; // positions for the individual peaks (lines over the bars)

void setup() {
  //booting display
  delay(1000);
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(19, 17);
display.println("Starting..");
display.display();
//leds
pinMode(1, OUTPUT);
digitalWrite(1, HIGH);
pinMode(2, OUTPUT);
digitalWrite(2, HIGH);
delay(1700);
//continuing boot display
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(39, 40);
display.println("David-DIY");
display.display();
//LEDs
pinMode(1, OUTPUT);
digitalWrite(1, LOW);
pinMode(2, OUTPUT);
digitalWrite(2, LOW);

delay(2000);
pinMode(3, OUTPUT);
digitalWrite(3, HIGH);
//preparing sound animation
display.clearDisplay();
  u8g2.setColorIndex(1);  // set the color to white
  u8g2.begin(); // start the u8g2 library/display
  {
    delay(550);
  }
  //leds
  pinMode(1, OUTPUT);
digitalWrite(1, HIGH);
pinMode(2, OUTPUT);
digitalWrite(2, HIGH);
}
int frame = 0;
void loop() {
 // Low battery voltage
 int raw = analogRead(4);
  float voltage = (raw / ADC_MAX) * ADC_REF;
  float batteryVoltage = voltage * (133.0 /  33.0);
  //If the actual battery voltage will get to 9.5V  
  //The 6.9 is just a reference for the esp32 3C to work properly
  if (voltage < 6.9 ) {
    display.clearDisplay();
    display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(20, 20);
display.println(" Battery ");
display.setTextSize(2);
display.setCursor(30, 40);
display.println(" LOW..!");
display.display();
delay(2);
pinMode(1, OUTPUT);
digitalWrite(1, LOW);
pinMode(2, OUTPUT);
digitalWrite(2, LOW);
  }
     else {
  //sound animation
display.clearDisplay();
   
    u8g2.clearBuffer();  // clear buffer for storing display content in RAM

  for (int i=0; i<7; i++) { // loop for every fraquency (63Hz, 160Hz, 400Hz, 1kHz, 2.5kHz, 6.25kHz and 16kHz)

    int random_value = random(1024); // calculate random value between 0-1024
    audio_bar_height[i] = audio_bar_height[i] + ((map(random_value, 0, 1024, 0, 53) - audio_bar_height[i]) / 4.0); // update the bar with a new value (slowly)


    // calculate the peak position
    if (audio_bar_peak[i] < audio_bar_height[i]) { // if the peak is below the current bar size
      audio_bar_peak[i] = audio_bar_height[i]; // move peak to the new max. position (i.e. size of the bar)
    } else if (audio_bar_peak[i] > audio_bar_height[i]) { // if the bar is lower than the peak
      audio_bar_peak[i]--; // slowly move the peak down, one pixel every frame
    }

    u8g2.drawBox(2 + i*19, 53-audio_bar_height[i], 10, audio_bar_height[i]); // draw bar

    u8g2.drawBox(2 + i*19, 53-audio_bar_peak[i], 10, 1); // draw peak
  }
  // draw the individual labels
  u8g2.setFont(u8g2_font_nerhoe_tr); // set font
  u8g2.drawStr(21, 64, " ~ BlueTooth ON ~ ");     


  u8g2.sendBuffer(); // send buffer from RAM to display controller
  delay(10);
     }
}
 //David-DIY inventions
// This program is free of charge, to any person obtaining a copy
//of this software, however.
//The above copyright notice shall be included in all copies or substantial portions of the Software.


// The sound animation part was taken from Upir! Check out his chanel, he has a lot of very cool videos to!

//Copyright (c) 2023 upir
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions

//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.
