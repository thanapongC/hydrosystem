
//include//
#include <ArduinoJson.h>
#include <ESP8266WiFi.h> 
#include <FirebaseArduino.h>
#include <time.h>
#define relay1 D0
#define relay2 D1
#define relay3 D2
#define relay4 D3
#define relay5 D5
#define relay6 D6
#define relay7 D7
#define relay8 D8

int timezone = 7 * 3600;
int dst = 0;
bool PTest = true;
bool Receive = false;
bool StartRX = false;
//end//
//const char* ssid = "TT wifi 2.4 GHz"; //สร้างตัวแปรไว้เก็บชื่อ ssid ของ AP ของเรา
//const char* pass = "thailand"; //สร้างตัวแปรไว้เก็บชื่อ password ของ AP ของเรา

const char* ssid = "ICTES_Lab-2G"; //สร้างตัวแปรไว้เก็บชื่อ ssid ของ AP ของเรา
const char* pass = "officett4321"; //สร้างตัวแปรไว้เก็บชื่อ password ของ AP ของเรา
//***************config***************//
const String deviceNumber = "120";
//***************config***************//

//define//https://basketofgrow.firebaseio.com
#define FIREBASE_HOST "basketofgrow.firebaseio.com"
#define FIREBASE_KEY "CPZ7GRwCunbmhTPXlXtSAynaJxH9BiRSPduqdGte"

unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long SendingMillis = 0;        // will store last time LED was updated
unsigned long interval = 1500;

String ireadString; //main captured String 
float WaterTemp = 0.00;
float EC = 0.00;
float PH = 0.00;
float AirTemp = 0.00;
float AirHumi = 0.00;
String WaterAlarm = "TRUE";


void setup() {
        configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");
      Serial.println("\nLoading time");
              while (!time(nullptr)) {
              Serial.print("*");
              delay(1000);
              }
  
  Serial.begin(9600);     //set aside some bytes for receiving data from Atlas Scientific product
  ireadString.reserve(200);
  //Serial.println("***"); 
  Firebase.begin(FIREBASE_HOST, FIREBASE_KEY);
      WiFi.begin(ssid, pass); //ทำการเชื่อมต่อไปยัง AP

  while (WiFi.status() != WL_CONNECTED) { 
    if(PTest) Serial.print("."); 
    delay(500);
   digitalWrite(relay1,HIGH);
   digitalWrite(relay2,HIGH);
   digitalWrite(relay3,HIGH);
   digitalWrite(relay4,LOW);
   digitalWrite(relay5,LOW);
   digitalWrite(relay6,LOW);
   digitalWrite(relay7,LOW);
   digitalWrite(relay8,LOW);
  } 

  if(PTest)
  {
    Serial.println(""); 
    Serial.println("Wi-Fi connected"); 
    Serial.print("IP Address : ");
    Serial.println(WiFi.localIP()); 
  }

  previousMillis = millis();        // will store last time LED was updated
  SendingMillis = millis();        // will store last time LED was updated
   pinMode(relay1,OUTPUT);
   pinMode(relay2,OUTPUT);
   pinMode(relay3,OUTPUT);
   pinMode(relay4,OUTPUT);
   pinMode(relay5,OUTPUT);
   pinMode(relay6,OUTPUT);
   pinMode(relay7,OUTPUT);
   pinMode(relay8,OUTPUT);
   digitalWrite(relay1,HIGH);
   digitalWrite(relay2,HIGH);
   digitalWrite(relay3,HIGH);
   digitalWrite(relay4,LOW);
   digitalWrite(relay5,LOW);
   digitalWrite(relay6,LOW);
   digitalWrite(relay7,LOW);
   digitalWrite(relay8,LOW);
}

