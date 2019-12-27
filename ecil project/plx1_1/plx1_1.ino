//DATA LOGGER SYSTEM
#include "Wire.h"  //INCLUDES LIBRARY FOR RTC
#include "dht.h" //INCLUDES LIBRARY FOR DHT SENSOR
#include <SPI.h> //INCLUDES LIBRARY FOR SD CARD MODULE
#include <SD.h> //INCLUDES LIBRARY FOR SD CARD MODULE
File myFile;
#define dht_apin A0 // Analog Pin sensor is connected to
dht DHT;
#define DS3231_I2C_ADDRESS 0x68
void setup() 
{
  Wire.begin();
 Serial.begin(9600);
 while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
   if (!SD.begin(4)) {
    
    while (1);
  }
   myFile = SD.open("neww.txt", FILE_WRITE);
   if(myFile)
 { myFile.println("randomno,rtctime,date,day,tem,humidity,");
 }
 myFile.close();
  setDS3231time(43,44,11,5,31,5,19);
  
 Serial.println("CLEARDATA");//clear excel sheet
 Serial.println("LABEL,current time,random no,rtctime,date,day,temparature,humidity");//label for ms-excel
}
void loop()
{
  myFile = SD.open("neww.txt", FILE_WRITE);
 if(myFile)
 { 
Serial.print("DATA,TIME");   Serial.print(",");
Serial.print(random(0,100));//random number generator from 0 to 100
//print "," for different value
 myFile.print(random(0,100));
 displayTime(); 
  DHT.read11(dht_apin);
 Serial.print(DHT.temperature); myFile.print(DHT.temperature); Serial.print("C  "); myFile.print("C ");Serial.print(",");myFile.print(",");
 Serial.print(DHT.humidity);myFile.print(DHT.humidity);  Serial.print("%  "); myFile.print("%");Serial.println(",");myFile.println(",");
 }
 
myFile.close();
delay(1000);
}
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}

byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
  

 
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0);
  Wire.write(decToBcd(second)); 
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour)); 
  Wire.write(decToBcd(dayOfWeek)); 
  Wire.write(decToBcd(dayOfMonth)); 
  Wire.write(decToBcd(month)); 
  Wire.write(decToBcd(year)); 
  Wire.endTransmission();
}
void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); 
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}
void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  Serial.print(",");myFile.print(",");
  
  Serial.print(hour, DEC);myFile.print(hour, DEC);
 
  Serial.print("h");myFile.print("h");
  if (minute<10)
  {
    Serial.print("0");myFile.print("0");
  }
  Serial.print(minute, DEC);myFile.print(minute, DEC);
  Serial.print("min"); myFile.print("min");
  if (second<10)
  {
    Serial.print("0");myFile.print("0");
  }
  Serial.print(second, DEC);myFile.print(second, DEC);
  Serial.print("sec  ");myFile.print("sec ");   Serial.print(",");myFile.print(",");
  Serial.print(dayOfMonth, DEC);myFile.print(dayOfMonth, DEC);
  Serial.print("//");myFile.print("//");
  Serial.print(month, DEC);myFile.print(month, DEC);
  Serial.print("//");myFile.print("//");
  Serial.print(year, DEC);myFile.print(year, DEC);
  Serial.print(" ");myFile.print(" ");  Serial.print(",");myFile.print(",");
  
  switch(dayOfWeek)
  {
  case 0:
    Serial.print("Sunday"); myFile.print("sunday");Serial.print(",");myFile.print(",");
    break;
  case 1:
    Serial.print("Monday"); myFile.print("monday");Serial.print(",");myFile.print(",");
    break;
  case 2:
    Serial.print("Tuesday");myFile.print("tuesday");Serial.print(",");myFile.print(",");
    break;
  case 3:
    Serial.print("Wednesday");myFile.print("wednesday");Serial.print(",");myFile.print(",");
    break;
  case 4:
    Serial.print("Thursday");myFile.print("thursday");Serial.print(",");myFile.print(",");
    break;
  case 5:
    Serial.print("Friday");myFile.print("friday");Serial.print(",");myFile.print(",");
    break;
  case 6:
    Serial.println("Saturday");myFile.print("saturday");Serial.print(",");myFile.print(",");
    break;
    
  }
}
