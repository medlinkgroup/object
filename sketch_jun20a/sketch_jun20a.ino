#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <math.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <stdio.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 0

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

const char* ssid = "VM"; 
//const char* ssid = "iPhone"; 
//const char* ssid = "Fabiana Montiel"; 
//replace with your own wifi ssid 
const char* password = "mchagall";
//const char* password = "cici97ci";  
//const char* password = "fmfmfmfm";  
//replace with your own //wifi ssid password 
const char* host = "https://medlinkapi.herokuapp.com/";
const char* hostTime = "http://api.timezonedb.com/v2/get-time-zone?key=VYIBB3N5KKEM&format=xml&fields=formatted&by=zone&zone=Europe/Berlin";

String payloadTime;               // Variables to accept data
String nowday;
String nowmonth;
String nowyear;

unsigned long prevMillis = millis();
unsigned long interval = 60000;                 // Refresh in loop every 60 seconds

int count = 0;

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

String sensorReadings;
float sensorReadingsArr[3];

const int BLU = 2;
const int YEL =  14;// the number of the LED pin
const int GRE = 12;

int ledState = LOW;

const int button = 13;

long onTime = 0;

int lastReading = LOW;
int bounceTime = 50;
int holdTime = 250;
int hold = 0;
int single = 0;
int singlePress = 0;
int doublePress = 0;

long lastSwitchTime = 0;
long doubleTime = 500;


/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(1);

String DEVICE_ID = "5ef65dca0169920017cca3b8";
 
