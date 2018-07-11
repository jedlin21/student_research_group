#define BLYNK_PRINT Serial

#include <SD.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include "Timer.h"

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "LG K8 LTE";
char pass[] = "asdfg123";
char auth[] = "c0a680cfe8dc4738b56369ba1e4753a6";

//SD CARD
#define SD_CARD_CD_DIO D8
File SDFileData;




Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
// Prepare tables for pressure and weight.
unsigned short pressure[5000][2];  
unsigned short weight[5000][2];    
int i_pressure = 0;
int i_weight = 0;


Timer t;
int start_time;

bool password_verification = false;
bool launch = false;

// Attach virtual serial terminal to Virtual Pin V1
WidgetTerminal terminal(V1);


// You can send commands from Terminal to your hardware. Just use
// the same Virtual Pin as your Terminal Widget
BLYNK_WRITE(V1)
{

  // if you type "password" into Terminal Widget - it will set variable "start" on True"
  if (!password_verification){
    if (String("password") == param.asStr()) {
      terminal.println("Password correct");
      
      password_verification = true;

    } else {
  
      // Send "wrong" message
      terminal.println("Wrong password. Type correct password.");
    }
    }
  if (password_verification){
    if (String("launch") == param.asStr()) {
       launch = true;
       terminal.println("Start\n");
       start_time = millis();
       
    } else {
      terminal.println("Check if sector clear and type 'launch' to start");
    }
  }
  // Ensure everything is sent
  terminal.flush();
}

 
  
  void setup()
{  
  
  // Debug console
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  // This will print Blynk Software version to the Terminal Widget when
  // your hardware gets connected to Blynk Server
  terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
  terminal.println(F("-------------"));
  terminal.println(F("Type password to start"));
  terminal.flush();

  /* Initialise the microSD card */
  if (!SD.begin(SD_CARD_CD_DIO)) 
  {
    /* If there was an error output this to the serial port and go no further */
    terminal.println("ERROR: SD card failed to initiliase");
    return;
  }else
  {
    terminal.println("SD Card OK");
  }

  /* Check if the text file already exists */
   while(SD.exists("test.txt"))
   {
     /* If so then delete it */
     terminal.println("test.txt already exists...DELETING");
     SD.remove("test.txt");
   }

   /* Create a new text file on the microSD card */
  terminal.println("Creating test.txt");
  SDFileData = SD.open("test.txt", FILE_WRITE);

  /* If the file was created ok then add come content */
  if (SDFileData)
  {
    SDFileData.println("It worked !!!");
  
    /* Close the file */
    SDFileData.close();   
    
    terminal.println("done.");
  }else
  {
      terminal.println("Error writing to file !");
  }
  
  SDFileData = SD.open("test.txt", FILE_WRITE);
  Blynk.virtualWrite(V2, 2);

}

int zapis = 0;

int czas_k;


void loop()
{
  
    Blynk.run();
  
    /*
  if (zapis <= 10000 && launch)
  {
    SDFileData.println(15, DEC);
    SDFileData.println("\t");
    SDFileData.println(0.625, 3);
    zapis++;
    terminal.println( zapis);
  }

  if (zapis == 10001){
    launch = false;
    czas_k = start_time - millis();
    SDFileData.println("\n");
    SDFileData.println("CZAS [ms]");
    SDFileData.println(czas_k,6);
    SDFileData.close();
    terminal.println( czas_k,6);
    zapis++;
  }
*/
  if(launch){
    /*
    pressure[i_pressure][0] = ads.readADC_SingleEnded(0);
    pressure[i_pressure][1] = millis() - start_time;
    weight[i_weight][0] = ads.readADC_SingleEnded(1);
    weight[i_weight][1] = millis() - start_time;
    i_pressure++;
    i_weight++;
      */
    i_pressure = ads.readADC_SingleEnded(0);
    SDFileData.println(i_pressure, DEC);
    time_k = millis() - start_time;
    SDFileData.println(time_k, DEC);
    i_weight = ads.readADC_SingleEnded(1);
    SDFileData.println(i_weight, DEC);
    time_k = millis() - start_time;
    SDFileData.println(time_k, DEC);
    
    if(i_weight == 4999){
      terminal.println("Done");
      /*We send data to SD here.
      We can also send some plotable data to Blynk*/
      terminal.println( i_weight);
      terminal.println( pressure[i_pressure][0]);
      terminal.println( weight[i_weight][0]);
      terminal.println( "; ");
      terminal.println( pressure[i_pressure][1]);
      terminal.println( weight[i_weight][1]);
      launch = false;
      password_verification = false; 
      launch = false;
      }
  }
}









