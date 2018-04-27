/********************************************************************/
// First we include the libraries
/*
  Software serial multple serial test

 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)

 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Not all pins on the Leonardo and Micro support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example

 This example code is in the public domain.

 */
#include <SoftwareSerial.h>
#define rx 8                                          //define what pin rx is going to be
#define tx 9                                          //define what pin tx is going to be

SoftwareSerial mySerial(rx, tx);                      //define how the soft serial port is going to work

bool PTest = false;
bool StartRX = false;

#include <OneWire.h> 
#include <DallasTemperature.h>
// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"

#define DHTPIN 2     // what digital pin we're connected to

//Use A1 as digital input for Non-contact Liquid Level Switch
#define NCLPIN A1     // what digital pin we're connected to

/*
 # This sample code is used to test the pH meter V1.0.
 # Editor : YouYou
 # Ver    : 1.0
 # Product: analog pH meter
 # SKU    : SEN0161
*/
#define SensorPin A0            //pH meter Analog output to Arduino Analog Input 0
#define Offset -0.74            //deviation compensate
//#define LED 13
#define samplingInterval 20
#define ArrayLenth  40    //times of collection
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;    

/********************************************************************/
// Data wire is plugged into pin 10 on the Arduino 
#define ONE_WIRE_BUS 3 

/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 

/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
/********************************************************************/ 
/*
Please note as I have used the android app you told , the string inputs are taken as 
A,B,C,D,E,F,G and a,b,c,d,e,f,g.
I have also written code for voice on and off which has all on and all off function
but since the android app we are using dosent have such key so i didnt took them
*/
String inputs;
#define relay1 13 //Connect relay1 to pin 9
#define relay2 12 //Connect relay2 to pin 8
#define relay3 11 //Connect relay3 to pin 7
#define relay4 10 //Connect relay4 to pin 6
#define relay5 7 //Connect relay5 to pin 5
#define relay6 6 //Connect relay6 to pin 4
#define relay7 5 //Connect relay7 to pin 3
#define relay8 4 //Connect relay8 to pin 2

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 500;           // interval at which to blink (milliseconds)