void loop() {
  
     configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");  
     time_t now = time(nullptr);
     struct tm* p_tm = localtime(&now);
  
      String timenowhour = String(p_tm->tm_hour);
    
    if(timenowhour == "0" || timenowhour == "1" || timenowhour == "2" || timenowhour == "3" || timenowhour == "4" ||
       timenowhour == "5" || timenowhour == "6" || timenowhour == "7" || timenowhour == "8" || timenowhour == "9")
    {
     timenowhour = "0"+timenowhour;
    }else{
     timenowhour = timenowhour;
      }
      
    String timenowmin = String(p_tm->tm_min);

    if(timenowmin == "0" || timenowmin == "1" || timenowmin == "2" || timenowmin == "3" || timenowmin == "4" ||
       timenowmin == "5" || timenowmin == "6" || timenowmin == "7" || timenowmin == "8" || timenowmin == "9")
    {
     timenowmin = "0"+timenowmin;
    }else{
     timenowmin = timenowmin;
      }

    String timenowsec = String(p_tm->tm_sec);

    if(timenowsec == "0" || timenowsec == "1" || timenowsec == "2" || timenowsec == "3" || timenowsec == "4" ||
       timenowsec == "5" || timenowsec == "6" || timenowsec == "7" || timenowsec == "8" || timenowsec == "9")
    {
     timenowsec = "0"+timenowsec;
    }else{
     timenowsec = timenowsec;
      }
      
    String timenow = timenowhour+timenowmin+timenowsec;
    int inttimenow = timenow.toInt();
           
    //Code for receive data from arduino signal conditioning that send automatic every 1 second or more
    //expect a string like 1.1,2.1,3.1,4.1,5.1,TRUE,*
    if (Serial.available())
    {
      char c = Serial.read();  //gets one byte from serial buffer
      if (c == '#') { StartRX = true;}
      else if ((StartRX == true) && (c == '*')) {
        //do stuff
        if (PTest) Serial.print(c);
        
        int ind[6] = {0,0,0,0,0,0};
        String tmp = "";
        
        ind[0] = ireadString.indexOf(',');  //finds location of first ,
        tmp = ireadString.substring(0, ind[0]);   //captures WaterTemp data String
        WaterTemp = tmp.toFloat();
        
        ind[1] = ireadString.indexOf(',', ind[0]+1 );   //finds location of second ,
        tmp = ireadString.substring(ind[0]+1, ind[1]);   //captures EC data String
        EC = tmp.toFloat();
  
        ind[2] = ireadString.indexOf(',', ind[1]+1 );   //finds location of second ,
        tmp = ireadString.substring(ind[1]+1, ind[2]);   //captures PH data String
        PH = tmp.toFloat();
        
        ind[3] = ireadString.indexOf(',', ind[2]+1 );   //finds location of second ,
        tmp = ireadString.substring(ind[2]+1, ind[3]);   //captures AirTemp data String
        AirTemp = tmp.toFloat();
        
        ind[4] = ireadString.indexOf(',', ind[3]+1 );   //finds location of second ,
        tmp = ireadString.substring(ind[3]+1, ind[4]);   //captures AirHumi data String
        AirHumi = tmp.toFloat();
        
        ind[5] = ireadString.indexOf(',', ind[4]+1 );   //finds location of second ,
        WaterAlarm = ireadString.substring(ind[4]+1, ind[5]);   //captures WaterAlarm data String "TRUE" or "FALSE" by "TRUE" mean not found water
  
        if(PTest)
        {
          // Report data 
          // comment if not monitor
          Serial.println();
          Serial.print("captured String is : "); 
          Serial.println(ireadString); //prints string to serial port out
   
          Serial.print("WaterTemp = ");
          Serial.println(WaterTemp); 
          Serial.print("EC = ");
          Serial.println(EC); 
          Serial.print("PH = ");
          Serial.println(PH); 
          Serial.print("AirTemp = ");
          Serial.println(AirTemp); 
          Serial.print("AirHumi = ");
          Serial.println(AirHumi); 
          Serial.print("WaterAlarm = ");
          Serial.println(WaterAlarm); 
          Serial.println();
        }
        
        ireadString=""; //clears variable for new input
        StartRX = false;
        Receive = true;
        previousMillis = millis() + interval;
      //end
      }  
      else if (StartRX){     
        ireadString += c; //makes the string readString
      }
    }

  if (!StartRX && Receive && (millis() - SendingMillis >= 1000)) {
        // save the last time you blinked the LED
        
      //Data JSON object (send)
         StaticJsonBuffer<200> jsonBuffer;
         JsonObject& valueObject = jsonBuffer.createObject();
         valueObject["AirTemp"] = AirTemp;
         valueObject["AirHumid"] = AirHumi;
         valueObject["WaterTemp"] = WaterTemp;
         valueObject["PHvalue"] = PH;
         valueObject["ECvalue"] = EC;
         valueObject["WaterAlarm"] = WaterAlarm;
         delay(10000);
       //end
  
      //pust data to fisebase (object)
        Firebase.set(deviceNumber + "/value/", valueObject);
        if(PTest)
        {
          // Report data 
          // comment if not monitor
          Serial.print("pust data to fisebase : ");
          Serial.println(deviceNumber);
        }
        //Initial to get new data
        ireadString=""; //clears variable for new input
        StartRX = false;
        Receive = false;
        while(Serial.available()){Serial.read();}
        SendingMillis = millis();
  }
      // Main get relay status here 
      
      if (!StartRX && (millis() - previousMillis >= interval)) {
        // save the last time you blinked the LED
        
        String relay220v1 = Firebase.getString(deviceNumber + "/device/relay220v1");
        String relay220v2 = Firebase.getString(deviceNumber + "/device/relay220v2");
        String relay220v3 = Firebase.getString(deviceNumber + "/device/relay220v3");
        String relay220v4 = Firebase.getString(deviceNumber + "/device/relay220v4");
        String relay12v1 = Firebase.getString(deviceNumber + "/device/relay12v1");
        String relay12v2 = Firebase.getString(deviceNumber + "/device/relay12v2");
        String relay5v1 = Firebase.getString(deviceNumber + "/device/relay5v1");
        String relay5v2 = Firebase.getString(deviceNumber + "/device/relay5v2");
        String switchselect = Firebase.getString(deviceNumber + "/switchselect");
        String startrelay220v1 = Firebase.getString(deviceNumber + "/device/relay220v1/start");
        int intstartrelay220v1 = startrelay220v1.toInt();
        String startrelay220v2 = Firebase.getString(deviceNumber + "/device/relay220v2/start");
        int intstartrelay220v2 = startrelay220v2.toInt();
        String startrelay220v3 = Firebase.getString(deviceNumber + "/device/relay220v3/start");
        int intstartrelay220v3 = startrelay220v3.toInt();
        String startrelay220v4 = Firebase.getString(deviceNumber + "/device/relay220v4/start");
        int intstartrelay220v4 = startrelay220v4.toInt();
        String startrelay12v1 = Firebase.getString(deviceNumber + "/device/relay12v1/start");
        int intstartrelay12v1 = startrelay12v1.toInt();
        String startrelay12v2 = Firebase.getString(deviceNumber + "/device/relay12v2/start");
        int intstartrelay12v2 = startrelay12v2.toInt();
        String startrelay5v1 = Firebase.getString(deviceNumber + "/device/relay5v1/start");
        int intstartrelay5v1 = startrelay5v1.toInt();
        String startrelay5v2 = Firebase.getString(deviceNumber + "/device/relay5v2/start");
        int intstartrelay5v2 = startrelay5v2.toInt();
        String endrelay220v1 = Firebase.getString(deviceNumber + "/device/relay220v1/end");
        int intendrelay220v1 = endrelay220v1.toInt();
        String endrelay220v2 = Firebase.getString(deviceNumber + "/device/relay220v2/end");
        int intendrelay220v2 = endrelay220v2.toInt();
        String endrelay220v3 = Firebase.getString(deviceNumber + "/device/relay220v3/end");
        int intendrelay220v3 = endrelay220v3.toInt();
        String endrelay220v4 = Firebase.getString(deviceNumber + "/device/relay220v4/end");
        int intendrelay220v4 = endrelay220v4.toInt();
        String endrelay12v1 = Firebase.getString(deviceNumber + "/device/relay12v1/end");
        int intendrelay12v1 = endrelay12v1.toInt();
        String endrelay12v2 = Firebase.getString(deviceNumber + "/device/relay12v2/end");
        int intendrelay12v2 = endrelay12v2.toInt();
        String endrelay5v1 = Firebase.getString(deviceNumber + "/device/relay5v1/end");
        int intendrelay5v1 = endrelay5v1.toInt();
        String endrelay5v2 = Firebase.getString(deviceNumber + "/device/relay5v2/end");
        int intendrelay5v2 = endrelay5v2.toInt();

        if(PTest)
        {
          // Report data 
          // comment if not monitor
          Serial.print("relay220v1 = ");
          Serial.println(relay220v1); 
          Serial.print("relay220v2 = ");
          Serial.println(relay220v2); 
          Serial.print("relay220v3 = ");
          Serial.println(relay220v3); 
          Serial.print("relay220v4 = ");
          Serial.println(relay220v4); 
          Serial.print("relay12v1 = ");
          Serial.println(relay12v1); 
          Serial.print("relay12v2 = ");
          Serial.println(relay12v2); 
          Serial.print("relay5v1 = ");
          Serial.println(relay5v1); 
          Serial.print("relay5v2 = ");
          Serial.println(relay5v2); 
          Serial.println();
        }

        
  if(switchselect == "manual"){

    Serial.println("Manual-System");
    
   if(relay220v1 == "on"){
    digitalWrite(relay1,HIGH);
   }else{
    digitalWrite(relay1,LOW);
    }
   if(relay220v2 == "on"){
    digitalWrite(relay2,HIGH);
    }else{
    digitalWrite(relay2,LOW); 
    } 
     if(relay220v3 == "on"){
    digitalWrite(relay3,HIGH);
    }else{
    digitalWrite(relay3,LOW); 
    } 
     if(relay220v4 == "on"){
    digitalWrite(relay4,HIGH);
    }else{
    digitalWrite(relay4,LOW); 
    } 
     if(relay12v1 == "on"){
    digitalWrite(relay5,HIGH);
    }else{
    digitalWrite(relay5,LOW); 
    } 
    if(relay12v2 == "on"){
    digitalWrite(relay6,HIGH);
    }else{
    digitalWrite(relay6,LOW); 
    } 
     if(relay5v1 == "on"){
    digitalWrite(relay7,HIGH);
    }else{
    digitalWrite(relay7,LOW); 
    } 
       if(relay5v2 == "on"){
    digitalWrite(relay8,HIGH);
    }else{
    digitalWrite(relay8,LOW); 
    } 
    
   }

   else if(switchselect == "time"){

    Serial.println("Time-System");

       if(intstartrelay220v1 <= inttimenow && intendrelay220v1 >= inttimenow){
        
        digitalWrite(relay1,HIGH);
        }
      else{
        digitalWrite(relay1,LOW);
        }  

       if(intstartrelay220v2 <= inttimenow && intendrelay220v2 >= inttimenow){
        
        digitalWrite(relay2,HIGH);
        }
      else{
        digitalWrite(relay2,LOW);
        }  

       if(intstartrelay220v3 <= inttimenow && intendrelay220v3 >= inttimenow){
        
        digitalWrite(relay3,HIGH);
        }
      else{
        digitalWrite(relay3,LOW);
        }  

      if(intstartrelay220v4 <= inttimenow && intendrelay220v4 >= inttimenow){
        
        digitalWrite(relay4,HIGH);
        }
      else{
        digitalWrite(relay4,LOW);
        }  

      if(intstartrelay12v1 <= inttimenow && intendrelay12v1 >= inttimenow){
        
        digitalWrite(relay5,HIGH);
        }
      else{
        digitalWrite(relay5,LOW);
        }  

       if(intstartrelay12v2 <= inttimenow && intendrelay12v2 >= inttimenow){
        
        digitalWrite(relay6,HIGH);
        }
      else{
        digitalWrite(relay6,LOW);
        }  

       if(intstartrelay5v1 <= inttimenow && intendrelay5v1 >= inttimenow){
        
        digitalWrite(relay7,HIGH);
        }
      else{
        digitalWrite(relay7,LOW);
        }  

      if(intstartrelay5v2 <= inttimenow && intendrelay5v2 >= inttimenow){
        
        digitalWrite(relay8,HIGH);
        }
      else{
        digitalWrite(relay8,LOW);
        }  
    
    }

   else if(switchselect == "auto"){

    Serial.println("Auto-System");
    
      if(06000 <= inttimenow && 180000 >= inttimenow){
        
        digitalWrite(relay1,HIGH);
        }
      else{
        digitalWrite(relay1,LOW);
        }  
        digitalWrite(relay2,HIGH);
        digitalWrite(relay3,HIGH);
        digitalWrite(relay4,LOW);
        digitalWrite(relay5,LOW);
        digitalWrite(relay6,LOW);
        digitalWrite(relay7,LOW);
        digitalWrite(relay8,LOW);
       
   }else{
   digitalWrite(relay1,LOW);
   digitalWrite(relay2,LOW);
   digitalWrite(relay3,LOW);
   digitalWrite(relay4,LOW);
   digitalWrite(relay5,LOW);
   digitalWrite(relay6,LOW);
   digitalWrite(relay7,LOW);
   digitalWrite(relay8,LOW);
    }
   
            
        //Initial to get new data
        ireadString=""; //clears variable for new input
        StartRX = false;
        while(Serial.available()){Serial.read();}
        previousMillis = millis();
      }
}
