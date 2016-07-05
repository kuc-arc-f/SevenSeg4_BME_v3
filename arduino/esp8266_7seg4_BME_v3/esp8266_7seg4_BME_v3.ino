/*
 weather info, send with esp8266
 BME280 use, UART Data receive
*/
#include <ESP8266WiFi.h>

const char* ssid = "";
const char* password = "";

const char* host = "api.thingspeak.com";
String mTimeStr="";
static uint32_t mTimerTmp;

String mAPI_KEY="your-KEY";

//
void setup() {
  Serial.begin( 9600 );
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());
}

//
//void proc_http(String sTemp, String sHum){
void proc_http(String sTemp, String sHum, String sPress){
//Serial.print("connecting to ");
//Serial.println(host);  
      WiFiClient client;
      const int httpPort = 80;
      if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
      }
//      String url = "/update?key="+ mAPI_KEY + "&field1="+ sTemp +"&field2=" + sHum;    
      String url = "/update?key="+ mAPI_KEY + "&field1="+ sTemp +"&field2=" + sHum +"&field3="+ sPress;    
      //    
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
        "Host: " + host + "\r\n" + 
        "Connection: close\r\n\r\n");
      delay(10);      
      int iSt=0;
      while(client.available()){
          String line = client.readStringUntil('\r');
Serial.print(line);
      }    
}

//
void loop() {
  delay( 100 );
  String sRecv="";
  while( Serial.available()){
      char c = Serial.read();
      sRecv.concat(c);
      if(  sRecv.length() >= 16 ){
          if (millis() > mTimerTmp) {
              mTimerTmp = millis()+ 30000;
              String sHead= sRecv.substring(0,4);
              //if(sHead =="tmp="){
              if(sHead =="dat="){
                  String sTmp = sRecv.substring( 4, 8 );
                  String sHum = sRecv.substring( 8, 12 );
                  String sPres= sRecv.substring( 12, 16 );
  Serial.print("sTmp=");
  Serial.println( sTmp );       
  Serial.print("sHum=");
  Serial.println( sHum );       
  Serial.print("sPres=");
  Serial.println( sPres );       
                  proc_http(sTmp, sHum, sPres);     
                  delay(1000);    
              }               
          }
      }
  } //end_while    
    
}




