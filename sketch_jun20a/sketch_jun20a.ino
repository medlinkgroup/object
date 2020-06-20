#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <math.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

//const char* ssid = "VM iPhone"; 
//const char* ssid = "iPhone"; 
const char* ssid = "Fabiana Montiel"; 
//replace with your own wifi ssid 
//const char* password = "mchagall";
//const char* password = "cici97ci";  
const char* password = "fmfmfmfm";  
//replace with your own //wifi ssid password 
const char* host = "https://medlinkapi.herokuapp.com/";

int count = 0;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

String sensorReadings;
float sensorReadingsArr[3];

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(1);
 
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
 
void setup(void) 
{
 Serial.begin(115200);
 delay(10);  
 
 Serial.println("Accelerometer Test"); Serial.println("");
 
 /* Initialise the sensor */
 if(!accel.begin())
 {
 /* There was a problem detecting the ADXL345 ... check your connections */
 Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
 while(1);
 }

 
 /* Set the range to whatever is appropriate for your project */
 accel.setRange(ADXL345_RANGE_16_G);
 // displaySetRange(ADXL345_RANGE_8_G);
 // displaySetRange(ADXL345_RANGE_4_G);
 // displaySetRange(ADXL345_RANGE_2_G);
 
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
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

float tempValueArray[10];
float accelerometerXArray[10];
float accelerometerYArray[10];
float accelerometerZArray[10];

 
void loop(void) 
{
  
  int valeur_brute = analogRead(A0);
  float temperature_celcius = valeur_brute * (3.3 / 1023.0 * 100.0);
  Serial.print("Temperature: ");
  Serial.println(temperature_celcius);  
  
 /* Get a new sensor event */ 
 sensors_event_t event; 
 accel.getEvent(&event);
 
 /* Display the results (acceleration is measured in m/s^2) */
 Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print(" ");
 Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print(" ");
 Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print(" ");Serial.println("m/s^2 ");
 delay(500);

if (count >= 5 && count <= 14) {
   tempValueArray[count - 5] = temperature_celcius;
   accelerometerXArray[count - 5] = event.acceleration.x;
   accelerometerYArray[count - 5] = event.acceleration.y;
   accelerometerZArray[count - 5] = event.acceleration.z;
} 


if ((WiFi.status() == WL_CONNECTED) && count == 15) { //Check WiFi connection status

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
     StaticJsonBuffer<1000> JSONbufferACC;   //Declaring static JSON buffer
    JsonObject& JSONencoderACC = JSONbufferACC.createObject(); 
 
    JSONencoderTEMP["deviceID"] = "1";
    JSONencoderTEMP["date"] = "2020-06-20T12:30:00.000Z";
    
    JSONencoderACC["deviceID"] = "1";
    JSONencoderACC["date"] = "2020-06-20T12:30:00.000Z";
 
    JsonArray& tempValues = JSONencoderTEMP.createNestedArray("tempValues"); //JSON array
    tempValues.add(tempValueArray[5]); //Add value to array
    tempValues.add(tempValueArray[6]); 
    tempValues.add(tempValueArray[7]); 
    tempValues.add(tempValueArray[8]); 
    tempValues.add(tempValueArray[9]); 
    tempValues.add(tempValueArray[10]); 
    tempValues.add(tempValueArray[11]); 
    tempValues.add(tempValueArray[12]); 
    tempValues.add(tempValueArray[13]); 
    tempValues.add(tempValueArray[14]); 

    JsonArray& accelerometerXValues = JSONencoderACC.createNestedArray("accelerometerXValues"); //JSON array
    accelerometerXValues.add(accelerometerXArray[5]); //Add value to array
    accelerometerXValues.add(accelerometerXArray[6]); 
    accelerometerXValues.add(accelerometerXArray[7]); 
    accelerometerXValues.add(accelerometerXArray[8]); 
    accelerometerXValues.add(accelerometerXArray[9]); 
    accelerometerXValues.add(accelerometerXArray[10]); 
    accelerometerXValues.add(accelerometerXArray[11]); 
    accelerometerXValues.add(accelerometerXArray[12]); 
    accelerometerXValues.add(accelerometerXArray[13]); 
    accelerometerXValues.add(accelerometerXArray[14]); 

    JsonArray& accelerometerYValues = JSONencoderACC.createNestedArray("accelerometerYValues"); //JSON array
    accelerometerYValues.add(accelerometerYArray[5]); //Add value to array
    accelerometerYValues.add(accelerometerYArray[6]); 
    accelerometerYValues.add(accelerometerYArray[7]); 
    accelerometerYValues.add(accelerometerYArray[8]); 
    accelerometerYValues.add(accelerometerYArray[9]); 
    accelerometerYValues.add(accelerometerYArray[10]); 
    accelerometerYValues.add(accelerometerYArray[11]); 
    accelerometerYValues.add(accelerometerYArray[12]); 
    accelerometerYValues.add(accelerometerYArray[13]); 
    accelerometerYValues.add(accelerometerYArray[14]); 

    JsonArray& accelerometerZValues = JSONencoderACC.createNestedArray("accelerometerZValues"); //JSON array
    accelerometerZValues.add(accelerometerZArray[5]); //Add value to array
    accelerometerZValues.add(accelerometerZArray[6]); 
    accelerometerZValues.add(accelerometerZArray[7]); 
    accelerometerZValues.add(accelerometerZArray[8]); 
    accelerometerZValues.add(accelerometerZArray[9]); 
    accelerometerZValues.add(accelerometerZArray[10]); 
    accelerometerZValues.add(accelerometerZArray[11]); 
    accelerometerZValues.add(accelerometerZArray[12]); 
    accelerometerZValues.add(accelerometerZArray[13]); 
    accelerometerZValues.add(accelerometerZArray[14]); 
    

    char JSONmessageBufferTEMP[500];
    char JSONmessageBufferACC[1000];
    
    JSONencoderTEMP.prettyPrintTo(JSONmessageBufferTEMP, sizeof(JSONmessageBufferTEMP));
    Serial.println(JSONmessageBufferTEMP);
    JSONencoderACC.prettyPrintTo(JSONmessageBufferACC, sizeof(JSONmessageBufferACC));
    Serial.println(JSONmessageBufferACC);
 
    HTTPClient httpTEMP; //Declare object of class HTTPClient
    HTTPClient httpACC; //Declare object of class HTTPClient

    httpTEMP.begin("http://medlinkapi.herokuapp.com/temperatures");      //Specify request destination
    httpTEMP.addHeader("Content-Type", "application/json");  //Specify content-type header
    
    httpACC.begin("http://medlinkapi.herokuapp.com/accelerometers");      //Specify request destination
    httpACC.addHeader("Content-Type", "application/json");  //Specify content-type header
 
    int httpCodeTEMP = httpTEMP.POST(JSONmessageBufferTEMP);   //Send the request
    String payloadTEMP = httpTEMP.getString();                 //Get the response payload
     
    int httpCodeACC = httpACC.POST(JSONmessageBufferTEMP);   //Send the request
    String payloadACC = httpACC.getString();                 //Get the response payload

    Serial.println("httpTEMPCode: ");   //Print HTTP return code
    Serial.println(httpCodeTEMP);   //Print HTTP return code
    Serial.println("payloadTEMP : ");    //Print request response payload
    Serial.println(payloadTEMP);    //Print request response payload
    Serial.println("httpCodeACC: ");   //Print HTTP return code
    Serial.println(httpCodeACC);   //Print HTTP return code
    Serial.println("payloadACC : ");    //Print request response payload
    Serial.println(payloadACC);    //Print request response payload
 
    httpTEMP.end();  //Close connection
    httpACC.end();  //Close connection
 
} 
  count++;
  delay(1000);  //Send a request every 1 second
 

 
}