String ireadString = "";
//String inputstring = "";                              //a string to hold incoming data from the PC
String sensorstring = "";                             //a string to hold the data from the Atlas Scientific product
//boolean input_string_complete = false;                //have we received all the data from the PC
boolean sensor_string_complete = false;               //have we received all the data from the Atlas Scientific product
float F_EC = 0.0;
int EC_Count = 0;
String NCLStr = "FALSE";
void setup(void) 
{ 
  // start serial port 
  Serial.begin(9600); 
  if(PTest) Serial.println("Smart Hydro Signal Conditioning V1.0."); 
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  //mySerial.println("SignalConStart");
  //inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
  sensorstring.reserve(100);                           //set aside some bytes for receiving data from Atlas Scientific product
  ireadString.reserve(200);
  
  pinMode(relay1, OUTPUT); //Set relay1 as an output
  pinMode(relay2, OUTPUT); //Set relay2 as an output
  pinMode(relay3, OUTPUT); //Set relay1 as an output
  pinMode(relay4, OUTPUT); //Set relay2 as an output
  pinMode(relay5, OUTPUT); //Set relay1 as an output
  pinMode(relay6, OUTPUT); //Set relay2 as an output
  pinMode(relay7, OUTPUT); //Set relay1 as an output
  pinMode(relay8, OUTPUT); //Set relay2 as an output
 digitalWrite(relay1, LOW); //Switch relay1 off
        digitalWrite(relay2, LOW); //Swtich relay2 off
        digitalWrite(relay3, LOW); //Switch relay1 off
        digitalWrite(relay4, LOW); //Swtich relay2 off
        digitalWrite(relay5, LOW); //Switch relay1 off
        digitalWrite(relay6, LOW); //Swtich relay2 off
        digitalWrite(relay7, LOW); //Switch relay1 off
        digitalWrite(relay8, LOW); //Swtich relay2 off
       
  
  pinMode(NCLPIN, INPUT); //Set Non-contact Liquid Level Switch as an input
  digitalWrite(NCLPIN, INPUT_PULLUP);  // set pull-up on Non-contact Liquid Level Switch 
  
  // Start up the library 
  sensors.begin(); 
  
  dht.begin();

  mySerial.print("C,1\r\n");                      //send that string to the Atlas Scientific product
  //mySerial.print('\r'); 
  if(PTest) Serial.println("Send C,1\\r\\n to EC"); 
} 
void loop(void) 
{ static unsigned long samplingTime = millis();
  //unsigned long currentMillis = millis();
  static float pHValue,voltage;

  //PH Sensor http://www.thaieasyelec.com/products/sensors/ph-d-o-orp/analog-ph-meter-kit-detail.html
  if(millis()-samplingTime > samplingInterval)
  {
      pHArray[pHArrayIndex++]=analogRead(SensorPin);
      if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;
      voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;
      pHValue = 3.5*voltage+Offset;
      samplingTime=millis();
  }

  //EC Sensor https://www.atlas-scientific.com/product_pages/kits/ec_k1_0_kit.html
  if (mySerial.available() > 0) 
  {                     //if we see that the Atlas Scientific product has sent a character
    char inchar = (char)mySerial.read();              //get the char we just received
    sensorstring += inchar;                           //add the char to the var called sensorstring
    if (inchar == '\r') {                             //if the incoming character is a <CR>
      sensor_string_complete = true;                  //set the flag
    }
  }

  if (sensor_string_complete == true) {               //if a string from the Atlas Scientific product has been received in its entirety
    if (isdigit(sensorstring[0]) != false) {          //if the first character in the string is a digit
      Get_EC_data();                                //then call this function 
    }
    if(PTest) Serial.print("EC Receive: ");                   //send that string to the PC's serial monitor
    if(PTest) Serial.println(sensorstring);                   //send that string to the PC's serial monitor
    sensorstring = "";                                //clear the string
    sensor_string_complete = false;                   //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
  }


  // Relay Control Receive
  //      RelayNum is relay number channel 1 - 8
  //      RStatus is relay status when 0 = no drive relay and 1 = drive relay
  // Example:
  // "#R,1,S,0,**"; // no drive relay 1 
  // "#R,2,S,1,**"; // drive relay 2 
  // "#R,3,S,1,**"; // drive relay 3 
  // "#R,4,S,0,**"; // no drive relay 4 
    if (Serial.available())
    {
      char c = Serial.read();  //gets one byte from serial buffer
      if (c == '#') { StartRX = true;}
      else if ((StartRX == true) && (c == '*')) 
      {
        //do stuff
        
        int ind[4] = {0,0,0,0}, RelayN = 0, Status = 2;
        String tmp = "";
        
        ind[0] = ireadString.indexOf(',');  //finds location of first ,
        tmp = ireadString.substring(0, ind[0]);   //captures 'R' data String
        
        ind[1] = ireadString.indexOf(',', ind[0]+1 );   //finds location of second ,
        tmp = ireadString.substring(ind[0]+1, ind[1]);   //captures Relay data String
        RelayN = tmp.toInt();
      
        ind[2] = ireadString.indexOf(',', ind[1]+1 );   //finds location of second ,
        tmp = ireadString.substring(ind[1]+1, ind[2]);   //captures 'S' data String
        
        ind[3] = ireadString.indexOf(',', ind[2]+1 );   //finds location of second ,
        tmp = ireadString.substring(ind[2]+1, ind[3]);   //captures Status data String
        Status = tmp.toInt();
  
        //Serial.print(RelayN);
        //Serial.println(Status);
        
        if (RelayN == 1)
        {
          if (Status == 0) digitalWrite(relay1, LOW); //Switch relay1 off
          else digitalWrite(relay1, HIGH); //Switch relay1 on
        }
        else if (RelayN == 2)
        {
          if (Status == 0) digitalWrite(relay2, LOW); //Switch relay2 off
          else digitalWrite(relay2, HIGH); //Switch relay2 on
        }
        else if (RelayN == 3)
        {
          if (Status == 0) digitalWrite(relay3, LOW); //Switch relay3 off
          else digitalWrite(relay3, HIGH); //Switch relay3 on
        }
        else if (RelayN == 4)
        {
          if (Status == 0) digitalWrite(relay4, LOW); //Switch relay4 off
          else digitalWrite(relay4, HIGH); //Switch relay4 on
        }
        else if (RelayN == 5)
        {
          if (Status == 0) digitalWrite(relay5, LOW); //Switch relay5 off
          else digitalWrite(relay5, HIGH); //Switch relay5 on
        }
        else if (RelayN == 6)
        {
          if (Status == 0) digitalWrite(relay6, LOW); //Switch relay6 off
          else digitalWrite(relay6, HIGH); //Switch relay6 on
        }
        else if (RelayN == 7)
        {
          if (Status == 0) digitalWrite(relay7, LOW); //Switch relay7 off
          else digitalWrite(relay7, HIGH); //Switch relay7 on
        }
        else if (RelayN == 8)
        {
          if (Status == 0) digitalWrite(relay8, LOW); //Switch relay8 off
          else digitalWrite(relay8, HIGH); //Switch relay8 on
        }
        
        // Report data 
        // comment if not monitor
        if(PTest) 
        {
          Serial.println();
          Serial.print("captured String is : "); 
          Serial.println(ireadString); //prints string to serial port out
          
          Serial.print("Relay = ");
          Serial.println(RelayN); 
          Serial.print("Status = ");
          Serial.println(Status);  
          Serial.println();
        }
        
        ireadString=""; //clears variable for new input
        StartRX = false;
  
        previousMillis = millis();
      }  
      else if (StartRX)
      {     
        ireadString += c; //makes the string readString
      }
    }
   
    //DS1820 , DHT11 and 
    // WaterAlarm Sensor http://www.thaieasyelec.com/products/sensors/liquid-air-level-flow/non-contact-liquid-level-switch-detail.html
    if (!StartRX && (millis() - previousMillis >= interval)) {
      // save the last time you blinked the LED
      previousMillis = millis();
      if (EC_Count++ >5)
      {
        EC_Count = 0;
        mySerial.print("C,1\r\n");                      //send that string to the Atlas Scientific product
        //mySerial.print('\r\n'); 
        if(PTest) Serial.println("Send C,1\\r\\n to EC"); 
      }
      // call sensors.requestTemperatures() to issue a global temperature 
      // request to all devices on the bus 
      /********************************************************************/
      if(PTest) Serial.print(" Requesting temperatures..."); 
      sensors.requestTemperatures(); // Send the command to get temperature readings 
      if(PTest) Serial.println("DONE"); 
      /********************************************************************/
      if(PTest) Serial.print("Temperature is: "); 
      if(PTest) Serial.println(sensors.getTempCByIndex(0)); // Why "byIndex"?  
      // You can have more than one DS18B20 on the same bus.  
      // 0 refers to the first IC on the wire 
  
      
      if(PTest) 
      {
        Serial.print("Voltage:");
        Serial.print(voltage,2);
        Serial.print("    pH value: ");
        Serial.println(pHValue,2);
      }
      // digitalWrite(LED,digitalRead(LED)^1);
  
      // Reading temperature or humidity takes about 250 milliseconds!
      // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
      float h = dht.readHumidity();
      // Read temperature as Celsius (the default)
      float t = dht.readTemperature();
      // Read temperature as Fahrenheit (isFahrenheit = true)
      float f = dht.readTemperature(true);
    
      // Check if any reads failed and exit early (to try again).
      if (isnan(h) || isnan(t) || isnan(f)) {
        if(PTest) Serial.println("Failed to read from DHT sensor!");
      }
      
      if (isnan(h)) h = 0.0;
      if (isnan(t)) t = 0.0;
      if (isnan(f)) f = 0.0;
      // Compute heat index in Fahrenheit (the default)
      //float hif = dht.computeHeatIndex(f, h);
      // Compute heat index in Celsius (isFahreheit = false)
      //float hic = dht.computeHeatIndex(t, h, false);
  
      //read the switch value into a variable
      int sensorVal = digitalRead(NCLPIN);
      
      //print out the value of the liquid level
      if(PTest) Serial.println(sensorVal);
      if (sensorVal == 1) 
      {
        // Found water not alarm
        NCLStr = "FALSE";
      }
      else 
      {
        //Not found water then alarm
        NCLStr = "TRUE";
      }
      if(PTest) Serial.println(NCLStr);
      
    
      Serial.print("#");
      Serial.print(sensors.getTempCByIndex(0));
      Serial.print(",");
      Serial.print(F_EC);
      Serial.print(",");
      Serial.print(pHValue);
      Serial.print(",");
      Serial.print(t);
      Serial.print(",");
      Serial.print(h);
      Serial.print(",");
      Serial.print(NCLStr); // Add WaterAlarm "TRUE" or "FALSE"
      Serial.print(",*");
      Serial.print(digitalRead(relay1)); // relay list
      Serial.print(digitalRead(relay2)); // relay list
      Serial.print(digitalRead(relay3)); // relay list
      Serial.print(digitalRead(relay4)); // relay list
      Serial.print(digitalRead(relay5)); // relay list
      Serial.print(digitalRead(relay6)); // relay list
      Serial.print(digitalRead(relay7)); // relay list
      Serial.println(digitalRead(relay8)); // relay list

      ireadString=""; //clears variable for new input
      StartRX = false;
    } 
}

