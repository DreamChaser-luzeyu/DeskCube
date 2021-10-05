#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
boolean wifiStatus;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "cn.ntp.org.cn");
void setup(){
    String ssid = "luzeyu";
    String password = "abcd020124";
    Serial.begin(9600);
    wifiStatus = connectToWifi(ssid, password);
    timeClient.begin();
    timeClient.setTimeOffset(28800);
}

void loop(){
    if(wifiStatus){
        timeClient.update();
        String formattedTime = timeClient.getFormattedTime();
        Serial.print("Formatted Time: ");
        Serial.println(formattedTime + "@");
        delay(500);
    }
    else{
        //wifiStatus = connectToWifi(ssid, password);
    }
}

boolean connectToWifi(String ssid,String password){
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    return true;
}
