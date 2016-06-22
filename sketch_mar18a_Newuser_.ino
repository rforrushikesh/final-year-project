
//THIS ARDUINO CODE IS RUNNING WITH ALL CONDITIONS. SHOW THIS FOR INC.


// IMPORTANT: Adafruit_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Adafruit_TFTLCD.h FOR SETUP.

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <Arduino.h>
#include <Wire.h>         // this #include still required because the RTClib depends on it
#include "RTClib.h"
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Keypad.h>

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;


//for RTC
#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
#define Serial SerialUSB
#endif

RTC_Millis rtc;

//fingerprint
int getFingerprintIDez();
uint8_t getFingerprintEnroll();
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
//SoftwareSerial mySerial(2, 3);
//Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial3);




int pir,x;
int id=100,m;
int i=0,j=0,k;
char holdkey;
String inputString = "",account="",pass="",withdraw="",pwd="",acc="",amd="",option="",pass1="",newid="",check = "",passorfail = "";         // a string to hold incoming data
String newacc="",newpass="";
boolean stringComplete = false;
char customKey,accKey,Key,Key2=0,wd,start;
char incomingByte;
String readString;


const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','E'},
  {'4','5','6','C'},
  {'7','8','9','B'},
  {'*','0','#','F'}
};
byte rowPins[ROWS] = {5,4,3,2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9,8,7,6}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
   
void setup(void) 
{
              Serial.begin(9600);   
              // following line sets the RTC to the date & time this sketch was compiled
                rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
                // This line sets the RTC with an explicit date & time, for example to set
                // January 21, 2014 at 3am you would call:
                 //rtc.adjust(DateTime(2085, 1, 4, 3, 0, 0));  //(year,month,date,hrs,mins,sec)  
             // Serial.println(F("TFT LCD test"));
            
            #ifdef USE_ADAFRUIT_SHIELD_PINOUT
             // Serial.println(F("Using Adafruit 2.8\" TFT Arduino Shield Pinout"));
            #else
             // Serial.println(F("Using Adafruit 2.8\" TFT Breakout Board Pinout"));
            #endif
            
             // Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());
              tft.reset();
              uint16_t identifier = tft.readID();
              tft.begin(identifier);
            //  Serial.println(F("Benchmark                Time (microseconds)")); 
              // set the data rate for the sensor serial port
             finger.begin(57600);
              
              if (finger.verifyPassword()) {
               // Serial.println("Found fingerprint sensor!");
              } else {
                //Serial.println("Did not find fingerprint sensor :(");
                while (1);
              }
              //Serial.println("Waiting for valid finger..."); 
            pinMode(10,OUTPUT);
              pinMode(12,OUTPUT);
            pinMode(11,INPUT);            //pir ip
            pinMode(13,OUTPUT);          //led op
            digitalWrite(10,HIGH);  
            digitalWrite(12,LOW); 
}