void Get_EC_data(void) {                            //this function will pars the string  

  char sensorstring_array[30];                        //we make a char array
  char *EC;                                           //char pointer used in string parsing
  char *TDS;                                          //char pointer used in string parsing
  char *SAL;                                          //char pointer used in string parsing
  char *GRAV;                                         //char pointer used in string parsing
  float f_ec;                                         //used to hold a floating point number that is the EC
  
  sensorstring.toCharArray(sensorstring_array, 30);   //convert the string to a char array 
  EC = strtok(sensorstring_array, ",");               //let's pars the array at each comma
  TDS = strtok(NULL, ",");                            //let's pars the array at each comma
  SAL = strtok(NULL, ",");                            //let's pars the array at each comma
  GRAV = strtok(NULL, ",");                           //let's pars the array at each comma

  if(PTest)
  {
    Serial.print("EC:");                                //we now print each value we parsed separately
    Serial.println(EC);                                 //this is the EC value
  
    Serial.print("TDS:");                               //we now print each value we parsed separately
    Serial.println(TDS);                                //this is the TDS value
  
    Serial.print("SAL:");                               //we now print each value we parsed separately
    Serial.println(SAL);                                //this is the salinity value
  
    Serial.print("GRAV:");                              //we now print each value we parsed separately
    Serial.println(GRAV);                               //this is the specific gravity
    Serial.println();                                   //this just makes the output easier to read
  }
  F_EC= atof(EC);                                     //uncomment this line to convert the char to a float
}

double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    //Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}
