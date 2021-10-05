#include <SoftwareSerial.h>
SoftwareSerial timeSerial(8,9);
#include <Adafruit_GFX.h> 
#include <Adafruit_ST7735.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#define TFT_CS         7
#define TFT_RST        6
#define TFT_DC         5
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
void setup(){
    Serial.begin(9600);
    timeSerial.begin(9600);
    tft.initR(INITR_144GREENTAB);
    tft.fillScreen(ST77XX_BLACK);
    tft.setRotation(2);
    tft.setTextColor(ST77XX_GREEN);
    Serial.println("Setup executed.");
    tft.print("Waiting...");
    delay(5000);
}

void loop(){
    String content = "";
    while (timeSerial.available())
    {
        content += char(timeSerial.read());
        //delay(1);
    }
    tft.fillScreen(ST77XX_BLACK);
    if(content != ""){
        Serial.println(content);
        Serial.println("*********************");
        tft.setCursor(10, 30); 
        tft.print(content);
    }
    delay(500);
}
