#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
boolean wifiStatus;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp2.aliyun.com");
void setup(){
    String ssid = "luzeyu";
    String password = "abcd020124";
    Serial.begin(9600);
    wifiStatus = connectToWifi(ssid, password);
    timeClient.begin();
    timeClient.setTimeOffset(28800);
    delay(5000);
}

void loop(){
    String content = "";
    while (Serial.available() > 0)
    {
        content += char(Serial.read());
        delay(2);
    }
    //Serial.println(".");
    if(content != ""){
        if(content == "Request_TIME" || content == "Request_TIMERequest_TIME"){
            //if(wifiStatus){
                timeClient.update();
                String formattedTime = timeClient.getFormattedTime();
                //Serial.print("Formatted Time: ");
                Serial.println(formattedTime);
                delay(10);
            //}
        }
        else{
            
            Serial.print("Wrong command:");
            Serial.println(content);
        }
    }
    //while(Serial.available() >= 0){
    //    Serial.read();
    //}
    delay(100);
}

boolean connectToWifi(String ssid,String password){
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.print("Connected.");
    return true;
}