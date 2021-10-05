/*
提示：线程内这样延时！！否则影响其他线程！线程内别重复定义！
    static unsigned long lastTimeBlink = 0;
    lastTimeBlink = millis();
    PT_WAIT_UNTIL(pt, millis() - lastTimeBlink > 10);

提示：如何创建线程
    Step1.在程序开头创建对象 static struct pt pt1,pt2;
    Step2.用下面的格式创建程序块：
static int protothreadFunction(struct pt *pt){//这里传递pt类型的形参pt
    PT_BEGIN(pt);
    //在此书写代码，前后的pt不要乱动，可以调用全局变量，局部变量要加上static修饰符
    //局部变量不可在声明时用变量赋值
    PT_END(pt);
}
    Step3.在setup函数中添加：
    PT_INIT(&pt1);//pt1与声明的对象一致
    Step4.在loop函数中添加：
    protothreadFunction(&pt1);//pt1与声明的对象一致，函数名与第二步一致
*/
static boolean isLocked = false;
static String time_Content = "";
//static int rotation = 0;
#include <SoftwareSerial.h>
SoftwareSerial timeSerial(8,9);
#include <Adafruit_GFX.h> 
#include <Adafruit_ST7735.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
//#include <Fonts/FreeSansBold18pt7b.h>
#include <SD.h>
//#define CS 8
//#define TFT_CS         7
//#define TFT_RST        6
//#define TFT_DC         5
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
//Adafruit_ST7735 tft = Adafruit_ST7735(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst);
Adafruit_ST7735 tft = Adafruit_ST7735(7, 5, 2, 4, 6);
File myFile;
#include <pt.h>
// Create your protothread(s)
static struct pt pt1,pt2,pt3,pt4,pt5,pt6;
// A protothread function
static int serialGetTime(struct pt *pt){
    // Start a protothread
    PT_BEGIN(pt);
    /* Your code inside this thread */
    tft.setTextColor(ST77XX_YELLOW,ST77XX_BLUE);
    tft.setTextSize(1);
    time_Content = "";
    timeSerial.print("Request_TIME");
    static unsigned long lastTimeBlink = 0;
    lastTimeBlink = millis();
    PT_WAIT_UNTIL(pt, millis() - lastTimeBlink > 20);
    //static String time_Content = "";
    while (timeSerial.available())
    {
        time_Content += char(timeSerial.read());
    }
    
    if(time_Content != ""){
        time_Content = time_Content.substring(0, 8);
        tft.setFont();
        //tft.fillScreen(ST77XX_BLACK);
        Serial.println(time_Content);
        Serial.println("*********************");
        tft.setCursor(0, 0); 
        tft.print(time_Content);
        //time_Content = "";
    }
    lastTimeBlink = millis();
    PT_WAIT_UNTIL(pt, millis() - lastTimeBlink > 300);
    // Stop a protothread
    PT_END(pt);
}


static int protothreadBlinkLED2(struct pt *pt)
{
  static unsigned long lastTimeBlink = 0;
  PT_BEGIN(pt);
  while(1) {
    lastTimeBlink = millis();
    PT_WAIT_UNTIL(pt, millis() - lastTimeBlink > 200);
    digitalWrite(A3, HIGH);
    lastTimeBlink = millis();
    PT_WAIT_UNTIL(pt, millis() - lastTimeBlink > 200);
    digitalWrite(A3, LOW);
  }
  PT_END(pt);
}


static byte rotation;
static int screenRotation(struct pt *pt){//这里传递pt类型的形参pt
    PT_BEGIN(pt);
    //在此书写代码，前后的pt不要乱动，不能调用全局变量，局部变量要加上static修饰符
    //static int rotation;
    if(isLocked == false){//锁定后不旋转

    /*
    if(digitalRead(2) == LOW)
    {
        //tft.setRotation(1);
        //tft.fillScreen(ST77XX_BLACK);
        rotation = 1;
    }
    if(digitalRead(2) == HIGH)
    {
        //tft.setRotation(0);
        //tft.fillScreen(ST77XX_BLACK);
        rotation = 0;
    }*/
    if(digitalRead(A1) == HIGH and digitalRead(A2) == HIGH){
        rotation = 0;
    }
    if(digitalRead(A1) == HIGH and digitalRead(A2) == LOW){
        rotation = 1;
    }
    if(digitalRead(A1) == LOW and digitalRead(A2) == LOW){
        rotation = 2;
    }
    if(digitalRead(A1) == LOW and digitalRead(A2) == HIGH){
        rotation = 3;
    }

    static int tmp;
    tmp = rotation;
    static unsigned long lastTimeBlink = 0;
    lastTimeBlink = millis();
    PT_WAIT_UNTIL(pt, millis() - lastTimeBlink > 500);
    //待补充,与上一段if判断相同
    /*
    if(digitalRead(2) == LOW)
    {
        //tft.setRotation(1);
        //tft.fillScreen(ST77XX_BLACK);
        rotation = 1;
    }
    if(digitalRead(2) == HIGH)
    {
        //tft.setRotation(0);
        //tft.fillScreen(ST77XX_BLACK);
        rotation = 0;
    }*/
    if(digitalRead(A1) == HIGH and digitalRead(A2) == HIGH){
        rotation = 0;
    }
    if(digitalRead(A1) == HIGH and digitalRead(A2) == LOW){
        rotation = 1;
    }
    if(digitalRead(A1) == LOW and digitalRead(A2) == LOW){
        rotation = 2;
    }
    if(digitalRead(A1) == LOW and digitalRead(A2) == HIGH){
        rotation = 3;
    }

    if(tmp != rotation){
        Serial.println(rotation);
        Serial.print(",");
        Serial.print(tmp);
        
        tft.setRotation(rotation);

        tft.fillScreen(ST77XX_BLACK);
    }

    }
    //Serial.println("rotation:" + rotation);
    PT_END(pt);
}