void loop(void) {
 for(uint8_t rotation=0; rotation<2; rotation++)
 {
   tft.setRotation(rotation);
 }
// while(digitalRead(11)!=HIGH)
{ 
//  if(digitalRead(11)==HIGH)
// break; 
  
  
 DateTime now = rtc.now();
  tft.fillScreen(WHITE);
  unsigned long start = micros();
  tft.fillScreen(WHITE);
  tft.setCursor(0, 0);
  tft.setTextColor(BLUE); 
  tft.setTextSize(4);
  tft.println();
  tft.println();
  tft.println(" WELCOME");
  tft.println();
  tft.println("    TO");
  tft.println();
  tft.println(" SBI BANK");
  tft.print("  ");
    tft.setTextSize(3);
    tft.setCursor(5,5);
    tft.print("   ");
  tft.print(now.day());
  tft.print("/");
  tft.print(now.month());
  tft.print("/");
  tft.println(now.year());
  tft.print("   ");
  tft.print(now.hour());
  tft.print(":");
  tft.print(now.minute());
   tft.print(":");
   tft.print(now.second());
   delay(2000);
}

// delay(5000);
  
  
  //if(digitalRead(pir)==HIGH)
  {
    
  tft.fillScreen(WHITE);
  tft.setCursor(5, 5);
  tft.setTextSize(3);
  
  tft.println("  Plese select  ");
   tft.println("  an option:");
   tft.println("  1.Existing Customer");
   tft.println("  2.New Customer");
   
   
   
  
       customKey = customKeypad.waitForKey();
     delay(1000);
    
     if(customKey)
     {
     switch(customKey)
     {
     case '1':
     delay(1000);
     Serial.println("HI");
          // Serial.println(operation); 
      tft.fillScreen(WHITE);
  tft.setCursor(5, 5);
  tft.setTextSize(3);
  tft.println();
  tft.println();
  tft.println();
  tft.println("  Plese put ");
   tft.println("     your ");
   tft.println("  authorised");
   tft.println("    finger ");
    tft.println("    on the ");
    tft.println("    sensor");
     delay(1000);
   for(i = 0; i < 25 ; i++)
     {
    id=getFingerprintIDez();
    m=id;
    if(id==0 || id== 15 || id==20 || (id>=100 && id<=200))
    break;
    delay(100); 
     }
    if(id==0 || id== 15 || id==20 || (id>=100 && id<=200))
    {
          tft.fillScreen(WHITE);
  tft.setCursor(5, 5);
  tft.setTextSize(3);
  tft.println();
  tft.println();
  tft.println("  Plese enter");
   tft.println("     your ");
   tft.println("  account");
   tft.println("    number");
   tft.println();
    //delay(1000);
    
  
    while(accKey!='E')
    {
           char accKey = customKeypad.waitForKey();//this is very imp line!!!
           if(accKey=='E')
           break;
           else
           {
           account +=accKey;
          tft.print(accKey);
          
           }
          
    
    }

     
      tft.fillScreen(WHITE);
  tft.setCursor(5, 5);
  tft.setTextSize(3);
  tft.println();
  tft.println();
  tft.println("  Plese enter");
   tft.println("     your ");
   tft.println(" password");
  // delay(1000);
     
     
        while(Key!='E')
    {
           char Key = customKeypad.waitForKey();//this is very imp line!!!
            if(Key=='E')
           break;
           else
           {
           pass += Key;
          tft.print("X");
         
           }
     
   
    }
    
   // Key='';'
   Serial.println(m);
    Serial.println(account);
    Serial.println(pass);
    delay(1000);
   while(!Serial.available())
   {}
     while (Serial.available()>0) {
  delay(10);  // if the data came

   incomingByte = Serial.read(); // read byte
 //Serial.println(incomingByte);

   readString +=incomingByte;

  }
     
     
    delay(2000);
    
     if(m==0 || m==15 || m==20 || (m>=100 && m<=200))
      {
         
                  
              if(readString!="yesn")
              {
                tft.fillScreen(WHITE);
                tft.setCursor(5, 5);
                tft.setTextSize(3);
                tft.println("NOT DETECTED");
                tft.println("You are not ");
                tft.println("Authenticated user");
                delay(3000);
                 break;
            }
              if(readString == "yesn")
                {
                       readString="";   
                          
                          
                         tft.fillScreen(WHITE);
                      tft.setCursor(5, 5);
                      tft.setTextSize(3);
                      tft.println();
                      tft.println();
                      tft.println();
                      tft.println("    Welcome ");
                      if(m==0)
                       tft.println("    Mr. Madki ");
                       else if(m==15)
                         tft.println("  Mr. Kulkarni ");
                       else if(m==20)
                         tft.println("    Mr. Mapari ");
                         else if(m>=100 && m<=200)
                           tft.println("    Mr. Guest ");
                       delay(3000);
                         tft.fillScreen(WHITE);
                      tft.setCursor(5, 5);
                      tft.setTextSize(3);
                   
                      tft.println("Select Transaction");
                       tft.println("1.Cash Transfer");
                        tft.println("2.Cash Withdrawal");
                         tft.println("3.Balance Enquiry");
                         tft.println("4.Pin Change");
                         delay(1000);
                         
                         char Key2 = customKeypad.waitForKey();
                         delay(1000);
                        
                         if(Key2)
                         {
                         switch(Key2)
                             {
                             case '1':
                             tft.fillScreen(WHITE);
                          tft.setCursor(5, 5);
                          tft.setTextSize(3);
                          tft.println("Deposit");
                          delay(3000);
                          break;
                          
                          case '2':
                          option="2)";
                          tft.fillScreen(WHITE);
                          tft.setCursor(5, 5);
                          tft.setTextSize(3);
                          tft.println("Cash Withdrawal");
                          tft.println();
                          tft.println();
                          tft.println("Enter amount");
                          {
                                            while(wd!='E')
                                          {
                                               char  wd = customKeypad.waitForKey();//this is very imp line!!!
                                                 if(wd=='E')
                                                 {
                                                  break;
                                                 }
                                                 else
                                                 {
                                                 withdraw  += wd;
                                                tft.print(wd);
                                            
                                                 }
                                         
                                          }
                                         // option +=withdraw;
                                         //option.concat(withdraw);
                                         Serial.print("2)");
                                          Serial.println(withdraw);
                                        //  Serial.print(withdraw);
                          
                          
                          }
                           delay(3000);
                           //Serial.print(option);
                              while(!Serial.available())
                                   {}
                              while (Serial.available()>0) 
                              {
                                        delay(10);  // if the data came
                                         incomingByte = Serial.read(); // read byte
                                       //Serial.println(incomingByte);
                                         readString +=incomingByte;
                                      
                              }
                            if(readString == "yesn")
                             {
                                      tft.fillScreen(WHITE);
                                  tft.setCursor(5, 5);
                                  tft.setTextSize(3);
                                  tft.println("Transaction Successful");
                                   delay(3000);
                             }
                             else
                             {
                               
                                tft.fillScreen(WHITE);
                                  tft.setCursor(5, 5);
                                  tft.setTextSize(3);
                               tft.println("Transaction is not Successful");
                               delay(3000);
                             }
                          break;
                          
                          case '3':
                                tft.fillScreen(WHITE);
                                tft.setCursor(5, 5);
                                tft.setTextSize(3);
                                tft.println("Balance Enquiry");
                                Serial.println("3)");
                                while(!Serial.available())
                                   {}
                                while (Serial.available()>0) 
                                {
                                        delay(10);  // if the data came
                                         incomingByte = Serial.read(); // read byte
                                       //Serial.println(incomingByte);
                                         readString +=incomingByte;        
                               }
                               tft.println("Account balance = ");
                              tft.println(readString);
                                 delay(3000);
                                 
                          break;
                          
                         
                           case '4':
                              tft.fillScreen(WHITE);
                              tft.setCursor(5, 5);
                              tft.setTextSize(3);
                              tft.println("Pin Change");        
                               delay(300);
                               tft.println("Enter 5 digit pin");
                               i = 0;
                             while(i!=5)
                             {
                                     char newpass = customKeypad.waitForKey();//this is very imp line!!!
                                     if(newpass=='E')
                                       break;
                                     else
                                     {
                                      pass1 += newpass;
                                      tft.print("X");
                                     }
                                     i++;
                             }
                             tft.println(); 
                             tft.println("Press Enter");
                             char newpass = customKeypad.waitForKey();
                             while(newpass != 'E')
                             {
                               newpass = customKeypad.waitForKey();
                             }
                             Serial.print("4)");
                             Serial.println(pass1);
                             delay(3000);
                               //Serial.print(option);
                              while(!Serial.available())
                                   {}
                              while (Serial.available()>0) 
                              {
                                        delay(10);  // if the data came
                                         incomingByte = Serial.read(); // read byte
                                       //Serial.println(incomingByte);
                                         readString +=incomingByte;
                                      
                              }
                            if(readString == "yesn")
                             {
                                      tft.fillScreen(WHITE);
                                  tft.setCursor(5, 5);
                                  tft.setTextSize(3);
                                  tft.println("Password changed"); 
                                  tft.println("Successfully");
                                   delay(3000);
                             }
                             else
                             {
                               
                                tft.fillScreen(WHITE);
                                  tft.setCursor(5, 5);
                                  tft.setTextSize(3);
                               tft.println("Transaction is ");
                               tft.println("not Successful");
                               delay(3000);
                             }
                            break;
//                          default:
//                              tft.fillScreen(WHITE);
//                              tft.setCursor(5, 5);
//                              tft.setTextSize(3);
//                              tft.println("Invalid");
//                               delay(3000);
//                              break;
                        }
                        tft.println("Press Enter to ");
                       tft.print("   exit.");
                       char Key3 = customKeypad.waitForKey();
                       while(Key3 != 'E')
                       {
                            Key3 = customKeypad.waitForKey();
                       }
                         
                     
                         }
                         
                        }
                       
      }    
                }
    
else
    {
    tft.fillScreen(WHITE);
  tft.setCursor(5, 5);
  tft.setTextSize(3);
  tft.println();
  tft.println();
 tft.println();
  tft.println("    Plese ");
   tft.println("     input ");
   tft.println("    a valid");
   tft.println("    finger ");
    tft.println("    on the ");
    tft.println("    sensor");
    delay(2000);
   
    
    }    
     break;
     
     case '2':
     //big deal!!!
     Serial.println("new");
     delay(500);
     while(!Serial.available())
                                   {}
                              while (Serial.available()>0) 
                              {
                                        delay(10);  // if the data came
                                         incomingByte = Serial.read(); // read byte
                                       //Serial.println(incomingByte);
                                         readString +=incomingByte;
                                      
                              }
                              newacc=readString;
                              readString="";
                              Serial.println("newpass received");
                              
                               while(!Serial.available())
                                   {}
                              while (Serial.available()>0) 
                              {
                                        delay(10);  // if the data came
                                         incomingByte = Serial.read(); // read byte
                                       //Serial.println(incomingByte);
                                         readString +=incomingByte;
                                      
                              }
                              newpass=readString;
                              readString="";
                              Serial.println("newacc received");
                              
                              
                              while(!Serial.available())
                                   {}
                              while (Serial.available()>0) 
                              {
                                        delay(10);  // if the data came
                                         incomingByte = Serial.read(); // read byte
                                       //Serial.println(incomingByte);
                                         readString +=incomingByte;
                                      
                              }
                              newid=readString;
                              x=newid.toInt();
                              readString="";
                              Serial.println("newid received"); 
     tft.fillScreen(WHITE);
  tft.setCursor(5, 5);
  tft.setTextSize(3);
 tft.println("  Plese put ");
   tft.println("     your ");
   tft.println("    finger ");
    tft.println("    on the ");
    tft.println("  sensor for");
     tft.println("    enrollment");
     delay(3000);
      
     
     while (!  Serial.println(getFingerprintEnroll()) )
     {};
          while(!Serial.available())
                                   {}
                              while (Serial.available()>0) 
                              {
                                        delay(10);  // if the data came
                                         incomingByte = Serial.read(); // read byte
                                       //Serial.println(incomingByte);
                                         readString +=incomingByte;
                                      
                              }
                               delay(2000);
    // Serial.println("Enrollment successful");
   Serial1.flush(); 
    Serial2.flush(); 
     Serial3.flush();
    if(readString == "yesn")
     {                         
     tft.fillScreen(WHITE);
  tft.setCursor(5, 5);
  tft.setTextSize(3);
 tft.println("New user ");
   tft.println("generated ");
   tft.println("successfully");
    tft.println("with fingerprint");
    tft.print("ID=");
    tft.println(newid);
     delay(3000);
     
     tft.fillScreen(WHITE);
  tft.setCursor(5, 5);
  tft.setTextSize(3);
 tft.println("Your account no. is:");
   tft.println(newacc);
   tft.println("Your password is:");
    tft.println(newpass);
    tft.println("Your fingerid is:");
    tft.println(newid);
    delay(5000);
//       if (stringComplete) {
//    //Serial.println(inputString);
//    // clear the string:
//    inputString = "";
//    stringComplete = false;
//  }
     
     }
     else
     {
        tft.fillScreen(WHITE);
        tft.setCursor(5, 5);
        tft.setTextSize(3);
        tft.println("New user ");
        tft.println(" NOT Created ");
        tft.println("Try again !!");
       delay(3000);
     }
     break;
     
     default:
     tft.fillScreen(WHITE);
  tft.setCursor(5, 5);
  tft.setTextSize(3);
  tft.println("Invalid");
   delay(3000);
  break;
     }
     }   
  }
  
  

  
  
}//loop




uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
    //  Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
     // Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
    //  Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
    //  Serial.println("Imaging error");
      return p;
    default:
     // Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
     // Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
     // Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
     // Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Could not find fingerprint features");
      return p;
    default:
     // Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
   // Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
   // Serial.println("Did not find a match");
    return p;
  } else {
    //Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  //Serial.print("Found ID #"); Serial.print(finger.fingerID); 
 // Serial.print(" with confidence of "); Serial.println(finger.confidence); 
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() 
{
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  //Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  //Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}

uint8_t getFingerprintEnroll() 
{

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(x);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(x);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(x);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID ");
  Serial.println(x);
  p = finger.storeModel(x);
  if (p == FINGERPRINT_OK) 
  {
    Serial.println("Stored!");
    return 123;
  } 
  else if (p == FINGERPRINT_PACKETRECIEVEERR) 
  {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) 
  {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) 
  {
    Serial.println("Error writing to flash");
    return p;
  } 
  else 
  {
    Serial.println("Unknown error");
    return p;
  }   
}