void displaySensorDetails(void)
{
 sensor_t sensor;
 accel.getSensor(&sensor);
 Serial.println("------------------------------------");
 Serial.print ("Sensor: "); Serial.println(sensor.name);
 Serial.print ("Driver Ver: "); Serial.println(sensor.version);
 Serial.print ("Unique ID: "); Serial.println(sensor.sensor_id);
 Serial.print ("Max Value: "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
 Serial.print ("Min Value: "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
 Serial.print ("Resolution: "); Serial.print(sensor.resolution); Serial.println(" m/s^2"); 
 Serial.println("------------------------------------");
 Serial.println("");
 delay(500);
}
 
void displayDataRate(void)
{
 Serial.print ("Data Rate: "); 
 
 switch(accel.getDataRate())
 {
 case ADXL345_DATARATE_3200_HZ:
 Serial.print ("3200 "); 
 break;
 case ADXL345_DATARATE_1600_HZ:
 Serial.print ("1600 "); 
 break;
 case ADXL345_DATARATE_800_HZ:
 Serial.print ("800 "); 
 break;
 case ADXL345_DATARATE_400_HZ:
 Serial.print ("400 "); 
 break;
 case ADXL345_DATARATE_200_HZ:
 Serial.print ("200 "); 
 break;
 case ADXL345_DATARATE_100_HZ:
 Serial.print ("100 "); 
 break;
 case ADXL345_DATARATE_50_HZ:
 Serial.print ("50 "); 
 break;
 case ADXL345_DATARATE_25_HZ:
 Serial.print ("25 "); 
 break;
 case ADXL345_DATARATE_12_5_HZ:
 Serial.print ("12.5 "); 
 break;
 case ADXL345_DATARATE_6_25HZ:
 Serial.print ("6.25 "); 
 break;
 case ADXL345_DATARATE_3_13_HZ:
 Serial.print ("3.13 "); 
 break;
 case ADXL345_DATARATE_1_56_HZ:
 Serial.print ("1.56 "); 
 break;
 case ADXL345_DATARATE_0_78_HZ:
 Serial.print ("0.78 "); 
 break;
 case ADXL345_DATARATE_0_39_HZ:
 Serial.print ("0.39 "); 
 break;
 case ADXL345_DATARATE_0_20_HZ:
 Serial.print ("0.20 "); 
 break;
 case ADXL345_DATARATE_0_10_HZ:
 Serial.print ("0.10 "); 
 break;
 default:
 Serial.print ("???? "); 
 break;
 } 
 Serial.println(" Hz"); 
}
 
void displayRange(void)
{
 Serial.print ("Range: +/- "); 
 
 switch(accel.getRange())
 {
 case ADXL345_RANGE_16_G:
 Serial.print ("16 "); 
 break;
 case ADXL345_RANGE_8_G:
 Serial.print ("8 "); 
 break;
 case ADXL345_RANGE_4_G:
 Serial.print ("4 "); 
 break;
 case ADXL345_RANGE_2_G:
 Serial.print ("2 "); 
 break;
 default:
 Serial.print ("?? "); 
 break;
 } 
 Serial.println(" g"); 
}

float tempValueArray[10];
float accelerometerXArray[10];
float accelerometerYArray[10];
float accelerometerZArray[10];
 
void setup(void) 
{
 Serial.begin(115200);
   sensors.begin();

 delay(10);  

  pinMode(YEL, OUTPUT);
  pinMode(GRE, OUTPUT);
  pinMode(BLU, OUTPUT);

 pinMode(button, INPUT);
 
 /* Initialise the sensor */
 if(!accel.begin())
 {
 /* There was a problem detecting the ADXL345 ... check your connections */
 Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
 while(1);
 }

 /* Set the range to whatever is appropriate for your project */
 accel.setRange(ADXL345_RANGE_16_G);
 
 /* Display some basic information on this sensor */
 displaySensorDetails();
 
 /* Display additional settings (outside the scope of sensor_t) */
 displayDataRate();
 displayRange();
 Serial.println("");

  WiFi.begin(ssid, password);
  Serial.println("Connecting");

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(BLU, HIGH);
 
  for (byte i = 0; i < 10; i = i + 1) {
    tempValueArray[i] = 0;
  } 
  
  for (byte i = 0; i < 10; i = i + 1) {
    accelerometerXArray[i] = 0;
    accelerometerYArray[i] = 0;
    accelerometerZArray[i] = 0;
  } 
}


  
 void onRelease() {
    
     if ((millis() - lastSwitchTime) >= doubleTime) {
       single = 1;
       lastSwitchTime = millis();
       return;
     }  
    
     if ((millis() - lastSwitchTime) < doubleTime) {
       Serial.println("double press");
       single = 0;
       lastSwitchTime = millis();
     }    
 }


void loop(void) 
{ 
  
  sensors.requestTemperatures(); // Send the command to get temperatures
  float temperature_celcius = sensors.getTempCByIndex(0);
  Serial.print("Temperature: ");
  Serial.println(temperature_celcius);  
  
 /* Get a new sensor event */ 
 sensors_event_t event; 
 accel.getEvent(&event);
 
 /* Display the results (acceleration is measured in m/s^2) */
 Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print(" ");
 Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print(" ");
 Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print(" ");Serial.println("m/s^2 ");


int lenTEMP = 10;
int lenACC = 10;


  for (int i = 0; i < lenTEMP; i = i + 1) {
    if(i < lenTEMP - 1) {
        tempValueArray[i] = tempValueArray[i+1];
      } else if (i == lenTEMP - 1) {
          tempValueArray[i] = temperature_celcius;
        }
   } 

  for (int i = 0; i < lenACC; i = i + 1) {
    if(i< lenACC - 1) {
        accelerometerXArray[i] = accelerometerXArray[i+1];
        accelerometerYArray[i] = accelerometerYArray[i+1];
        accelerometerZArray[i] = accelerometerZArray[i+1];
      } else if (i == lenACC - 1) {
        accelerometerXArray[i] = event.acceleration.x;
        accelerometerYArray[i] = event.acceleration.y;
        accelerometerZArray[i] = event.acceleration.z;
        }
   } 



     int reading = digitalRead(button);
    
     if (reading == HIGH && lastReading == LOW) {
       onTime = millis();
     }
    
    //held
     if (reading == HIGH && lastReading == HIGH) {
       if ((millis() - onTime) > holdTime) {
             hold = 1;
       }
     }
     
     //released
     if (reading == LOW && lastReading == HIGH) {
       if (((millis() - onTime) > bounceTime) && hold != 1) {
         onRelease();
       }
       if (hold = 1) {
         hold = 0;
       }  
     }
     lastReading = reading;
    
     if (single == 1 && (millis() - lastSwitchTime) > doubleTime) {
       Serial.println("single press");
       single = 0;


                  if ((WiFi.status() == WL_CONNECTED) ) { //Check WiFi connection status
                  
                    int httpCode = 0;                                        // Variable to hold received data
                    HTTPClient http;                                          // Declare an object of class HTTPClient
                   
                    Serial.println("Connecting to TimezoneDB...");
                  
                    http.begin(hostTime);                                        // Connect to site
                    httpCode = http.GET();                               // Check if data is coming in
                  
                    while (httpCode == 0) {                             // if no data is in
                      delay(1000);                                           // wait a sec
                      http.begin(hostTime);                                     // and try again
                      httpCode = http.GET();
                    }
                  
                    payloadTime = http.getString();                        // Save response as string
                    Serial.println(payloadTime);                              // Show response
                   
                    http.end();    // Close connection to timezonedb
                  
                  
                    int colon = payloadTime.indexOf(':');                 // Set the first colon in time as reference point
                  
                    String DATE = payloadTime.substring(colon - 13, colon - 3) + "T00:00:00.000Z";
                    
                    for (byte i = 0; i < 10; i = i + 1) {
                      Serial.println(tempValueArray[i]);
                    } 
                    for (byte i = 0; i < 10; i = i + 1) {
                      Serial.print("X: "); Serial.print(accelerometerXArray[i]); Serial.print(" ");
                      Serial.print("Y: "); Serial.print(accelerometerYArray[i]); Serial.print(" ");
                      Serial.print("Z: "); Serial.print(accelerometerZArray[i]); Serial.print(" ");
                      Serial.println("m/s^2 ");
                    } 
                  
                      StaticJsonBuffer<500> JSONbufferTEMP;   //Declaring static JSON buffer
                      JsonObject& JSONencoderTEMP = JSONbufferTEMP.createObject(); 
                  
                      JSONencoderTEMP["deviceID"] = DEVICE_ID;
                      JSONencoderTEMP["date"] = DATE;
                  
                      JsonArray& tempValues = JSONencoderTEMP.createNestedArray("tempValues"); //JSON array
                      for (int i = 0; i < 10; i++) {
                        tempValues.add(tempValueArray[i]); //Add value to array
                      }
                  
                      char JSONmessageBufferTEMP[500];
                      JSONencoderTEMP.prettyPrintTo(JSONmessageBufferTEMP, sizeof(JSONmessageBufferTEMP));
                      Serial.println(JSONmessageBufferTEMP);
                      HTTPClient httpTEMP; //Declare object of class HTTPClient
                      httpTEMP.begin("http://medlinkapi.herokuapp.com/temperatures");      //Specify request destination
                      httpTEMP.addHeader("Content-Type", "application/json");  //Specify content-type header    
                      int httpCodeTEMP = httpTEMP.POST(JSONmessageBufferTEMP);   //Send the request
                      String payloadTEMP = httpTEMP.getString();                 //Get the response payload
                      Serial.println("httpTEMPCode: ");   //Print HTTP return code
                      Serial.println(httpCodeTEMP);   //Print HTTP return code
                      Serial.println("payloadTEMP : ");    //Print request response payload
                      Serial.println(payloadTEMP);    //Print request response payload   
                      httpTEMP.end();  //Close connection 
                      if(httpCodeTEMP == 200){
                         digitalWrite(GRE, HIGH);
                      } 
                  
                      StaticJsonBuffer<700> JSONbufferACC;   //Declaring static JSON buffer
                      JsonObject& JSONencoderACC = JSONbufferACC.createObject(); 
                   
                      JSONencoderACC["deviceID"] = DEVICE_ID;
                      JSONencoderACC["date"] = DATE;
                  
                      JsonArray& accelerometerXValues = JSONencoderACC.createNestedArray("accelerometerXValues"); //JSON array
                      for (int i = 0; i < 10; i++) {
                        accelerometerXValues.add(accelerometerXArray[i]); //Add value to array
                      }
                  
                      JsonArray& accelerometerYValues = JSONencoderACC.createNestedArray("accelerometerYValues"); //JSON array
                      for (int i = 0; i < 10; i++) {
                        accelerometerYValues.add(accelerometerYArray[i]); //Add value to array
                      }
                  
                      JsonArray& accelerometerZValues = JSONencoderACC.createNestedArray("accelerometerZValues"); //JSON array
                      for (int i = 0; i < 10; i++) {
                        accelerometerZValues.add(accelerometerZArray[i]); //Add value to array
                      }
                  
                      char JSONmessageBufferACC[700];
                      JSONencoderACC.prettyPrintTo(JSONmessageBufferACC, sizeof(JSONmessageBufferACC));
                      Serial.println(JSONmessageBufferACC);
                      HTTPClient httpACC; //Declare object of class HTTPClient
                      httpACC.begin("http://medlinkapi.herokuapp.com/accelerometres");      //Specify request destination
                      httpACC.addHeader("Content-Type", "application/json");  //Specify content-type header
                      int httpCodeACC = httpACC.POST(JSONmessageBufferACC);   //Send the request
                      String payloadACC = httpACC.getString();                 //Get the response payload
                      
                      Serial.println("httpCodeACC: ");   //Print HTTP return code
                      Serial.println(httpCodeACC);   //Print HTTP return code
                      Serial.println("payloadACC : ");    //Print request response payload
                      Serial.println(payloadACC);    //Print request response payload
                      httpACC.end();  //Close connection 
                  
                       if(httpCodeACC == 200){
                         digitalWrite(YEL, HIGH);
                      } 
                    } 
   
     }


  delay(100);
  count++;
 
}