static int showTime(struct pt *pt){//功能线程1，显示大时钟
    PT_BEGIN(pt);
    //在此书写代码，前后的pt不要乱动，可以调用全局变量，局部变量要加上static修饰符
    while(rotation == 1){
        //Serial.println("rotation != 1");
        //static unsigned long lastTimeBlink = 0;
        //lastTimeBlink = millis();
        //PT_WAIT_UNTIL(pt, millis() - lastTimeBlink > 500);
        //tft.setFont(&FreeSansBold18pt7b);
        tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
        tft.setTextSize(4);
        static String hour;
        hour = time_Content.substring(0, 2);
        static String minute;
        minute = time_Content.substring(3, 5);
        //static String maohao = ":";
        if(hour != "" and minute != ""){
            tft.fillScreen(ST77XX_BLACK);
        }
        tft.setCursor(40, 30);
        tft.print(hour);
        tft.setCursor(40, 62);
        tft.print(minute);
        static unsigned long lastTimeBlink = 0;
        lastTimeBlink = millis();
        PT_WAIT_UNTIL(pt, millis() - lastTimeBlink > 5000);
        
    }

    PT_END(pt);
}



static int lockScreen(struct pt *pt){//这里传递pt类型的形参pt
    PT_BEGIN(pt);
    //在此书写代码，前后的pt不要乱动，可以调用全局变量，局部变量要加上static修饰符
    //局部变量不可在声明时用变量赋值
    if(digitalRead(3) == LOW){
        static unsigned long lastTimeBlink = 0;
        lastTimeBlink = millis();
        PT_WAIT_UNTIL(pt, millis() - lastTimeBlink > 100);
        if(digitalRead(3) == LOW){
            isLocked = !isLocked;
            if(isLocked == true){
                tft.setFont();
                tft.setCursor(40, 0); 
                tft.print("locked");
                tft.fillScreen(ST77XX_BLACK);
                static unsigned long lastTimeBlink = 0;
                lastTimeBlink = millis();
                PT_WAIT_UNTIL(pt, millis() - lastTimeBlink > 1000);
            }
            else{
                tft.setFont();
                tft.setCursor(40, 0); 
                tft.print("auto");
                tft.fillScreen(ST77XX_BLACK);
                static unsigned long lastTimeBlink = 0;
                lastTimeBlink = millis();
                PT_WAIT_UNTIL(pt, millis() - lastTimeBlink > 1000);
            }
        }
    }

    
    





    PT_END(pt);
}

static int readSDCard(struct pt *pt){//这里传递pt类型的形参pt
    PT_BEGIN(pt);
    //在此书写代码，前后的pt不要乱动，可以调用全局变量，局部变量要加上static修饰符
    //局部变量不可在声明时用变量赋值
    if(rotation == 3){
        tft.setRotation(3);
        tft.setCursor(0, 10);
        tft.setTextColor(ST77XX_YELLOW,ST77XX_BLUE);
        tft.setFont();
        tft.println("SDCard files list");
        Serial.print("Initializing SD card...");
        if (!SD.begin(A3)) {
            Serial.println("initialization failed!");
            tft.print("SDCard read fail");
            //while (1);
        }else{
            tft.print("SDCard read success");
        }
        myFile = SD.open("/");
        while (true) {
            static File entry =  myFile.openNextFile();
            if (! entry) {
                // no more files
                tft.setTextColor(ST77XX_WHITE,ST77XX_GREEN);
                tft.print("(no more...)");
                break;
            }
            //for (uint8_t i = 0; i < numTabs; i++) {
            //    Serial.print('\t');
            //}
            Serial.println(entry.name());
            if (entry.isDirectory()) {
                Serial.print("/");
                //printDirectory(entry, numTabs + 1);
            } else {
                // files have sizes, directories do not
                Serial.print("\t\t");
                Serial.println(entry.size(), DEC);
            }
            entry.close();
        }


    }



    PT_END(pt);
}



void setup() {
    Serial.begin(9600);
    timeSerial.begin(9600);
    tft.initR(INITR_144GREENTAB);
    tft.fillScreen(ST77XX_BLACK);
    tft.setRotation(1);
    //tft.setFont(&FreeSansBold18pt7b);
    tft.setTextColor(ST77XX_YELLOW,ST77XX_BLUE);
    
    tft.print("MultiThread:Waiting..");
    //delay(5000);
    pinMode(A3, OUTPUT);
    //pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    pinMode(A1, INPUT_PULLUP);
    pinMode(A2, INPUT_PULLUP);
    // Init your protothread(s)
    
    PT_INIT(&pt1);
    PT_INIT(&pt2);
    PT_INIT(&pt3);
    PT_INIT(&pt4);
    PT_INIT(&pt5);
    PT_INIT(&pt6);

    tft.fillScreen(ST77XX_BLACK);
    Serial.println("Setup executed.");
}
void loop() {
  // Execute your protothread(s)
  serialGetTime(&pt1);
  protothreadBlinkLED2(&pt2);
  screenRotation(&pt3);
  showTime(&pt4);
  lockScreen(&pt5);
  readSDCard(&pt6);
  //delay(100);
}